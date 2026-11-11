// CL Toolkit — особые (квестовые) флоты: список в разделе «Мир» и схема архипелага с метками.
// Своя функция по просьбе пользователя (в «Навигаторе» 1.9 такой нет).
// Откуда данные (только чтение, ничего не меняем):
//   worldMap.encounters.<id> — кораблики глобальной карты; x/z — позиция в координатах карты
//     (их же читает islands_loader.c при входе в море), quest.chrID — квестовый энкаунтер
//     (worldmap_events.c ставит его всем Map_Create*: золотой флот, охотники, курьеры, эскадры осады...);
//   worldMap.addQuestEncounters — квестовые, ещё не вышедшие на карту (появятся при следующем входе),
//     у Map_CreateTraderXZ есть цель x2/z2 (так плывёт золотой флот, GoldFleet.c);
//   золотой флот — персонаж Head_of_Gold_Squadron (GoldFleet.c, кораблик Manowar_gold);
//   нация флота — chr.nation капитана: именно её игра берёт для квестового энкаунтера
//     (encounters_map.c GenerateMapEncounter_Alone: rEncounter.Nation = rCharacter.Nation);
//     имя — свой список (тексты мода русские), флаг на схеме — группа NATIONS базового pictures.ini.
// Позиция героя и пересчёт в картинку — как в атласе (interface\mapview.c GetCorrectShipCoords,
// ShowBestMapWindow; узлы MAIN_MAP / MAP в RESOURCE\INI\INTERFACES\mapview.ini).
// Вне глобальной карты позиции те, что были при последнем выходе с неё (игра их не двигает).

#define CLT_FLEET_MAX		20
#define CLT_FLEET_ROWS		9	// строк списка на странице «Мир»
#define CLT_FLEET_GOLD		"Head_of_Gold_Squadron"
// картинка карты на странице (пропорции MAIN_MAP атласа 1330x900)
#define CLT_FM_L			80
#define CLT_FM_T			150
#define CLT_FM_W			1241
#define CLT_FM_H			840

string clt_fName[CLT_FLEET_MAX];
string clt_fDest[CLT_FLEET_MAX];
float clt_fX[CLT_FLEET_MAX];
float clt_fZ[CLT_FLEET_MAX];
bool clt_fPos[CLT_FLEET_MAX];
bool clt_fGold[CLT_FLEET_MAX];
int clt_fNat[CLT_FLEET_MAX];
string clt_fChr[CLT_FLEET_MAX];
int clt_nFleets;
float clt_fMeX;
float clt_fMeZ;

bool CLT_Fleet_On()
{
	return CheckAttribute(pchar, "CLToolkit.Fleets") && pchar.CLToolkit.Fleets == "1";
}

void CLT_Fleet_Toggle()
{
	if (CLT_Fleet_On()) pchar.CLToolkit.Fleets = "0";
	else pchar.CLToolkit.Fleets = "1";
	CLT_Log("Поиск особых флотов: " + CLT_OnOff(CLT_Fleet_On()));
}

// позиция героя в координатах глобальной карты (mapview.c GetCorrectShipCoords)
void CLT_Fleet_PlayerPos()
{
	clt_fMeX = 0.0;
	clt_fMeZ = 0.0;
	if (IsEntity(&worldMap))
	{
		clt_fMeX = float(worldMap.playerShipX);
		clt_fMeZ = float(worldMap.playerShipZ);
		return;
	}
	if (!bSeaActive && CheckAttribute(pchar, "shipx") && CheckAttribute(pchar, "shipz"))
	{
		clt_fMeX = GetSeaShipX(float(pchar.shipx));
		clt_fMeZ = GetSeaShipZ(float(pchar.shipz));
		return;
	}
	if (CheckAttribute(pchar, "Ship.Pos.X") && CheckAttribute(pchar, "Ship.Pos.Z"))
	{
		clt_fMeX = GetSeaShipX(float(pchar.Ship.Pos.X));
		clt_fMeZ = GetSeaShipZ(float(pchar.Ship.Pos.Z));
		return;
	}
	if (CheckAttribute(&worldMap, "playerShipX"))
	{
		clt_fMeX = float(worldMap.playerShipX);
		clt_fMeZ = float(worldMap.playerShipZ);
	}
}

