// CL Toolkit — раздел «Трекер» (заменил «Инспектор»): таймеры квестов, триггеры мест и запреты, особые флоты.
// Своя функция по просьбе пользователя (в «Навигаторе» 1.9 такой нет). Только чтение, ничего не меняет.
// Откуда данные:
//   pchar.quest.<имя> — все квестовые прерывания игры (quests\quests.c: SetTimerCondition, SetFunctionTimerCondition,
//     SetFunctionLocationCondition и т. п.; проверка — quests\quests_check.c QuestsCheck/ProcessCondition):
//     win_condition.lN / fail_condition.lN, тип условия — значение lN; over = "yes" — уже сработало
//     (удаляется при следующей проверке), again — повторяющееся (служебное, по умолчанию скрыто).
//   Таймер — lN = "Timer", lN.date.year/month/day[/hour] (ProcessCondition case "Timer").
//     Так же ставится и золотой флот: GenerateGoldFleet (старт, Rumour_generate.c) и EndOfGoldFleet (GoldFleet.c).
//   Места — типы location, locator, ExitFromLocation, Location_Type, MapEnter, EnterToSea и др. (ProcessCondition).
//   Название квеста — заголовок журнала: ключ = часть имени функции до "_" в texts\<язык>\QuestBook\*.txt
//     (Patria_DiplomatTimeOver -> Patria -> «На службе Отечеству»); не нашлось — показываем имя функции.
//   Закрытые двери города — Locations[].reload.lN.disable (LocatorReloadEnterDisable, QuestsUtilite.c), только двери
//     в таверну/магазин/церковь/верфь/резиденцию/портовое управление/банк и выходы из города (они не закрыты в начале игры);
//   высадка на остров — Islands[].reload_enable (Island_SetReloadEnableGlobal, islands.c), без островов, закрытых с начала игры;
//   глобальные запреты — chrDisableReloadToLocation, bDisableFastReload, bDisableMapEnter.
//   Запреты выводятся сводкой (до 3 строк: флаги / острова / двери), полный список — по щелчку.
// Особые флоты — clt_sec_fleet.c (перенесены сюда из раздела «Мир»).

#define CLT_TRK_MAX			48
#define CLT_TRK_TROWS		12	// строк таймеров на странице
#define CLT_TRK_PROWS		9	// строк триггеров мест на странице
#define CLT_TRK_BMAX		3	// строк сводки запретов
#define CLT_TRK_LNG			6

string clt_tkTLine[CLT_TRK_MAX];
string clt_tkTInfo[CLT_TRK_MAX];
int clt_tkTKey[CLT_TRK_MAX];
int clt_nTkT;
string clt_tkPLine[CLT_TRK_MAX];
string clt_tkPInfo[CLT_TRK_MAX];
int clt_nTkP;
string clt_tkBLine[CLT_TRK_BMAX];
string clt_tkBInfo[CLT_TRK_BMAX];
int clt_nTkB;
int clt_nTkTPage;
int clt_nTkPPage;
bool clt_bTkAll;				// показывать повторяющиеся (служебные) прерывания
int clt_aTkLng[CLT_TRK_LNG];
bool clt_bTkLngOpen;
object clt_oTkBook;			// кэш заголовков журнала: префикс -> название ("" — не найдено)

void CLT_Trk_Reset()
{
	clt_nTkTPage = 0;
	clt_nTkPPage = 0;
	clt_bTkLngOpen = false;
}

// ---------------------------------------------------------------------------
// Строки и названия
// ---------------------------------------------------------------------------

string CLT_Trk_A(aref c, string a)
{
	if (CheckAttribute(c, a)) return c.(a);
	return "";
}

bool CLT_Trk_Has(string s, string a, string b)
{
	if (findsubstr(s, a, 0) != -1) return true;
	return findsubstr(s, b, 0) != -1;
}

string CLT_Trk_D2(int n)
{
	if (n < 10) return "0" + n;
	return "" + n;
}

