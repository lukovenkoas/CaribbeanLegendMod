// CL Toolkit — окно (ядро), этап 4.1: оформление по образцу мода «Навигатор» 2.0.
// Сегмент загружается при каждом открытии окна и выгружается при закрытии,
// поэтому правки этого файла и подключённых clt_sec_*.c применяются при следующем
// открытии без перезапуска игры (_Mod_Research.md §13.5).
// Ini окна (clt_main.ini, генерируется _mod_tools\gen_clt_ini.py) без перезапуска НЕ
// перечитывается: в нём только пул узлов, подписи/видимость/картинки — отсюда.
//
// Экраны:
//   хаб (узлы CLTH_*) — карточка героя, плитки разделов, журнал под ними, справа быстрые команды
//     (переключатели героя — clt_sec_hero.c, затем кнопки);
//   страница раздела (CLTP_* + CLTG_*) — 3 колонки по 15 строк. Вид строки:
//     T — печать-переключатель, A — кнопка, S — «подпись  значение  < >», H — заголовок/пояснение.
// Хаб под страницей прячется целиком: у игры есть предел текста на кадр (опыт «Навигатора»).
// Раздел описывает строки функцией CLT_<Раздел>_Build() и выполняет команды в CLT_<Раздел>_Do(cmd, dir, big):
//   dir = 0 — щелчок по кнопке/печати, -1/+1 — «<»/«>»; big — правая кнопка мыши (крупный шаг).

#define CLT_TILE_COUNT		8
#define CLT_TILE_POOL		8
#define CLT_QUICK_COUNT		4
#define CLT_SEAL_COUNT		8
#define CLT_GP_COLS			3
#define CLT_GP_ROWS			15
#define CLT_GP_ALL			45
#define CLT_LOG_LINES		8
#define CLT_MAX_TARGETS		16
#define CLT_TEX				"cltoolkit\hub\"

#define CLT_SEC_CHAR		1
#define CLT_SEC_SKILLS		2
#define CLT_SEC_ITEMS		3
#define CLT_SEC_SHIP		4
#define CLT_SEC_CARGO		5
#define CLT_SEC_MODES		6
#define CLT_SEC_WORLD		7
#define CLT_SEC_INSP		8
#define CLT_SEC_FLEETMAP	9	// не плитка: схема особых флотов, открывается из «Мира»

bool clt_bClosing;
int clt_nSec;					// 0 — хаб, иначе открытый раздел
int clt_nHover;					// плитка под мышью или 0
int clt_nTick;
int clt_nQty;					// шаг для денег/дублонов (поле «Количество»)
string clt_sOut;				// ответ последней команды (строка внизу страницы)
string clt_aLog[CLT_LOG_LINES];

int clt_aTarget[CLT_MAX_TARGETS];	// индексы персонажей: капитан, компаньоны, пассажиры
int clt_nTargets;
int clt_nTargetPos;

// строки страницы
string clt_gKind[CLT_GP_ALL];
string clt_gCmd[CLT_GP_ALL];
string clt_gLab[CLT_GP_ALL];
string clt_gVal[CLT_GP_ALL];
bool clt_gOn[CLT_GP_ALL];
bool clt_gEn[CLT_GP_ALL];
int clt_gFill[CLT_GP_COLS];
int clt_nGCur;

#include "cltoolkit\clt_sec_char.c"
#include "cltoolkit\clt_sec_skills.c"
#include "cltoolkit\clt_sec_ship.c"
#include "cltoolkit\clt_sec_items.c"
#include "cltoolkit\clt_sec_fleet.c"
#include "cltoolkit\clt_sec_world.c"
#include "cltoolkit\clt_sec_insp.c"
#include "cltoolkit\clt_sec_misc.c"
#include "cltoolkit\clt_sec_hero.c"
#include "cltoolkit\clt_sec_cargo.c"

void CLT_Window_Init(string iniName)
{
	int i;
	clt_bClosing = false;
	clt_nSec = 0;
	clt_nHover = 0;
	clt_nTick = 0;
	clt_nQty = 1000;
	clt_sOut = "";
	for (i = 0; i < CLT_LOG_LINES; i++) clt_aLog[i] = "";

	StartAboveForm(true);
	bGameMenuStart = true; // как в blackmark_cheat.c: скрываем landinterface
	SendMessage(&GameInterface, "ls", MSG_INTERFACE_INIT, iniName);

	SetEventHandler("InterfaceBreak", "CLT_Window_OnBreak", 0);
	SetEventHandler("exitCancel", "CLT_Window_OnCancel", 0);
	SetEventHandler("ievnt_command", "CLT_Window_OnCommand", 0);
	SetEventHandler("CLT_EditConfirm", "CLT_Window_OnEditConfirm", 0);
	SetEventHandler("CLT_QtyConfirm", "CLT_Window_OnQtyConfirm", 0);
	SetEventHandler("frame", "CLT_Window_OnFrame", 0);

	CLT_BuildTargets();
	CLT_Items_Reset();
	CLT_Insp_Reset();
	CLT_Cargo_Reset();

	GameInterface.clt_qty.str = "" + clt_nQty;
	CLT_Log("Тулкит открыт. Выберите раздел.");
	CLT_HubStatic();
	CLT_ShowSec(0);
	trace("[CLT] window opened");
}

// ---------------------------------------------------------------------------
// Общие помощники
// ---------------------------------------------------------------------------

string CLT_OnOff(bool b)
{
	if (b) return "вкл";
	return "выкл";
}

// Подпись кнопки ("#" — текст как есть, не ключ строки: utilite.c SetFormatedTextButton)
void CLT_BtnText(string node, string s)
{
	SetFormatedTextButton(node, "#" + s);
}