// ближайшая колония (позиции городов — как в атласе: worldMap.islands.<остров>.<город>_town.position)
string CLT_Fleet_Near(float x, float z)
{
	int i;
	float d;
	float best = -1.0;
	string res = "";
	string isl, town;
	for (i = 0; i < MAX_COLONIES; i++)
	{
		if (!CheckAttribute(&colonies[i], "island")) continue;
		isl = colonies[i].island;
		town = colonies[i].id + "_town";
		if (!CheckAttribute(&worldMap, "islands." + isl + "." + town + ".position.x")) continue;
		d = GetDistance2D(x, z, float(worldMap.islands.(isl).(town).position.x), float(worldMap.islands.(isl).(town).position.z));
		if (best < 0.0 || d < best)
		{
			best = d;
			res = XI_ConvertString("Colony" + colonies[i].id);
		}
	}
	return res;
}

// «СЗ 340» — направление и расстояние от героя (север — +z, восток — +x, как в атласе)
string CLT_Fleet_Dir(float x, float z)
{
	float dx = x - clt_fMeX;
	float dz = z - clt_fMeZ;
	float d = sqrt(dx * dx + dz * dz);
	if (d < 5.0) return "рядом";
	float a = atan2(dx, dz) * 180.0 / PI;
	if (a < 0.0) a = a + 360.0;
	int k = int((a + 22.5) / 45.0) % 8;
	string s = "С";
	switch (k)
	{
		case 1: s = "СВ"; break;
		case 2: s = "В"; break;
		case 3: s = "ЮВ"; break;
		case 4: s = "Ю"; break;
		case 5: s = "ЮЗ"; break;
		case 6: s = "З"; break;
		case 7: s = "СЗ"; break;
	}
	return s + " " + int(d);
}

// нация флота в списке (тексты мода — русские, §16.4; ключи NationNameNominative_* не нужны)
string CLT_Fleet_NationName(int n)
{
	switch (n)
	{
		case ENGLAND:	return "Англия"; break;
		case FRANCE:	return "Франция"; break;
		case SPAIN:		return "Испания"; break;
		case HOLLAND:	return "Голландия"; break;
		case PIRATE:	return "пираты"; break;
	}
	return "";
}

void CLT_Fleet_Add(string chrID, bool bPos, float x, float z, string dest)
{
	int i;
	if (clt_nFleets >= CLT_FLEET_MAX) return;
	for (i = 0; i < clt_nFleets; i++)
	{
		if (clt_fChr[i] == chrID) return;
	}
	int idx = GetCharacterIndex(chrID);
	if (idx < 0) return;
	ref chr = &Characters[idx];
	if (CharacterIsDead(chr)) return;
	int nat = -1;
	if (CheckAttribute(chr, "nation")) nat = sti(chr.nation);
	if (nat < 0 || nat >= MAX_NATIONS) nat = -1;
	string s = "";
	if (CheckAttribute(chr, "mapEnc.Name")) s = chr.mapEnc.Name;
	if (s == "") s = GetFullName(chr);
	i = clt_nFleets;
	clt_nFleets++;
	clt_fChr[i] = chrID;
	clt_fName[i] = s;
	clt_fDest[i] = dest;
	clt_fX[i] = x;
	clt_fZ[i] = z;
	clt_fPos[i] = bPos;
	clt_fGold[i] = (chrID == CLT_FLEET_GOLD);
	clt_fNat[i] = nat;
	// золотой флот — первым
	if (clt_fGold[i] && i > 0)
	{
		clt_fChr[i] = clt_fChr[0];	clt_fChr[0] = chrID;
		clt_fName[i] = clt_fName[0];	clt_fName[0] = s;
		clt_fDest[i] = clt_fDest[0];	clt_fDest[0] = dest;
		clt_fX[i] = clt_fX[0];		clt_fX[0] = x;
		clt_fZ[i] = clt_fZ[0];		clt_fZ[0] = z;
		clt_fPos[i] = clt_fPos[0];	clt_fPos[0] = bPos;
		clt_fGold[i] = false;		clt_fGold[0] = true;
		clt_fNat[i] = clt_fNat[0];	clt_fNat[0] = nat;
	}
}

