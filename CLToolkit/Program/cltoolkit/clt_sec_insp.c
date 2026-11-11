// CL Toolkit — раздел «Инспектор» (только чтение).
// Обход дерева атрибутов: корень — выбранный персонаж, экземпляр его корабля (RealShips) или шаблон (ShipsTypes).
// Спуск — по именам через GetAttributeN/GetAttributeName (как items.c, locations), без записи.
// Левая колонка — корень, вверх, страницы; средняя и правая — 30 детей текущего узла (щелчок — спуститься).

#define CLT_INSP_MAX_DEPTH	16
#define CLT_INSP_ROWS		30

#define CLT_INSP_ROOT_CHAR	1
#define CLT_INSP_ROOT_SHIP	2
#define CLT_INSP_ROOT_TYPE	3

int clt_nInspRoot;
string clt_aInspPath[CLT_INSP_MAX_DEPTH];
int clt_nInspDepth;
int clt_nInspPage;

void CLT_Insp_Reset()
{
	clt_nInspRoot = CLT_INSP_ROOT_CHAR;
	clt_nInspDepth = 0;
	clt_nInspPage = 0;
}

string CLT_Insp_RootName()
{
	int idx = CLT_TargetIndex();
	int st = GetCharacterShipType(GetCharacter(idx));
	switch (clt_nInspRoot)
	{
		case CLT_INSP_ROOT_CHAR: return "Characters[" + idx + "]"; break;
		case CLT_INSP_ROOT_SHIP: return "RealShips[" + st + "]"; break;
		case CLT_INSP_ROOT_TYPE:
			if (st == SHIP_NOTUSED) return "ShipsTypes[?]";
			return "ShipsTypes[" + RealShips[st].BaseType + "]";
		break;
	}
	return "?";
}

// путь: корень и последние 3 уровня (строка должна влезть в одну строку колонки)
string CLT_Insp_PathText()
{
	string s = CLT_Insp_RootName();
	int i;
	int first = 0;
	if (clt_nInspDepth > 3) { first = clt_nInspDepth - 3; s = s + " ..."; }
	for (i = first; i < clt_nInspDepth; i++) s = s + "." + clt_aInspPath[i];
	return s;
}

// Индекс ребёнка с именем name или -1
int CLT_Insp_FindChild(aref ar, string name)
{
	int i, n;
	aref arC;
	n = GetAttributesNum(ar);
	for (i = 0; i < n; i++)
	{
		arC = GetAttributeN(ar, i);
		if (GetAttributeName(arC) == name) return i;
	}
	return -1;
}

// у цели есть корабль для корня «корабль/шаблон»
bool CLT_Insp_RootOk()
{
	if (clt_nInspRoot == CLT_INSP_ROOT_CHAR) return true;
	return GetCharacterShipType(CLT_Target()) != SHIP_NOTUSED;
}

// текущий узел (вызывать после CLT_Insp_RootOk)
aref CLT_Insp_Node()
{
	aref ar;
	int idx = CLT_TargetIndex();
	int st = GetCharacterShipType(GetCharacter(idx));
	int i, k;
	switch (clt_nInspRoot)
	{
		case CLT_INSP_ROOT_CHAR: makearef(ar, Characters[idx]); break;
		case CLT_INSP_ROOT_SHIP: makearef(ar, RealShips[st]); break;
		case CLT_INSP_ROOT_TYPE: makearef(ar, ShipsTypes[sti(RealShips[st].BaseType)]); break;
	}
	// Спуск по пути; если узел исчез — обрезаем путь
	for (i = 0; i < clt_nInspDepth; i++)
	{
		k = CLT_Insp_FindChild(ar, clt_aInspPath[i]);
		if (k < 0)
		{
			clt_nInspDepth = i;
			break;
		}
		ar = GetAttributeN(ar, k);
	}
	return ar;
}

int CLT_Insp_Pages(int n)
{
	if (n <= 0) return 1;
	return (n + CLT_INSP_ROWS - 1) / CLT_INSP_ROWS;
}

