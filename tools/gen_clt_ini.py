# Генератор разметки окна CL Toolkit (этап 4.1, оформление по образцу «Навигатора» 2.0).
# Запуск: python -I _mod_tools\gen_clt_ini.py  -> пишет mod_cltoolkit\Resource\INI\interfaces\mods\cltoolkit\clt_main.ini
# Ini без перезапуска игры не перечитывается, поэтому здесь только статичный пул узлов;
# подписи, видимость и картинки задаёт скрипт Program\cltoolkit\clt_window.c.
import os

OUT = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'mod_cltoolkit', 'Resource', 'INI',
                   'interfaces', 'mods', 'cltoolkit', 'clt_main.ini')
TEX = 'cltoolkit\\hub\\'
TEX_MAP = 'interfaces\\maps\\russian\\map_good.tga'   # картинка атласа (mapview.ini MAIN_MAP)

GP_COLS = 3
GP_ROWS = 15
TILES = 8
SEALS = 8
QUICK = 4
CARGO_COLS = 5
CARGO_ROWS = 10
ITEM_CATS = 22
ITEM_COLS = 9
ITEM_ROWS = 5
FLEETS = 20      # метки особых флотов на схеме (CLT_FLEET_MAX в clt_sec_fleet.c)

items = []      # строки [MAIN]
sections = []   # тексты секций

GOLD = '255,226,190,98'
PALE = '255,240,226,191'
DIM = '255,185,164,122'
TEXT = '255,232,216,176'
GREEN = '255,150,220,140'


def pos(x1, y1, x2, y2):
    return 'position = %d,%d,%d,%d' % (x1, y1, x2, y2)


def node(kind, name, body, prio=None):
    items.append(('item = %d,%s,%s' % (prio, kind, name)) if prio is not None else ('item = %s,%s' % (kind, name)))
    sections.append('[%s]\n%s\n' % (name, '\n'.join(body)))


def rect(name, box, argb, prio=None):
    node('RECTANGLE', name, [pos(*box), 'leftColor = ' + argb, 'topColor = ' + argb, 'rightColor = ' + argb,
                             'bottomColor = ' + argb, 'borderColor = 0,0,0,0'], prio)


def text(name, box, scale, space, align, color, prio=None):
    node('FORMATEDTEXT', name, [pos(*box), 'fontScale = %s' % scale, 'lineSpace = %d' % space,
                                'alignment = ' + align, 'Color = ' + color, 'bShowGlowCursor = 0'], prio)


def btn(name, box, scale=None, extra=(), prio=None):
    body = ['bBreakCommand = 1', 'command = click', 'command = activate', 'command = rclick',
            'command = deactivate,event:exitCancel', pos(*box), 'string = CLT_Empty']
    if scale is not None:
        body.append('fontScale = %s' % scale)
    body += list(extra)
    node('TEXTBUTTON4', name, body, prio)


def pic(name, box, tex, prio=None, group=None):
    # group — картинка из группы pictures.ini (textureName не задаётся, как SHIP_PERK1 в ship.ini)
    body = [pos(*box)]
    if group is None:
        body.append('textureName = ' + tex)
    else:
        body.append('groupName = ' + group)
        body.append('picName = empty')
    node('PICTURE', name, body, prio)


# ---------------------------------------------------------------- хаб
# Слева — карточка героя (без переключения персонажей), в центре — плитки разделов 4x2 и журнал под ними,
# справа — быстрые команды: переключатели героя, затем обычные кнопки.
rect('CLTH_FADER', (0, 0, 1920, 1080), '225,8,6,4', 0)
node('FRAME', 'CLTH_FRAME', [pos(30, 22, 1890, 1058)], 1)
node('FRAME_GOLD', 'CLTH_GOLD', [pos(30, 22, 1890, 1058)], 2)
text('CLTH_TITLE', (80, 42, 790, 98), 1.8, 40, 'left', GOLD)
text('CLTH_SUB', (80, 98, 790, 130), 0.9, 26, 'left', DIM)