// Значение атрибута obj.a или obj.a.b (b = "" — один уровень). Сегменты по одному:
// .(x).(y) — проверенный в базовом коде синтаксис (characterUtilite.c:2622), точку внутри .(x) не используем.
string CLT_AttrStr(ref obj, string a, string b, string def)
{
	if (b == "")
	{
		if (!CheckAttribute(obj, a)) return def;
		return obj.(a);
	}
	if (!CheckAttribute(obj, a + "." + b)) return def;
	return obj.(a).(b);
}

int CLT_AttrInt(ref obj, string a, string b)
{
	return int(CLT_AttrStr(obj, a, b, "0"));
}

int CLT_Clamp(int v, int lo, int hi)
{
	if (v < lo) return lo;
	if (v > hi) return hi;
	return v;
}

// шаг «< >»: малый или крупный (правая кнопка)
int CLT_Step(int dir, bool big, int small, int large)
{
	if (big) return dir * large;
	return dir * small;
}

// число с разделением тысяч: 1 234 567
string CLT_Num(int n)
{
	string s = "" + n;
	bool neg = false;
	if (n < 0) { neg = true; s = "" + (0 - n); }
	string r = "";
	int len = strlen(s);
	int i;
	for (i = 0; i < len; i++)
	{
		if (i > 0 && (len - i) % 3 == 0) r = r + " ";
		r = r + strcut(s, i, i);
	}
	if (neg) r = "-" + r;
	return r;
}

void CLT_Log(string s)
{
	int i;
	if (s == "") return;
	for (i = CLT_LOG_LINES - 1; i > 0; i--) clt_aLog[i] = clt_aLog[i - 1];
	clt_aLog[0] = s;
	clt_sOut = s;
	trace("[CLT] " + s);
}

// ---------------------------------------------------------------------------
// Цели: капитан, компаньоны, пассажиры (офицеры, пленники и т. д.)
// ---------------------------------------------------------------------------

void CLT_AddTarget(int idx)
{
	int i;
	if (idx < 0) return;
	if (clt_nTargets >= CLT_MAX_TARGETS) return;
	for (i = 0; i < clt_nTargets; i++)
	{
		if (clt_aTarget[i] == idx) return;
	}
	clt_aTarget[clt_nTargets] = idx;
	clt_nTargets++;
}

void CLT_BuildTargets()
{
	int i;
	clt_nTargets = 0;
	clt_nTargetPos = 0;
	CLT_AddTarget(GetMainCharacterIndex());
	for (i = 1; i < COMPANION_MAX; i++)
	{
		CLT_AddTarget(GetCompanionIndex(pchar, i));
	}
	for (i = 0; i < GetPassengersQuantity(pchar); i++)
	{
		CLT_AddTarget(GetPassenger(pchar, i));
	}
}

int CLT_TargetIndex()
{
	if (clt_nTargets <= 0) return GetMainCharacterIndex();
	return clt_aTarget[clt_nTargetPos];
}

ref CLT_Target()
{
	return GetCharacter(CLT_TargetIndex());
}

bool CLT_TargetIsHero()
{
	return CLT_TargetIndex() == GetMainCharacterIndex();
}

void CLT_NextTarget(int dir)
{
	if (clt_nTargets <= 0) return;
	clt_nTargetPos = clt_nTargetPos + dir;
	if (clt_nTargetPos < 0) clt_nTargetPos = clt_nTargets - 1;
	if (clt_nTargetPos >= clt_nTargets) clt_nTargetPos = 0;
}

string CLT_TargetRole(int idx)
{
	int i;
	if (idx == GetMainCharacterIndex()) return "капитан";
	for (i = 1; i < COMPANION_MAX; i++)
	{
		if (GetCompanionIndex(pchar, i) == idx) return "компаньон";
	}
	return "пассажир";
}

// ---------------------------------------------------------------------------
// Общие действия (хаб и разделы)
// ---------------------------------------------------------------------------

void CLT_DoHeal(ref chr)
{
	float mhp = LAi_GetCharacterMaxHP(chr);
	LAi_SetHP(chr, mhp, mhp);
	SetEnergyToCharacter(chr);
	CLT_Log(GetFullName(chr) + ": здоровье и энергия восстановлены");
}

// Максимум HP и энергии ±d — рецепт игры ChickenGod_ModifyStats (ChickenGod.c): постоянный модификатор
// M_HP_MAX / M_ENERGY_MAX в статичной таблице (источник "CLToolkit"), затем CT_UpdateCashTables —
// только полный пересчёт переносит статичную таблицу в таблицу экипировки, откуда land_table.c берёт максимум.
// Прежние способы не годились: LAi_SetHP (cheat_character F4, «Навигатор») сбрасывается любым пересчётом таблиц
// (смена экипировки), AddBonusEnergyToCharacter пересчитывает только таблицу суши — энергия не менялась,
// а HP сбрасывался. Cheat Menu «Энергия +10» (cheats.energyplus) нигде не читается.
// Без клинка таблицы суши не считаются (CT_UpdateLandTable) — тогда HP по-старому через LAi_SetHP.
void CLT_DoMaxHP(ref chr, int d)
{
	float mhp = LAi_GetCharacterMaxHP(chr);
	if (mhp + d < 1.0) d = int(1.0 - mhp);
	if (GetCharacterEquipByGroup(chr, BLADE_ITEM_TYPE) == "")
	{
		mhp = mhp + d;
		LAi_SetHP(chr, mhp, mhp);
	}
	else
	{
		if (d != 0) IncreaseChrModifier(chr, M_HP_MAX, d, "CLToolkit");
		CT_UpdateCashTables(chr);
		mhp = LAi_GetCharacterMaxHP(chr);
		LAi_SetHP(chr, mhp, mhp);
	}
	if (CheckAttribute(chr, "chr_ai.hp_max_back")) chr.chr_ai.hp_max_back = mhp;
	SetEnergyToCharacter(chr);
	CLT_Log(GetFullName(chr) + ": макс. здоровье " + int(mhp));
}