// часть имени до первого "_" (Saga_JackmanSink -> Saga)
string CLT_Trk_Prefix(string s)
{
	int p = findsubstr(s, "_", 0);
	if (p > 0) return strcut(s, 0, p - 1);
	return s;
}

void CLT_Trk_LngOpen()
{
	clt_aTkLng[0] = LanguageOpenFile("QuestBook\QuestBook_Part1.txt");
	clt_aTkLng[1] = LanguageOpenFile("QuestBook\QuestBook_Part2.txt");
	clt_aTkLng[2] = LanguageOpenFile("QuestBook\QuestBook_Part3.txt");
	clt_aTkLng[3] = LanguageOpenFile("QuestBook\QuestBook_New.txt");
	clt_aTkLng[4] = LanguageOpenFile("QuestBook\ClockTower.txt");
	clt_aTkLng[5] = LanguageOpenFile("QuestBook\SharlieEpilog.txt");
	clt_bTkLngOpen = true;
}

void CLT_Trk_LngClose()
{
	int i;
	if (!clt_bTkLngOpen) return;
	for (i = 0; i < CLT_TRK_LNG; i++)
	{
		if (clt_aTkLng[i] >= 0) LanguageCloseFile(clt_aTkLng[i]);
	}
	clt_bTkLngOpen = false;
}

// заголовок квеста в журнале по ключу (с кэшем)
string CLT_Trk_Book(string key)
{
	int i;
	string t = "";
	if (key == "") return "";
	if (CheckAttribute(&clt_oTkBook, key)) return clt_oTkBook.(key);
	if (!clt_bTkLngOpen) CLT_Trk_LngOpen();
	for (i = 0; i < CLT_TRK_LNG; i++)
	{
		if (clt_aTkLng[i] < 0) continue;
		t = LanguageConvertString(clt_aTkLng[i], key);
		if (t == " ") t = "";
		if (t != "") break;
	}
	clt_oTkBook.(key) = t;
	return t;
}

// известные системные события (их заголовка в журнале нет)
string CLT_Trk_Known(string fn)
{
	if (findsubstr(fn, "GoldFleet", 0) != -1) return "Золотой флот";
	if (findsubstr(fn, "SeaHunterCheck", 0) != -1) return "Охотники за головами";
	if (findsubstr(fn, "Siege", 0) != -1) return "Осада колонии";
	if (findsubstr(fn, "CharleePrince", 0) != -1) return "Чарли Принц";
	if (findsubstr(fn, "Helen", 0) == 0) return "Элен";
	if (findsubstr(fn, "Mary", 0) == 0) return "Мэри";
	return "";
}

string CLT_Trk_Title(string fn, string qn)
{
	string t = CLT_Trk_Known(fn);
	if (t != "") return t;
	t = CLT_Trk_Book(CLT_Trk_Prefix(fn));
	if (t != "") return t;
	t = CLT_Trk_Book(CLT_Trk_Prefix(qn));
	if (t != "") return t;
	return fn;
}

// что делает событие — по имени функции (догадка; точное имя — в подробностях)
string CLT_Trk_Kind(string fn, bool bFail)
{
	if (bFail) return "провал";
	if (CLT_Trk_Has(fn, "Fail", "fail") || CLT_Trk_Has(fn, "TimeOver", "Timeover") || CLT_Trk_Has(fn, "Late", "TimeOut")) return "срок / провал";
	if (CLT_Trk_Has(fn, "Lost", "Timeout")) return "срок / провал";
	if (CLT_Trk_Has(fn, "Build", "build")) return "стройка готова";	// IslaMona_BuildDone/Harbour/Tavern/Plantation (IslaMona_NPC.c)
	if (CLT_Trk_Has(fn, "Scuadron", "Squadron")) return "прибытие эскадры";	// Terrapin_SetCromvelScuadron (SharlieFinal.c)
	if (CLT_Trk_Has(fn, "End", "Clear") || CLT_Trk_Has(fn, "Del", "Remove")) return "завершение";
	if (CLT_Trk_Has(fn, "Start", "Next") || CLT_Trk_Has(fn, "Begin", "Init")) return "старт";
	if (CLT_Trk_Has(fn, "Create", "Generate")) return "старт";
	if (CLT_Trk_Has(fn, "Check", "check")) return "проверка";
	return "событие";
}