btn('CLTH_QDN', (800, 54, 856, 100), 0.85)
node('PCEDITBOX', 'CLT_QTY', ['command = click,select:CLT_QTY', 'command = deactivate,event:exitCancel',
                              'command = activate,event:CLT_QtyConfirm', pos(860, 54, 1000, 98),
                              'strFont = bold_numbers', 'fontScale = 1.1', 'argbFontColor = 255,255,255,255',
                              'stringLength = 7', 'stringWidth = 130', 'stringalign = center', 'stringoffset = 0,12',
                              'MiddleImage = color=(255,128,128,128), piclist=le_icons, picname=editbox',
                              'excludechars = *?.-_|/qwertyuiop[]asdfghjkl\'zxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM ,+=!@#$%^&()`"{}:<>;'])
btn('CLTH_QUP', (1004, 54, 1060, 100), 0.85)
text('CLTH_QL', (800, 100, 1060, 126), 0.75, 20, 'center', DIM)
btn('CLTH_MDN', (1080, 52, 1136, 100))
text('CLTH_MONEY', (1136, 60, 1346, 100), 1.15, 30, 'center', PALE)
btn('CLTH_MUP', (1346, 52, 1402, 100))
btn('CLTH_DDN', (1420, 52, 1476, 100))
text('CLTH_DUB', (1476, 60, 1646, 100), 1.15, 30, 'center', PALE)
btn('CLTH_DUP', (1646, 52, 1702, 100))
btn('CLTH_CLOSE', (1720, 52, 1860, 104), 0.9)
pic('CLTH_LINE', (70, 134, 1850, 140), TEX + 'line.tga')

# карточка героя
pic('CLTH_CARD', (70, 156, 500, 1000), TEX + 'card.tga')
pic('CLTH_FACE', (135, 176, 435, 476), 'interfaces\\le\\portraits\\256\\face_1.tga')
pic('CLTH_FACEFR', (127, 168, 443, 484), TEX + 'faceframe.tga')
text('CLTH_NAME', (84, 492, 486, 536), 1.4, 30, 'center', PALE)
text('CLTH_RANKT', (85, 536, 485, 566), 0.95, 30, 'center', DIM)
text('CLTH_R1T', (95, 584, 380, 612), 1.0, 28, 'left', TEXT)
btn('CLTH_R1D', (380, 576, 428, 618))
btn('CLTH_R1U', (432, 576, 480, 618))
text('CLTH_R2T', (95, 630, 380, 658), 1.0, 28, 'left', TEXT)
btn('CLTH_R2D', (380, 622, 428, 664))
btn('CLTH_R2U', (432, 622, 480, 664))
pic('CLTH_INFOLINE', (90, 674, 480, 678), TEX + 'line.tga')
text('CLTH_INFO', (95, 680, 485, 990), 0.82, 26, 'left', TEXT)

# плитки разделов 4x2; кнопка под картинкой плитки ловит щелчок
for i in range(1, TILES + 1):
    c, r = (i - 1) % 4, (i - 1) // 4
    x, y = 530 + c * 232, 156 + r * 286
    btn('CLTH_TC%d' % i, (x, y, x + 212, y + 266), extra=('selectButtonMiddle = textbutton0',))
    pic('CLTH_TB%d' % i, (x, y, x + 212, y + 266), TEX + 'tile.tga')
    pic('CLTH_TI%d' % i, (x + 56, y + 36, x + 156, y + 136), TEX + 'i_hero.tga')
    text('CLTH_TL%d' % i, (x + 6, y + 150, x + 206, y + 190), 1.25, 30, 'center', PALE)
    text('CLTH_TS%d' % i, (x + 10, y + 192, x + 202, y + 256), 0.8, 22, 'center', DIM)

# журнал — под плитками
pic('CLTH_LOGBG', (530, 730, 1458, 1000), TEX + 'card.tga')
text('CLTH_LOGT', (556, 746, 1430, 782), 1.15, 28, 'left', GOLD)
text('CLTH_LOG', (556, 788, 1430, 990), 0.85, 25, 'left', TEXT)

# быстрые команды: переключатели героя (печать + кнопка), затем кнопки
text('CLTH_ST', (1480, 156, 1850, 196), 1.3, 32, 'left', GOLD)
for i in range(1, SEALS + 1):
    y = 200 + (i - 1) * 52
    pic('CLTH_SEAL%d' % i, (1480, y + 3, 1524, y + 47), TEX + 'seal_off.tga')
    btn('CLTH_TG%d' % i, (1530, y, 1850, y + 50), 0.85)