void CLT_DoMaxEnergy(ref chr, int d)
{
	if (GetCharacterEquipByGroup(chr, BLADE_ITEM_TYPE) == "")
	{
		CLT_Log(GetFullName(chr) + ": без клинка максимум энергии не меняется");
		return;
	}
	float cur = LAi_GetCharacterMaxEnergy(chr);
	if (cur + d < 10.0) d = int(10.0 - cur);
	if (d != 0) IncreaseChrModifier(chr, M_ENERGY_MAX, d, "CLToolkit");
	float hp = LAi_GetCharacterHP(chr);
	CT_UpdateCashTables(chr);
	SetEnergyToCharacter(chr);
	chr.chr_ai.energy = LAi_GetCharacterMaxEnergy(chr);
	LAi_SetHP(chr, hp, LAi_GetCharacterMaxHP(chr));
	CLT_Log(GetFullName(chr) + ": макс. энергия " + int(LAi_GetCharacterMaxEnergy(chr)));
}

void CLT_DoMoney(int d)
{
	if (d < 0 && sti(pchar.money) < (0 - d)) d = 0 - sti(pchar.money);
	AddMoneyToCharacter(pchar, d);
	CLT_Log("Песо: " + CLT_Num(sti(pchar.money)));
}

void CLT_DoDublons(int d)
{
	int have = GetCharacterItem(pchar, "gold_dublon");
	if (d < 0 && have < (0 - d)) d = 0 - have;
	if (d != 0) TakeNItems(pchar, "gold_dublon", d);
	CLT_Log("Дублоны: " + CLT_Num(GetCharacterItem(pchar, "gold_dublon")));
}

// ---------------------------------------------------------------------------
// Хаб
// ---------------------------------------------------------------------------

string CLT_TileName(int i)
{
	switch (i)
	{
		case CLT_SEC_CHAR:		return "Персонаж"; break;
		case CLT_SEC_SKILLS:	return "Умения"; break;
		case CLT_SEC_ITEMS:		return "Предметы"; break;
		case CLT_SEC_SHIP:		return "Корабль"; break;
		case CLT_SEC_CARGO:		return "Трюм"; break;
		case CLT_SEC_MODES:		return "Режимы"; break;
		case CLT_SEC_WORLD:		return "Мир"; break;
		case CLT_SEC_INSP:		return "Инспектор"; break;
		case CLT_SEC_FLEETMAP:	return "Особые флоты"; break;
	}
	return "";
}

string CLT_TileSub(int i)
{
	switch (i)
	{
		case CLT_SEC_CHAR:		return "здоровье и репутация: герой и спутники"; break;
		case CLT_SEC_SKILLS:	return "PIRATES, умения, очки: герой и спутники"; break;
		case CLT_SEC_ITEMS:		return "все предметы игры по категориям"; break;
		case CLT_SEC_SHIP:		return "класс, характеристики, команда, орудия, свойства"; break;
		case CLT_SEC_CARGO:		return "товары в трюме: добавить и убрать"; break;
		case CLT_SEC_MODES:		return "флаги штатного Cheat Menu"; break;
		case CLT_SEC_WORLD:		return "нации, где мы, случайные встречи"; break;
		case CLT_SEC_INSP:		return "дерево атрибутов (чтение)"; break;
		case CLT_SEC_FLEETMAP:	return "схема архипелага: квестовые флоты и ваш корабль; Назад — в Мир"; break;
	}
	return "";
}

string CLT_TileIcon(int i)
{
	string s = "hero";
	switch (i)
	{
		case CLT_SEC_SKILLS:	s = "book"; break;
		case CLT_SEC_ITEMS:		s = "chest"; break;
		case CLT_SEC_SHIP:		s = "ship"; break;
		case CLT_SEC_CARGO:		s = "anchor"; break;
		case CLT_SEC_MODES:		s = "gear"; break;
		case CLT_SEC_WORLD:		s = "glass"; break;
		case CLT_SEC_INSP:		s = "compass"; break;
	}
	return CLT_TEX + "i_" + s + ".tga";
}

// кнопки быстрых команд (после переключателей) — для героя и его корабля
string CLT_QuickName(int i)
{
	switch (i)
	{
		case 1: return "PIRATES на максимум"; break;
		case 2: return "Восстановить здоровье и энергию"; break;
		case 3: return "+500 боеприпасов"; break;
		case 4: return "Починить корабль"; break;
	}
	return "";
}

void CLT_DoQuick(int i)
{
	switch (i)
	{
		case 1: CLT_Skills_SpecialMax(pchar); break;
		case 2: CLT_DoHeal(pchar); break;
		case 3: CLT_Ship_Ammo(pchar); break;
		case 4: CLT_Ship_Repair(pchar); break;
	}
}

void CLT_HubStatic()
{
	int i;
	SetFormatedText("CLTH_TITLE", "Caribbean Legend Toolkit");
	CLT_BtnText("CLTH_QDN", "/10");
	CLT_BtnText("CLTH_QUP", "x10");
	SetFormatedText("CLTH_QL", "Количество");
	CLT_BtnText("CLTH_MDN", "-");
	CLT_BtnText("CLTH_MUP", "+");
	CLT_BtnText("CLTH_DDN", "-");
	CLT_BtnText("CLTH_DUP", "+");
	CLT_BtnText("CLTH_CLOSE", "Закрыть");
	CLT_BtnText("CLTH_R1D", "-");
	CLT_BtnText("CLTH_R1U", "+");
	CLT_BtnText("CLTH_R2D", "-");
	CLT_BtnText("CLTH_R2U", "+");
	CLT_BtnText("CLTP_FPREV", "<");
	CLT_BtnText("CLTP_FNEXT", ">");
	for (i = 1; i <= CLT_TILE_POOL; i++)
	{
		if (i > CLT_TILE_COUNT) continue;
		CLT_BtnText("CLTH_TC" + i, "");
		SetNewPicture("CLTH_TI" + i, CLT_TileIcon(i));
		SetFormatedText("CLTH_TL" + i, CLT_TileName(i));
		SetFormatedText("CLTH_TS" + i, CLT_TileSub(i));
	}
	SetFormatedText("CLTH_ST", "Герой");
	SetFormatedText("CLTH_QT", "Быстро");
	for (i = 1; i <= CLT_QUICK_COUNT; i++) CLT_BtnText("CLTH_Q" + i, CLT_QuickName(i));
	SetFormatedText("CLTH_LOGT", "Журнал");
	SetFormatedText("CLTH_HINT", "Insert или Esc — назад и закрыть   |   < > и - + у чисел: щелчок — малый шаг, правая кнопка — крупный   |   песо, дублоны, товары: шаг из поля «Количество» (в «Трюме» — селектор /10 x10 справа)");
	CLT_BtnText("CLTP_BACK", "< К разделам");
	CLT_BtnText("CLTP_CPREV", "<");
	CLT_BtnText("CLTP_CNEXT", ">");
	CLT_BtnText("CLTP_SD", "<");
	CLT_BtnText("CLTP_SU", ">");
	CLT_BtnText("CLTP_TD", "<");
	CLT_BtnText("CLTP_TU", ">");
}

