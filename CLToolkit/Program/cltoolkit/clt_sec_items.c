// CL Toolkit — раздел «Предметы» (оформление по образцу «Навигатора» 2.0, без правой части с экипировкой).
// Слева — список категорий во всю высоту (узлы CLTI_C*, печать CLTI_CS* у выбранной); сверху — поиск (CLT_EDIT),
// количество и «найдено»; в центре — сетка 9 x 5 (значок, подпись, «есть N», печать у квестовых); снизу — листание.
// Список — базовые предметы Items[0 .. ITEMS_QUANTITY-1]. Категории и правила отбора — как в «Навигаторе» 2.0
// (_ref/Navigator_3813949512, nav_core.c Nav_InitItemGroups / Nav_BuildItemList): 19 групп, предмет может быть в нескольких;
// без значка и сгенерированные копии клинков (DefItemID) не показываются. Свои: «Все» сверху, «Прочее» (ни в одной группе) снизу.
// Квестовые (ItemType QUESTITEMS/QUESTITEM) помечены печатью во всех категориях.
// Значок: SetNewGroupPicture(узел, itm.picTexture, "itm" + itm.picIndex) — как inventory.c:1742.
// Выдача: генерируемые (IsGenerableItem) — GiveItem2Character(GetBestGeneratedItem(id)), как cheat_items.c F7;
// остальные — TakeNItems, как прототип этапа 2. Получатель — всегда главный герой.
// Щелчок по ячейке — выдать «Количество», правая кнопка — в 10 раз больше.

#define CLT_MAX_ITEMS		1200
#define CLT_MAX_ITEM_CATS	22
#define CLT_ITEM_CATS		21
#define CLT_ITEM_CELLS		45

int clt_aItemIdx[CLT_MAX_ITEMS];
int clt_nItemCount;
int clt_nItemCats;
int clt_nItemCat;
int clt_nItemPage;
int clt_nItemQty;
string clt_sItemFilter;

bool CLT_Item_IsQuest(ref itm)
{
	if (!CheckAttribute(itm, "ItemType")) return false;
	if (itm.ItemType == "QUESTITEMS" || itm.ItemType == "QUESTITEM") return true;
	return false;
}

// 0 — «Все», 1..19 — группы «Навигатора» 2.0, 20 — «Прочее»
string CLT_Item_CatName(int cat)
{
	switch (cat)
	{
		case 0:		return "Все"; break;
		case 1:		return "Клинки"; break;
		case 2:		return "Пистолеты"; break;
		case 3:		return "Мушкеты"; break;
		case 4:		return "Боеприпасы"; break;
		case 5:		return "Кирасы"; break;
		case 6:		return "Шляпы"; break;
		case 7:		return "Подзорные трубы"; break;
		case 8:		return "Карты"; break;
		case 9:		return "Инструменты"; break;
		case 10:	return "Талисманы"; break;
		case 11:	return "Фонари"; break;
		case 12:	return "Патенты"; break;
		case 13:	return "Слоты и амулеты"; break;
		case 14:	return "Особые"; break;
		case 15:	return "Ключи"; break;
		case 16:	return "Квестовые"; break;
		case 17:	return "Драгоценности"; break;
		case 18:	return "Материалы для крафта"; break;
		case 19:	return "Зелья, травы и рецепты"; break;
	}
	return "Прочее";
}

string CLT_Item_CatGroup(int cat)
{
	switch (cat)
	{
		case 1:		return BLADE_ITEM_TYPE; break;
		case 2:		return GUN_ITEM_TYPE; break;
		case 3:		return MUSKET_ITEM_TYPE; break;
		case 4:		return AMMO_ITEM_TYPE; break;
		case 5:		return CIRASS_ITEM_TYPE; break;
		case 6:		return HAT_ITEM_TYPE; break;
		case 7:		return SPYGLASS_ITEM_TYPE; break;
		case 8:		return MAPS_ITEM_TYPE; break;
		case 9:		return TOOL_ITEM_TYPE; break;
		case 10:	return TALISMAN_ITEM_TYPE; break;
		case 11:	return LANTERN_ITEM_TYPE; break;
		case 12:	return PATENT_ITEM_TYPE; break;
		case 13:	return ITEM_SLOT_TYPE; break;
		case 14:	return SPECIAL_ITEM_TYPE; break;
	}
	return "";
}

// показывается ли предмет вообще (Nav_BuildItemList: без значка и сгенерированные копии — нет)
bool CLT_Item_Listed(ref itm)
{
	if (!CheckAttribute(itm, "id") || itm.id == "") return false;
	if (!CheckAttribute(itm, "picTexture") || itm.picTexture == "") return false;
	if (!CheckAttribute(itm, "picIndex")) return false;
	if (CheckAttribute(itm, "DefItemID")) return false;
	return true;
}

