float DamageSkillMtp(ref chr, string type)
{
	float level;
	if (type == "range") level = LAi_GetCharacterGunLevel(chr);
	else level = LAi_GetCharacterFightLevel(chr);

	float fMtp = pow(level, SKILL_MTP_POWER);
	// --> CL Toolkit: метку «Навигатора» 1.9 оставляем, чтобы его собственная панель
	// «Множитель урона» продолжала работать. Но двойного счёта нет: при первом тике
	// CLT переносит старое значение в pchar.CLToolkit.Dmg и удаляет Navigator.dmg
	// (см. cltoolkit\clt_sec_misc.c, CLT_Misc_Dmg).
	if (IsMainCharacter(chr) && CheckAttribute(pchar, "Navigator.dmg"))
		fMtp = fMtp * stf(pchar.Navigator.dmg);
	// <-- CL Toolkit
	return fMtp;
}

// --> CL Toolkit: множитель урона героя — своя метка, не зависит от «Навигатора».
// Применяется в конце расчёта (melee.c / range.c), поэтому умножает итоговый урон
// клинком и огнестрелом, а не только разброс (как было у Navigator.dmg в range.c).
float CLT_HeroDmgMtp(ref chr)
{
	if (!IsMainCharacter(chr)) return 1.0;
	if (!CheckAttribute(pchar, "CLToolkit.Dmg")) return 1.0;
	float mtp = stf(pchar.CLToolkit.Dmg);
	if (mtp <= 0.0) return 1.0;
	return mtp;
}
// <-- CL Toolkit