text('CLTH_QT', (1480, 212 + SEALS * 52, 1850, 252 + SEALS * 52), 1.3, 32, 'left', GOLD)
for i in range(1, QUICK + 1):
    y = 256 + SEALS * 52 + (i - 1) * 58
    btn('CLTH_Q%d' % i, (1480, y, 1850, y + 52), 0.85)
text('CLTH_HINT', (70, 1010, 1850, 1046), 0.85, 24, 'center', DIM)

# ---------------------------------------------------------------- страница раздела (3 колонки x 15 строк)
rect('CLTP_BG', (0, 0, 1920, 1080), '255,12,9,6', 100)
node('FRAME', 'CLTP_FRAME', [pos(30, 22, 1890, 1058)], 101)
node('FRAME_GOLD', 'CLTP_GOLD', [pos(30, 22, 1890, 1058)], 102)
pic('CLTP_LINE', (70, 134, 1850, 140), TEX + 'line.tga', 110)
text('CLTP_TITLE', (80, 40, 1380, 100), 2.0, 40, 'left', GOLD, 115)
text('CLTP_SUB', (80, 98, 1560, 128), 0.95, 26, 'left', DIM, 115)
btn('CLTP_CPREV', (1400, 52, 1450, 104), 1.3, prio=120)
btn('CLTP_CNEXT', (1455, 52, 1505, 104), 1.3, prio=120)
btn('CLTP_BACK', (1610, 52, 1860, 104), 1.0, prio=120)

for c in range(GP_COLS):
    bx = 84 + c * 610
    text('CLTG_CH%d' % c, (bx + 4, 156, bx + 558, 196), 1.3, 32, 'left', GOLD, 115)
    for r in range(GP_ROWS):
        y = 204 + r * 52
        n = '%d_%d' % (c, r)
        pic('CLTG_S' + n, (bx, y + 5, bx + 40, y + 45), TEX + 'seal_off.tga', 110)
        btn('CLTG_B' + n, (bx + 44, y, bx + 564, y + 48), 0.85, prio=120)
        text('CLTG_L' + n, (bx + 4, y + 12, bx + 208, y + 46), 0.9, 26, 'left', PALE, 115)
        text('CLTG_V' + n, (bx + 212, y + 14, bx + 464, y + 46), 0.8, 24, 'right', GREEN, 115)
        btn('CLTG_D' + n, (bx + 470, y + 3, bx + 514, y + 47), prio=120)
        btn('CLTG_U' + n, (bx + 520, y + 3, bx + 564, y + 47), prio=120)
        text('CLTG_H' + n, (bx + 4, y + 16, bx + 558, y + 48), 0.8, 22, 'left', GOLD, 115)

# картинка корабля (раздел «Корабль»): над строками 0-7 левой колонки, строки оставляются пустыми
pic('CLTP_SHIPCARD', (100, 200, 632, 616), TEX + 'card.tga', 105)
pic('CLTP_SHIPPIC', (158, 204, 574, 612), 'interfaces\le\ships\empty_ship.tga', 110)

# модификаторы корабля (раздел «Корабль», правая колонка, вместо блока парусов): селекторы
# специализации (Spec) и свойства (Traits), их значки из группы PERKS_SHIPS базовой игры
# (pictures.ini, как SHIP_PERK1 в ship.ini) и описание выбранного свойства (ShipsPerksDescribe.txt)
BX2 = 84 + 2 * 610   # левый край правой колонки сетки
text('CLTP_STXT', (BX2 + 4, 724, BX2 + 462, 760), 0.95, 26, 'left', PALE, 115)
btn('CLTP_SD', (BX2 + 470, 719, BX2 + 514, 763), prio=120)
btn('CLTP_SU', (BX2 + 520, 719, BX2 + 564, 763), prio=120)
text('CLTP_TTXT', (BX2 + 4, 776, BX2 + 462, 812), 0.95, 26, 'left', PALE, 115)
btn('CLTP_TD', (BX2 + 470, 771, BX2 + 514, 815), prio=120)
btn('CLTP_TU', (BX2 + 520, 771, BX2 + 564, 815), prio=120)
pic('CLTP_SBKG', (BX2 + 190, 828, BX2 + 270, 908), '', 105, 'PERKS_SHIPS')
pic('CLTP_SPIC', (BX2 + 190, 828, BX2 + 270, 908), '', 110, 'PERKS_SHIPS')
pic('CLTP_SBRD', (BX2 + 190, 828, BX2 + 270, 908), '', 111, 'PERKS_SHIPS')
pic('CLTP_TBKG', (BX2 + 290, 828, BX2 + 370, 908), '', 105, 'PERKS_SHIPS')
pic('CLTP_TPIC', (BX2 + 290, 828, BX2 + 370, 908), '', 110, 'PERKS_SHIPS')
pic('CLTP_TBRD', (BX2 + 290, 828, BX2 + 370, 908), '', 111, 'PERKS_SHIPS')
text('CLTP_MODDESC', (BX2 + 4, 920, BX2 + 554, 1080), 0.78, 20, 'left', TEXT, 115)