// сводка на карточке героя: только чтение
string CLT_HubInfo()
{
	string s = "";
	string n;
	int i;
	int st = GetCharacterShipType(pchar);
	if (st != SHIP_NOTUSED)
	{
		ref rShip = GetRealShip(st);
		s = s + "Корабль: " + CLT_AttrStr(pchar, "Ship", "Name", "?") + NewStr();
		s = s + "  " + XI_ConvertString(ShipsTypes[sti(rShip.BaseType)].Name) + ", класс " + rShip.Class + NewStr();
		s = s + "Корпус " + CLT_AttrStr(pchar, "Ship", "HP", "?") + " / " + rShip.HP + "   трюм " + GetCargoLoad(pchar) + " / " + GetCargoMaxSpace(pchar) + NewStr();
		s = s + "Команда " + GetCrewQuantity(pchar) + " / " + GetMaxCrewQuantity(pchar) + "   мораль " + GetCharacterCrewMorale(pchar, false) + "   опыт " + int(GetCrewExp(pchar, "")) + NewStr();
	}
	else s = s + "Корабля нет" + NewStr();
	s = s + "Благородство " + CLT_AttrStr(pchar, "reputation", "nobility", "-") + "   известность " + CLT_AttrStr(pchar, "reputation", "fame", "-") + NewStr();
	s = s + "Авторитет " + CLT_AttrStr(pchar, "reputation", "authority", "-") + NewStr();
	string sp = "";
	for (i = 1; i <= 7; i++)
	{
		n = GetSkillNameByTRIdx("SPECIALType", i);
		sp = sp + GetSpecialBase(pchar, n) + " ";
	}
	s = s + "PIRATES: " + sp + NewStr();
	s = s + "Свободные очки умений: " + CLT_AttrStr(pchar, "skill", "FreeSkill", "0");
	return s;
}

void CLT_HubRefresh()
{
	int i;
	string s;

	SetFormatedText("CLTH_SUB", GetDataDay() + "." + GetDataMonth() + "." + GetDataYear() + "   " + int(GetHour()) + ":" + int(GetMinute()) + "    |    " + CLT_AttrStr(pchar, "location", "", ""));
	SetFormatedText("CLTH_MONEY", "Песо: " + CLT_Num(sti(pchar.money)));
	SetFormatedText("CLTH_DUB", "Дублоны: " + CLT_Num(GetCharacterItem(pchar, "gold_dublon")));

	string sFace = "1";
	if (CheckAttribute(pchar, "FaceId")) sFace = pchar.FaceId;
	SetNewPicture("CLTH_FACE", "interfaces\le\portraits\256\face_" + sFace + ".tga");
	SetFormatedText("CLTH_NAME", GetFullName(pchar));
	SetFormatedText("CLTH_RANKT", "Ранг " + CLT_AttrStr(pchar, "rank", "", "?"));
	SetFormatedText("CLTH_R1T", "Здоровье: " + int(LAi_GetCharacterHP(pchar)) + " / " + int(LAi_GetCharacterMaxHP(pchar)));
	SetFormatedText("CLTH_R2T", "Энергия: " + int(Lai_CharacterGetEnergy(pchar)) + " / " + int(LAi_GetCharacterMaxEnergy(pchar)));
	SetFormatedText("CLTH_INFO", CLT_HubInfo());

	for (i = 1; i <= CLT_SEAL_COUNT; i++)
	{
		CLT_BtnText("CLTH_TG" + i, CLT_Hero_Name(i));
		if (CLT_Hero_On(i)) SetNewPicture("CLTH_SEAL" + i, CLT_TEX + "seal_on.tga");
		else SetNewPicture("CLTH_SEAL" + i, CLT_TEX + "seal_off.tga");
	}

	s = "";
	for (i = 0; i < CLT_LOG_LINES; i++)
	{
		if (clt_aLog[i] == "") break;
		s = s + clt_aLog[i] + NewStr();
	}
	SetFormatedText("CLTH_LOG", s);
}

