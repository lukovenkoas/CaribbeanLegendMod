// CL Toolkit — раздел «Персонаж»: выбранный персонаж (герой, компаньоны, пассажиры).
// Портрет со стрелками < > (узлы CLTP_FACE*, над строками 0-7 левой колонки) — выбор персонажа.
// Здоровье — рецепт cheat_character.c F4 / «Навигатора» 1.9 (п. 9-10), энергия — штатный AddBonusEnergyToCharacter.
// Благородство — у всех; известность и авторитет — только у героя (у остальных не используются).
// У героя благородство — reputation.nobility (0-100), у остальных — само значение reputation (1-90,
// названия «Славный малый»/«Негодяй» — GetReputationName), см. CLT_DoNpcNobility.
// Лояльность — только у не-героя: chr.loyality (0-35, MAX_LOYALITY), при нуле офицер просит расчёта;
// закрепление OfficerWantToGo.DontGo останавливает и уход, и игровые пересчёты (см. CLT_DoLoyality).
// Только героя касаются бессмертие, энергия, заряд, вес (переключатели на хабе, clt_sec_hero.c),
// деньги (шапка хаба) и нации (раздел «Мир»). Умения и PIRATES — раздел «Умения».
// Ранг героя (правая колонка) — только число для квестов, см. CLT_DoRank.

void CLT_Char_Build()
{
	ref chr = CLT_Target();
	int i, rep, loy;
	bool bHero = CLT_TargetIsHero();

	string sFace = "1";
	if (CheckAttribute(chr, "FaceId")) sFace = chr.FaceId;
	SetNewPicture("CLTP_FACE", "interfaces\le\portraits\256\face_" + sFace + ".tga");
	SetFormatedText("CLTP_FNAME", GetFullName(chr));
	SetSelectable("CLTP_FPREV", clt_nTargets > 1);
	SetSelectable("CLTP_FNEXT", clt_nTargets > 1);

	CLT_GCol(0, CLT_TargetRole(CLT_TargetIndex()) + "  " + (clt_nTargetPos + 1) + " / " + clt_nTargets + "    ранг " + CLT_AttrStr(chr, "rank", "", "?"));
	for (i = 0; i < 8; i++) CLT_GSkip();
	CLT_GS("hp", "Здоровье", int(LAi_GetCharacterHP(chr)) + " / " + int(LAi_GetCharacterMaxHP(chr)), true);
	CLT_GS("energy", "Энергия", int(Lai_CharacterGetEnergy(chr)) + " / " + int(LAi_GetCharacterMaxEnergy(chr)), true);
	CLT_GA("heal", "Восстановить здоровье и энергию", true);
	CLT_GH("< > меняют максимум. Здоровье: шаг 100,");
	CLT_GH("правая кнопка — 1000; энергия: 10 и 50.");

	CLT_GCol(1, "Репутация");
	if (bHero)
	{
		CLT_GS("nobility", "Благородство", CLT_AttrStr(chr, "reputation", "nobility", "-"), true);
		CLT_GS("fame", "Известность", CLT_AttrStr(chr, "reputation", "fame", "-"), true);
		CLT_GS("authority", "Авторитет", CLT_AttrStr(chr, "reputation", "authority", "-"), true);
		CLT_GH("< > — шаг 5, правая кнопка — 25.");
	}
	else
	{
		CLT_GCol(1, "Репутация и лояльность");
		rep = CLT_NpcNobility(chr);
		CLT_GS("nobility", "Благородство", "" + rep, true);
		CLT_GH(XI_ConvertString(GetReputationName(rep)) + " (шкала 1-90)");
		if (CheckAttribute(chr, "alignment")) CLT_GH("Склонность: " + CLT_AlignStr(chr.alignment));
		CLT_GH("< > — шаг 5, правая кнопка — 25.");
		CLT_GH("Склонность пересчитывается: больше 41 —");
		CLT_GH("благородный, иначе нет. Лояльность растёт,");
		CLT_GH("если дела героя совпадают со склонностью.");
		loy = CLT_NpcLoyality(chr);
		CLT_GS("loyality", "Лояльность", loy + " / " + MAX_LOYALITY, true);
		CLT_GH(XI_ConvertString(GetLoyalityName(loy * 100 / MAX_LOYALITY)) + " (шкала 0-" + MAX_LOYALITY + ")");
		CLT_GT("loylock", "Закрепить (сам не уйдёт)", CheckAttribute(chr, "OfficerWantToGo.DontGo"), true);
		CLT_GH("При нуле офицер просит расчёта. < > —");
		CLT_GH("шаг 1, правая кнопка — 5.");
		if (!CheckAttribute(chr, "loyality")) CLT_GH("Атрибута нет — считается 0, правка создаст его.");
		if (IsEquipCharacterByArtefact(chr, "totem_04")) CLT_GH("«Клеврет» надет: игра не трогает лояльность.");
	}

	CLT_GCol(2, "Ранг героя");
	CLT_GS("rank", "Ранг", CLT_AttrStr(pchar, "rank", "", "?"), true);
	CLT_GH("< > — шаг 1, правая кнопка — 5.");
	CLT_GH("Меняется только число ранга (его");
	CLT_GH("проверяют квесты). Очки умений и");
	CLT_GH("перков не даются, макс. здоровье");
	CLT_GH("и энергия остаются прежними.");
}

