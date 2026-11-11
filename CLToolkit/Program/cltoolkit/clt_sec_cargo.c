// CL Toolkit — раздел «Трюм»: все товары игры с картинками, у каждого «-» и «+».
// Выдача — рецепт «Навигатора» 1.9 (Nav_CmdGoods: AddCharacterGoods, шаг — селектор справа
// и поле «Количество» в хабе, одна переменная clt_nQty);
// раскладка ячейкой «значок — название — количество — - +» по образцу трюма «Навигатора» 2.0.
// Значок: SetNewGroupPicture(узел, "GOODS", Goods[i].Name) — как ColonyCapture.c:531.
// Корабль — у выбранного персонажа (стрелки в шапке страницы).

#define CLT_CARGO_CELLS		50

void CLT_Cargo_Reset()
{
}

void CLT_Cargo_Nodes(bool bShow)
{
	int k;
	bool bUse;
	SetNodeUsing("CLTC_HT", bShow);
	SetNodeUsing("CLTC_QDN", bShow);
	SetNodeUsing("CLTC_QV", bShow);
	SetNodeUsing("CLTC_QUP", bShow);
	for (k = 1; k <= CLT_CARGO_CELLS; k++)
	{
		bUse = bShow && (k <= GOODS_QUANTITY);
		SetNodeUsing("CLTC_I" + k, bUse);
		SetNodeUsing("CLTC_N" + k, bUse);
		SetNodeUsing("CLTC_Q" + k, bUse);
		SetNodeUsing("CLTC_D" + k, bUse);
		SetNodeUsing("CLTC_U" + k, bUse);
	}
}

void CLT_Cargo_Refresh()
{
	ref chr = CLT_Target();
	bool bShip = CLT_Ship_Has(chr);
	int k, g, have;
	if (bShip) SetFormatedText("CLTC_HT", "Трюм:  " + GetCargoLoad(chr) + " / " + GetCargoMaxSpace(chr) + "    свободно " + GetCargoFreeSpace(chr) + "    (правая кнопка — x10)");
	else SetFormatedText("CLTC_HT", GetFullName(chr) + ": нет корабля");
	CLT_BtnText("CLTC_QDN", "/10");
	SetFormatedText("CLTC_QV", "шаг " + CLT_Num(clt_nQty));
	CLT_BtnText("CLTC_QUP", "x10");
	for (k = 1; k <= CLT_CARGO_CELLS; k++)
	{
		g = k - 1;
		if (g >= GOODS_QUANTITY) continue;
		if (!CheckAttribute(&Goods[g], "Name"))
		{
			SetNodeUsing("CLTC_I" + k, false);
			SetNodeUsing("CLTC_D" + k, false);
			SetNodeUsing("CLTC_U" + k, false);
			SetFormatedText("CLTC_N" + k, "");
			SetFormatedText("CLTC_Q" + k, "");
			continue;
		}
		SetNewGroupPicture("CLTC_I" + k, "GOODS", Goods[g].Name);
		SetFormatedText("CLTC_N" + k, XI_ConvertString(Goods[g].Name));
		have = 0;
		if (bShip) have = GetCargoGoods(chr, g);
		SetFormatedText("CLTC_Q" + k, CLT_Num(have));
		CLT_BtnText("CLTC_D" + k, "-");
		CLT_BtnText("CLTC_U" + k, "+");
		SetSelectable("CLTC_D" + k, bShip && have > 0);
		SetSelectable("CLTC_U" + k, bShip);
	}
}

void CLT_Cargo_OnCommand(string nodName, bool big)
{
	// селектор шага работает и без корабля — это общая настройка, как поле «Количество» в хабе
	if (nodName == "CLTC_QUP")
	{
		CLT_SetQty(clt_nQty * 10);
		return;
	}
	if (nodName == "CLTC_QDN")
	{
		CLT_SetQty(clt_nQty / 10);
		return;
	}
	ref chr = CLT_Target();
	if (!CLT_Ship_Has(chr)) return;
	int q = clt_nQty;
	if (big) q = q * 10;
	int g, have;
	string pre = strcut(nodName, 0, 5); // CLTC_D / CLTC_U
	if (pre != "CLTC_D" && pre != "CLTC_U") return;
	g = sti(strcut(nodName, 6, strlen(nodName) - 1)) - 1;
	if (g < 0 || g >= GOODS_QUANTITY) return;
	if (!CheckAttribute(&Goods[g], "Name")) return;
	have = GetCargoGoods(chr, g);
	if (pre == "CLTC_U") AddCharacterGoods(chr, g, q);
	else
	{
		if (q > have) q = have;
		if (q > 0) RemoveCharacterGoods(chr, g, q);
	}
	CLT_Log(XI_ConvertString(Goods[g].Name) + ": " + GetCargoGoods(chr, g));
}