// название локации: LocLables по id, иначе по метке типа + колония; морская локация — остров
string CLT_Trk_LocName(string id)
{
	string s, lb;
	int li, ii;
	if (id == "") return "?";
	li = FindLocation(id);
	s = GetConvertStr(id, "LocLables.txt");
	if (s == " ") s = "";
	if (li < 0)
	{
		ii = FindIsland(id);
		if (ii >= 0)
		{
			if (s == "" && CheckAttribute(&Islands[ii], "name")) s = GetConvertStr(Islands[ii].name, "LocLables.txt");
			if (s == "" || s == " ") s = id;
			return "море у о. " + s;
		}
		if (s == "") return id;
		return s;
	}
	if (s == "" && CheckAttribute(&Locations[li], "id.label"))
	{
		lb = GetConvertStr(Locations[li].id.label, "LocLables.txt");
		if (lb != " ") s = lb;
	}
	if (s == "") s = id;
	if (CheckAttribute(&Locations[li], "fastreload") && CLT_AttrStr(&Locations[li], "type", "", "") != "town")
	{
		lb = XI_ConvertString("Colony" + Locations[li].fastreload);
		if (lb != "" && lb != s) s = s + " (" + lb + ")";
	}
	return s;
}

string CLT_Trk_LocType(string t)
{
	switch (t)
	{
		case "tavern":		return "любая таверна"; break;
		case "town":		return "любой город"; break;
		case "shop":		return "любой магазин"; break;
		case "church":		return "любая церковь"; break;
		case "shipyard":	return "любая верфь"; break;
		case "residence":	return "любая резиденция"; break;
		case "seashore":	return "любая бухта"; break;
		case "jungle":		return "любые джунгли"; break;
		case "cave":		return "любая пещера"; break;
		case "house":		return "любой дом"; break;
	}
	return "локация типа " + t;
}

string CLT_Trk_Nation(aref c)
{
	if (!CheckAttribute(c, "nation")) return "?";
	return XI_ConvertString(GetNationNameByType(sti(c.nation)));
}

// условие-место: текст или "" (не место)
string CLT_Trk_Place(aref c)
{
	string t = GetAttributeValue(c);
	// сначала само место, действие — в скобках (вход — без пометки)
	switch (t)
	{
		case "location":			return CLT_Trk_LocName(CLT_Trk_A(c, "location")); break;
		case "locator":				return CLT_Trk_LocName(CLT_Trk_A(c, "location")) + " (точка)"; break;
		case "ExitFromLocation":	return CLT_Trk_LocName(CLT_Trk_A(c, "location")) + " (выход)"; break;
		case "Ship_location":		return CLT_Trk_LocName(CLT_Trk_A(c, "location")) + " (высадка)"; break;
		case "Location_Type":		return CLT_Trk_LocType(CLT_Trk_A(c, "location_type")); break;
		case "location_port":		return "любой порт"; break;
		case "location_seashore":	return "любая бухта"; break;
		case "Location_Coast":		return "любая бухта или порт"; break;
		case "MapEnter":			return "выход на глобальную карту"; break;
		case "EnterToSea":			return "выход в море"; break;
		case "ExitFromSea":			return "уход из моря"; break;
		case "ComeToIsland":		return "подход к острову"; break;
		case "Coordinates":			return "точка в море (координаты)"; break;
		case "My_Cabine":			return "своя каюта"; break;
		case "nation_location":		return "город нации: " + CLT_Trk_Nation(c); break;
		case "Nation_City":			return "город нации: " + CLT_Trk_Nation(c); break;
	}
	return "";
}

// ---------------------------------------------------------------------------
// Сбор
// ---------------------------------------------------------------------------