# портрет со стрелками (раздел «Персонаж»): над строками 0-7 левой колонки, строки оставляются пустыми
pic('CLTP_FACECARD', (100, 200, 632, 616), TEX + 'card.tga', 105)
pic('CLTP_FACE', (216, 212, 516, 512), 'interfaces\\le\\portraits\\256\\face_1.tga', 110)
pic('CLTP_FACEFR', (208, 204, 524, 520), TEX + 'faceframe.tga', 111)
btn('CLTP_FPREV', (110, 534, 166, 590), 1.4, prio=120)
text('CLTP_FNAME', (170, 544, 562, 586), 1.3, 30, 'center', PALE, 115)
btn('CLTP_FNEXT', (566, 534, 622, 590), 1.4, prio=120)

# трюм (раздел «Трюм»): 5 x 10 ячеек — значок товара, название, количество, - +
# справа — селектор шага (сколько добавляют «- +» у товаров): /10, значение, x10 — как поле «Количество» в хабе
text('CLTC_HT', (84, 156, 1200, 196), 1.3, 32, 'left', GOLD, 115)
btn('CLTC_QDN', (1220, 152, 1290, 198), 0.85, prio=120)
text('CLTC_QV', (1294, 156, 1766, 198), 1.0, 28, 'center', GREEN, 115)
btn('CLTC_QUP', (1770, 152, 1840, 198), 0.85, prio=120)
for k in range(CARGO_COLS * CARGO_ROWS):
    c, r = k // CARGO_ROWS, k % CARGO_ROWS
    x, y = 84 + c * 352, 204 + r * 78
    n = '%d' % (k + 1)
    pic('CLTC_I' + n, (x, y + 4, x + 64, y + 68), 'interfaces\le\portraits\store.tga', 110)
    text('CLTC_N' + n, (x + 70, y + 6, x + 344, y + 32), 0.75, 20, 'left', PALE, 115)
    text('CLTC_Q' + n, (x + 70, y + 40, x + 226, y + 70), 0.85, 24, 'left', GREEN, 115)
    btn('CLTC_D' + n, (x + 230, y + 34, x + 284, y + 74), prio=120)
    btn('CLTC_U' + n, (x + 290, y + 34, x + 344, y + 74), prio=120)

# предметы (раздел «Предметы», по образцу «Навигатора» 2.0): слева список категорий во всю высоту,
# сверху поиск / количество / найдено, в центре сетка ITEM_COLS x ITEM_ROWS (значок, подпись, «есть N»,
# печать у квестовых), снизу листание. Кнопка ячейки лежит под картинками и ловит щелчок (как плитки хаба).
pic('CLTI_LBG', (60, 150, 412, 990), TEX + 'card.tga', 104)
for i in range(1, ITEM_CATS + 1):
    y = 160 + (i - 1) * 36
    pic('CLTI_CS%d' % i, (74, y + 4, 102, y + 32), TEX + 'seal_on.tga', 110)
    btn('CLTI_C%d' % i, (106, y, 404, y + 34), 0.85, prio=120)
