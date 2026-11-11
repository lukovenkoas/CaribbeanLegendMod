// CL Toolkit — переключатели героя (правая колонка хаба). Только главный герой.
// Пункты 1-6 — рецепты «Навигатора» 1.9 (Workshop 3813948983, nav_commands.c 15-18, 32-33; nav_core.c Nav_ApplyEffects):
// флаг pchar.Navigator.<Имя> ставится и снимается так же, как у «Навигатора» (имя метки сохранено ради сейвов,
// где она уже стоит). Сам «Навигатор» с тулкитом несовместим: подменяет те же базовые файлы.
// Флаги 2-6 читает не скрипт тулкита, а копии базовых файлов игры в составе тулкита (рецепт «Навигатора» 1.9):
//   Energy   — Loc_ai\LAi_character.c (энергия = максимум каждый кадр)
//   Ammo     — cltoolkit\fight\range.c (скорость перезарядки пистолета/мушкета; подключён через Loc_ai\fightparams\common.c)
//   Weight   — characters\RPG\weight.c (предел веса 100000)
//   NoReload — sea_ai\AICannon.c Cannon_GetRechargeTime (0.3 с)
//   SuperAim — sea_ai\AICannon.c Cannon_GetFireTime (залп без разброса по времени)
// Имена атрибутов без учёта регистра: Navigator.Energy и Navigator.energy — один и тот же атрибут.
// 7-8 — флаги штатного Cheat Menu / blackmark_cheat.c (проверены на этапе 3).

string CLT_Hero_Name(int i)
{
	switch (i)
	{
		case 1: return "Бессмертие"; break;
		case 2: return "Бесконечная энергия"; break;
		case 3: return "Оружие всегда заряжено"; break;
		case 4: return "Переносимый вес без ограничений"; break;
		case 5: return "Пушки без перезарядки"; break;
		case 6: return "Супер-точность залпа"; break;
		case 7: return "Без штрафа за класс корабля"; break;
		case 8: return "Без случайных встреч"; break;
	}
	return "";
}

string CLT_Hero_Flag(int i)
{
	switch (i)
	{
		case 2: return "Energy"; break;
		case 3: return "Ammo"; break;
		case 4: return "Weight"; break;
		case 5: return "NoReload"; break;
		case 6: return "SuperAim"; break;
	}
	return "";
}

bool CLT_Hero_On(int i)
{
	switch (i)
	{
		case 1: return LAi_IsImmortal(pchar); break;
		case 7: return CheckAttribute(pchar, "NoNavyPenalty"); break;
		case 8: return bEncOffGlobal; break;
	}
	string f = CLT_Hero_Flag(i);
	if (f == "") return false;
	return CheckAttribute(pchar, "Navigator." + f);
}

void CLT_Hero_Toggle(int i)
{
	if (i == 7) { CLT_Misc_Toggle(4); return; }
	if (i == 8) { CLT_World_ToggleEnc(); return; }
	if (i == 1)
	{
		// nav_commands.c 15: флаг Immortal + LAi_SetImmortal
		if (LAi_IsImmortal(pchar))
		{
			LAi_SetImmortal(pchar, false);
			DeleteAttribute(pchar, "Navigator.Immortal");
		}
		else
		{
			LAi_SetImmortal(pchar, true);
			pchar.Navigator.Immortal = true;
		}
		CLT_Log("Бессмертие: " + CLT_OnOff(LAi_IsImmortal(pchar)));
		return;
	}
	string f = CLT_Hero_Flag(i);
	if (f == "") return;
	if (CheckAttribute(pchar, "Navigator." + f))
	{
		DeleteAttribute(pchar, "Navigator." + f);
		CLT_Log(CLT_Hero_Name(i) + ": выкл");
		return;
	}
	pchar.Navigator.(f) = true;
	// Nav_ApplyEffects: при включении сразу доливаем энергию и заряды
	if (i == 2)
	{
		float emax = 125.0;
		if (CheckAttribute(pchar, "chr_ai.energyMax")) emax = stf(pchar.chr_ai.energyMax);
		pchar.chr_ai.energy = emax;
	}
	if (i == 3)
	{
		if (CheckAttribute(pchar, "chr_ai.gun.charge_max")) pchar.chr_ai.gun.charge = stf(pchar.chr_ai.gun.charge_max);
		if (CheckAttribute(pchar, "chr_ai.musket.charge_max")) pchar.chr_ai.musket.charge = stf(pchar.chr_ai.musket.charge_max);
	}
	CLT_Log(CLT_Hero_Name(i) + ": вкл");
}