// узлы хаба: показать/спрятать целиком
void CLT_HubNodes(bool bShow)
{
	int i;
	bool bTile;
	SetNodeUsing("CLTH_FADER", bShow);
	SetNodeUsing("CLTH_FRAME", bShow);
	SetNodeUsing("CLTH_GOLD", bShow);
	SetNodeUsing("CLTH_TITLE", bShow);
	SetNodeUsing("CLTH_SUB", bShow);
	SetNodeUsing("CLTH_QDN", bShow);
	SetNodeUsing("CLT_QTY", bShow);
	SetNodeUsing("CLTH_QUP", bShow);
	SetNodeUsing("CLTH_QL", bShow);
	SetNodeUsing("CLTH_MDN", bShow);
	SetNodeUsing("CLTH_MONEY", bShow);
	SetNodeUsing("CLTH_MUP", bShow);
	SetNodeUsing("CLTH_DDN", bShow);
	SetNodeUsing("CLTH_DUB", bShow);
	SetNodeUsing("CLTH_DUP", bShow);
	SetNodeUsing("CLTH_CLOSE", bShow);
	SetNodeUsing("CLTH_LINE", bShow);
	SetNodeUsing("CLTH_CARD", bShow);
	SetNodeUsing("CLTH_FACE", bShow);
	SetNodeUsing("CLTH_FACEFR", bShow);
	SetNodeUsing("CLTH_NAME", bShow);
	SetNodeUsing("CLTH_RANKT", bShow);
	SetNodeUsing("CLTH_R1T", bShow);
	SetNodeUsing("CLTH_R1D", bShow);
	SetNodeUsing("CLTH_R1U", bShow);
	SetNodeUsing("CLTH_R2T", bShow);
	SetNodeUsing("CLTH_R2D", bShow);
	SetNodeUsing("CLTH_R2U", bShow);
	SetNodeUsing("CLTH_INFOLINE", bShow);
	SetNodeUsing("CLTH_INFO", bShow);
	SetNodeUsing("CLTH_ST", bShow);
	for (i = 1; i <= CLT_SEAL_COUNT; i++)
	{
		SetNodeUsing("CLTH_SEAL" + i, bShow);
		SetNodeUsing("CLTH_TG" + i, bShow);
	}
	for (i = 1; i <= CLT_TILE_POOL; i++)
	{
		bTile = bShow && (i <= CLT_TILE_COUNT);
		SetNodeUsing("CLTH_TC" + i, bTile);
		SetNodeUsing("CLTH_TB" + i, bTile);
		SetNodeUsing("CLTH_TI" + i, bTile);
		SetNodeUsing("CLTH_TL" + i, bTile);
		SetNodeUsing("CLTH_TS" + i, bTile);
	}
	SetNodeUsing("CLTH_QT", bShow);
	for (i = 1; i <= CLT_QUICK_COUNT; i++) SetNodeUsing("CLTH_Q" + i, bShow);
	SetNodeUsing("CLTH_LOGBG", bShow);
	SetNodeUsing("CLTH_LOGT", bShow);
	SetNodeUsing("CLTH_LOG", bShow);
	SetNodeUsing("CLTH_HINT", bShow);
}

// ---------------------------------------------------------------------------
// Страница раздела: строки
// ---------------------------------------------------------------------------

string CLT_GNode(string pre, int i)
{
	return pre + (i / CLT_GP_ROWS) + "_" + (i % CLT_GP_ROWS);
}

void CLT_GBegin()
{
	int i;
	for (i = 0; i < CLT_GP_ALL; i++)
	{
		clt_gKind[i] = "";
		clt_gCmd[i] = "";
		clt_gLab[i] = "";
		clt_gVal[i] = "";
		clt_gOn[i] = false;
		clt_gEn[i] = true;
	}
	for (i = 0; i < CLT_GP_COLS; i++)
	{
		clt_gFill[i] = 0;
		SetFormatedText("CLTG_CH" + i, "");
	}
	clt_nGCur = 0;
}

void CLT_GCol(int c, string head)
{
	clt_nGCur = c;
	SetFormatedText("CLTG_CH" + c, head);
}

int CLT_GAdd(string kind, string cmd, string lab, string val, bool on, bool en)
{
	int c = clt_nGCur;
	if (clt_gFill[c] >= CLT_GP_ROWS) return -1;
	int i = c * CLT_GP_ROWS + clt_gFill[c];
	clt_gFill[c] = clt_gFill[c] + 1;
	clt_gKind[i] = kind;
	clt_gCmd[i] = cmd;
	clt_gLab[i] = lab;
	clt_gVal[i] = val;
	clt_gOn[i] = on;
	clt_gEn[i] = en;
	return i;
}

void CLT_GT(string cmd, string lab, bool on, bool en)	{ CLT_GAdd("T", cmd, lab, "", on, en); }
void CLT_GA(string cmd, string lab, bool en)			{ CLT_GAdd("A", cmd, lab, "", false, en); }
void CLT_GS(string cmd, string lab, string val, bool en)	{ CLT_GAdd("S", cmd, lab, val, false, en); }
void CLT_GH(string lab)									{ CLT_GAdd("H", "", lab, "", false, true); }
void CLT_GSkip()										{ CLT_GAdd("", "", "", "", false, true); }

void CLT_GShow()
{
	int i;
	string k;
	for (i = 0; i < CLT_GP_ALL; i++)
	{
		k = clt_gKind[i];
		SetNodeUsing(CLT_GNode("CLTG_S", i), k == "T");
		SetNodeUsing(CLT_GNode("CLTG_B", i), k == "T" || k == "A");
		SetNodeUsing(CLT_GNode("CLTG_L", i), k == "S");
		SetNodeUsing(CLT_GNode("CLTG_V", i), k == "S");
		SetNodeUsing(CLT_GNode("CLTG_D", i), k == "S");
		SetNodeUsing(CLT_GNode("CLTG_U", i), k == "S");
		SetNodeUsing(CLT_GNode("CLTG_H", i), k == "H");
		if (k == "T" || k == "A")
		{
			CLT_BtnText(CLT_GNode("CLTG_B", i), clt_gLab[i]);
			SetSelectable(CLT_GNode("CLTG_B", i), clt_gEn[i]);
			if (k == "T")
			{
				if (clt_gOn[i]) SetNewPicture(CLT_GNode("CLTG_S", i), CLT_TEX + "seal_on.tga");
				else SetNewPicture(CLT_GNode("CLTG_S", i), CLT_TEX + "seal_off.tga");
			}
			continue;
		}
		if (k == "S")
		{
			SetFormatedText(CLT_GNode("CLTG_L", i), clt_gLab[i]);
			SetFormatedText(CLT_GNode("CLTG_V", i), clt_gVal[i]);
			CLT_BtnText(CLT_GNode("CLTG_D", i), "<");
			CLT_BtnText(CLT_GNode("CLTG_U", i), ">");
			SetSelectable(CLT_GNode("CLTG_D", i), clt_gEn[i]);
			SetSelectable(CLT_GNode("CLTG_U", i), clt_gEn[i]);
			continue;
		}
		if (k == "H") SetFormatedText(CLT_GNode("CLTG_H", i), clt_gLab[i]);
	}
}

