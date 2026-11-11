// CL Toolkit — раздел «Корабль» (корабль выбранного персонажа): картинка, класс, характеристики,
// команда и орудия (рецепты «Навигатора» 1.9), состояние, модификаторы (специализация и свойство).
// chr.Ship.Type — индекс экземпляра в RealShips[]; RealShips[].BaseType — шаблон в ShipsTypes[].
// Класс (ранг), который читают квесты и штраф навигации, — RealShips[].Class (GetCharacterShipClass):
// ключевая функция для пользователя, сохранять в любой версии (_Mod_Research.md §14.4).
// Характеристики экземпляра HP/Capacity/SpeedRate/TurnRate — рецепт «Навигатора» 1.9 (Nav_ShipTune).
// Ремонт и товары — blackmark_cheat.c F23/F24/F25.
//
// Модификаторы — по явной просьбе пользователя (в «Навигаторе» 1.9 такой опции нет). Рецепты игры:
// специализация RealShips[].Spec (0 Торговый, 1 Военный, 2 Рейдер, 3 Многоцелевой — ships.h:83)
// хранится на экземпляре (копия шаблона при создании), правится записью атрибута, как в миграции
// 0029_Cabins.c; свойство RealShips[].Traits — одно, смена записью атрибута (change_ship_trait.c
// SetNewTrait), список свойств по специализации — SetPossibleTraits оттуда же. Имена и описания —
// ShipsPerksDescribe.txt через DLG_Convert (internal.c:111), значки — группа PERKS_SHIPS из
// pictures.ini базовой игры (ship_perks.c). Эффекты свойств читают HasShipTrait (ShipsUtilites.c)
// динамически и только у героя; исключение — trait11 «Поворотные лафеты»: сектора орудий +7°
// запекаются при создании корабля (ShipsUtilites.c:456), поэтому при смене свойства сектора
// возвращаются к шаблону и при надобности расширяются (штатная смена свойства этого не делает).
// Бонус мощи эскадры от специализации игра считает по шаблону (GetBaseShipPower), не по экземпляру.

string CLT_Ship_TypeName(int base)
{
	return XI_ConvertString(ShipsTypes[base].Name);
}

// ---------------------------------------------------------------------------
// Модификаторы: специализация (RealShips[].Spec) и свойство (RealShips[].Traits).
// Узлы страницы — CLTP_S*/CLTP_T*/CLTP_MODDESC (gen_clt_ini.py), значки — группа PERKS_SHIPS.
// ---------------------------------------------------------------------------

#define CLT_MOD_TRAITS		13

// порядок свойств: торговые 01-05, военные 11-14, рейдерские 21-24 (change_ship_trait.c SetPossibleTraits)
string CLT_Mod_TraitId(int i)
{
	if (i < 5) return "trait0" + (i + 1);
	if (i < 9) return "trait1" + (i - 4);
	return "trait2" + (i - 8);
}

string CLT_Mod_SpecName(int spec)
{
	return DLG_Convert("special" + spec, "ShipsPerksDescribe.txt", &NullObject);
}

string CLT_Mod_TraitName(string id)
{
	if (id == "") return "нет";
	return DLG_Convert(id, "ShipsPerksDescribe.txt", &NullObject);
}

string CLT_Mod_CurTrait(ref rShip)
{
	if (!CheckAttribute(rShip, "Traits")) return "";
	return rShip.Traits;
}

// позиция в селекторе: 0 — «нет», 1..CLT_MOD_TRAITS — свойства
int CLT_Mod_TraitPos(string id)
{
	int i;
	if (id == "") return 0;
	for (i = 0; i < CLT_MOD_TRAITS; i++)
	{
		if (CLT_Mod_TraitId(i) == id) return i + 1;
	}
	return 0;
}

