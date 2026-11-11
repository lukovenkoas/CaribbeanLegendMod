// CL Toolkit — точка входа.
// Файл из mod_inits загружается движком при старте и не выгружается,
// поэтому правки ЗДЕСЬ применяются только после перезапуска игры.
// Вся логика окна — в cltoolkit\clt_window.c (грузится при каждом открытии).

#define CLT_WINDOW_SEGMENT	"cltoolkit\clt_window.c"
#define CLT_WINDOW_INI		"RESOURCE\INI\INTERFACES\mods\cltoolkit\clt_main.ini"
#define CLT_INTERFACE_ID	99901
#define CLT_OPEN_CONTROL	"BOAL_ControlDebug"	// Insert: назначен в init_pc.c:21, в релизе ничего не делает (controls_func.c:816)

#event_handler("Control Activation", "CLT_OnControl");

extern void CLT_Window_Init(string iniFile);
extern void CLT_Window_Close();

bool clt_bOpen = false; // окно открыто нами (сбрасывается в CLT_FrameAfterClose)

void CLT_OnControl()
{
	string controlName = GetEventData();
	if (controlName != CLT_OPEN_CONTROL) return;
	// Повторный Insert закрывает окно. Проверка Launched/CurrentInterface — на случай
	// быстрой загрузки при открытом окне: OnLoad (seadogs.c:915) сбрасывает Launched.
	if (clt_bOpen && int(InterfaceStates.Launched) == true && CurrentInterface == CLT_INTERFACE_ID)
	{
		CLT_Window_Close();
		return;
	}
	CLT_Open();
}

// Те же условия, что procEnableInterfaceLaunch + procInterfacePrepare (interface.c:1487, 1599)
bool CLT_CanOpen()
{
	if (bMainMenu) return false;
	if (int(InterfaceStates.Launched) == true) return false;
	if (IsEntity(reload_fader)) return false;
	if (DialogRun != 0) return false;
	if (g_ibVideoExecuting) return false;
	aref arFader;
	if (GetEntity(arFader, "fader")) return false;
	return true;
}

void CLT_Open()
{
	if (!CLT_CanOpen())
	{
		trace("[CLT] open refused (interface/dialog/fader/video active)");
		return;
	}
	if (!LoadSegment(CLT_WINDOW_SEGMENT))
	{
		trace("[CLT] ERROR: cannot load segment " + CLT_WINDOW_SEGMENT);
		return;
	}

	// Повтор procInterfacePrepare (interface.c:1613-1626)
	Telescope_Off();
	Event("Interface_Started");
	InterfaceStates.Launched = true;
	InterfaceStates.doUnFreeze = false;
	EngineLayersOffOn(false);
	ISetSoundEvents();
	DeleteAttribute(&GameInterface, "");
	DeleteAttribute(&InterfaceStates, "tooltip");
	TimeScale_Info("");
	LayerFreeze(INTERFACE_REALIZE, false);
	LayerFreeze(INTERFACE_EXECUTE, false);

	nPrevInterface = -1;
	CurrentInterface = CLT_INTERFACE_ID;
	trace("[CLT] window opening");
	clt_bOpen = true;
	CLT_Window_Init(CLT_WINDOW_INI);
}

// Вызывается окном после EndAboveForm и снятия своих обработчиков.
// Повтор EndCancelInterface(true) (interface.c:1133) без Interfaces[CurrentInterface].
void CLT_FinishClose()
{
	KillMessageBox();
	KillIconSelector();
	bGamePadChangeEnable = false;
	interfaceResultCommand = RC_INTERFACE_DO_NOTHING;
	InterfaceStates.doUnFreeze = true;
	IDeleteSoundEvents();
	// Сегмент окна выгружаем в следующем кадре, не изнутри его же функции
	SetEventHandler("frame", "CLT_FrameAfterClose", 1);
}

void CLT_FrameAfterClose()
{
	DelEventHandler("frame", "CLT_FrameAfterClose");
	clt_bOpen = false;
	UnloadSegment(CLT_WINDOW_SEGMENT);
	// InterfaceDoExit (seadogs.c): Launched=false, MSG_INTERFACE_RELEASE, разморозка слоёв
	Start_InterfaceDoExit();
	PostEvent("evntQuestsCheck", 1);
	trace("[CLT] window closed");
}

// ---------- Стоянка кораблей в порту ----------
// База (dialogs\<lang>\Common_Portman.c) отказывает только при int(NPChar.Portman) >= 3,
// а ссылку «забрать корабль» рисует при Portman > 0. Та же схема у Workshop-модов стоянки
// («Extended Mooring» 3535021577 — порог 6, вариант 3535061146 — 10). Поэтому счётчик
// держим в «понятном диалогу» диапазоне 0..2 и перед каждым диалогом пересчитываем его
// из фактического числа кораблей на стоянке у этого портмана:
//   меньше лимита   -> 0/1/2 (места есть, ссылка «забрать» видна),
//   лимит достигнут -> 99    (срабатывает любой порог: 3, 6, 10 — штатная реплика «нет мест»).
// Лимит — pchar.CLToolkit.PortStock (страница «Режимы»), по умолчанию атрибута нет =
// «авто»: не вмешиваемся, работает штатный порог базы или установленного мода стоянки.
// Диалоги при этом не копируются (12 языков), правка одна и не зависит от языка.
#event_handler("evntDialogStart", "CLT_PortmanSync");