void CLT_PageNodes(bool bShow)
{
	int i;
	SetNodeUsing("CLTP_BG", bShow);
	SetNodeUsing("CLTP_FRAME", bShow);
	SetNodeUsing("CLTP_GOLD", bShow);
	SetNodeUsing("CLTP_LINE", bShow);
	SetNodeUsing("CLTP_TITLE", bShow);
	SetNodeUsing("CLTP_SUB", bShow);
	SetNodeUsing("CLTP_CPREV", bShow);
	SetNodeUsing("CLTP_CNEXT", bShow);
	SetNodeUsing("CLTP_BACK", bShow);
	SetNodeUsing("CLTG_OUT", bShow);
	SetNodeUsing("CLT_EDIT", bShow && (clt_nSec == CLT_SEC_ITEMS || clt_nSec == CLT_SEC_MODES));
	SetNodeUsing("CLTP_SHIPCARD", bShow && clt_nSec == CLT_SEC_SHIP);
	SetNodeUsing("CLTP_SHIPPIC", bShow && clt_nSec == CLT_SEC_SHIP);
	// модификаторы корабля (специализация и свойство): содержимое задаёт CLT_Ship_Build
	SetNodeUsing("CLTP_STXT", bShow && clt_nSec == CLT_SEC_SHIP);
	SetNodeUsing("CLTP_SD", bShow && clt_nSec == CLT_SEC_SHIP);
	SetNodeUsing("CLTP_SU", bShow && clt_nSec == CLT_SEC_SHIP);
	SetNodeUsing("CLTP_TTXT", bShow && clt_nSec == CLT_SEC_SHIP);
	SetNodeUsing("CLTP_TD", bShow && clt_nSec == CLT_SEC_SHIP);
	SetNodeUsing("CLTP_TU", bShow && clt_nSec == CLT_SEC_SHIP);
	SetNodeUsing("CLTP_SBKG", bShow && clt_nSec == CLT_SEC_SHIP);
	SetNodeUsing("CLTP_SPIC", bShow && clt_nSec == CLT_SEC_SHIP);
	SetNodeUsing("CLTP_SBRD", bShow && clt_nSec == CLT_SEC_SHIP);
	SetNodeUsing("CLTP_TBKG", bShow && clt_nSec == CLT_SEC_SHIP);
	SetNodeUsing("CLTP_TPIC", bShow && clt_nSec == CLT_SEC_SHIP);
	SetNodeUsing("CLTP_TBRD", bShow && clt_nSec == CLT_SEC_SHIP);
	SetNodeUsing("CLTP_MODDESC", bShow && clt_nSec == CLT_SEC_SHIP);
	SetNodeUsing("CLTP_FACECARD", bShow && clt_nSec == CLT_SEC_CHAR);
	SetNodeUsing("CLTP_FACE", bShow && clt_nSec == CLT_SEC_CHAR);
	SetNodeUsing("CLTP_FACEFR", bShow && clt_nSec == CLT_SEC_CHAR);
	SetNodeUsing("CLTP_FPREV", bShow && clt_nSec == CLT_SEC_CHAR);
	SetNodeUsing("CLTP_FNAME", bShow && clt_nSec == CLT_SEC_CHAR);
	SetNodeUsing("CLTP_FNEXT", bShow && clt_nSec == CLT_SEC_CHAR);
	CLT_Cargo_Nodes(bShow && clt_nSec == CLT_SEC_CARGO);
	CLT_Items_Nodes(bShow && clt_nSec == CLT_SEC_ITEMS);
	CLT_Fleet_Nodes(bShow && clt_nSec == CLT_SEC_FLEETMAP);
	for (i = 0; i < CLT_GP_COLS; i++) SetNodeUsing("CLTG_CH" + i, bShow && clt_nSec != CLT_SEC_CARGO && clt_nSec != CLT_SEC_ITEMS && clt_nSec != CLT_SEC_FLEETMAP);
	if (bShow) return;
	for (i = 0; i < CLT_GP_ALL; i++)
	{
		SetNodeUsing(CLT_GNode("CLTG_S", i), false);
		SetNodeUsing(CLT_GNode("CLTG_B", i), false);
		SetNodeUsing(CLT_GNode("CLTG_L", i), false);
		SetNodeUsing(CLT_GNode("CLTG_V", i), false);
		SetNodeUsing(CLT_GNode("CLTG_D", i), false);
		SetNodeUsing(CLT_GNode("CLTG_U", i), false);
		SetNodeUsing(CLT_GNode("CLTG_H", i), false);
	}
}

// перестроить открытую страницу
void CLT_PageRender()
{
	ref chr = CLT_Target();
	CLT_GBegin();
	switch (clt_nSec)
	{
		case CLT_SEC_CHAR:		CLT_Char_Build(); break;
		case CLT_SEC_SKILLS:	CLT_Skills_Build(); break;
		case CLT_SEC_ITEMS:		CLT_Items_Refresh(); break;
		case CLT_SEC_SHIP:		CLT_Ship_Build(); break;
		case CLT_SEC_CARGO:		CLT_Cargo_Refresh(); break;
		case CLT_SEC_MODES:		CLT_Misc_Build(); break;
		case CLT_SEC_WORLD:		CLT_World_Build(); break;
		case CLT_SEC_INSP:		CLT_Insp_Build(); break;
		case CLT_SEC_FLEETMAP:	CLT_Fleet_MapRefresh(); break;
	}
	CLT_GShow();
	if (clt_nSec == CLT_SEC_FLEETMAP) CLT_BtnText("CLTP_BACK", "< В раздел Мир");
	else CLT_BtnText("CLTP_BACK", "< К разделам");
	SetFormatedText("CLTP_TITLE", CLT_TileName(clt_nSec) + ":  " + GetFullName(chr));
	SetFormatedText("CLTP_SUB", CLT_TileSub(clt_nSec) + "    |    " + CLT_TargetRole(CLT_TargetIndex()) + " " + (clt_nTargetPos + 1) + " / " + clt_nTargets);
	SetSelectable("CLTP_CPREV", clt_nTargets > 1);
	SetSelectable("CLTP_CNEXT", clt_nTargets > 1);
	SetFormatedText("CLTG_OUT", clt_sOut);
}