// группы 1..19 «Навигатора» 2.0
bool CLT_Item_InGroup(ref itm, int cat)
{
	string sId = itm.id;
	string sGrp;
	if (cat >= 1 && cat <= 14)
	{
		if (!CheckAttribute(itm, "groupID")) return false;
		sGrp = itm.groupID;
		if (sGrp == CLT_Item_CatGroup(cat)) return true;
		return false;
	}
	if (cat == 15)
	{
		if (findsubstr(sId, "key", 0) >= 0) return true;
		if (findsubstr(sId, "Key", 0) >= 0) return true;
		return false;
	}
	if (cat == 16) return CLT_Item_IsQuest(itm);
	if (cat == 17)
	{
		if (!CheckAttribute(itm, "TradeType")) return false;
		if (sti(itm.TradeType) == ITEM_TRADE_JEWELRY) return true;
		return false;
	}
	if (cat == 18)
	{
		if (findsubstr(sId, "mineral", 0) == 0) return true;
		return false;
	}
	if (cat == 19)
	{
		if (findsubstr(sId, "potion", 0) == 0) return true;
		if (findsubstr(sId, "cannabis", 0) == 0) return true;
		if (findsubstr(sId, "recipe", 0) == 0) return true;
		return false;
	}
	return false;
}

// 0 — «Все», 20 — «Прочее» (ни в одной группе)
bool CLT_Item_InCat(ref itm, int cat)
{
	int k;
	if (cat == 0) return true;
	if (cat < 20) return CLT_Item_InGroup(itm, cat);
	for (k = 1; k <= 19; k++)
	{
		if (CLT_Item_InGroup(itm, k)) return false;
	}
	return true;
}

void CLT_Items_Reset()
{
	clt_nItemCats = CLT_ITEM_CATS;
	clt_nItemCat = 0;
	clt_nItemPage = 0;
	clt_nItemQty = 1;
	clt_sItemFilter = "";
	CLT_Items_Collect();
}

bool CLT_Item_Match(ref itm)
{
	if (!CLT_Item_Listed(itm)) return false;
	if (!CLT_Item_InCat(itm, clt_nItemCat)) return false;
	if (clt_sItemFilter == "") return true;
	if (findsubstr(itm.id, clt_sItemFilter, 0) != -1) return true;
	if (findsubstr(GetItemName(itm), clt_sItemFilter, 0) != -1) return true;
	return false;
}

void CLT_Items_Collect()
{
	int i;
	ref itm;
	clt_nItemCount = 0;
	for (i = 0; i < ITEMS_QUANTITY; i++)
	{
		if (clt_nItemCount >= CLT_MAX_ITEMS) break;
		makeref(itm, Items[i]);
		if (!CLT_Item_Match(itm)) continue;
		clt_aItemIdx[clt_nItemCount] = i;
		clt_nItemCount++;
	}
	if (clt_nItemPage * CLT_ITEM_CELLS >= clt_nItemCount) clt_nItemPage = 0;
}

int CLT_Items_Pages()
{
	int pages = 1;
	if (clt_nItemCount > 0) pages = (clt_nItemCount + CLT_ITEM_CELLS - 1) / CLT_ITEM_CELLS;
	return pages;
}

void CLT_Items_Nodes(bool bShow)
{
	int k;
	SetNodeUsing("CLTI_LBG", bShow);
	for (k = 1; k <= CLT_MAX_ITEM_CATS; k++)
	{
		SetNodeUsing("CLTI_C" + k, bShow && k <= clt_nItemCats);
		SetNodeUsing("CLTI_CS" + k, bShow && k == clt_nItemCat + 1);
	}
	SetNodeUsing("CLTI_FL", bShow);
	SetNodeUsing("CLTI_CLR", bShow);
	SetNodeUsing("CLTI_QTY", bShow);
	SetNodeUsing("CLTI_FOUND", bShow);
	for (k = 1; k <= CLT_ITEM_CELLS; k++) CLT_Items_Cell(k, bShow);
	SetNodeUsing("CLTI_PREV", bShow);
	SetNodeUsing("CLTI_PAGE", bShow);
	SetNodeUsing("CLTI_NEXT", bShow);
}

void CLT_Items_Cell(int k, bool bShow)
{
	SetNodeUsing("CLTI_B" + k, bShow);
	SetNodeUsing("CLTI_F" + k, bShow);
	SetNodeUsing("CLTI_I" + k, bShow);
	SetNodeUsing("CLTI_M" + k, bShow);
	SetNodeUsing("CLTI_Q" + k, bShow);
	SetNodeUsing("CLTI_N" + k, bShow);
}