void CLT_Char_Do(string cmd, int dir, bool big)
{
	ref chr = CLT_Target();

	switch (cmd)
	{
		case "hp": CLT_DoMaxHP(chr, CLT_Step(dir, big, 100, 1000)); return; break;
		case "energy": CLT_DoMaxEnergy(chr, CLT_Step(dir, big, 10, 50)); return; break;
		case "heal": CLT_DoHeal(chr); return; break;
		case "rank": CLT_DoRank(pchar, CLT_Step(dir, big, 1, 5)); return; break;
		case "loyality": CLT_DoLoyality(chr, CLT_Step(dir, big, 1, 5)); return; break;
		case "loylock": CLT_DoLoyalityLock(chr); return; break;
	}
	if (cmd == "nobility" && !CLT_TargetIsHero())
	{
		CLT_DoNpcNobility(chr, CLT_Step(dir, big, 5, 25));
		return;
	}
	if (cmd == "nobility" || cmd == "fame" || cmd == "authority")
	{
		ChangeCharacterComplexReputation(chr, cmd, CLT_Step(dir, big, 5, 25));
		CLT_Log(GetFullName(chr) + ": " + cmd + " = " + CLT_AttrStr(chr, "reputation", cmd, "-"));
	}
}

// Благородство не-героя: chr.reputation, 1-90 (REPUTATION_MIN/MAX) — его показывают окна персонажа
// и офицеров (character.c, inventory.c: GetReputationName(chr.reputation)). Меняем штатным
// ChangeCharacterReputation (characterUtilite.c). От него при найме выводится склонность alignment
// (officers.c:491, PsHero.c, Ransack_captain_dialog.c: reputation > 41 — "good", иначе "bad"),
// а по склонности OfficersReaction / ChangeOfficersLoyality (officers.c) меняют лояльность.
// Игра склонность потом не пересчитывает — пересчитываем мы, по тому же правилу (по просьбе пользователя).
int CLT_NpcNobility(ref chr)
{
	if (!CheckAttribute(chr, "reputation") || chr.reputation == "") return REPUTATION_NEUTRAL;
	return sti(chr.reputation);
}

string CLT_AlignStr(string a)
{
	if (a == "good") return "благородный (good)";
	if (a == "bad") return "неблагородный (bad)";
	return a;
}

void CLT_DoNpcNobility(ref chr, int d)
{
	if (CheckAttribute(chr, "reputation") && chr.reputation == "") chr.reputation = REPUTATION_NEUTRAL;
	int r = ChangeCharacterReputation(chr, d);
	string s = GetFullName(chr) + ": благородство " + r + " (" + XI_ConvertString(GetReputationName(r)) + ")";
	if (CheckAttribute(chr, "alignment"))
	{
		if (r > 41) chr.alignment = "good";
		else chr.alignment = "bad";
		s = s + ", склонность " + chr.alignment;
	}
	CLT_Log(s);
}

// Лояльность не-героя: chr.loyality, 0-MAX_LOYALITY (35) — шкалу показывает окно персонажа
// (character.c, inventory.c: GetCharacterLoyality), название — GetLoyalityName по проценту
// (interface_utils.c). При нуле офицер просит расчёта (officers.c OfficersReactionResult ->
// Enc_Officer_dialog.c "WantToRemove"). Игра меняет лояльность сама: OfficersReaction (дела героя
// против склонности, ±1 в день), ChangeOfficersLoyality (сюжет, совпадение склонности даёт x2),
// невыплата жалования (salary.c, -1), голод и бунт (food.c). Флаг OfficerWantToGo.DontGo (у
// квестовых офицеров) и талисман «Клеврет» (totem_04) защищают от игровых пересчётов.
int CLT_NpcLoyality(ref chr)
{
	return GetCharacterLoyality(chr); // заодно нормализует атрибут: обрезает до 0-35
}