// 0 — хаб, иначе раздел
void CLT_ShowSec(int sec)
{
	clt_nSec = sec;
	clt_sOut = "";
	if (sec == 0)
	{
		CLT_PageNodes(false);
		CLT_HubNodes(true);
		CLT_HubRefresh();
		clt_nHover = 0;
		return;
	}
	if (clt_nHover > 0)
	{
		SetNewPicture("CLTH_TB" + clt_nHover, CLT_TEX + "tile.tga");
		clt_nHover = 0;
	}
	CLT_HubNodes(false);
	CLT_PageNodes(true);
	if (sec == CLT_SEC_ITEMS) GameInterface.clt_edit.str = clt_sItemFilter;
	if (sec == CLT_SEC_MODES) GameInterface.clt_edit.str = clt_sAchFilter;
	CLT_PageRender();
}

void CLT_Refresh()
{
	if (clt_nSec == 0) CLT_HubRefresh();
	else CLT_PageRender();
}

// ---------------------------------------------------------------------------
// События
// ---------------------------------------------------------------------------

void CLT_SecDo(string cmd, int dir, bool big)
{
	trace("[CLT] sec " + clt_nSec + " cmd " + cmd + " dir " + dir);
	switch (clt_nSec)
	{
		case CLT_SEC_CHAR:		CLT_Char_Do(cmd, dir, big); break;
		case CLT_SEC_SKILLS:	CLT_Skills_Do(cmd, dir, big); break;
		case CLT_SEC_SHIP:		CLT_Ship_Do(cmd, dir, big); break;
		case CLT_SEC_MODES:		CLT_Misc_Do(cmd, dir, big); break;
		case CLT_SEC_WORLD:		CLT_World_Do(cmd, dir, big); break;
		case CLT_SEC_INSP:		CLT_Insp_Do(cmd, dir, big); break;
	}
}

void CLT_Window_OnCommand()
{
	string comName = GetEventData();
	string nodName = GetEventData();
	if (comName != "activate" && comName != "click" && comName != "rclick") return;
	bool big = (comName == "rclick");
	int i, p, c, r;
	string pre, rest;

	if (clt_nSec != 0)
	{
		if (nodName == "CLTP_BACK")
		{
			if (clt_nSec == CLT_SEC_FLEETMAP) CLT_ShowSec(CLT_SEC_WORLD);
			else CLT_ShowSec(0);
			return;
		}
		if (nodName == "CLTP_FPREV") nodName = "CLTP_CPREV";
		if (nodName == "CLTP_FNEXT") nodName = "CLTP_CNEXT";
		if (nodName == "CLTP_CPREV") { CLT_NextTarget(-1); CLT_SecTargetChanged(); CLT_PageRender(); return; }
		if (nodName == "CLTP_CNEXT") { CLT_NextTarget(1); CLT_SecTargetChanged(); CLT_PageRender(); return; }
		// селекторы модификаторов корабля (узлы вне сетки, раздел «Корабль»)
		if (nodName == "CLTP_SD") { CLT_SecDo("spec", -1, big); CLT_PageRender(); return; }
		if (nodName == "CLTP_SU") { CLT_SecDo("spec", 1, big); CLT_PageRender(); return; }
		if (nodName == "CLTP_TD") { CLT_SecDo("trait", -1, big); CLT_PageRender(); return; }
		if (nodName == "CLTP_TU") { CLT_SecDo("trait", 1, big); CLT_PageRender(); return; }
		if (findsubstr(nodName, "CLTC_", 0) == 0) { CLT_Cargo_OnCommand(nodName, big); CLT_PageRender(); return; }
		if (findsubstr(nodName, "CLTI_", 0) == 0) { CLT_Items_OnCommand(nodName, big); CLT_PageRender(); return; }
		if (findsubstr(nodName, "CLTG_", 0) != 0) return;
		pre = strcut(nodName, 0, 5);					// CLTG_X
		rest = strcut(nodName, 6, strlen(nodName) - 1);	// c_r
		p = findsubstr(rest, "_", 0);
		if (p <= 0) return;
		c = sti(strcut(rest, 0, p - 1));
		r = sti(strcut(rest, p + 1, strlen(rest) - 1));
		i = c * CLT_GP_ROWS + r;
		if (i < 0 || i >= CLT_GP_ALL) return;
		if (clt_gCmd[i] == "" || !clt_gEn[i]) return;
		if (pre == "CLTG_B") CLT_SecDo(clt_gCmd[i], 0, big);
		if (pre == "CLTG_D") CLT_SecDo(clt_gCmd[i], -1, big);
		if (pre == "CLTG_U") CLT_SecDo(clt_gCmd[i], 1, big);
		if (clt_nSec != 0) CLT_PageRender();
		return;
	}

	if (findsubstr(nodName, "CLTH_", 0) != 0) return;
	int step = clt_nQty;
	if (big) step = step * 10;

	if (nodName == "CLTH_CLOSE") { CLT_Window_Close(); return; }
	if (nodName == "CLTH_QDN" || nodName == "CLTH_QUP")
	{
		if (nodName == "CLTH_QUP") CLT_SetQty(clt_nQty * 10);
		else CLT_SetQty(clt_nQty / 10);
		return;
	}
	if (nodName == "CLTH_MUP") CLT_DoMoney(step);
	if (nodName == "CLTH_MDN") CLT_DoMoney(0 - step);
	if (nodName == "CLTH_DUP") CLT_DoDublons(step);
	if (nodName == "CLTH_DDN") CLT_DoDublons(0 - step);
	if (nodName == "CLTH_R1D") CLT_DoMaxHP(pchar, CLT_Step(-1, big, 100, 1000));
	if (nodName == "CLTH_R1U") CLT_DoMaxHP(pchar, CLT_Step(1, big, 100, 1000));
	if (nodName == "CLTH_R2D") CLT_DoMaxEnergy(pchar, CLT_Step(-1, big, 10, 50));
	if (nodName == "CLTH_R2U") CLT_DoMaxEnergy(pchar, CLT_Step(1, big, 10, 50));
	if (findsubstr(nodName, "CLTH_TG", 0) == 0) CLT_Hero_Toggle(sti(strcut(nodName, 7, strlen(nodName) - 1)));
	if (findsubstr(nodName, "CLTH_Q", 0) == 0 && strlen(nodName) == 7) CLT_DoQuick(sti(strcut(nodName, 6, 6)));
	if (findsubstr(nodName, "CLTH_TC", 0) == 0)
	{
		i = sti(strcut(nodName, 7, strlen(nodName) - 1));
		if (i >= 1 && i <= CLT_TILE_COUNT) { CLT_ShowSec(i); return; }
	}
	CLT_HubRefresh();
}

