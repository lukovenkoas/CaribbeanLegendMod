// CL Toolkit — раздел «Мир»: отношение наций, индейцев и контрабандистов к герою, где мы, дата и время, случайные встречи.
// Нации — blackmark_cheat.c F13/F14 (ChangeCharacterNationReputation), всегда у главного героя.
// Индейцы — ChangeIndianRelation (characterUtilite.c, отладчик Debuger.c F8/F9), 0..100.
// Контрабандисты — ChangeContrabandRelation (smuggling.c, «Навигатор» 1.9 команда 29), 0..100.
// Подпись уровня у обоих — GetFactionReputationName, как в окне отношений (NationRelation.c).
// День вперёд — AddDataToCurrent(0, 0, 1) («Навигатор» 1.9 команда 1). День назад — та же запись
// даты, что в AddDataToCurrent, но без события NextDay (суточный пересчёт назад не нужен).
// Часы — SetCurrentTime (calendar.c, так время ставят квесты), только в пределах текущего дня.
// Случайные встречи — bEncOffGlobal (blackmark_cheat.c F27).
// Поиск особых флотов — clt_sec_fleet.c (своя функция по просьбе пользователя).

void CLT_World_ToggleEnc()
{
	bEncOffGlobal = !bEncOffGlobal;
	CLT_Log("Случайные встречи отключены: " + CLT_OnOff(bEncOffGlobal));
}

int CLT_World_IndRel()
{
	if (!CheckAttribute(pchar, "questTemp.Indian.relation")) return 0;
	return int(pchar.questTemp.Indian.relation);
}

int CLT_World_SmgRel()
{
	if (!CheckAttribute(pchar, "questTemp.Contraband.relation")) return 0;
	return sti(pchar.questTemp.Contraband.relation);
}

string CLT_World_Date()
{
	return GetDataDay() + "." + GetDataMonth() + "." + GetDataYear();
}

string CLT_World_Time()
{
	int m = int(GetMinute());
	string s = int(GetHour()) + ":";
	if (m < 10) s = s + "0";
	return s + m;
}

// время можно двигать только на суше: SetCurrentTime пересоздаёт погоду, квесты делают это на суше
bool CLT_World_TimeOk()
{
	return !bSeaActive && !IsEntity(&worldMap);
}

// дата на день назад (запись как в AddDataToCurrent, calendar.c)
void CLT_World_DayBack()
{
	int d = GetDataDay() - 1;
	int m = GetDataMonth();
	int y = GetDataYear();
	if (d < 1)
	{
		m--;
		if (m < 1) { m = 12; y--; }
		d = GetMonthDays(m);
	}
	Environment.date.year = y;
	Environment.date.month = m;
	Environment.date.day = d;
	worldMap.date.year = y;
	worldMap.date.month = m;
	worldMap.date.day = d;
	RefreshLandTime();
}

// сдвиг на n часов, не выходя за 0:00 и 23:59 текущего дня
void CLT_World_Hours(int n)
{
	int cur = int(GetHour()) * 60 + int(GetMinute());
	int nxt = cur + n * 60;
	if (nxt < 0) nxt = 0;
	if (nxt > 23 * 60 + 59) nxt = 23 * 60 + 59;
	if (nxt == cur)
	{
		CLT_Log("Время: " + CLT_World_Time() + " — предел дня, дата не меняется");
		return;
	}
	SetCurrentTime(nxt / 60, nxt % 60);
	CLT_Log("Время: " + CLT_World_Time());
}