void CLT_Trk_AddT(int key, string line, string info)
{
	if (clt_nTkT >= CLT_TRK_MAX) return;
	clt_tkTKey[clt_nTkT] = key;
	clt_tkTLine[clt_nTkT] = line;
	clt_tkTInfo[clt_nTkT] = info;
	clt_nTkT++;
}

void CLT_Trk_AddP(string line, string info)
{
	if (clt_nTkP >= CLT_TRK_MAX) return;
	clt_tkPLine[clt_nTkP] = line;
	clt_tkPInfo[clt_nTkP] = info;
	clt_nTkP++;
}

// одно прерывание: conds — win_condition или fail_condition
void CLT_Trk_Scan(aref q, aref conds, bool bFail)
{
	aref c, ct;
	int m, nc, y, mo, d, h, key, left;
	bool bTimer = false;
	string place = "";
	string s, fn, qn, title, kind, date, info;

	nc = GetAttributesNum(conds);
	for (m = 0; m < nc; m++)
	{
		c = GetAttributeN(conds, m);
		if (GetAttributeValue(c) == "Timer")
		{
			if (!bTimer && CheckAttribute(c, "date.year")) { bTimer = true; ct = c; }
			continue;
		}
		s = CLT_Trk_Place(c);
		if (s == "") continue;
		if (place != "") place = place + " + ";
		place = place + s;
	}
	if (!bTimer && place == "") return;

	qn = GetAttributeName(q);
	fn = CLT_Trk_A(q, "function");
	if (fn == "") fn = GetAttributeValue(conds);
	if (fn == "" || fn == "no") fn = qn;
	title = CLT_Trk_Title(fn, qn);
	kind = CLT_Trk_Kind(fn, bFail);
	info = "квест: " + title + "  |  прерывание pchar.quest." + qn + "  |  функция " + fn;
	if (CheckAttribute(q, "again")) info = info + "  |  повторяется";
	if (bFail) info = info + "  |  условие провала";

	date = "";
	if (bTimer)
	{
		y = sti(ct.date.year);
		mo = sti(ct.date.month);
		d = sti(ct.date.day);
		h = 0;
		if (CheckAttribute(ct, "date.hour")) h = sti(ct.date.hour);
		date = CLT_Trk_D2(d) + "." + CLT_Trk_D2(mo) + "." + y;
		if (CheckAttribute(ct, "date.hour")) date = date + " " + h + "ч";
		key = ((y * 12 + mo) * 31 + d) * 24 + h;
		left = GetPastTime("day", GetDataYear(), GetDataMonth(), GetDataDay(), GetTime(), y, mo, d, float(h));
		s = date;
		if (left > 0) s = s + " (" + left + " дн.)";
		if (left == 0) s = s + " (сегодня)";
		if (left < 0) s = s + " (просрочен)";
		s = s + "  " + CLT_Fleet_Short(title, 34) + " — " + kind;
		if (place != "") s = s + " *";
		if (place != "") CLT_Trk_AddT(key, s, "срок " + date + "  |  " + info + "  |  и условие: " + place);
		else CLT_Trk_AddT(key, s, "срок " + date + "  |  " + info);
	}
	if (place == "") return;
	s = CLT_Fleet_Short(place, 46);
	if (bTimer) s = s + " (с " + CLT_Trk_D2(d) + "." + CLT_Trk_D2(mo) + ")";
	info = "место: " + place + "  |  " + kind + "  |  " + info;
	if (bTimer) info = info + "  |  не раньше " + date;
	CLT_Trk_AddP(s, info);
}

// сортировка таймеров по дате (вставками, их немного)
void CLT_Trk_SortT()
{
	int i, j, k;
	string a, b;
	for (i = 1; i < clt_nTkT; i++)
	{
		k = clt_tkTKey[i];
		a = clt_tkTLine[i];
		b = clt_tkTInfo[i];
		j = i - 1;
		while (j >= 0)
		{
			if (clt_tkTKey[j] <= k) break;	// без «j >= 0 && ...»: короткое замыкание в Storm не гарантировано
			clt_tkTKey[j + 1] = clt_tkTKey[j];
			clt_tkTLine[j + 1] = clt_tkTLine[j];
			clt_tkTInfo[j + 1] = clt_tkTInfo[j];
			j--;
		}
		clt_tkTKey[j + 1] = k;
		clt_tkTLine[j + 1] = a;
		clt_tkTInfo[j + 1] = b;
	}
}