// Сектора орудий: trait11 («Поворотные лафеты») расширяет их на 7° при создании корабля
// (ShipsUtilites.c:456), поэтому при смене свойства сначала возвращаем значения шаблона,
// затем при trait11 расширяем снова. Штатный экран смены свойства сектора не трогает.
void CLT_Mod_FireZone(ref rShip, bool bTrait11)
{
	ref rBase;
	float d = 0.0;
	if (bTrait11) d = Degree2Radian(7.0);
	makeref(rBase, ShipsTypes[sti(rShip.BaseType)]);
	if (CheckAttribute(rBase, "Cannons.Borts.cannonf.FireZone")) rShip.Cannons.Borts.cannonf.FireZone = stf(rBase.Cannons.Borts.cannonf.FireZone) + d;
	if (CheckAttribute(rBase, "Cannons.Borts.cannonr.FireZone")) rShip.Cannons.Borts.cannonr.FireZone = stf(rBase.Cannons.Borts.cannonr.FireZone) + d;
	if (CheckAttribute(rBase, "Cannons.Borts.cannonb.FireZone")) rShip.Cannons.Borts.cannonb.FireZone = stf(rBase.Cannons.Borts.cannonb.FireZone) + d;
	if (CheckAttribute(rBase, "Cannons.Borts.cannonl.FireZone")) rShip.Cannons.Borts.cannonl.FireZone = stf(rBase.Cannons.Borts.cannonl.FireZone) + d;
}

void CLT_Mod_SetTrait(ref chr, ref rShip, string id)
{
	string old = CLT_Mod_CurTrait(rShip);
	if (id == old) return;
	CLT_Mod_FireZone(rShip, id == "trait11");
	if (id == "") DeleteAttribute(rShip, "Traits");
	else rShip.Traits = id;
	if (id == "") CLT_Log(GetFullName(chr) + ": свойство корабля снято (было " + CLT_Mod_TraitName(old) + ")");
	else CLT_Log(GetFullName(chr) + ": свойство корабля — " + CLT_Mod_TraitName(id));
}

void CLT_Mod_SetSpec(ref chr, ref rShip, int spec)
{
	int old = sti(rShip.Spec);
	if (spec == old) return;
	rShip.Spec = spec;
	UpdatePlayerSquadronPower(); // кэш мощи эскадры (interface\ship.c:265 — «если что-то менялось»)
	CLT_Log(GetFullName(chr) + ": специализация корабля — " + CLT_Mod_SpecName(spec) + " (мощь эскадры игра считает по шаблону)");
}

// скрыть блок модификаторов (нет корабля или уходит со страницы «Корабль»)
void CLT_Mod_Hide()
{
	SetNodeUsing("CLTP_STXT", false);
	SetNodeUsing("CLTP_SD", false);
	SetNodeUsing("CLTP_SU", false);
	SetNodeUsing("CLTP_TTXT", false);
	SetNodeUsing("CLTP_TD", false);
	SetNodeUsing("CLTP_TU", false);
	SetNodeUsing("CLTP_SBKG", false);
	SetNodeUsing("CLTP_SPIC", false);
	SetNodeUsing("CLTP_SBRD", false);
	SetNodeUsing("CLTP_TBKG", false);
	SetNodeUsing("CLTP_TPIC", false);
	SetNodeUsing("CLTP_TBRD", false);
	SetNodeUsing("CLTP_MODDESC", false);
}