void CLT_World_Build()
{
	int j, rep;
	string loc = CLT_AttrStr(pchar, "location", "", "");
	int li = -1;
	if (loc != "") li = FindLocation(loc);

	CLT_GCol(0, "Отношение к герою");
	for (j = 0; j < MAX_NATIONS; j++)
	{
		rep = ChangeCharacterNationReputation(pchar, j, 0);
		CLT_GS("nat" + j, XI_ConvertString(GetNationNameByType(j)), rep + "  " + GetNationReputationName(j, rep), true);
	}
	rep = CLT_World_IndRel();
	CLT_GS("ind", "Индейцы", rep + "  " + GetFactionReputationName(rep), true);
	rep = CLT_World_SmgRel();
	CLT_GS("smg", "Контрабандисты", rep + "  " + GetFactionReputationName(rep), true);
	CLT_GA("natall+", "Все нации +10", true);
	CLT_GA("natall-", "Все нации -10", true);
	CLT_GH("< > — шаг 10, правая кнопка — 50.");
	CLT_GH("Индейцы и контрабандисты: 0..100.");

	CLT_GCol(1, "Где мы");
	CLT_GH("Локация: " + loc);
	if (li >= 0)
	{
		CLT_GH("Остров: " + CLT_AttrStr(&Locations[li], "islandId", "", "-"));
		CLT_GH("Тип: " + CLT_AttrStr(&Locations[li], "type", "", "-"));
	}
	CLT_GH("В море: " + CLT_OnOff(bSeaActive) + "    абордаж: " + CLT_OnOff(bAbordageStarted));
	CLT_GH("Карта мира: " + CLT_OnOff(IsEntity(&worldMap)));
	CLT_GSkip();
	CLT_GS("day", "Дата", CLT_World_Date(), true);
	CLT_GS("hour", "Время", CLT_World_Time(), CLT_World_TimeOk());
	CLT_GH("Дата: < > — на день назад / вперёд.");
	CLT_GH("Время: < > — 1 ч, правая кнопка — 8 ч.");
	CLT_GH("Часы не переходят через полночь.");
	if (!CLT_World_TimeOk()) CLT_GH("Время меняется только на суше.");

	CLT_GCol(2, "Мир");
	CLT_GT("enc", "Без случайных встреч", bEncOffGlobal, true);
	CLT_GSkip();
	CLT_Fleet_WorldRows();
}

void CLT_World_Do(string cmd, int dir, bool big)
{
	int j, d;
	switch (cmd)
	{
		case "enc": CLT_World_ToggleEnc(); return; break;
		case "fleets": CLT_Fleet_Toggle(); return; break;
		case "fleetmap": CLT_ShowSec(CLT_SEC_FLEETMAP); return; break;
		case "natall+":
			for (j = 0; j < MAX_NATIONS; j++) ChangeCharacterNationReputation(pchar, j, 10); // blackmark_cheat.c F13
			CLT_Log("Все нации +10");
			return;
		break;
		case "natall-":
			for (j = 0; j < MAX_NATIONS; j++) ChangeCharacterNationReputation(pchar, j, -10); // blackmark_cheat.c F14
			CLT_Log("Все нации -10");
			return;
		break;
		case "ind":
			if (!CheckAttribute(pchar, "questTemp.Indian.relation")) pchar.questTemp.Indian.relation = 0;
			d = int(ChangeIndianRelation(float(CLT_Step(dir, big, 10, 50))));
			CLT_Log("Индейцы: " + d);
			return;
		break;
		case "smg":
			if (!CheckAttribute(pchar, "questTemp.Contraband.relation")) pchar.questTemp.Contraband.relation = 0;
			d = ChangeContrabandRelation(pchar, CLT_Step(dir, big, 10, 50));
			CLT_Log("Контрабандисты: " + d);
			return;
		break;
		case "day":
			if (dir > 0) AddDataToCurrent(0, 0, 1); // «Навигатор» 1.9 команда 1
			else CLT_World_DayBack();
			CLT_Log("Дата: " + CLT_World_Date());
			return;
		break;
		case "hour":
			if (!CLT_World_TimeOk()) { CLT_Log("Время меняется только на суше"); return; }
			CLT_World_Hours(CLT_Step(dir, big, 1, 8));
			return;
		break;
	}
	if (strlen(cmd) == 4 && strcut(cmd, 0, 2) == "nat")
	{
		j = sti(strcut(cmd, 3, 3));
		d = ChangeCharacterNationReputation(pchar, j, CLT_Step(dir, big, 10, 50));
		CLT_Log(XI_ConvertString(GetNationNameByType(j)) + ": " + d);
	}
}