void CLT_Fleet_Collect()
{
	int i, n;
	aref encs, enc;
	string dest;
	clt_nFleets = 0;
	CLT_Fleet_PlayerPos();
	if (CheckAttribute(&worldMap, "encounters"))
	{
		makearef(encs, worldMap.encounters);
		n = GetAttributesNum(encs);
		for (i = 0; i < n; i++)
		{
			enc = GetAttributeN(encs, i);
			if (!CheckAttribute(enc, "quest.chrID")) continue;
			if (CheckAttribute(enc, "needDelete")) continue;
			if (!CheckAttribute(enc, "x") || !CheckAttribute(enc, "z")) continue;
			CLT_Fleet_Add(enc.quest.chrID, true, float(enc.x), float(enc.z), "");
		}
	}
	if (CheckAttribute(&worldMap, "addQuestEncounters"))
	{
		makearef(encs, worldMap.addQuestEncounters);
		n = GetAttributesNum(encs);
		for (i = 0; i < n; i++)
		{
			enc = GetAttributeN(encs, i);
			if (!CheckAttribute(enc, "characterID")) continue;
			dest = "";
			if (CheckAttribute(enc, "XZGoto") && CheckAttribute(enc, "x2") && CheckAttribute(enc, "z2"))
				dest = CLT_Fleet_Near(float(enc.x2), float(enc.z2));
			CLT_Fleet_Add(enc.characterID, false, 0.0, 0.0, dest);
		}
	}
}

// укоротить до n байт по границе слова (strcut режет байты: кириллицу резать только перед пробелом)
string CLT_Fleet_Short(string s, int n)
{
	if (strlen(s) <= n) return s;
	int cut = -1;
	int p = findsubstr(s, " ", 0);
	while (p > 0 && p <= n)
	{
		cut = p;
		p = findsubstr(s, " ", p + 1);
	}
	if (cut <= 0) return s;
	return strcut(s, 0, cut - 1) + "..";
}

// строка списка: "1. Золотой конвой (Испания): СЗ 340, у Гаваны"; nameLen — в байтах
string CLT_Fleet_Line(int i, int nameLen)
{
	string s = (i + 1) + ". " + CLT_Fleet_Short(clt_fName[i], nameLen);
	if (clt_fNat[i] >= 0) s = s + " (" + CLT_Fleet_NationName(clt_fNat[i]) + ")";
	s = s + ": ";
	if (clt_fPos[i])
	{
		s = s + CLT_Fleet_Dir(clt_fX[i], clt_fZ[i]);
		string sNear = CLT_Fleet_Near(clt_fX[i], clt_fZ[i]);
		if (sNear != "") s = s + ", у " + sNear;
		return s;
	}
	s = s + "ещё не на карте";
	if (clt_fDest[i] != "") s = s + ", идёт к " + clt_fDest[i];
	return s;
}

// строки на странице «Мир» (колонка «Мир»)
void CLT_Fleet_WorldRows()
{
	int i;
	CLT_GT("fleets", "Поиск особых флотов", CLT_Fleet_On(), true);
	if (!CLT_Fleet_On())
	{
		CLT_GH("Квестовые флоты: список и схема карты.");
		return;
	}
	CLT_Fleet_Collect();
	CLT_GA("fleetmap", "Схема: особые флоты на карте", true);
	if (clt_nFleets == 0) CLT_GH("Особых флотов сейчас нет.");
	for (i = 0; i < clt_nFleets && i < CLT_FLEET_ROWS; i++) CLT_GH(CLT_Fleet_Line(i, 40));
	if (clt_nFleets > CLT_FLEET_ROWS) CLT_GH("...ещё " + (clt_nFleets - CLT_FLEET_ROWS) + " — на схеме.");
	CLT_GH("Курс и расстояние — от героя.");
}

// ---------------------------------------------------------------------------
// Страница-схема (CLT_SEC_FLEETMAP): картинка атласа, метки флотов, герой, список справа
// ---------------------------------------------------------------------------

void CLT_Fleet_Nodes(bool bShow)
{
	int i;
	SetNodeUsing("CLTM_MAP", bShow);
	SetNodeUsing("CLTM_LBG", bShow);
	SetNodeUsing("CLTM_LIST", bShow);
	SetNodeUsing("CLTM_ME", bShow);
	for (i = 1; i <= CLT_FLEET_MAX; i++)
	{
		SetNodeUsing("CLTM_P" + i, false);
		SetNodeUsing("CLTM_F" + i, false);
		SetNodeUsing("CLTM_N" + i, false);
	}
}