// значение для строки: длинные обрезаем (strcut режет байты — кириллица в конце может испортиться)
string CLT_Insp_Short(string v)
{
	if (strlen(v) > 40) return strcut(v, 0, 39) + "...";
	return v;
}

void CLT_Insp_Build()
{
	aref ar, arC;
	int i, n, k, first, pages;
	string label;

	CLT_GCol(0, "Узел");
	CLT_GT("rootchar", "Корень: персонаж", clt_nInspRoot == CLT_INSP_ROOT_CHAR, true);
	CLT_GT("rootship", "Корень: корабль (RealShips)", clt_nInspRoot == CLT_INSP_ROOT_SHIP, true);
	CLT_GT("roottype", "Корень: шаблон (ShipsTypes)", clt_nInspRoot == CLT_INSP_ROOT_TYPE, true);
	if (!CLT_Insp_RootOk())
	{
		CLT_GH("У выбранного персонажа нет корабля.");
		return;
	}
	ar = CLT_Insp_Node();
	n = GetAttributesNum(ar);
	pages = CLT_Insp_Pages(n);
	if (clt_nInspPage >= pages) clt_nInspPage = 0;
	CLT_GA("up", "Вверх", clt_nInspDepth > 0);
	CLT_GS("page", "Страница", (clt_nInspPage + 1) + " / " + pages, pages > 1);
	CLT_GH(CLT_Insp_PathText());
	CLT_GH("значение: '" + CLT_Insp_Short(GetAttributeValue(ar)) + "'");
	CLT_GH("детей: " + n);
	CLT_GH("Только чтение. «>» в строке — есть дети.");

	first = clt_nInspPage * CLT_INSP_ROWS;
	for (i = 0; i < CLT_INSP_ROWS; i++)
	{
		if (i == 0) CLT_GCol(1, "Атрибуты");
		if (i == CLT_GP_ROWS) CLT_GCol(2, "");
		if (first + i >= n) break;
		arC = GetAttributeN(ar, first + i);
		k = GetAttributesNum(arC);
		label = GetAttributeName(arC) + " = '" + CLT_Insp_Short(GetAttributeValue(arC)) + "'";
		if (k > 0) label = label + "   > " + k;
		CLT_GA("go" + (first + i), label, k > 0);
	}
}

void CLT_Insp_Do(string cmd, int dir, bool big)
{
	aref ar, arC;
	int k;

	switch (cmd)
	{
		case "rootchar": clt_nInspRoot = CLT_INSP_ROOT_CHAR; clt_nInspDepth = 0; clt_nInspPage = 0; return; break;
		case "rootship": clt_nInspRoot = CLT_INSP_ROOT_SHIP; clt_nInspDepth = 0; clt_nInspPage = 0; return; break;
		case "roottype": clt_nInspRoot = CLT_INSP_ROOT_TYPE; clt_nInspDepth = 0; clt_nInspPage = 0; return; break;
		case "up":
			if (clt_nInspDepth > 0) clt_nInspDepth--;
			clt_nInspPage = 0;
			return;
		break;
		case "page":
			clt_nInspPage = clt_nInspPage + dir;
			if (clt_nInspPage < 0) clt_nInspPage = 0; // переполнение сбрасывает Build
			return;
		break;
	}

	if (strlen(cmd) < 3 || strcut(cmd, 0, 1) != "go") return;
	if (clt_nInspDepth >= CLT_INSP_MAX_DEPTH) return;
	if (!CLT_Insp_RootOk()) return;
	ar = CLT_Insp_Node();
	k = sti(strcut(cmd, 2, strlen(cmd) - 1));
	if (k >= GetAttributesNum(ar)) return;
	arC = GetAttributeN(ar, k);
	clt_aInspPath[clt_nInspDepth] = GetAttributeName(arC);
	clt_nInspDepth++;
	clt_nInspPage = 0;
}