// острова, закрытые для высадки с начала игры (islands_init.c, migrations 0040/0041/0045) — не показываем;
// Исла-Мона и Коче открывает сюжет, пока закрыты — это не квестовый запрет
bool CLT_Trk_IslStatic(string id)
{
	switch (id)
	{
		case "LostShipsCity": return true; break;
		case "KhaelRoa": return true; break;
		case "Ksochitam": return true; break;
		case "RockIsland": return true; break;
		case "Providence": return true; break;
		case "SantaQuiteria": return true; break;
		case "IslaDeVieques": return true; break;
		case "IslaMona": return true; break;
		case "IslaDeCoche": return true; break;
		case "Panama": return true; break;
		case "Tenotchitlan": return true; break;
		case "MarieGalante": return true; break;
		case "Tobago": return true; break;
		case "Pearl": return true; break;
		case "LosTeques": return true; break;
	}
	return false;
}

// дверь города, которую игра в начале не закрывает: что за ней ("" — не следим)
string CLT_Trk_DoorKind(string go)
{
	if (findsubstr(go, "_tavern", 0) != -1) return "таверна";
	if (findsubstr(go, "_store", 0) != -1) return "магазин";
	if (findsubstr(go, "_church", 0) != -1) return "церковь";
	if (findsubstr(go, "_shipyard", 0) != -1) return "верфь";
	if (findsubstr(go, "_townhall", 0) != -1) return "резиденция";
	if (findsubstr(go, "_PortOffice", 0) != -1) return "портовое управление";
	if (findsubstr(go, "_bank", 0) != -1) return "ростовщик";
	if (findsubstr(go, "_ExitTown", 0) != -1) return "выход из города";
	if (FindLocation(go) < 0 && FindIsland(go) >= 0) return "выход в море";
	return "";
}

// сводка запретов (не больше CLT_TRK_BMAX строк): общие флаги, острова, двери городов
void CLT_Trk_AddB(string line, string info)
{
	if (clt_nTkB >= CLT_TRK_BMAX) return;
	clt_tkBLine[clt_nTkB] = line;
	clt_tkBInfo[clt_nTkB] = info;
	clt_nTkB++;
}

string CLT_Trk_Join(string a, string b, string sep)
{
	if (a == "") return b;
	return a + sep + b;
}