// Прямая запись chr.loyality — как ChangeOfficerLoyality (officers.c), только с обрезкой на месте:
// у игры обрезка происходит при чтении (GetCharacterLoyality). Отсутствующий атрибут создаём,
// иначе персонаж (например, пленник) навсегда останется с нулём.
void CLT_DoLoyality(ref chr, int d)
{
	if (!CheckAttribute(chr, "loyality")) chr.loyality = 0;
	int loy = CLT_Clamp(sti(chr.loyality) + d, 0, MAX_LOYALITY);
	chr.loyality = loy;
	CLT_Log(GetFullName(chr) + ": лояльность " + loy + " (" + XI_ConvertString(GetLoyalityName(loy * 100 / MAX_LOYALITY)) + ")");
}

// Переключатель OfficerWantToGo.DontGo — тот же флаг, что у квестовых офицеров: его проверяют
// OfficersReaction, ChangeOfficersLoyality, salary.c, food.c и OfficersReactionResult.
void CLT_DoLoyalityLock(ref chr)
{
	if (CheckAttribute(chr, "OfficerWantToGo.DontGo"))
	{
		DeleteAttribute(chr, "OfficerWantToGo.DontGo");
		CLT_Log(GetFullName(chr) + ": закрепление лояльности снято");
	}
	else
	{
		chr.OfficerWantToGo.DontGo = true;
		CLT_Log(GetFullName(chr) + ": лояльность закреплена (сам не уйдёт)");
	}
}

// Ранг ±d — как «Навигатор» 1.9 команда 57 (nav_commands.c): прямая запись chr.rank, rank_exp = 0;
// повышение там же идёт через AddCharacterExpToSkill (leveling.c) — оно дало бы очки умений/перков
// и полное здоровье, поэтому не используется. От ранга игра считает максимум HP и энергии
// (land_table.c CT_SetMaxHealth / CT_SetMaxEnergy: M_HP_PER_RANK, M_ENERGY_PER_RANK × (rank - 1)) —
// разницу гасим нашим постоянным модификатором "CLToolkit" (как CLT_DoMaxHP), максимум не меняется.
// Ранг ещё влияет на вес у перка «Мул» (land_table.c:281) — это не компенсируется.
void CLT_DoRank(ref chr, int d)
{
	int r = sti(chr.rank);
	if (r + d < 1) d = 1 - r;
	if (d == 0) return;
	bool bTables = true;
	if (GetCharacterEquipByGroup(chr, BLADE_ITEM_TYPE) == "") bTables = false; // без клинка таблицы суши не считаются
	float hp = LAi_GetCharacterHP(chr);
	float mhp = LAi_GetCharacterMaxHP(chr);
	float men = LAi_GetCharacterMaxEnergy(chr);
	float en = Lai_CharacterGetEnergy(chr);

	chr.rank = r + d;
	chr.rank_exp = 0;
	if (bTables)
	{
		CT_UpdateCashTables(chr);
		CLT_KeepMax(chr, true, mhp);
		CLT_KeepMax(chr, false, men);
		SetEnergyToCharacter(chr);
		if (en > LAi_GetCharacterMaxEnergy(chr)) en = LAi_GetCharacterMaxEnergy(chr);
		chr.chr_ai.energy = en;
		mhp = LAi_GetCharacterMaxHP(chr);
		if (hp > mhp) hp = mhp;
		LAi_SetHP(chr, hp, mhp);
	}
	else CLT_Log("Без клинка максимум не пересчитан: при следующем пересчёте он сдвинется на разницу рангов.");
	CLT_Log(GetFullName(chr) + ": ранг " + sti(chr.rank) + " (макс. здоровье " + int(LAi_GetCharacterMaxHP(chr)) + ", энергия " + int(LAi_GetCharacterMaxEnergy(chr)) + ")");
}

float CLT_GetMax(ref chr, bool bHp)
{
	if (bHp) return LAi_GetCharacterMaxHP(chr);
	return LAi_GetCharacterMaxEnergy(chr);
}

// Вернуть максимум HP (bHp) или энергии к target модификатором "CLToolkit". Максимум = (база + бонус) × (1 + множитель),
// поэтому после первой поправки k измеряем фактический сдвиг и досчитываем остаток пропорционально.
void CLT_KeepMax(ref chr, bool bHp, float target)
{
	string sMod = M_ENERGY_MAX;
	if (bHp) sMod = M_HP_MAX;
	float cur = CLT_GetMax(chr, bHp);
	float k = target - cur;
	if (k < 0.5 && k > -0.5) return;
	IncreaseChrModifier(chr, sMod, k, "CLToolkit");
	CT_UpdateCashTables(chr);
	float got = CLT_GetMax(chr, bHp) - cur;
	if (got < 0.01 && got > -0.01) return;
	float k2 = k * k / got - k;
	if (k2 < 0.01 && k2 > -0.01) return;
	IncreaseChrModifier(chr, sMod, k2, "CLToolkit");
	CT_UpdateCashTables(chr);
}