// координаты карты -> экран (атлас: X = x + 1000, Y = 1000 - z на базе 2048, узел MAP 1270x845
// со сдвигом 40,35 внутри картинки 1330x900)
int CLT_Fleet_SX(float x)
{
	float k = (40.0 + (x + 1000.0) * 1270.0 / 2048.0) / 1330.0;
	int v = CLT_FM_L + int(k * CLT_FM_W);
	return CLT_Clamp(v, CLT_FM_L, CLT_FM_L + CLT_FM_W);
}

int CLT_Fleet_SY(float z)
{
	float k = (35.0 + (1000.0 - z) * 845.0 / 2048.0) / 900.0;
	int v = CLT_FM_T + int(k * CLT_FM_H);
	return CLT_Clamp(v, CLT_FM_T, CLT_FM_T + CLT_FM_H);
}

void CLT_Fleet_MapRefresh()
{
	int i, x, y, n;
	// папка картинки — как в mapview.c (у атласа: russian / english / chinese)
	string sLng = LanguageGetLanguage();
	string lang = "english";
	if (sLng == "Russian" || sLng == "russian") lang = "russian";
	if (sLng == "Chinese" || sLng == "chinese") lang = "chinese";
	SetNewPicture("CLTM_MAP", "interfaces\maps\" + lang + "\map_good.tga");

	CLT_Fleet_Collect();
	for (i = 1; i <= CLT_FLEET_MAX; i++)
	{
		SetNodeUsing("CLTM_P" + i, false);
		SetNodeUsing("CLTM_F" + i, false);
		SetNodeUsing("CLTM_N" + i, false);
	}

	x = CLT_Fleet_SX(clt_fMeX);
	y = CLT_Fleet_SY(clt_fMeZ);
	SetNodePosition("CLTM_ME", x - 20, y - 20, x + 20, y + 20);
	SendMessage(&GameInterface, "lsll", MSG_INTERFACE_MSG_TO_NODE, "CLTM_ME", 4, argb(255, 120, 230, 255));

	string s = "Особые флоты" + NewStr();
	if (IsEntity(&worldMap)) s = s + "позиции сейчас" + NewStr();
	else s = s + "позиции на момент выхода с глобальной карты" + NewStr();
	s = s + NewStr();
	if (clt_nFleets == 0) s = s + "Сейчас нет ни одного." + NewStr();
	n = 0;
	for (i = 0; i < clt_nFleets; i++)
	{
		s = s + CLT_Fleet_Line(i, 56) + NewStr();
		if (!clt_fPos[i]) continue;
		n++;
		x = CLT_Fleet_SX(clt_fX[i]);
		y = CLT_Fleet_SY(clt_fZ[i]);
		SetNodeUsing("CLTM_P" + n, true);
		SetNodeUsing("CLTM_N" + n, true);
		SetNodePosition("CLTM_P" + n, x - 16, y - 16, x + 16, y + 16);
		SetNodePosition("CLTM_N" + n, x + 14, y - 30, x + 74, y - 2);
		SetFormatedText("CLTM_N" + n, "" + (i + 1));
		// флаг нации — под печатью (группа NATIONS из pictures.ini, как Nat1 в NationLegend.c)
		if (clt_fNat[i] >= 0)
		{
			SetNodeUsing("CLTM_F" + n, true);
			SetNodePosition("CLTM_F" + n, x - 14, y + 18, x + 14, y + 46);
			SetNewGroupPicture("CLTM_F" + n, "NATIONS", GetNationNameByType(clt_fNat[i]));
		}
		if (clt_fGold[i]) SendMessage(&GameInterface, "lsll", MSG_INTERFACE_MSG_TO_NODE, "CLTM_P" + n, 4, argb(255, 255, 215, 40));
		else SendMessage(&GameInterface, "lsll", MSG_INTERFACE_MSG_TO_NODE, "CLTM_P" + n, 4, argb(255, 210, 110, 255));
	}
	s = s + NewStr() + "Голубой корабль — вы." + NewStr();
	s = s + "Золотая печать — золотой флот," + NewStr() + "лиловая — прочие квестовые." + NewStr();
	s = s + "Флаг под меткой — нация флота." + NewStr();
	s = s + "Ещё не на карте — появится" + NewStr() + "при входе на глобальную карту.";
	SetFormatedText("CLTM_LIST", s);
}