void CLT_Trk_Blocks()
{
	int i, j, n, nAll, nOff, nIsl, nIslOff, nTowns, nDoors;
	aref arR, arD;
	string go, k, town, offK, onK, ph, s, inf, islOff, islOn, doors, doorsInf;

	s = "";
	inf = "";
	if (chrDisableReloadToLocation) { s = CLT_Trk_Join(s, "выход из локации", ", "); inf = CLT_Trk_Join(inf, "chrDisableReloadToLocation — квест закрыл выходы из текущей локации", "; "); }
	if (bDisableFastReload) { s = CLT_Trk_Join(s, "быстрый переход", ", "); inf = CLT_Trk_Join(inf, "bDisableFastReload — быстрый переход по городу отключён", "; "); }
	if (bDisableMapEnter) { s = CLT_Trk_Join(s, "выход на карту", ", "); inf = CLT_Trk_Join(inf, "bDisableMapEnter — выход на глобальную карту запрещён (бой рядом или квест)", "; "); }
	if (s != "") CLT_Trk_AddB(CLT_Fleet_Short("Нельзя: " + s, 52), inf);

	// острова: закрытые с начала игры не считаем
	nIsl = 0;
	nIslOff = 0;
	islOff = "";
	islOn = "";
	for (i = 0; i < MAX_ISLANDS; i++)
	{
		if (!CheckAttribute(&Islands[i], "id") || !CheckAttribute(&Islands[i], "reload_enable")) continue;
		if (CLT_Trk_IslStatic(Islands[i].id)) continue;
		nIsl++;
		if (bool(Islands[i].reload_enable)) { islOn = CLT_Trk_Join(islOn, CLT_Trk_LocName(Islands[i].id), ", "); continue; }
		nIslOff++;
		islOff = CLT_Trk_Join(islOff, CLT_Trk_LocName(Islands[i].id), ", ");
	}
	if (nIslOff > 0)
	{
		if (nIslOff <= 2) ph = islOff;
		else
		{
			if (nIsl - nIslOff == 0) ph = "все острова";
			else
			{
				if (nIsl - nIslOff == 1) ph = "все, кроме " + islOn;
				else ph = nIslOff + " из " + nIsl + " островов";
			}
		}
		CLT_Trk_AddB(CLT_Fleet_Short("Высадка закрыта: " + ph, 52), "Квест закрыл высадку (Island_SetReloadEnableGlobal): " + islOff);
	}

	// двери городов: по городу одна фраза, всё вместе — одна строка
	nTowns = 0;
	nDoors = 0;
	doors = "";
	doorsInf = "";
	for (i = 0; i < MAX_LOCATIONS; i++)
	{
		if (!CheckAttribute(&Locations[i], "type") || !CheckAttribute(&Locations[i], "reload")) continue;
		if (Locations[i].type != "town") continue;
		makearef(arR, Locations[i].reload);
		n = GetAttributesNum(arR);
		nAll = 0;
		nOff = 0;
		offK = "";
		onK = "";
		for (j = 0; j < n; j++)
		{
			arD = GetAttributeN(arR, j);
			if (!CheckAttribute(arD, "go")) continue;
			go = arD.go;
			k = CLT_Trk_DoorKind(go);
			if (k == "") continue;
			nAll++;
			if (CheckAttribute(arD, "disable") && sti(arD.disable) != 0) { nOff++; offK = CLT_Trk_Join(offK, k, ", "); }
			else onK = CLT_Trk_Join(onK, k, ", ");
		}
		if (nOff == 0) continue;
		town = CLT_Trk_LocName(Locations[i].id);
		if (nOff == nAll) ph = "закрыто всё";
		else
		{
			if (nAll - nOff == 1 && nOff >= 3) ph = "открыто только: " + onK;
			else
			{
				if (nOff <= 2) ph = offK;
				else ph = "закрыто " + nOff + " из " + nAll;
			}
		}
		nTowns++;
		nDoors = nDoors + nOff;
		doors = town + ": " + ph;
		doorsInf = CLT_Trk_Join(doorsInf, town + " — " + offK, "; ");
	}
	if (nTowns == 1) CLT_Trk_AddB(CLT_Fleet_Short("Двери: " + doors, 52), "Закрыто квестом (LocatorReloadEnterDisable): " + doorsInf);
	if (nTowns > 1) CLT_Trk_AddB("Двери: закрыто " + nDoors + " в " + nTowns + " городах", "Закрыто квестом (LocatorReloadEnterDisable): " + doorsInf);
}

void CLT_Trk_Collect()
{
	aref quests, q, conds;
	int i, n;

	clt_nTkT = 0;
	clt_nTkP = 0;
	clt_nTkB = 0;
	makearef(quests, pchar.quest);
	n = GetAttributesNum(quests);
	for (i = 0; i < n; i++)
	{
		q = GetAttributeN(quests, i);
		if (CheckAttribute(q, "over") && q.over == "yes") continue;
		if (CheckAttribute(q, "again") && !clt_bTkAll && CLT_Trk_Known(CLT_Trk_A(q, "function")) == "") continue;
		if (CheckAttribute(q, "win_condition"))
		{
			makearef(conds, q.win_condition);
			CLT_Trk_Scan(q, conds, false);
		}
		if (CheckAttribute(q, "fail_condition"))
		{
			makearef(conds, q.fail_condition);
			CLT_Trk_Scan(q, conds, true);
		}
	}
	CLT_Trk_LngClose();
	CLT_Trk_SortT();
	CLT_Trk_Blocks();
}