void CLT_Mod_Build(ref chr, ref rShip)
{
	int spec = SHIP_SPEC_UNIVERSAL;
	if (CheckAttribute(rShip, "Spec")) spec = sti(rShip.Spec);
	if (spec < 0 || spec > 3) spec = SHIP_SPEC_UNIVERSAL;
	string trait = CLT_Mod_CurTrait(rShip);

	// показать блок модификаторов (мог быть скрыт CLT_Mod_Hide)
	SetNodeUsing("CLTP_STXT", true);
	SetNodeUsing("CLTP_SD", true);
	SetNodeUsing("CLTP_SU", true);
	SetNodeUsing("CLTP_TTXT", true);
	SetNodeUsing("CLTP_TD", true);
	SetNodeUsing("CLTP_TU", true);
	SetNodeUsing("CLTP_SBKG", true);
	SetNodeUsing("CLTP_SPIC", true);
	SetNodeUsing("CLTP_SBRD", true);
	SetNodeUsing("CLTP_MODDESC", true);

	SetFormatedText("CLTP_STXT", "Специализация:  " + CLT_Mod_SpecName(spec));
	SetFormatedText("CLTP_TTXT", "Свойство:  " + CLT_Mod_TraitName(trait));

	SetNewGroupPicture("CLTP_SBKG", "PERKS_SHIPS", "background");
	SetNewGroupPicture("CLTP_SPIC", "PERKS_SHIPS", "special" + spec);
	SetNewGroupPicture("CLTP_SBRD", "PERKS_SHIPS", "border");
	if (trait == "")
	{
		SetNodeUsing("CLTP_TBKG", false);
		SetNodeUsing("CLTP_TPIC", false);
		SetNodeUsing("CLTP_TBRD", false);
	}
	else
	{
		SetNodeUsing("CLTP_TBKG", true);
		SetNodeUsing("CLTP_TPIC", true);
		SetNodeUsing("CLTP_TBRD", true);
		SetNewGroupPicture("CLTP_TBKG", "PERKS_SHIPS", "background");
		SetNewGroupPicture("CLTP_TPIC", "PERKS_SHIPS", trait);
		SetNewGroupPicture("CLTP_TBRD", "PERKS_SHIPS", "border");
	}

	// описание выбранного свойства (ShipsPerksDescribe.txt), под ним — эффект специализации и заметки
	string s = "";
	if (trait == "")
	{
		s = "Свойство не назначено. В игре оно случайно при создании корабля и меняется за дублоны у мастера Часовой башни.";
	}
	else
	{
		s = DLG_Convert(trait + "_desc", "ShipsPerksDescribe.txt", &NullObject);
		string d2 = DLG_Convert(trait + "_desc2", "ShipsPerksDescribe.txt", &NullObject);
		if (d2 != "") s = s + NewStr() + d2;
	}
	s = s + NewStr() + "Специализация: " + DLG_Convert("special" + spec + "_desc2", "ShipsPerksDescribe.txt", &NullObject);
	if (CheckAttribute(rShip, "QuestSP")) s = s + NewStr() + "Особый корабль: игра показывает свой значок (sp1-sp4) и свойства не выдаёт; записанное свойство действует, но значок не меняет.";
	if (!CLT_TargetIsHero()) s = s + NewStr() + "Эффекты свойства действуют только на корабле героя.";
	SetFormatedText("CLTP_MODDESC", s);
}

// ---------------------------------------------------------------------------
// Общие действия (вызываются и с хаба)
// ---------------------------------------------------------------------------

bool CLT_Ship_Has(ref chr)
{
	return GetCharacterShipType(chr) != SHIP_NOTUSED;
}

void CLT_Ship_ClassStep(ref chr, int dir)
{
	if (!CLT_Ship_Has(chr)) return;
	ref rShip = GetRealShip(GetCharacterShipType(chr));
	rShip.Class = CLT_Clamp(sti(rShip.Class) + dir, 1, 7);
	CLT_Log(GetFullName(chr) + ": класс корабля " + rShip.Class + " (шаблон " + ShipsTypes[sti(rShip.BaseType)].Class + ")");
}

void CLT_Ship_Repair(ref chr)
{
	if (!CLT_Ship_Has(chr)) { CLT_Log(GetFullName(chr) + ": нет корабля"); return; }
	// blackmark_cheat.c F23
	chr.ship.hp = GetCharacterShipHP(chr);
	DeleteAttribute(chr, "ship.hulls");
	DeleteAttribute(chr, "ship.blots");
	chr.ship.sp = GetCharacterShipSP(chr);
	DeleteAttribute(chr, "ship.sails");
	DeleteAttribute(chr, "ship.masts");
	CLT_Log(GetFullName(chr) + ": корабль отремонтирован");
}

void CLT_Ship_Crew(ref chr)
{
	if (!CLT_Ship_Has(chr)) { CLT_Log(GetFullName(chr) + ": нет корабля"); return; }
	SetCrewQuantityFull(chr);
	CLT_Log(GetFullName(chr) + ": команда " + GetCrewQuantity(chr));
}

void CLT_Ship_Ammo(ref chr)
{
	if (!CLT_Ship_Has(chr)) { CLT_Log(GetFullName(chr) + ": нет корабля"); return; }
	// blackmark_cheat.c F25 (часть)
	AddCharacterGoods(chr, GOOD_BALLS, 500);
	AddCharacterGoods(chr, GOOD_GRAPES, 500);
	AddCharacterGoods(chr, GOOD_KNIPPELS, 500);
	AddCharacterGoods(chr, GOOD_BOMBS, 500);
	AddCharacterGoods(chr, GOOD_POWDER, 500);
	CLT_Log(GetFullName(chr) + ": +500 ядер, картечи, книппелей, бомб и пороха");
}