// смена выбранного персонажа на странице
void CLT_SecTargetChanged()
{
	clt_sOut = "";
	if (clt_nSec == CLT_SEC_INSP) CLT_Insp_Reset();
}

void CLT_SetQty(int q)
{
	if (q < 1) q = 1;
	if (q > 10000000) q = 10000000;
	clt_nQty = q;
	GameInterface.clt_qty.str = "" + q;
	CLT_HubRefresh();
}

// Enter в поле «Количество»
void CLT_Window_OnQtyConfirm()
{
	CLT_SetQty(sti(GameInterface.clt_qty.str));
}

// Enter в поле фильтра (CLT_EDIT, command = activate,event:CLT_EditConfirm)
void CLT_Window_OnEditConfirm()
{
	string s = GameInterface.clt_edit.str;
	trace("[CLT] edit confirm: '" + s + "'");
	if (clt_nSec == CLT_SEC_ITEMS)
	{
		CLT_Items_OnEdit(s);
		CLT_PageRender();
	}
	if (clt_nSec == CLT_SEC_MODES)
	{
		CLT_Misc_OnEdit(s);
		CLT_PageRender();
	}
}

// подсветка плитки под мышью (как Nav_HubFrame «Навигатора»)
void CLT_Window_OnFrame()
{
	if (clt_bClosing) return;
	int iHover = 0;
	int i, c, r, x, y;
	if (clt_nSec == 0 && CheckAttribute(&GameInterface, "mousepos.x"))
	{
		float mx = stf(GameInterface.mousepos.x);
		float my = stf(GameInterface.mousepos.y);
		for (i = 1; i <= CLT_TILE_COUNT; i++)
		{
			c = (i - 1) % 4;
			r = (i - 1) / 4;
			x = 530 + c * 232;
			y = 156 + r * 286;
			if (mx >= x && mx <= x + 212 && my >= y && my <= y + 266) iHover = i;
		}
	}
	if (iHover != clt_nHover)
	{
		if (clt_nHover > 0)
		{
			SetNewPicture("CLTH_TB" + clt_nHover, CLT_TEX + "tile.tga");
			SendMessage(&GameInterface, "lsll", MSG_INTERFACE_MSG_TO_NODE, "CLTH_TI" + clt_nHover, 4, argb(255, 128, 128, 128));
		}
		if (iHover > 0) SetNewPicture("CLTH_TB" + iHover, CLT_TEX + "tile_h.tga");
		clt_nHover = iHover;
		clt_nTick = 0;
	}
	if (clt_nHover > 0)
	{
		clt_nTick++;
		float t = sin(clt_nTick * 0.12);
		int v = 150 + int(30.0 * t);
		SendMessage(&GameInterface, "lsll", MSG_INTERFACE_MSG_TO_NODE, "CLTH_TI" + clt_nHover, 4, argb(255, v, v, v));
	}
}

// ---------------------------------------------------------------------------
// Закрытие
// ---------------------------------------------------------------------------

void CLT_Window_OnBreak()
{
	CLT_Window_Close();
}

// Esc: со страницы — на хаб, с хаба — закрыть
void CLT_Window_OnCancel()
{
	if (clt_nSec == CLT_SEC_FLEETMAP)
	{
		CLT_ShowSec(CLT_SEC_WORLD);
		return;
	}
	if (clt_nSec != 0)
	{
		CLT_ShowSec(0);
		return;
	}
	CLT_Window_Close();
}

// Повтор IDoExit из blackmark_cheat.c, затем общий хвост в mod_inits\cltoolkit.c
void CLT_Window_Close()
{
	if (clt_bClosing) return;
	clt_bClosing = true;

	bGameMenuStart = false;
	EndAboveForm(true);

	DelEventHandler("InterfaceBreak", "CLT_Window_OnBreak");
	DelEventHandler("exitCancel", "CLT_Window_OnCancel");
	DelEventHandler("ievnt_command", "CLT_Window_OnCommand");
	DelEventHandler("CLT_EditConfirm", "CLT_Window_OnEditConfirm");
	DelEventHandler("CLT_QtyConfirm", "CLT_Window_OnQtyConfirm");
	DelEventHandler("frame", "CLT_Window_OnFrame");

	if (bSeaActive) RefreshBattleInterface();

	CLT_FinishClose();
}
