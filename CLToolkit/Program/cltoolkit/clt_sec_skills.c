// CL Toolkit — раздел «Умения»: PIRATES (в коде SPECIAL), личные и корабельные умения, свободные очки.
// Рецепты из «Навигатора» 1.9 (Workshop 3813948983, работает в 1.9): chr.SPECIAL.X = v / chr.skill.X = v,
// затем DelBakSkillAttr (кэш умений, RPG.c:63) и CT_UpdateCashTables (кэш-таблицы, cash_tables.c:74).
// Имена — GetSkillNameByTRIdx (RPG\utility.c:34), подписи — XI_ConvertString(имя).

void CLT_Skills_Update(ref chr)
{
	DelBakSkillAttr(chr);
	CT_UpdateCashTables(chr);
}

// все 7 PIRATES = SPECIAL_MAX (10)
void CLT_Skills_SpecialMax(ref chr)
{
	int i;
	string s;
	for (i = 1; i <= 7; i++)
	{
		s = GetSkillNameByTRIdx("SPECIALType", i);
		chr.SPECIAL.(s) = SPECIAL_MAX;
	}
	CLT_Skills_Update(chr);
	CLT_Log(GetFullName(chr) + ": все PIRATES = " + SPECIAL_MAX);
}

void CLT_Skills_AllMax(ref chr)
{
	int i;
	string s;
	for (i = 1; i <= 7; i++)
	{
		s = GetSkillNameByTRIdx("SelfType", i);
		chr.skill.(s) = SKILL_MAX;
		s = GetSkillNameByTRIdx("ShipType", i);
		chr.skill.(s) = SKILL_MAX;
	}
	CLT_Skills_Update(chr);
	CLT_Log(GetFullName(chr) + ": все умения = " + SKILL_MAX);
}

// перки всех четырёх предысторий — «Навигатор» 1.9 (п. 65, nav_core.c Nav_GiveBackstoryPerks):
// метка-перк HT1..HT4 и перки предыстории. Диалоги остаются по основной предыстории. Только герой.
void CLT_Skills_Backstories()
{
	int i;
	for (i = 1; i <= 4; i++) SetCharacterPerk(pchar, "HT" + i);
	SetCharacterPerk(pchar, "TannedLeather");	// 1 Дуэлянт
	SetCharacterPerk(pchar, "RatsWolf");		// 2 Счетовод
	SetCharacterPerk(pchar, "Mimicry");
	SetCharacterPerk(pchar, "TreasureHunter");
	SetCharacterPerk(pchar, "Mule");			// 3 Атлет
	SetCharacterPerk(pchar, "Bombardier");		// 4 Стрелок
	SetCharacterPerk(pchar, "Sniper");
	CLT_Skills_Update(pchar);
	CLT_Log("Выданы перки всех четырёх предысторий (диалоги не менялись)");
}

void CLT_Skills_Build()
{
	ref chr = CLT_Target();
	int i;
	string s;

	CLT_GCol(0, "PIRATES (база)");
	for (i = 1; i <= 7; i++)
	{
		s = GetSkillNameByTRIdx("SPECIALType", i);
		CLT_GS("sp:" + s, XI_ConvertString(s), "" + GetSpecialBase(chr, s), true);
	}
	CLT_GA("specmax", "Все PIRATES на максимум (" + SPECIAL_MAX + ")", true);
	CLT_GH("< > — шаг 1, правая кнопка — 10. Пределы 1..." + SPECIAL_MAX + ".");

	CLT_GCol(1, "Личные умения");
	for (i = 1; i <= 7; i++)
	{
		s = GetSkillNameByTRIdx("SelfType", i);
		CLT_GS("sk:" + s, XI_ConvertString(s), CLT_AttrStr(chr, "skill", s, "1"), true);
	}
	CLT_GH("Корабельные умения");
	for (i = 1; i <= 7; i++)
	{
		s = GetSkillNameByTRIdx("ShipType", i);
		CLT_GS("sk:" + s, XI_ConvertString(s), CLT_AttrStr(chr, "skill", s, "1"), true);
	}

	CLT_GCol(2, "Очки и всё сразу");
	CLT_GS("pt:skill.FreeSkill", "Своб. очки умений", CLT_AttrStr(chr, "skill", "FreeSkill", "0"), true);
	CLT_GS("pt:skill.FreeSPECIAL", "Своб. очки PIRATES", CLT_AttrStr(chr, "skill", "FreeSPECIAL", "0"), true);
	CLT_GS("pt:perks.FreePoints_self", "Очки личных перков", CLT_AttrStr(chr, "perks", "FreePoints_self", "0"), true);
	CLT_GS("pt:perks.FreePoints_ship", "Очки корабельных перков", CLT_AttrStr(chr, "perks", "FreePoints_ship", "0"), true);
	CLT_GA("skillmax", "Все умения на " + SKILL_MAX, true);
	CLT_GA("allmax", "Всё на максимум (умения и PIRATES)", true);
	CLT_GH("Умения: шаг 1, правая кнопка — 10.");
	CLT_GSkip();
	CLT_GA("backstory", "Перки всех четырёх предысторий (герой)", CLT_TargetIsHero());
	CLT_GH("Дуэлянт, Счетовод, Атлет, Стрелок: перки всех;");
	CLT_GH("диалоги — по основной предыстории.");
}

void CLT_Skills_Do(string cmd, int dir, bool big)
{
	ref chr = CLT_Target();
	string pre = strcut(cmd, 0, 2);
	string s = strcut(cmd, 3, strlen(cmd) - 1);
	int v, dot;
	string grp, leaf;

	switch (cmd)
	{
		case "specmax": CLT_Skills_SpecialMax(chr); return; break;
		case "skillmax": CLT_Skills_AllMax(chr); return; break;
		case "backstory": CLT_Skills_Backstories(); return; break;
		case "allmax":
			CLT_Skills_AllMax(chr);
			CLT_Skills_SpecialMax(chr);
			return;
		break;
	}
	if (pre == "sp:")
	{
		v = CLT_Clamp(GetSpecialBase(chr, s) + CLT_Step(dir, big, 1, 10), 1, SPECIAL_MAX);
		chr.SPECIAL.(s) = v;
		CLT_Skills_Update(chr);
		CLT_Log(GetFullName(chr) + ": " + XI_ConvertString(s) + " = " + v);
		return;
	}
	if (pre == "sk:")
	{
		v = CLT_Clamp(CLT_AttrInt(chr, "skill", s) + CLT_Step(dir, big, 1, 10), 1, SKILL_MAX);
		chr.skill.(s) = v;
		CLT_Skills_Update(chr);
		CLT_Log(GetFullName(chr) + ": " + XI_ConvertString(s) + " = " + v);
		return;
	}
	if (pre == "pt:")
	{
		dot = findsubstr(s, ".", 0);
		grp = strcut(s, 0, dot - 1);
		leaf = strcut(s, dot + 1, strlen(s) - 1);
		v = CLT_AttrInt(chr, grp, leaf) + CLT_Step(dir, big, 1, 10);
		if (v < 0) v = 0;
		chr.(grp).(leaf) = v;
		CLT_Log(GetFullName(chr) + ": " + s + " = " + v);
	}
}