text('CLTI_FL', (430, 160, 530, 194), 1.0, 28, 'left', GOLD, 115)
btn('CLTI_CLR', (1010, 152, 1150, 198), 0.8, prio=120)
btn('CLTI_QTY', (1166, 152, 1446, 198), 0.85, prio=120)
text('CLTI_FOUND', (1460, 162, 1840, 196), 0.9, 26, 'right', DIM, 115)
for k in range(ITEM_COLS * ITEM_ROWS):
    c, r = k % ITEM_COLS, k // ITEM_COLS
    x, y = 430 + c * 156, 206 + r * 148
    n = '%d' % (k + 1)
    btn('CLTI_B' + n, (x + 2, y, x + 154, y + 146), extra=('selectButtonMiddle = textbutton0',), prio=106)
    pic('CLTI_F' + n, (x + 28, y + 4, x + 128, y + 104), TEX + 'card.tga', 108)
    pic('CLTI_I' + n, (x + 34, y + 10, x + 122, y + 98), 'interfaces\le\portraits\store.tga', 110)
    pic('CLTI_M' + n, (x + 6, y + 4, x + 34, y + 32), TEX + 'seal_on.tga', 112)
    text('CLTI_Q' + n, (x + 96, y + 6, x + 152, y + 28), 0.7, 18, 'right', GREEN, 115)
    text('CLTI_N' + n, (x + 4, y + 106, x + 152, y + 146), 0.66, 17, 'center', PALE, 115)
btn('CLTI_PREV', (430, 948, 640, 990), 0.9, prio=120)
text('CLTI_PAGE', (650, 956, 1610, 988), 0.95, 26, 'center', TEXT, 115)
btn('CLTI_NEXT', (1620, 948, 1834, 990), 0.9, prio=120)

# поле фильтра (раздел «Предметы»): сверху над сеткой
node('PCEDITBOX', 'CLT_EDIT', ['command = click,select:CLT_EDIT', 'command = deactivate,event:exitCancel',
                               'command = activate,event:CLT_EditConfirm', pos(530, 152, 994, 198),
                               'strFont = interface_normal', 'fontScale = 1.0', 'argbFontColor = 255,255,255,255',
                               'stringLength = 64', 'stringWidth = 440', 'stringalign = center', 'stringoffset = 0,10',
                               'MiddleImage = color=(255,128,128,128), piclist=le_icons, picname=editbox'], 120)
# схема особых флотов (страница CLT_SEC_FLEETMAP, clt_sec_fleet.c): картинка атласа map_good.tga
# в пропорциях MAIN_MAP атласа (1330x900), метки (печать + номер + флаг нации из группы NATIONS
# базового pictures.ini) и герой — позиции ставит скрипт; справа список
pic('CLTM_MAP', (80, 150, 1321, 990), TEX_MAP, 105)
pic('CLTM_LBG', (1336, 150, 1850, 990), TEX + 'card.tga', 104)
text('CLTM_LIST', (1352, 166, 1836, 980), 0.8, 24, 'left', TEXT, 115)
for i in range(1, FLEETS + 1):
    pic('CLTM_P%d' % i, (0, 0, 32, 32), TEX + 'seal_on.tga', 112)
    pic('CLTM_F%d' % i, (0, 0, 28, 28), '', 112, 'NATIONS')
    text('CLTM_N%d' % i, (0, 0, 60, 28), 1.1, 26, 'left', PALE, 116)
pic('CLTM_ME', (0, 0, 40, 40), TEX + 'i_ship.tga', 113)

text('CLTG_OUT', (80, 992, 1840, 1050), 0.85, 24, 'center', TEXT, 115)

head = ''';;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; CL Toolkit — window layout (stage 4.1: hub + section page, after the "Navigator" mod).
;; GENERATED by _mod_tools\\gen_clt_ini.py — edit the generator, not this file.
;; The ini is NOT re-read without a game restart (_Mod_Research.md §13.5): it only
;; declares a pool of nodes. Labels, visibility and pictures are set from
;; Program\\cltoolkit\\clt_window.c (hot-reloaded on each open).
;; Coordinates: 1920x1080 window space.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

'''
with open(OUT, 'w', encoding='utf-8', newline='\r\n') as f:
    f.write(head)
    f.write('[MAIN]\n' + '\n'.join(items) + '\nstart = CLTH_TC1\n\n')
    f.write('\n'.join(sections))
print(OUT, len(items), 'nodes')