// ---------------------------------------------------------------------------
// Орудия — рецепты «Навигатора» 1.9 (nav_commands.c 7, 31, 36, 37, 38+; nav_core.c Nav_CannonName, Nav_BestCannonFor).
// Тип пушек — chr.Ship.Cannons.Type (индекс Cannon[], 0..CANNON_TYPES_QUANTITY-1),
// предел калибра экземпляра — RealShips[].MaxCaliber; при выборе более крупных пушек он поднимается.
// Пушек крупнее 48 фунтов в игре нет. Число стволов «Навигатор» не меняет — тулкит тоже.
// ---------------------------------------------------------------------------

string CLT_Ship_CannonName(int t)
{
	if (t < 0 || t >= CANNON_TYPES_QUANTITY) return "не установлены";
	string s = "Пушки ";
	if (sti(Cannon[t].type) == CANNON_NAME_CULVERINE) s = "Кулеврины ";
	return s + GetCannonCaliber(t) + " фнт";
}

int CLT_Ship_CannonType(ref chr)
{
	if (!CheckAttribute(chr, "Ship.Cannons.Type")) return -1;
	int t = sti(chr.Ship.Cannons.Type);
	if (t < 0 || t >= CANNON_TYPES_QUANTITY) return -1;
	return t;
}

int CLT_Ship_BestCannonFor(int iCal)
{
	if (iCal >= 48) return CANNON_TYPE_CANNON_LBS48;
	if (iCal >= 42) return CANNON_TYPE_CANNON_LBS42;
	if (iCal >= 36) return CANNON_TYPE_CANNON_LBS36;
	if (iCal >= 32) return CANNON_TYPE_CANNON_LBS32;
	if (iCal >= 24) return CANNON_TYPE_CANNON_LBS24;
	if (iCal >= 20) return CANNON_TYPE_CANNON_LBS20;
	if (iCal >= 18) return CANNON_TYPE_CANNON_LBS18;
	if (iCal >= 16) return CANNON_TYPE_CANNON_LBS16;
	if (iCal >= 12) return CANNON_TYPE_CANNON_LBS12;
	if (iCal >= 8) return CANNON_TYPE_CANNON_LBS8;
	if (iCal >= 6) return CANNON_TYPE_CANNON_LBS6;
	return CANNON_TYPE_CANNON_LBS3;
}

// поставить тип t; калибр корабля поднимается, если мал (nav_commands.c 38+)
void CLT_Ship_SetCannon(ref chr, ref rShip, int t)
{
	int cal = GetCannonCaliber(t);
	string s = "";
	if (sti(rShip.MaxCaliber) < cal)
	{
		rShip.MaxCaliber = cal;
		s = ", калибр корабля поднят до " + cal;
	}
	chr.Ship.Cannons.Type = t;
	CLT_Log(GetFullName(chr) + ": установлены " + CLT_Ship_CannonName(t) + s);
}

// ---------------------------------------------------------------------------
// Страница
// ---------------------------------------------------------------------------

string CLT_Ship_StatText(ref rShip, ref rBase, string attr)
{
	string cur = CLT_AttrStr(rShip, attr, "", "?");
	string sDef = CLT_AttrStr(rBase, attr, "", "?");
	if (attr == "SpeedRate" || attr == "TurnRate" || attr == "SpeedDependWeight" || attr == "TurnDependWeight" || attr == "InertiaAccelerationY" || attr == "InertiaBrakingY" || attr == "InertiaBrakingZ" || attr == "InertiaAccelerationZ")
	{
		cur = FloatToString(stf(cur), 2);
		sDef = FloatToString(stf(sDef), 2);
	}
	if (cur == sDef) return cur;
	return cur + "  (шабл. " + sDef + ")";
}

// база максимума команды: MaxCrew экземпляра, при отсутствии — у шаблона
int CLT_Ship_MaxCrewBase(ref rShip, ref rBase)
{
	int n = CLT_AttrInt(rShip, "MaxCrew", "");
	if (n <= 0) n = CLT_AttrInt(rBase, "MaxCrew", "");
	return n;
}

