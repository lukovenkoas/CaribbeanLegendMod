# Steam Workshop — поля публикации

Загрузка — Storm Workshop Tool (Steam → Caribbean Legend → альтернативный запуск «Storm Workshop Tool»).

| Поле | Значение |
|---|---|
| Title | `CL Toolkit 1.9` |
| Contents folder | `D:\Progs\CaribbeanLegendMod\CLToolkit` (только эта папка: внутри `Program` и `Resource`) |
| Preview image | `poster.jpg` (2048×2048, 1,05 МБ — инструмент сам пересожмёт в JPEG ≈ 0,6 МБ, лимит Steam 1 МБ) |
| Visibility | сначала Private/Friends-only → проверка установки из Workshop → Public |
| Description | блок ниже (BBCode Steam) |
| Change notes | `Initial release / Первая версия` |

После первой загрузки игра скачает мод в `steamapps\workshop\content\2230980\<ID>`. Перед проверкой убрать
строку `mod_cltoolkit` из `GAME\overlays.txt`, иначе локальная копия и Workshop-копия займут одни пути и игра
отключит обе.

---

## Description (вставить целиком)

```
[h1]CL Toolkit 1.9[/h1]
[b]EN[/b] — A cheat / sandbox toolkit for Caribbean Legend 1.9.0. Press [b]Insert[/b] to open the window (the game pauses); Insert, ESC or the X button closes it. Works on land, at sea and in battle.
[i]Interface language: Russian.[/i]

[list]
[*][b]Hub[/b]: hero card, money, quick actions (max PIRATES, full heal, +500 ammo, repair ship), action log.
[*][b]Hero toggles[/b]: immortality, infinite energy, weapons always loaded, unlimited carry weight, cannons without reload, perfectly accurate broadsides, no ship-class penalty, no random encounters.
[*][b]Character[/b]: any party member (hero, companions, passengers) — max HP/energy, nobility, fame, authority, officer loyalty, hero rank.
[*][b]Skills[/b]: PIRATES, personal and ship skills, free points, background perks.
[*][b]Items[/b]: every item of the game by category, search, give any quantity; quest items are marked.
[*][b]Ship[/b]: change the ship [b]class (rank)[/b] to meet quest requirements; HP, capacity, speed, turn rate, hidden handling coefficients (weight dependency, inertia), crew and max crew, morale, cannons, repair, specialization and trait.
[*][b]Cargo[/b]: all 50 goods, add/remove.
[*][b]Modes[/b]: arcade sailing, sea teleport, fast travel in hostile towns, +1000 carry weight, hero damage multiplier x1–x100, fast regeneration, blows pierce any block, instant death of enemy ships, world-map speed x1/x2/x3, port ship storage limit 3/5/8/12, leave a battle without abandoning your fleet, unlock a Steam achievement by name.
[*][b]World[/b]: relations with nations, natives and smugglers, current location, date and time, random encounters on/off, finder of special (quest) fleets with an archipelago map.
[*][b]Inspector[/b]: read-only view of character and ship attributes.
[*]Squadron of up to [b]8 ships[/b]; ship coordinates on the sea HUD (same instruments as on the world map).
[/list]

[b]Compatibility[/b]
[list]
[*]Game version 1.9.0. Can be added to an existing save.
[*][b]Not compatible with the "Navigator 1.9" mod[/b] — CL Toolkit replaces it (same game files). Navigator toggles saved in your game keep working.
[*]Replaces game files: LAi_character.c, fightparams\common.c, weight.c, perks.c, characters.h, AICannon.c, companions.c, BattleInterface.c — not compatible with other mods that replace the same files.
[*]Before removing the mod, reduce your squadron to 5 ships.
[/list]

[hr][/hr]

[b]RU[/b] — Чит-тулкит / песочница для Caribbean Legend 1.9.0. Окно открывается клавишей [b]Insert[/b] (игра на паузе); закрыть — Insert, ESC или крестик. Работает на суше, в море и в бою.
[i]Язык интерфейса — русский.[/i]

[list]
[*][b]Главное окно[/b]: карточка героя, деньги, быстрые действия (PIRATES на максимум, восстановить здоровье и энергию, +500 боеприпасов, починить корабль), журнал.
[*][b]Переключатели героя[/b]: бессмертие, бесконечная энергия, оружие всегда заряжено, вес без ограничений, пушки без перезарядки, супер-точность залпа, без штрафа за класс корабля, без случайных встреч.
[*][b]Персонаж[/b]: любой член отряда (герой, компаньоны, пассажиры) — макс. здоровье и энергия, благородство, известность, авторитет, лояльность офицеров, ранг героя.
[*][b]Умения[/b]: PIRATES, личные и корабельные умения, свободные очки, перки предысторий.
[*][b]Предметы[/b]: все предметы игры по категориям, поиск, выдача любого количества; квестовые отмечены.
[*][b]Корабль[/b]: смена [b]класса (ранга)[/b] корабля под требования квестов; прочность, трюм, скорость, манёвренность, скрытые коэффициенты управления (зависимость от веса, инерция), команда и макс. команда, мораль, орудия, ремонт, специализация и свойство.
[*][b]Трюм[/b]: все 50 товаров, добавить/убрать.
[*][b]Режимы[/b]: аркадное плавание, морской телепорт, быстрые переходы во враждебных городах, +1000 к весу, множитель урона героя x1–x100, быстрая регенерация, удары пробивают любой блок, мгновенная смерть вражеских кораблей, скорость на глобальной карте x1/x2/x3, лимит стоянки кораблей в порту 3/5/8/12, выход из боя без бросания флота, открытие достижения Steam по названию.
[*][b]Мир[/b]: отношения наций, индейцев и контрабандистов, где мы, дата и время, случайные встречи вкл/выкл, поиск особых (квестовых) флотов со схемой архипелага.
[*][b]Инспектор[/b]: просмотр атрибутов персонажа и корабля (только чтение).
[*]Эскадра до [b]8 кораблей[/b]; координаты корабля на экране моря (по тем же приборам, что на глобальной карте).
[/list]

[b]Совместимость[/b]
[list]
[*]Версия игры 1.9.0. Можно ставить на текущее сохранение.
[*][b]Несовместим с модом «Навигатор 1.9»[/b] — CL Toolkit его заменяет (те же файлы игры). Включённые в «Навигаторе» переключатели в сейве продолжат работать.
[*]Заменяет файлы игры: LAi_character.c, fightparams\common.c, weight.c, perks.c, characters.h, AICannon.c, companions.c, BattleInterface.c — несовместим с модами, которые заменяют те же файлы.
[*]Перед удалением мода сократите эскадру до 5 кораблей.
[/list]

[hr][/hr]
Distributed under the terms of the [url=https://github.com/Blackmark-Studio/Caribbean-Legend/blob/main/LICENSE.md]Storm Engine EULA[/url]. / Распространяется на условиях Storm Engine EULA.
Source / Исходники: https://github.com/lukovenkoas/CaribbeanLegendMod
```