void CLT_PortmanSync()
{
	if (!CheckAttribute(pchar, "CLToolkit.PortStock")) return;	// «авто» — не вмешиваемся
	ref chr;
	makeref(chr, CharacterRef);				// диалог уже начался: CharacterRef = собеседник
	if (!CheckAttribute(chr, "Portman")) return;		// не портман
	// Только городские портманы (<City>_PortMan). У базы Исла Моны (Islamona_carpenter)
	// тот же атрибут portman, но своя логика на 2/3 места — её не трогаем.
	if (!HasSubStr(chr.id, "_PortMan")) return;

	int lim = int(pchar.CLToolkit.PortStock);
	if (lim < 3) lim = 3;

	int n = 0;
	ref c;
	int i;
	for (i = 0; i < MAX_CHARACTERS; i++)
	{
		makeref(c, Characters[i]);
		if (!CheckAttribute(c, "ShipInStockMan")) continue;
		if (c.ShipInStockMan != chr.id) continue;
		n++;
		if (n >= lim) break;	// дальше считать не нужно
	}
	if (n >= lim) chr.Portman = 99;
	else if (n > 2) chr.Portman = 2;
	else chr.Portman = n;
}

// ---------- Скорость своего корабля на глобальной карте ----------
// Флаги pchar.Navigator.MapSpeed2 / MapSpeed3 ставит страница «Режимы» (clt_sec_misc.c CLT_Misc_MapSpdStep).
// Применение — как в «Навигаторе» 1.9 (nav_core.c Nav_ApplyAll): множитель пишем в TEV.worldMap.kPlayerMaxSpeed
// и worldMap.kPlayerMaxSpeed, потому что база пересчитывает скорость из TEV (worldmap_coords.c UpdateWorldMapSpeed),
// а при каждом входе на карту сбрасывает её (worldmap.c wdmCreateWorldMap). Вместо таймера «Навигатора» подписываемся
// на событие самой карты EventCoordUpdate: база шлёт его каждые 200 мс, пока открыта глобальная карта
// (worldmap_coords.c wdmCoordUpdate). Событие НЕ перепосылаем — это делает база. x1 — не вмешиваемся.
#event_handler("EventCoordUpdate", "CLT_MapSpeedTick");

void CLT_MapSpeedTick()
{
	if (!IsEntity(&worldMap)) return;
	float k = 1.0;
	if (CheckAttribute(pchar, "Navigator.MapSpeed2")) k = 2.0;
	if (CheckAttribute(pchar, "Navigator.MapSpeed3")) k = 3.0;
	if (k <= 1.0) return;
	TEV.worldMap.kPlayerMaxSpeed = k;
	worldMap.kPlayerMaxSpeed = k;
}

// ---------- Поддержка переключателей героя на суше ----------
// Повтор таймера «Навигатора» 1.9 (nav_core.c Nav_LocTick -> Nav_ApplyAll -> Nav_ApplyEffects, только герой):
// раз в секунду на локации заново ставит бессмертие с доливом HP (квесты могут снять его через
// LAi_SetImmortal(pchar, false)), энергию и заряды пистолета/мушкета. Флаги ставит хаб (clt_sec_hero.c).
// Событие LocationTimeUpdate база шлёт каждый кадр на суше (locations_loader.c), данные — dlt в секундах.
#event_handler("LocationTimeUpdate", "CLT_HeroTick");

float clt_fHeroClock = 0.0;

void CLT_HeroTick()
{
	float dlt = GetEventData();
	clt_fHeroClock = clt_fHeroClock + dlt;
	if (clt_fHeroClock < 1.0) return;
	clt_fHeroClock = 0.0;
	if (!CheckAttribute(pchar, "index")) return;
	if (!CheckAttribute(pchar, "Navigator")) return;
	if (CheckAttribute(pchar, "Navigator.Immortal"))
	{
		pchar.chr_ai.immortal = "1";
		float mhp = LAi_GetCharacterMaxHP(pchar);
		if (stf(pchar.chr_ai.hp) < mhp) LAi_SetHP(pchar, mhp, mhp);
	}
	if (CheckAttribute(pchar, "Navigator.Energy"))
	{
		float emax = 125.0;
		if (CheckAttribute(pchar, "chr_ai.energyMax")) emax = stf(pchar.chr_ai.energyMax);
		pchar.chr_ai.energy = emax;
	}
	if (CheckAttribute(pchar, "Navigator.Ammo"))
	{
		if (CheckAttribute(pchar, "chr_ai.gun.charge_max")) pchar.chr_ai.gun.charge = stf(pchar.chr_ai.gun.charge_max);
		if (CheckAttribute(pchar, "chr_ai.musket.charge_max")) pchar.chr_ai.musket.charge = stf(pchar.chr_ai.musket.charge_max);
	}
}