// картинка корабля — как interface\ship.c:626
void CLT_Ship_Picture(ref chr)
{
	string pic = "interfaces\le\ships\empty_ship.tga";
	int st = GetCharacterShipType(chr);
	if (st != SHIP_NOTUSED)
	{
		ref rShip = GetRealShip(st);
		if (CheckAttribute(rShip, "BaseName") && CheckAttribute(rShip, "ship.upgrades.hull"))
			pic = "interfaces\le\ships\" + rShip.BaseName + rShip.ship.upgrades.hull + ".tga";
	}
	SetNewPicture("CLTP_SHIPPIC", pic);
}

void CLT_Ship_Build()
{
	ref chr = CLT_Target();
	int i;
	CLT_Ship_Picture(chr);
	int st = GetCharacterShipType(chr);
	if (st == SHIP_NOTUSED)
	{
		CLT_Mod_Hide();
		CLT_GCol(1, "Корабль");
		CLT_GH("У выбранного персонажа нет корабля.");
		CLT_GH("Другой персонаж — стрелки < > в шапке.");
		return;
	}
	ref rShip = GetRealShip(st);
	int base = sti(rShip.BaseType);
	ref rBase;
	makeref(rBase, ShipsTypes[base]); // как GetCharacterShipMaxHP (characterUtilite.c)
	bool bHero = CLT_TargetIsHero();

	// левая колонка: строки 0-7 под картинкой корабля
	CLT_GCol(0, CLT_AttrStr(chr, "Ship", "Name", "?") + " — " + CLT_Ship_TypeName(base));
	for (i = 0; i < 8; i++) CLT_GSkip();
	CLT_GS("class", "Класс (ранг)", rShip.Class + "  (шабл. " + rBase.Class + ")", true);
	CLT_GA("classreset", "Класс как у шаблона", sti(rShip.Class) != sti(rBase.Class));
	CLT_GS("HP", "Корпус (макс.)", CLT_Ship_StatText(rShip, rBase, "HP"), true);
	CLT_GS("Capacity", "Трюм", CLT_Ship_StatText(rShip, rBase, "Capacity"), true);
	CLT_GS("SpeedRate", "Скорость", CLT_Ship_StatText(rShip, rBase, "SpeedRate"), true);
	CLT_GS("TurnRate", "Поворотливость", CLT_Ship_StatText(rShip, rBase, "TurnRate"), true);
	CLT_GA("statsreset", "Характеристики как у шаблона", true);

	CLT_GCol(1, "Команда и орудия");
	int nMaxBase = CLT_Ship_MaxCrewBase(rShip, rBase);
	string sMaxCrew = "" + nMaxBase; // эффективный максимум может отличаться: квартирмейстер и тюнинг
	if (GetMaxCrewQuantity(chr) != nMaxBase) sMaxCrew = sMaxCrew + "  (эфф. " + GetMaxCrewQuantity(chr) + ")";
	CLT_GS("crewmax", "Команда (макс.)", sMaxCrew, true);
	CLT_GS("crewqty", "Команда (сейчас)", GetCrewQuantity(chr) + " / " + GetMaxCrewQuantity(chr) + " (мин. " + GetMinCrewQuantity(chr) + ")", true);
	CLT_GA("crew", "Пополнить команду", true);
	int mor = GetCharacterCrewMorale(chr, true);
	CLT_GH("Мораль: " + XI_ConvertString(GetMoraleName(mor)) + " (" + mor + ")");
	CLT_GA("morale", "Мораль на максимум", true);
	CLT_GH("Опыт команды: " + int(GetCrewExp(chr, "")) + " / " + EXP_MAX);
	CLT_GA("crewexp", "Опыт команды на максимум", true);
	CLT_GS("cannon", "Пушки", CLT_Ship_CannonName(CLT_Ship_CannonType(chr)), true);
	CLT_GH("Калибр корабля " + rShip.MaxCaliber + " (шабл. " + CLT_AttrStr(rBase, "MaxCaliber", "", "?") + "),  стволов " + GetCannonsNum(chr));
	CLT_GS("SpeedDependWeight", "Завис. скорости от веса (меньше = быстрее)", CLT_Ship_StatText(rShip, rBase, "SpeedDependWeight"), true);
	CLT_GS("TurnDependWeight", "Завис. манёвра от веса (меньше = поворотливее)", CLT_Ship_StatText(rShip, rBase, "TurnDependWeight"), true);
	CLT_GS("InertiaAccelerationY", "Инерция разгона поворота (больше = резче)", CLT_Ship_StatText(rShip, rBase, "InertiaAccelerationY"), true);
	CLT_GS("InertiaBrakingY", "Инерция тормож. поворота (больше = резче)", CLT_Ship_StatText(rShip, rBase, "InertiaBrakingY"), true);
	CLT_GS("InertiaBrakingZ", "Инерция тормож. хода (больше = короче тормозной путь)", CLT_Ship_StatText(rShip, rBase, "InertiaBrakingZ"), true);
	CLT_GT("noreload", "Пушки без перезарядки (герой)", CLT_Hero_On(5), bHero);
	CLT_GT("superaim", "Супер-точность залпа (герой)", CLT_Hero_On(6), bHero);

	CLT_GCol(2, "Состояние и модификаторы");
	CLT_GH("Корпус " + CLT_AttrStr(chr, "Ship", "HP", "?") + " / " + rShip.HP + "    паруса " + CLT_AttrStr(chr, "Ship", "SP", "?") + " / " + rShip.SP);
	CLT_GA("repair", "Ремонт корпуса и парусов", true);
	CLT_GA("ammo", "+500 боеприпасов", true);
	CLT_GA("mats", "+500 материалов (шёлк, канаты, дерево, смола)", true);
	CLT_GA("supply", "+500 провизии, лекарств, рома, оружия", true);
	CLT_GA("cannonbest", "Лучшие пушки под калибр", true);
	CLT_GA("cannonculv", "Кулеврины под калибр", true);
	CLT_GA("cannon48", "Фортовые 48-фунтовые", true);
	CLT_GA("cannonfix", "Штатный калибр и подходящие пушки", true);
	CLT_Mod_Build(chr, rShip);
}