void CLT_Items_Refresh()
{
	int k, n, have;
	ref itm;
	string sId;

	CLT_Items_Nodes(true);
	for (k = 1; k <= clt_nItemCats; k++) CLT_BtnText("CLTI_C" + k, CLT_Item_CatName(k - 1));

	SetFormatedText("CLTI_FL", "Поиск");
	CLT_BtnText("CLTI_CLR", "Сбросить");
	SetSelectable("CLTI_CLR", clt_sItemFilter != "");
	CLT_BtnText("CLTI_QTY", "Количество: " + clt_nItemQty);
	SetFormatedText("CLTI_FOUND", "Найдено: " + clt_nItemCount);

	for (k = 1; k <= CLT_ITEM_CELLS; k++)
	{
		n = clt_nItemPage * CLT_ITEM_CELLS + k - 1;
		if (n >= clt_nItemCount) { CLT_Items_Cell(k, false); continue; }
		makeref(itm, Items[clt_aItemIdx[n]]);
		sId = itm.id;
		SetNewGroupPicture("CLTI_I" + k, itm.picTexture, "itm" + itm.picIndex);
		SetNodeUsing("CLTI_M" + k, CLT_Item_IsQuest(itm));
		have = GetCharacterItem(pchar, sId);
		if (have > 0) SetFormatedText("CLTI_Q" + k, "" + have);
		else SetFormatedText("CLTI_Q" + k, "");
		SetFormatedText("CLTI_N" + k, GetItemName(itm));
	}

	CLT_BtnText("CLTI_PREV", "< Назад");
	CLT_BtnText("CLTI_NEXT", "Вперёд >");
	SetSelectable("CLTI_PREV", CLT_Items_Pages() > 1);
	SetSelectable("CLTI_NEXT", CLT_Items_Pages() > 1);
	string s = CLT_Item_CatName(clt_nItemCat) + "   |   Страница " + (clt_nItemPage + 1) + " / " + CLT_Items_Pages();
	s = s + "   |   щелчок — выдать герою, правая кнопка — x10";
	if (clt_nItemCat == 16) s = s + "   |   квестовые могут сломать сюжет";
	SetFormatedText("CLTI_PAGE", s);
}

void CLT_Items_OnEdit(string s)
{
	clt_sItemFilter = s;
	clt_nItemPage = 0;
	CLT_Items_Collect();
	CLT_Log("Поиск '" + s + "': найдено " + clt_nItemCount);
}

void CLT_Items_Give(int n, int qty)
{
	if (n < 0 || n >= clt_nItemCount) return;
	ref itm;
	makeref(itm, Items[clt_aItemIdx[n]]);
	string sId = itm.id;
	int before = GetCharacterItem(pchar, sId);
	int i;
	string sGen;

	if (IsGenerableItem(sId))
	{
		for (i = 0; i < qty; i++)
		{
			sGen = GetBestGeneratedItem(sId);
			GiveItem2Character(pchar, sGen);
		}
		CLT_Log("Выдано: " + GetItemName(itm) + " (" + sGen + ", лучший экземпляр) x" + qty);
		return;
	}

	TakeNItems(pchar, sId, qty);
	CLT_Log("Выдано: " + GetItemName(itm) + " x" + qty + "  (" + before + " -> " + GetCharacterItem(pchar, sId) + ")");
}

void CLT_Items_OnCommand(string nodName, bool big)
{
	int k;
	if (nodName == "CLTI_PREV" || nodName == "CLTI_NEXT")
	{
		if (nodName == "CLTI_PREV") clt_nItemPage = clt_nItemPage - 1;
		else clt_nItemPage = clt_nItemPage + 1;
		if (clt_nItemPage < 0) clt_nItemPage = CLT_Items_Pages() - 1;
		if (clt_nItemPage >= CLT_Items_Pages()) clt_nItemPage = 0;
		return;
	}
	if (nodName == "CLTI_QTY")
	{
		// 1 -> 10 -> 100 -> 1; правая кнопка — обратно
		if (!big)
		{
			if (clt_nItemQty >= 100) clt_nItemQty = 1;
			else clt_nItemQty = clt_nItemQty * 10;
		}
		else
		{
			if (clt_nItemQty <= 1) clt_nItemQty = 100;
			else clt_nItemQty = clt_nItemQty / 10;
		}
		return;
	}
	if (nodName == "CLTI_CLR")
	{
		GameInterface.clt_edit.str = "";
		CLT_Items_OnEdit("");
		return;
	}
	if (findsubstr(nodName, "CLTI_CS", 0) == 0) return;
	if (findsubstr(nodName, "CLTI_C", 0) == 0)
	{
		k = sti(strcut(nodName, 6, strlen(nodName) - 1)) - 1;
		if (k < 0 || k >= clt_nItemCats) return;
		clt_nItemCat = k;
		clt_nItemPage = 0;
		CLT_Items_Collect();
		return;
	}
	if (findsubstr(nodName, "CLTI_B", 0) == 0)
	{
		k = sti(strcut(nodName, 6, strlen(nodName) - 1));
		if (k < 1 || k > CLT_ITEM_CELLS) return;
		if (big) CLT_Items_Give(clt_nItemPage * CLT_ITEM_CELLS + k - 1, clt_nItemQty * 10);
		else CLT_Items_Give(clt_nItemPage * CLT_ITEM_CELLS + k - 1, clt_nItemQty);
	}
}