// ---------------------------------------------------------------------------
// Страница
// ---------------------------------------------------------------------------

int CLT_Trk_Pages(int n, int rows)
{
	if (n <= 0) return 1;
	return (n + rows - 1) / rows;
}

void CLT_Trk_Build()
{
	int i, first, pages;

	CLT_Trk_Collect();

	CLT_GCol(0, "Таймеры квестов");
	CLT_GT("all", "Показывать повторяющиеся (служебные)", clt_bTkAll, true);
	pages = CLT_Trk_Pages(clt_nTkT, CLT_TRK_TROWS);
	if (clt_nTkTPage >= pages) clt_nTkTPage = pages - 1;
	if (clt_nTkTPage < 0) clt_nTkTPage = 0;
	if (clt_nTkT == 0) CLT_GH("Активных таймеров нет.");
	first = clt_nTkTPage * CLT_TRK_TROWS;
	for (i = first; i < clt_nTkT && i < first + CLT_TRK_TROWS; i++) CLT_GA("t" + i, clt_tkTLine[i], true);
	if (pages > 1) CLT_GS("tpage", "Страница", (clt_nTkTPage + 1) + " / " + pages, true);
	CLT_GH("Щелчок — подробности внизу; * — есть условие места.");

	// места, вход в которые что-то запустит; ниже — короткая сводка квестовых запретов
	CLT_GCol(1, "Триггеры мест");
	pages = CLT_Trk_Pages(clt_nTkP, CLT_TRK_PROWS);
	if (clt_nTkPPage >= pages) clt_nTkPPage = pages - 1;
	if (clt_nTkPPage < 0) clt_nTkPPage = 0;
	if (clt_nTkP == 0) CLT_GH("Скрытых триггеров мест нет.");
	first = clt_nTkPPage * CLT_TRK_PROWS;
	for (i = first; i < clt_nTkP && i < first + CLT_TRK_PROWS; i++) CLT_GA("p" + i, clt_tkPLine[i], true);
	if (pages > 1) CLT_GS("ppage", "Страница", (clt_nTkPPage + 1) + " / " + pages, true);
	CLT_GH("Щелчок — квест и подробности внизу.");
	if (clt_nTkB == 0) CLT_GH("Квестовых запретов нет.");
	else CLT_GH("Квестовые запреты:");
	for (i = 0; i < clt_nTkB; i++) CLT_GA("b" + i, clt_tkBLine[i], true);

	CLT_GCol(2, "Особые флоты");
	CLT_Fleet_WorldRows();
}

void CLT_Trk_Do(string cmd, int dir, bool big)
{
	int k;
	switch (cmd)
	{
		case "all":
			clt_bTkAll = !clt_bTkAll;
			clt_nTkTPage = 0;
			clt_nTkPPage = 0;
			CLT_Log("Повторяющиеся прерывания: " + CLT_OnOff(clt_bTkAll));
			return;
		break;
		case "tpage": clt_nTkTPage = clt_nTkTPage + dir; return; break;
		case "ppage": clt_nTkPPage = clt_nTkPPage + dir; return; break;
		case "fleets": CLT_Fleet_Toggle(); return; break;
		case "fleetmap": CLT_ShowSec(CLT_SEC_FLEETMAP); return; break;
	}
	if (strlen(cmd) < 2) return;
	k = sti(strcut(cmd, 1, strlen(cmd) - 1));
	if (strcut(cmd, 0, 0) == "t" && k < clt_nTkT) clt_sOut = clt_tkTInfo[k];
	if (strcut(cmd, 0, 0) == "p" && k < clt_nTkP) clt_sOut = clt_tkPInfo[k];
	if (strcut(cmd, 0, 0) == "b" && k < clt_nTkB) clt_sOut = clt_tkBInfo[k];
}