void CLT_Ship_Tune(ref chr, ref rShip, ref rBase, string attr, float d)
{
	float fMin = 1.0;
	if (attr == "SpeedDependWeight" || attr == "TurnDependWeight") fMin = 0.01;
	if (attr == "InertiaAccelerationY" || attr == "InertiaBrakingY" || attr == "InertiaBrakingZ" || attr == "InertiaAccelerationZ" || attr == "InertiaAccelerationX" || attr == "InertiaBrakingX") fMin = 0.1;
	float f = stf(rShip.(attr)) + d;
	if (f < fMin) f = fMin;
	if (attr == "HP" || attr == "Capacity") rShip.(attr) = int(f);
	else rShip.(attr) = f;
	// корпус пересчитываем, иначе текущее HP останется от старого максимума («Навигатор» 1.9)
	if (attr == "HP") chr.ship.hp = GetCharacterShipHP(chr);
	CLT_Log(GetFullName(chr) + ": " + attr + " = " + rShip.(attr));
}

void CLT_Ship_Do(string cmd, int dir, bool big)
{
	ref chr = CLT_Target();
	int st = GetCharacterShipType(chr);
	if (st == SHIP_NOTUSED) return;
	ref rShip = GetRealShip(st);
	int base = sti(rShip.BaseType);
	ref rBase;
	makeref(rBase, ShipsTypes[base]);
	int t, cal, nMax, nMin, nStep, n;
	string sId;

	switch (cmd)
	{
		case "class": CLT_Ship_ClassStep(chr, dir); return; break;
		case "classreset":
			rShip.Class = rBase.Class;
			CLT_Log(GetFullName(chr) + ": класс корабля как у шаблона (" + rShip.Class + ")");
			return;
		break;
		case "HP": CLT_Ship_Tune(chr, rShip, rBase, cmd, CLT_Step(dir, big, 100, 1000)); return; break;
		case "Capacity": CLT_Ship_Tune(chr, rShip, rBase, cmd, CLT_Step(dir, big, 100, 1000)); return; break;
		case "SpeedRate":
			if (big) CLT_Ship_Tune(chr, rShip, rBase, cmd, 5.0 * dir);
			else CLT_Ship_Tune(chr, rShip, rBase, cmd, 0.5 * dir);
			return;
		break;
		case "TurnRate": CLT_Ship_Tune(chr, rShip, rBase, cmd, CLT_Step(dir, big, 1, 10)); return; break;
		case "SpeedDependWeight":
			if (big) CLT_Ship_Tune(chr, rShip, rBase, cmd, 0.1 * dir);
			else CLT_Ship_Tune(chr, rShip, rBase, cmd, 0.05 * dir);
			return;
		break;
		case "TurnDependWeight":
			if (big) CLT_Ship_Tune(chr, rShip, rBase, cmd, 0.1 * dir);
			else CLT_Ship_Tune(chr, rShip, rBase, cmd, 0.05 * dir);
			return;
		break;
		case "InertiaAccelerationY":
			if (big) CLT_Ship_Tune(chr, rShip, rBase, cmd, 2.0 * dir);
			else CLT_Ship_Tune(chr, rShip, rBase, cmd, 0.5 * dir);
			return;
		break;
		case "InertiaBrakingY":
			if (big) CLT_Ship_Tune(chr, rShip, rBase, cmd, 2.0 * dir);
			else CLT_Ship_Tune(chr, rShip, rBase, cmd, 0.5 * dir);
			return;
		break;
		case "InertiaBrakingZ":
			if (big) CLT_Ship_Tune(chr, rShip, rBase, cmd, 2.0 * dir);
			else CLT_Ship_Tune(chr, rShip, rBase, cmd, 0.5 * dir);
			return;
		break;
		case "statsreset":
			rShip.HP = rBase.HP;
			rShip.Capacity = rBase.Capacity;
			rShip.SpeedRate = rBase.SpeedRate;
			rShip.TurnRate = rBase.TurnRate;
			rShip.SpeedDependWeight = rBase.SpeedDependWeight;
			rShip.TurnDependWeight = rBase.TurnDependWeight;
			rShip.InertiaAccelerationY = rBase.InertiaAccelerationY;
			rShip.InertiaBrakingY = rBase.InertiaBrakingY;
			rShip.InertiaBrakingZ = rBase.InertiaBrakingZ;
			rShip.InertiaAccelerationZ = rBase.InertiaAccelerationZ;
			chr.ship.hp = GetCharacterShipHP(chr);
			CLT_Log(GetFullName(chr) + ": характеристики как у шаблона");
			return;
		break;
		case "crewmax":
			// своё: MaxCrew экземпляра — квесты правят так же (RealShips[].MaxCrew = N). Эффективный
			// максимум считает GetMaxCrewQuantity (characterUtilite.c): база × квартирмейстер × тюнинг.
			// Шаг: щелчок 1, правая кнопка 10, Shift 100 (XI_IsKeyPressed — как inventory.c:259).
			nStep = 1;
			if (big) nStep = 10;
			if (XI_IsKeyPressed("shift")) nStep = 100;
			nMax = CLT_Ship_MaxCrewBase(rShip, rBase) + nStep * dir;
			nMin = GetMinCrewQuantity(chr);
			if (nMin < 1) nMin = 1;
			rShip.MaxCrew = CLT_Clamp(nMax, nMin, 10000);
			// текущая команда не больше нового максимума (SetCrewQuantity ужал бы сам, но с Trace)
			SetCrewQuantity(chr, CLT_Clamp(GetCrewQuantity(chr), 0, GetMaxCrewQuantity(chr)));
			CLT_Log(GetFullName(chr) + ": макс. команда " + GetMaxCrewQuantity(chr) + " (база " + rShip.MaxCrew + ")");
			return;
		break;
		case "crew": CLT_Ship_Crew(chr); return; break;
		case "crewqty":
			// своё (в «Навигаторе» 1.9 только «пополнить»): шаг 1 / ПКМ 10 / Shift 100,
			// SetCrewQuantity в пределах 0..максимум
			nStep = 1;
			if (big) nStep = 10;
			if (XI_IsKeyPressed("shift")) nStep = 100;
			SetCrewQuantity(chr, CLT_Clamp(GetCrewQuantity(chr) + nStep * dir, 0, GetMaxCrewQuantity(chr)));
			CLT_Log(GetFullName(chr) + ": команда " + GetCrewQuantity(chr));
			return;
		break;
		case "morale":
			AddCrewMorale(chr, MORALE_MAX, false); // nav_commands.c 5
			CLT_Log(GetFullName(chr) + ": мораль " + GetCharacterCrewMorale(chr, true));
			return;
		break;
		case "crewexp":
			// «Навигатор» 1.9 (п. 6) пишет Ship.Crew.Exp.Sailors/Cannoners/Soldiers, но в 1.9 опыт один —
			// Ship.Crew.Exp (Crew.c GetCrewExp: «3 опыта сшиты в 1»), поэтому штатный SetCrewExp
			SetCrewExp(chr, EXP_MAX);
			CLT_Log(GetFullName(chr) + ": опыт команды " + int(GetCrewExp(chr, "")));
			return;
		break;
		case "cannon":
			t = CLT_Ship_CannonType(chr) + dir;
			if (t < 0) t = CANNON_TYPES_QUANTITY - 1;
			if (t >= CANNON_TYPES_QUANTITY) t = 0;
			CLT_Ship_SetCannon(chr, rShip, t);
			return;
		break;
		case "cannonbest": CLT_Ship_SetCannon(chr, rShip, CLT_Ship_BestCannonFor(sti(rShip.MaxCaliber))); return; break;
		case "cannonculv":
			cal = sti(rShip.MaxCaliber);
			t = CANNON_TYPE_CULVERINE_LBS8;
			if (cal >= 18) t = CANNON_TYPE_CULVERINE_LBS18;
			if (cal >= 36) t = CANNON_TYPE_CULVERINE_LBS36;
			CLT_Ship_SetCannon(chr, rShip, t);
			return;
		break;
		case "cannon48": CLT_Ship_SetCannon(chr, rShip, CANNON_TYPE_CANNON_LBS48); return; break;
		case "cannonfix":
			// nav_commands.c 31: калибр шаблона (3..48) и самые тяжёлые пушки под него
			cal = CLT_Clamp(sti(CLT_AttrStr(rBase, "MaxCaliber", "", "3")), 3, 48);
			rShip.MaxCaliber = cal;
			t = CLT_Ship_BestCannonFor(cal);
			rShip.Cannon = t;
			chr.Ship.Cannons.Type = t;
			CLT_Log(GetFullName(chr) + ": калибр " + cal + ", " + CLT_Ship_CannonName(t));
			return;
		break;
		case "noreload": CLT_Hero_Toggle(5); return; break;
		case "superaim": CLT_Hero_Toggle(6); return; break;
		case "repair": CLT_Ship_Repair(chr); return; break;
		case "ammo": CLT_Ship_Ammo(chr); return; break;
		case "mats":
			// blackmark_cheat.c F24
			AddCharacterGoods(chr, GOOD_SHIPSILK, 500);
			AddCharacterGoods(chr, GOOD_ROPES, 500);
			AddCharacterGoods(chr, GOOD_SANDAL, 500);
			AddCharacterGoods(chr, GOOD_OIL, 500);
			CLT_Log(GetFullName(chr) + ": +500 материалов");
			return;
		break;
		case "supply":
			AddCharacterGoods(chr, GOOD_FOOD, 500);
			AddCharacterGoods(chr, GOOD_MEDICAMENT, 500); // F25
			AddCharacterGoods(chr, GOOD_RUM, 500);        // F25
			AddCharacterGoods(chr, GOOD_WEAPON, 500);     // F25
			CLT_Log(GetFullName(chr) + ": +500 провизии, лекарств, рома, оружия");
			return;
		break;
		case "spec":
			// 4 специализации по кругу (ships.h:83)
			n = CLT_Clamp(sti(rShip.Spec), 0, 3);
			CLT_Mod_SetSpec(chr, rShip, (n + dir + 4) % 4);
			return;
		break;
		case "trait":
			// 0 — «нет», затем 13 свойств по кругу (change_ship_trait.c SetPossibleTraits)
			n = CLT_Mod_TraitPos(CLT_Mod_CurTrait(rShip)) + dir;
			if (n < 0) n = CLT_MOD_TRAITS;
			if (n > CLT_MOD_TRAITS) n = 0;
			sId = "";
			if (n > 0) sId = CLT_Mod_TraitId(n - 1);
			CLT_Mod_SetTrait(chr, rShip, sId);
			return;
		break;
	}
}
