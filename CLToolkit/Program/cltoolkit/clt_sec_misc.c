// CL Toolkit — раздел «Режимы»: флаги, которые штатное Cheat Menu ставит на pchar.
// Для каждого флага указано, где его читает базовый код (проверено поиском по Program\);
// эффекты проверены в игре на этапе 3 (_Mod_Research.md §14.4 п.8).
// cheats.gundamage и cheats.guncharge ставятся Cheat Menu, но нигде не читаются — не включены.

// Таблица достижений вшита ниже между маркерами (НЕ отдельным файлом: работающая игра
// подхватывает только изменение содержимого существующих файлов, новый файл — «can't load file»
// до рестарта; error.log в Documents\My Games\Caribbean Legend\Logs).
// ==== ACH TABLE BEGIN ====
// Таблица достижений. СГЕНЕРИРОВАНО _mod_tools\gen_ach_table.py
// из кэша схемы Steam (appcache\stats\UserGameStatsSchema_2230980.bin).
// Не править руками — перегенерировать скриптом. Ключи поиска — в нижнем регистре:
// EN-имя | RU-имя | EN-описание | RU-описание | API-имя.
// statNum/statMax — связанный стат для синхронизации прогресса (0 — нет).

#define CLT_ACH_N		251

string clt_achKey[CLT_ACH_N];		// ключи поиска (нижний регистр)
string clt_achApi[CLT_ACH_N];		// API-имя для Achievment_Set / GetAchievement
string clt_achName[CLT_ACH_N];		// "EN / RU" для показа
int clt_achStatNum[CLT_ACH_N];		// связанный стат (0 — нет)
int clt_achStatMax[CLT_ACH_N];		// максимум стата по коду игры
int clt_achCount;

void CLT_Ach_Add(string keys, string api, string name, int statNum, int statMax)
{
	if (clt_achCount >= CLT_ACH_N) return;
	clt_achKey[clt_achCount] = keys;
	clt_achApi[clt_achCount] = api;
	clt_achName[clt_achCount] = name;
	clt_achStatNum[clt_achCount] = statNum;
	clt_achStatMax[clt_achCount] = statMax;
	clt_achCount++;
}

void CLT_Ach_Init()
{
	clt_achCount = 0;
	CLT_Ach_Add("dating sim|симулятор свиданий|complete 'rumba' quest|пройти квест «румба»|ach_cl_82", "ach_CL_82", "Dating Sim / Симулятор свиданий", 0, 0);
	CLT_Ach_Add("my soulmate|моя судьба|achieve the maximum relationship score with helen|добиться максимального уровня отношений с элен|ach_cl_83", "ach_CL_83", "My Soulmate / Моя судьба", 0, 0);
	CLT_Ach_Add("very ******* funny!|шутник, *****!|return the rainbow to helen|вернуть элен «радугу»|ach_cl_84", "ach_CL_84", "Very ******* funny! / Шутник, *****!", 0, 0);
	CLT_Ach_Add("my temple, my rules!|бан за критику|infuriate aruba|поссориться с арубой|ach_cl_85", "ach_CL_85", "My temple, my rules! / Бан за критику", 0, 0);
	CLT_Ach_Add("vile little god|мерзкий божок|complete vile little god side quest|пройти квест «мерзкий божок»|ach_cl_86", "ach_CL_86", "Vile Little God / Мерзкий божок", 0, 0);
	CLT_Ach_Add("clean hands|чистые руки|pay aruba 30 000 doubloons in total|суммарно заплатить 30 тысяч дублонов арубе|ach_cl_87", "ach_CL_87", "Clean Hands / Чистые руки", 0, 0);
	CLT_Ach_Add("deadman|мертвец|hercule got his redemption|эркюль получил своё искупление|ach_cl_91", "ach_CL_91", "Deadman / Мертвец", 0, 0);
	CLT_Ach_Add("hogar, dulce hogar|hogar, dulce hogar|build up your colony to the maximum|отстроить свою колонию до конца|ach_cl_92", "ach_CL_92", "Hogar, dulce hogar / Hogar, dulce hogar", 0, 0);
	CLT_Ach_Add("all your base are belong to us|день независимости|destroy the invaders on isla mona|выгнать захватчиков с исла моны|ach_cl_93", "ach_CL_93", "All your base are belong to us / День независимости", 0, 0);
	CLT_Ach_Add("el comercio|el comercio|buy out five batches of ironwood from your factory|пять раз выкупить бакаут со своей фактории|ach_cl_94", "ach_CL_94", "El Comercio / El Comercio", 94, 5);
	CLT_Ach_Add("point blank|в упор!|shoot three people during a dialogue|застрелить троих персонажей в диалоге|ach_cl_95", "ach_CL_95", "Point Blank / В упор!", 95, 3);
	CLT_Ach_Add("first of many|первый из многих|collect your first adder stone|поднять куриного бога в первый раз|ach_cl_96", "ach_CL_96", "First of many / Первый из многих", 0, 0);
	CLT_Ach_Add("no regrets|никаких сожалений|don't repent for the sins of charlie prince|не раскаиваться в грехах чарли принца|ach_cl_97", "ach_CL_97", "No Regrets / Никаких сожалений", 0, 0);
	CLT_Ach_Add("money do buy happiness|деньги-деньги-дребеденьги|acquire 1 million pesos in the sandbox mode|заработать миллион песо в свободной игре|ach_cl_98", "ach_CL_98", "Money do buy happiness / Деньги-деньги-дребеденьги", 0, 0);
	CLT_Ach_Add("live by the sword|не мы такие, жизнь такая|plunder a town in the sandbox mode|разграбить свой первый город в свободной игре|ach_cl_99", "ach_CL_99", "Live By the Sword / Не мы такие, жизнь такая", 0, 0);
	CLT_Ach_Add("the most notorious pirate|самый известный пират в этих водах|complete every side quest in the sandbox mode|выполнить все побочные квесты в свободной игре|ach_cl_100", "ach_CL_100", "The Most Notorious Pirate / Самый известный пират в этих водах", 0, 0);
	CLT_Ach_Add("lootbox hunter|охотник за лутбоксами|find 20 pirate treasures in the sandbox mode|найти 20 кладов в свободной игре|ach_cl_101", "ach_CL_101", "Lootbox Hunter / Охотник за лутбоксами", 101, 20);
	CLT_Ach_Add("a*!|оценка пять!|get every achievement in the sandbox mode. a true caribbean legend!|открыть все достижения в свободной игре|ach_cl_102", "ach_CL_102", "A*! / Оценка пять!", 0, 0);
	CLT_Ach_Add("honoring the classics|это классика!|complete the story on the hardest difficuly with the classic mode on. thank you!|пройти игру на высшем уровне сложности в классическом режиме. спасибо вам.|ach_cl_103", "ach_CL_103", "Honoring the Classics / Это классика!", 0, 0);
	CLT_Ach_Add("caribbean virgin|карибский девственник|didn't have sex with anyone in the story mode.|ни разу ни с кем не переспать в сюжетном режиме|ach_cl_104", "ach_CL_104", "Caribbean Virgin / Карибский девственник", 0, 0);
	CLT_Ach_Add("baptism of fire|боевое крещение|gain your first victory in the battle at sea in the story mode|первая победа в морском сражении в сюжетном режиме|ach_01", "ach_01", "Baptism of fire / Боевое крещение", 0, 0);
	CLT_Ach_Add("boarder|абордажник|board 250 ships|взять на абордаж 250 кораблей|ach_02", "ach_02", "Boarder / Абордажник", 2, 250);
	CLT_Ach_Add("sea wolf|морской волк|destroy 500 ships with artillery fire|уничтожить артиллерийским огнем 500 кораблей|ach_03", "ach_03", "Sea Wolf / Морской волк", 3, 500);
	CLT_Ach_Add("gentleman of fortune|джентльмен удачи|completely destroy the gold fleet|полностью уничтожить золотой флот|ach_04", "ach_04", "Gentleman of Fortune / Джентльмен удачи", 0, 0);
	CLT_Ach_Add("a living legend|живая легенда|get to level 30|достичь 30-го ранга|ach_05", "ach_05", "A Living legend / Живая легенда", 0, 0);
	CLT_Ach_Add("covered in scars|покрытый шрамами|sustain over 10,000 damage “health” points|получить урон более 10'000 в общей сложности|ach_06", "ach_06", "Covered in scars / Покрытый шрамами", 6, 10000);
	CLT_Ach_Add("alchemist|алхимик|prepare over 200 potions or other objects|изготовить более 200 зелий или предметов|ach_07", "ach_07", "Alchemist / Алхимик", 7, 200);
	CLT_Ach_Add("treasure hunter|кладоискатель|find 100 pieces of treasure|обнаружить 100 кладов|ach_08", "ach_08", "Treasure hunter / Кладоискатель", 8, 100);
	CLT_Ach_Add("pugwash|врунгель|win the regatta|победить в регате|ach_09", "ach_09", "Pugwash / Врунгель", 0, 0);
	CLT_Ach_Add("home sweet home|home sweet home|become a house owner|получить дом в собственность|ach_10", "ach_10", "Home sweet home / Home sweet home", 0, 0);
	CLT_Ach_Add("lucky jim|баловень судьбы|develop your “luck” skill up to 100%|развить умение «везение» до 100%|ach_11", "ach_11", "Lucky Jim / Баловень судьбы", 0, 0);
	CLT_Ach_Add("swordsman|фехтовальщик|develop your light weapons skill up to 100%|развить умение «легкое оружие» до 100%|ach_12", "ach_12", "Swordsman / Фехтовальщик", 0, 0);
	CLT_Ach_Add("hussar|гусар|develop your medium weapons skill up to 100%|развить умение «среднее оружие» до 100%|ach_13", "ach_13", "Hussar / Гусар", 0, 0);
	CLT_Ach_Add("dragoon|драгун|develop your heavy weapons skill up to 100%|развить умение «тяжелое оружие» до 100%|ach_14", "ach_14", "Dragoon / Драгун", 0, 0);
	CLT_Ach_Add("spy|шпион|develop your “stealth” skill up to 100%|развить умение «скрытность» до 100%|ach_15", "ach_15", "Spy / Шпион", 0, 0);
	CLT_Ach_Add("die hard|крепкий орешек|develop your “defence” skill up to 100%|развить умение «защита» до 100%|ach_16", "ach_16", "Die hard / Крепкий орешек", 0, 0);
	CLT_Ach_Add("admiral|адмирал|develop your “navigation” skill up to 100%|развить умение «навигация» до 100%|ach_17", "ach_17", "Admiral / Адмирал", 0, 0);
	CLT_Ach_Add("merchant|негоциант|develop your “trade” skill up to 100%|развить умение «торговля» до 100%|ach_18", "ach_18", "Merchant / Негоциант", 0, 0);
	CLT_Ach_Add("jedi|джедай|increase your reputation to the highest level|достичь максимальной репутации|ach_19", "ach_19", "Jedi / Джедай", 0, 0);
	CLT_Ach_Add("sith|ситх|lower your reputation to the minimum level|достичь минимальной репутации|ach_20", "ach_20", "Sith / Ситх", 0, 0);
	CLT_Ach_Add("butcher|душегуб|lose 9,999 sailors in skirmishes with enemy ships|потерять погибшими в схватках 9'999 матросов в экипаже корабля протагониста|ach_21", "ach_21", "Butcher / Душегуб", 21, 9999);
	CLT_Ach_Add("veteran|ветеран|destroy 15 forts|разгромить 15 фортов|ach_22", "ach_22", "Veteran / Ветеран", 22, 15);
	CLT_Ach_Add("empty-headed|ветреный|visit a brothel 100 times|«посетить» бордель 100 раз|ach_23", "ach_23", "Empty-headed / Ветреный", 23, 100);
	CLT_Ach_Add("guest|постоялец|stay a night in a tavern 50 times|заночевать в таверне 50 раз|ach_24", "ach_24", "Guest / Постоялец", 24, 50);
	CLT_Ach_Add("gambler|азартный игрок|win 500 times in cards/dice|выиграть 500 партий в карты/кости|ach_25", "ach_25", "Gambler / Азартный игрок", 25, 500);
	CLT_Ach_Add("loser|транжира|lose 500 card games/games of dice|проиграть 500 партий в карты/кости|ach_26", "ach_26", "Loser / Транжира", 26, 500);
	CLT_Ach_Add("sniper|снайпер|shoot 500 enemies|уничтожить из огнестрельного оружия 500 противников|ach_27", "ach_27", "Sniper / Снайпер", 27, 500);
	CLT_Ach_Add("skinflint|скряга|build up your fortune of 10,000,000 peso|сколотить капитал 10'000'000 песо|ach_28", "ach_28", "Skinflint / Скряга", 0, 0);
	CLT_Ach_Add("kind hearted|доброе сердце|take 100 shipwrecked sailors into your crew|взять в команду 100 потерпевших кораблекрушение|ach_29", "ach_29", "Kind hearted / Доброе сердце", 29, 100);
	CLT_Ach_Add("naval commander|флотоводец|muster the biggest squadron|собрать максимальную эскадру|ach_30", "ach_30", "Naval commander / Флотоводец", 0, 0);
	CLT_Ach_Add("smuggler|контрабандист|strike 50 deals with smugglers|совершить 50 сделок с контрабандистами|ach_31", "ach_31", "Smuggler / Контрабандист", 31, 50);
	CLT_Ach_Add("sickly|болезненный|reduce your constitution reading to the absolute minimum|понизить показатель «здоровье» до минимально возможного|ach_32", "ach_32", "Sickly / Болезненный", 0, 0);
	CLT_Ach_Add("collector|коллекционер|collect 100 treasures|собрать коллекцию из 100 сокровищ|ach_33", "ach_33", "Collector / Коллекционер", 0, 0);
	CLT_Ach_Add("destroy the enemies|гибель врагам|slay 500 military personnel|уничтожить 500 военнослужащих|ach_34", "ach_34", "Destroy the enemies / Гибель врагам", 34, 500);
	CLT_Ach_Add("merciless|безжалостный|slay 50 civilians|уничтожить 50 гражданских лиц|ach_35", "ach_35", "Merciless / Безжалостный", 35, 50);
	CLT_Ach_Add("conquistador|конкистадор|slay 250 indians|уничтожить 250 индейцев|ach_36", "ach_36", "Conquistador / Конкистадор", 36, 250);
	CLT_Ach_Add("exorcist|экзорцист|slay 100 evil spirits|уничтожить 100 экземпляров нечисти|ach_37", "ach_37", "Exorcist / Экзорцист", 37, 100);
	CLT_Ach_Add("endangered species|красная книга|kill 35 crabs|уничтожить 35 крабов|ach_38", "ach_38", "Endangered species / Красная книга", 38, 35);
	CLT_Ach_Add("trader|торговец|earn 10,000,000 peso by trading or re-sales|заработать торговлей/перепродажей товаров 10'000'000 песо|ach_39", "ach_39", "Trader / Торговец", 39, 10000000);
	CLT_Ach_Add("explorer|исследователь|explore 50 dungeons, caves, grottos and mines|исследовать 50 подземелий, пещер, гротов и выработок|ach_40", "ach_40", "Explorer / Исследователь", 40, 50);
	CLT_Ach_Add("generous|щедрый|give alms to the poor 200 times|раздать милостыню нищим 200 раз|ach_41", "ach_41", "Generous / Щедрый", 41, 200);
	CLT_Ach_Add("money lender|ростовщик|open accounts with money lenders totalling 5,000,000 peso|открыть вклады у ростовщиков в общей сложности на 5'000'000 песо|ach_42", "ach_42", "Money lender / Ростовщик", 0, 0);
	CLT_Ach_Add("slave trader|работорговец|sell 5,000 slaves|продать 5'000 рабов|ach_43", "ach_43", "Slave trader / Работорговец", 43, 5000);
	CLT_Ach_Add("wind of change|ветер перемен|successfully complete the story quest “dutch gambit” in cooperation with g.w.c.|успешно выполнить сюжетный квест «голландский гамбит», сотрудничая с гвик|ach_44", "ach_44", "Wind of change / Ветер перемен", 0, 0);
	CLT_Ach_Add("at the service of the empire|на службе империи|successfully complete the story quest “dutch gambit” in cooperation with britain|успешно выполнить сюжетный квест «голландский гамбит» на стороне англии|ach_45", "ach_45", "At the service of the Empire / На службе Империи", 0, 0);
	CLT_Ach_Add("fearless|не зная страха|successfully complete the story quest “dutch gambit” in opposition to all other parties|разыграть «голландский гамбит», противостоя всем прочим заинтересованным сторонам|ach_46", "ach_46", "Fearless / Не зная страха", 0, 0);
	CLT_Ach_Add("guileless|простодушный|preserve kukulkan’s world|сохранить реальность кукулькана|ach_47", "ach_47", "Guileless / Простодушный", 0, 0);
	CLT_Ach_Add("diligent|усердный|destroy kukulkan’s world|разрушить реальность кукулькана|ach_48", "ach_48", "Diligent / Усердный", 0, 0);
	CLT_Ach_Add("sea devil|морской дьявол|become the captain of the legendary ghost-ship|стать капитаном легендарного корабля-призрака|ach_49", "ach_49", "Sea devil / Морской дьявол", 0, 0);
	CLT_Ach_Add("buccaneer|морской разбойник|fight under the jolly roger and send 100 ships to the bottom|отправить на дно более 100 кораблей, подняв в бою «весёлый роджер»|ach_50", "ach_50", "Buccaneer / Морской разбойник", 50, 100);
	CLT_Ach_Add("lesson learnt|урок усвоен|successfully complete 'the final lesson' quest|успешно выполнить несюжетный квест «последний урок»|ach_51", "ach_51", "Lesson Learnt / Урок усвоен", 0, 0);
	CLT_Ach_Add("vigilante|законник|slay 250 criminals|уничтожить 250 разбойников|ach_52", "ach_52", "Vigilante / Законник", 52, 250);
	CLT_Ach_Add("terror of the archipelago|гроза архипелага|eliminate 50 pirate captains|уничтожить 50 пиратских капитанов|ach_53", "ach_53", "Terror of the archipelago / Гроза Архипелага", 53, 50);
	CLT_Ach_Add("traveler|путешественник|visit every tavern and colony|посетить все колонии и таверны|ach_54", "ach_54", "Traveler / Путешественник", 0, 0);
	CLT_Ach_Add("dream team|команда профессионалов|fill every officer positions|назначить офицеров на все корабельные должности|ach_55", "ach_55", "Dream team / Команда профессионалов", 0, 0);
	CLT_Ach_Add("on a slippery road|на скользком пути|do a special mission for the pirate code keeper|выполнить специальное поручение хранителя пиратского кодекса|ach_57", "ach_57", "On a slippery road / На скользком пути", 0, 0);
	CLT_Ach_Add("fisher|ловильщик|gather 50 barrels from the sea|подобрать в открытом море 50 дрейфующих бочонков|ach_59", "ach_59", "Fisher / Ловильщик", 59, 50);
	CLT_Ach_Add("heart of gold|благодушный|save 20 characters, drifting in the open sea|спасти 20 персонажей, бороздящих местные воды на шлюпках|ach_60", "ach_60", "Heart of gold / Благодушный", 60, 20);
	CLT_Ach_Add("miner|минёр|plant 200 mines during combat|установить 200 мин в бою|ach_61", "ach_61", "Miner / Минёр", 61, 200);
	CLT_Ach_Add("herbalist|травник|make three mangarosa potions|приготовить три зелья из мангаросы|ach_62", "ach_62", "Herbalist / Травник", 0, 0);
	CLT_Ach_Add("best client|выгодный клиент|order repair works on every shipyard|заказать ремонт на всех верфях архипелага|ach_63", "ach_63", "Best client / Выгодный клиент", 0, 0);
	CLT_Ach_Add("shipwright|корабел|repair 50,000 hull points at sea|восстановить 50 000 корпуса ремонтом в открытом море|ach_64", "ach_64", "Shipwright / Корабел", 64, 50000);
	CLT_Ach_Add("аntiquarian|антиквар|return a collection of persian swords to it's owner|вернуть утраченную персидскую коллекцию настоящему владельцу|ach_65", "ach_65", "Аntiquarian / Антиквар", 0, 0);
	CLT_Ach_Add("engineer|конструктор|upgrade your ship|модернизировать судно|ach_66", "ach_66", "Engineer / Конструктор", 0, 0);
	CLT_Ach_Add("goof|профессиональный риск|buy five fake treasure maps|приобрести пять фальшивых карт пиратских кладов|ach_68", "ach_68", "Goof / Профессиональный риск", 68, 5);
	CLT_Ach_Add("flying the jolly roger|под чёрным флагом|complete 'flying the jolly roger' quest line|успешно завершить квест «под чёрным флагом»|ach_58", "ach_58", "Flying the Jolly Roger / Под чёрным флагом", 0, 0);
	CLT_Ach_Add("renter|арендатор|rent any warehourse anywhere|арендовать склад в порту любого поселения|ach_67", "ach_67", "Renter / Арендатор", 0, 0);
	CLT_Ach_Add("spiritual protection|духовная защита|hire a ship priest|пригласить капеллана в команду|ach_69", "ach_69", "Spiritual protection / Духовная защита", 0, 0);
	CLT_Ach_Add("caribbean poisoner|карибский отравитель|use a unique poison|применить уникальный яд|ach_71", "ach_71", "Caribbean poisoner / Карибский отравитель", 0, 0);
	CLT_Ach_Add("red face|сизый нос|win a drinking contest|победить в конкурсе «пей до дна»|ach_70", "ach_70", "Red face / Сизый нос", 0, 0);
	CLT_Ach_Add("hero of the nation|герой нации|reach the highest rank in french colonial navy|достигните высочайшего звания во французском колониальном флоте|ach_79", "ach_79", "Hero of the Nation / Герой нации", 0, 0);
	CLT_Ach_Add("red talisman|красный талисман|marry mary|взять в жены мэри|ach_80", "ach_80", "Red Talisman / Красный талисман", 0, 0);
	CLT_Ach_Add("prophecy came true|пророчество сбылось|marry helen|взять в жены элен|ach_81", "ach_81", "Prophecy Came True / Пророчество сбылось", 0, 0);
	CLT_Ach_Add("three's crowd|ром на троих|partake in a special event with a brothel manager|провести мероприятие с хозяйкой борделя|ach_cl_105", "ach_CL_105", "Three's Crowd / Ром на троих", 0, 0);
	CLT_Ach_Add("silver tongue|мастер-переговорщик|win an impossible bargain two times|дважды торговаться до победного|ach_cl_106", "ach_CL_106", "Silver Tongue / Мастер-переговорщик", 106, 2);
	CLT_Ach_Add("quick start|сразу к делу|murder your tutor|убить своего первого учителя|ach_cl_107", "ach_CL_107", "Quick Start / Сразу к делу", 0, 0);
	CLT_Ach_Add("from zero to hero|первая любовь|take your very first officer on the suicide mission|взять фулька делюка в таясаль|ach_cl_108", "ach_CL_108", "From Zero to Hero / Первая любовь", 0, 0);
	CLT_Ach_Add("my own trafalgar|мой личный трафальгар|sink or capture the victory|победить «виктори»|ach_cl_109", "ach_CL_109", "My Own Trafalgar / Мой личный Трафальгар", 0, 0);
	CLT_Ach_Add("assistant cartographer|помощник картографа|bring all quality maps to jose dios|собрать все карты для хосе диоса|ach_cl_110", "ach_CL_110", "Assistant Cartographer / Помощник картографа", 0, 0);
	CLT_Ach_Add("a pirate's life for me|пираты наглеют!|show your true colors early on|проявить свою истинную натуру как можно раньше|ach_cl_111", "ach_CL_111", "A Pirate's Life for Me / Пираты наглеют!", 0, 0);
	CLT_Ach_Add("sell a poor man a fish|продукты дорожают!|trade over 20,000 of food to layton dexter|продайте 20,000 провианта лейтону декстеру|ach_cl_112", "ach_CL_112", "Sell a Poor Man a Fish / Продукты дорожают!", 0, 0);
	CLT_Ach_Add("hostis humani generis|дальше будет только хуже!|get the maximum bounty on your head from all nations simultaneously|получить максимальную награду за голову у всех наций единовременно|ach_cl_113", "ach_CL_113", "Hostis Humani Generis / Дальше будет только хуже!", 0, 0);
	CLT_Ach_Add("long shadows of old sins|длинные тени старых грехов|confirm or dispel charlie's fears|развейте или подтвердите страхи чарли|ach_cl_114", "ach_CL_114", "Long Shadows of Old Sins / Длинные тени старых грехов", 0, 0);
	CLT_Ach_Add("chimera|химера|fully upgrade the chimera|полностью улучшить «химеру»|ach_cl_115", "ach_CL_115", "Chimera / Химера", 0, 0);
	CLT_Ach_Add("lived as a villain, died as the same|как жил, так и помер|deal with claude in the fittingly dreadful way|уготовьте чудовищную, но подходящую судьбу клоду дюрану|ach_cl_116", "ach_CL_116", "Lived as a Villain, Died as the Same / Как жил, так и помер", 0, 0);
	CLT_Ach_Add("powergamer|минмаксер|complete vile little god side quest before level 17|пройти квест «мерзкий божок» до 17 уровня|ach_cl_117", "ach_CL_117", "Powergamer / Минмаксер", 0, 0);
	CLT_Ach_Add("agent of the crown|курьер особых поручений|successfully deliver the secret diplomatic package|успешно доставить дипломатическую почту|ach_cl_118", "ach_CL_118", "Agent of the Crown / Курьер особых поручений", 0, 0);
	CLT_Ach_Add("chevalier de maure|герой-любовник|rescue mary from the dread of justice island|спасти мэри каспер от безнадёжной тоски острова справедливости|ach_cl_119", "ach_CL_119", "Chevalier de Maure / Герой-любовник", 0, 0);
	CLT_Ach_Add("duelist|дуэлянт|win ten duels|победить в 10 дуэлях|ach_cl_120", "ach_CL_120", "Duelist / Дуэлянт", 0, 0);
	CLT_Ach_Add("by the book|по всем правилам|siege tortuga and defeat levausser conventionally|победить левассёра по всем правилам военной науки|ach_cl_121", "ach_CL_121", "By the Book / По всем правилам", 0, 0);
	CLT_Ach_Add("lethal|несовместимо с жизнью|deliver 1,000 damage|нанести 1,000 единиц урона|ach_cl_122", "ach_CL_122", "Lethal / Несовместимо с жизнью", 0, 0);
	CLT_Ach_Add("outstanding move|гениальный ход|board an enemy while your ship is sinking|взять на абордаж противника, будучи на тонущем корабле|ach_cl_123", "ach_CL_123", "Outstanding Move / Гениальный ход", 0, 0);
	CLT_Ach_Add("not a canon event|неканонично|achieve the highest relations with spain as a non-spanish character|достичь максимального отношения с испанией, не будучи испанцем|ach_cl_124", "ach_CL_124", "Not a Canon Event / Неканонично", 0, 0);
	CLT_Ach_Add("pirate's dream|мечта пирата|gather 30,000 golden doubloons|собрать 30 тысяч дублонов в корабельном сундуке|ach_cl_125", "ach_CL_125", "Pirate's Dream / Мечта пирата", 0, 0);
	CLT_Ach_Add("extinguished star|погасшая звезда|fulfill longway's deepest desire|исполнить заветное желание лонгвэя|ach_cl_128", "ach_CL_128", "Extinguished Star / Погасшая звезда", 0, 0);
	CLT_Ach_Add("employee of the month|сотрудник месяца|deliver a special gift to de poincy|доставить особый «подарок» пуанси|ach_cl_129", "ach_CL_129", "Employee Of the Month / Сотрудник месяца", 0, 0);
	CLT_Ach_Add("dungeon master|dungeon master|experience the dread of tortuga's dungeons|пережить чудовищный опыт на тортуге|ach_cl_130", "ach_CL_130", "Dungeon Master / Dungeon Master", 0, 0);
	CLT_Ach_Add("black mark|чёрная метка|complete the black mark side quest|пройти побочный квест «чёрная метка»|ach_cl_131", "ach_CL_131", "Black Mark / Чёрная Метка", 0, 0);
	CLT_Ach_Add("the three musketeers|де мор и три мушкетера|have three musketeers in your team|взять с собой сразу трёх мушкетёров|ach_cl_132", "ach_CL_132", "The Three Musketeers / Де Мор и три мушкетера", 0, 0);
	CLT_Ach_Add("winds of highrock|ветра хайрока|feel a wave of nostalgia.|ощутить прилив ностальгии|ach_cl_133", "ach_CL_133", "Winds of Highrock / Ветра Хайрока", 0, 0);
	CLT_Ach_Add("the toothpick|зубочистка|incite a bloodbath over a shiny pointy thing|устроить кровавую баню из-за красивой вещицы|ach_cl_134", "ach_CL_134", "The Toothpick / Зубочистка", 0, 0);
	CLT_Ach_Add("santa misericordia|святое милосердие|defeat the royal galleon|победить королевский галеон в бою|ach_cl_136", "ach_CL_136", "Santa Misericordia / Святое Милосердие", 0, 0);
	CLT_Ach_Add("powergamer ii|минмаксер ii|defeat the holy mercy before reaching level 14|победить «святое милосердие» до 14 уровня|ach_cl_137", "ach_CL_137", "Powergamer II / Минмаксер II", 0, 0);
	CLT_Ach_Add("liber misericordiae|liber misericordiae|find de alamida's bible|найти библию де аламиды|ach_cl_138", "ach_CL_138", "Liber Misericordiae / Liber Misericordiae", 0, 0);
	CLT_Ach_Add("deus vult|deus vult|obtain all possible bonuses from the holy mercy|получить все возможные бонусы «святого милосердия»|ach_cl_139", "ach_CL_139", "Deus Vult / Deus Vult", 0, 0);
	CLT_Ach_Add("hearts and minds|сердца и умы|befriend the natives|подружиться с индейцами|ach_cl_140", "ach_CL_140", "Hearts and Minds / Сердца и умы", 0, 0);
	CLT_Ach_Add("general delivery|до востребования| find the first treasure cache note|найти первую записку из кладов|ach_cl_141", "ach_CL_141", "General Delivery / До востребования", 0, 0);
	CLT_Ach_Add("archaeologist|археолог|collect all treasure cache notes|собрать все записки из кладов|ach_cl_142", "ach_CL_142", "Archaeologist / Археолог", 0, 0);
	CLT_Ach_Add("style before substance|полная шляпа|collect the full hat collection|собрать полную коллекцию шляп|ach_cl_143", "ach_CL_143", "Style Before Substance / Полная шляпа", 0, 0);
	CLT_Ach_Add("the art of the deal|сделка века|redeem picard from the plantation|выкупить пикара с плантации|ach_cl_144", "ach_CL_144", "The Art of the Deal / Сделка века", 0, 0);
	CLT_Ach_Add("friend zone casualty|жертва френдзоны|uncover the secret of port royal|раскрыть секрет порт рояла|ach_cl_145", "ach_CL_145", "Friend Zone Casualty / Жертва френдзоны", 0, 0);
	CLT_Ach_Add("blade of curaçao|клинок кюрасао|uncover the secret of willemstad|раскрыть секрет виллемстада|ach_cl_146", "ach_CL_146", "Blade of Curaçao / Клинок Кюрасао", 0, 0);
	CLT_Ach_Add("lady beth|леди бет|defeat the patrol snow|захватить патрульную шняву|ach_cl_147", "ach_CL_147", "Lady Beth / Леди Бет", 0, 0);
	CLT_Ach_Add("it's all in the head|все проблемы – от головы|eliminate albert blackwood with a headshot at the beginning of the battle|уничтожить альберта блэквуда хэдшотом в начале сражения|ach_cl_148", "ach_CL_148", "It's All in the Head / Все проблемы – от головы", 0, 0);
	CLT_Ach_Add("caribbean tragedy|карибская трагедия|find albert blackwood's greatest treasure|найти главное сокровище альберта блэквуда|ach_cl_149", "ach_CL_149", "Caribbean Tragedy / Карибская трагедия", 0, 0);
	CLT_Ach_Add("powergamer iii|минмаксер iii|defeat the lady beth before reaching level 8|победить «леди бет» до 8 уровня|ach_cl_150", "ach_CL_150", "Powergamer III / Минмаксер III", 0, 0);
	CLT_Ach_Add("persistence rewarded|награда за упорство|find a maximum level treasure|найти клад максимального уровня|ach_cl_151", "ach_CL_151", "Persistence Rewarded / Награда за упорство", 0, 0);
	CLT_Ach_Add("deck broom|палубная метла|install all upgrades for the old arquebus|установить все улучшения для старой аркебузы|ach_cl_152", "ach_CL_152", "Deck Broom / Палубная метла", 0, 0);
	CLT_Ach_Add("hey, it's free!|капитан савин|install a modification|установить модификацию|ach_cl_153", "ach_CL_153", "Hey, It's Free! / Капитан Савин", 0, 0);
	CLT_Ach_Add("memento|memento|defeat the pirate brig|захватить пиратский бриг|ach_cl_154", "ach_CL_154", "Memento / Memento", 0, 0);
	CLT_Ach_Add("mori|mori|meet captain grim|познакомиться с капитаном гримом|ach_cl_155", "ach_CL_155", "Mori / Mori", 0, 0);
	CLT_Ach_Add("powergamer iv|минмаксер iv|defeat the memento before reaching level 11|победить «мементо» до 11 уровня|ach_cl_156", "ach_CL_156", "Powergamer IV / Минмаксер IV", 0, 0);
	CLT_Ach_Add("but deliver us from evil|избави нас от лукавого|make the right choice|сделать правильный выбор|ach_cl_157", "ach_CL_157", "But Deliver Us From Evil / Избави нас от лукавого", 0, 0);
	CLT_Ach_Add("el cazador|эль касадор|capture or sink the spanish pirate hunter|потопить или захватить испанского охотника на пиратов|ach_cl_158", "ach_CL_158", "El Cazador / Эль Касадор", 0, 0);
	CLT_Ach_Add("that was awkward|неловко вышло|make the el cazador you first defeated spanish ship|захватить «эль касадор» так, чтобы он стал вашим первым побежденным испанцем |ach_cl_159", "ach_CL_159", "That Was Awkward / Неловко вышло", 0, 0);
	CLT_Ach_Add("apex predator|высший хищник|defeat all punitive fleets|одолеть все карательные эскадры|ach_cl_160", "ach_CL_160", "Apex Predator / Высший хищник", 0, 0);
	CLT_Ach_Add("i'll take it from here|дай, я сам!|aim the guns yourself to sink an enemy ship|лично наводить пушки и потопить вражеский корабль|ach_cl_161", "ach_CL_161", "I'll Take It From Here / Дай, я сам!", 0, 0);
	CLT_Ach_Add("the prologue|пролог|complete the prologue|завершить пролог|ach_cl_162", "ach_CL_162", "The Prologue / Пролог", 0, 0);
	CLT_Ach_Add("the prologue+|пролог+|get the most out of the prologue|выжать максимум из пролога|ach_cl_163", "ach_CL_163", "The Prologue+ / Пролог+", 0, 0);
	CLT_Ach_Add("charles de mauron|шарль де кретинье|save the goodies from the ulysses for later|утащить сувенир с «улисса»|ach_cl_164", "ach_CL_164", "Charles de Mauron / Шарль де Кретинье", 0, 0);
	CLT_Ach_Add("voice of the crew|голос команды|attend alonso’s therapy session|пройти сеанс терапии алонсо|ach_cl_165", "ach_CL_165", "Voice of the Crew / Голос команды", 0, 0);
	CLT_Ach_Add("from russia with love|из россии с любовью|acquire the mirror armor|приобрести зерцальный нагрудник|ach_cl_166", "ach_CL_166", "From Russia With Love / Из России с любовью", 0, 0);
	CLT_Ach_Add("ann, dear!|дорогая энн!|uncover le francois's secret|раскрыть секрет ле франсуа|ach_cl_167", "ach_CL_167", "Ann, dear! / Дорогая Энн!", 0, 0);
	CLT_Ach_Add("the last knight|последний рыцарь|uncover saint-pierre's secret|раскрыть секрет сен-пьера|ach_cl_168", "ach_CL_168", "The Last Knight / Последний рыцарь", 0, 0);
	CLT_Ach_Add("trigger|триггер|begin the 'wild rose' quest|начать квест «дикая роза»|ach_cl_169", "ach_CL_169", "Trigger / Триггер", 0, 0);
	CLT_Ach_Add("coming of age|взросление|bid farewell to the city of abandoned ships|попрощаться с городом погибших кораблей|ach_cl_170", "ach_CL_170", "Coming of Age / Взросление", 0, 0);
	CLT_Ach_Add("gestalt|гештальт|save mary casper one last time|спасти мэри каспер в последний раз|ach_cl_171", "ach_CL_171", "Gestalt / Гештальт", 0, 0);
	CLT_Ach_Add("buildmaster|билдмастер|complete the story mode or reach level 30 in sandbox mode without using a perk reset potion|пройти сюжетный режим или достичь 30 уровня в режиме свободной игры, ни разу не применив зелье сброса навыков|ach_cl_172", "ach_CL_172", "Buildmaster / Билдмастер", 0, 0);
	CLT_Ach_Add("right to err|право на ошибку|use a perk reset potion|применить зелье сброса навыков|ach_cl_173", "ach_CL_173", "Right to Err / Право на ошибку", 0, 0);
	CLT_Ach_Add("useful connections|полезные связи|complete at least one town quest|выполнить хотя бы один «городской» квест|ach_cl_174", "ach_CL_174", "Useful Connections / Полезные связи", 0, 0);
	CLT_Ach_Add("trustworthy|вызывающий доверие|complete all 10 town quests|выполнить все 10 «городских квестов»|ach_cl_175", "ach_CL_175", "Trustworthy / Вызывающий доверие", 175, 10);
	CLT_Ach_Add("unconventional thinking|нестандартное мышление|solve the beliz gang problem in an alternative way|решить проблему шайки белиза альтернативным методом|ach_cl_176", "ach_CL_176", "Unconventional Thinking / Нестандартное мышление", 0, 0);
	CLT_Ach_Add("harpy|гарпия|obtain the harpy's key by force|получить ключ «гарпии» силой|ach_cl_177", "ach_CL_177", "Harpy / Гарпия", 0, 0);
	CLT_Ach_Add("reconstruction|реконструкция|restore the hull of the caleuche|восстановить корпус «калеуче»|ach_cl_178", "ach_CL_178", "Reconstruction / Реконструкция", 0, 0);
	CLT_Ach_Add("la espada del rey|la espada del rey|complete sharptown's town quest|пройти «городской» квест шарптауна|ach_cl_179", "ach_CL_179", "La Espada Del Rey / La Espada Del Rey", 0, 0);
	CLT_Ach_Add("rogue|ловкач|learn all duelist perks|выучить все навыки «дуэлянта»|ach_cl_180", "ach_CL_180", "Rogue / Ловкач", 0, 0);
	CLT_Ach_Add("adventurer|авантюрист|learn all adventurer perks|выучить все навыки «авантюриста»|ach_cl_181", "ach_CL_181", "Adventurer / Авантюрист", 0, 0);
	CLT_Ach_Add("master|мастер|gain a permanent 10% damage bonus from alchemy|получить постоянный 10% бонус к урону от алхимии|ach_cl_182", "ach_CL_182", "Master / Мастер", 0, 0);
	CLT_Ach_Add("soldier|солдат|learn all soldier perks|выучить все навыки «солдата»|ach_cl_183", "ach_CL_183", "Soldier / Солдат", 0, 0);
	CLT_Ach_Add("pirate|пират|learn all pirate perks|выучить все навыки «пирата»|ach_cl_184", "ach_CL_184", "Pirate / Пират", 0, 0);
	CLT_Ach_Add("musketeer|мушкетёр|learn all musketeer perks|выучить все навыки «мушкетёра»|ach_cl_185", "ach_CL_185", "Musketeer / Мушкетёр", 0, 0);
	CLT_Ach_Add("peaked|предел совершенства|learn all mastery perks|выучить все навыки «мастерства»|ach_cl_186", "ach_CL_186", "Peaked / Предел совершенства", 0, 0);
	CLT_Ach_Add("i don't play fair|я не играю честно|backstab 50 times|нанести удар в спину 50 раз|ach_cl_187", "ach_CL_187", "I Don't Play Fair / Я не играю честно", 187, 50);
	CLT_Ach_Add("brown pants|коричневые штаны|flee on the global map 50 times|сбежать на глобальной карте 50 раз|ach_cl_188", "ach_CL_188", "Brown Pants / Коричневые штаны", 188, 50);
	CLT_Ach_Add("exotic|экзотичный|achieve maximum fleet power bonus from items|собрать максимальный бонус мощи эскадры от предметов|ach_cl_189", "ach_CL_189", "Exotic / Экзотичный", 0, 0);
	CLT_Ach_Add("fancy|вычурный|achieve maximum leadership bonus from items|собрать максимальный бонус харизмы от предметов|ach_cl_190", "ach_CL_190", "Fancy / Вычурный", 0, 0);
	CLT_Ach_Add("plain|неприметный|achieve maximum stealth bonus from items|собрать максимальный бонус скрытности от предметов|ach_cl_191", "ach_CL_191", "Plain / Неприметный", 0, 0);
	CLT_Ach_Add("la putain|путана|sleep with both helen and mary in a single playthrough|переспать и с элен, и с мэри за одно прохождение|ach_cl_192", "ach_CL_192", "La Putain / Путана", 0, 0);
	CLT_Ach_Add("lone wolf|волк-одиночка|achieve maximum fleet power bonus from all sources|собрать максимальный бонус мощи эскадры от всех источников|ach_cl_193", "ach_CL_193", "Lone Wolf / Волк-одиночка", 0, 0);
	CLT_Ach_Add("blockbuster|блокбастер|engage five fleets in a single battle on the global map|собрать в одном бою пять эскадр с глобальной карты|ach_cl_194", "ach_CL_194", "Blockbuster / Блокбастер", 0, 0);
	CLT_Ach_Add("powergamer v|минмаксер v|defeat the amsterdam before reaching level 19|победить «амстердам» до 19 уровня|ach_cl_195", "ach_CL_195", "Powergamer V / Минмаксер V", 0, 0);
	CLT_Ach_Add("amsterdam|амстердам|defeat the company flagship|захватить флагман компании|ach_cl_196", "ach_CL_196", "Amsterdam / Амстердам", 0, 0);
	CLT_Ach_Add("corporate culture|корпоративная культура|convince johan the nice way|убедить йохана по-хорошему|ach_cl_197", "ach_CL_197", "Corporate Culture / Корпоративная культура", 0, 0);
	CLT_Ach_Add("got a taste for it|вошел во вкус|refuse doorn's surrender|не принять капитуляцию дорна|ach_cl_198", "ach_CL_198", "Got a Taste for It / Вошел во вкус", 0, 0);
	CLT_Ach_Add("ink rules the world|бумага всё стерпит|use the services of the ship registry office|воспользоваться услугами конторы судового учёта|ach_cl_199", "ach_CL_199", "Ink Rules the World / Бумага всё стерпит", 0, 0);
	CLT_Ach_Add("pest control|борьба с вредителями|collect 50 pirate ship logs|собрать 50 пиратских судовых журналов|ach_cl_200", "ach_CL_200", "Pest Control / Борьба с вредителями", 200, 50);
	CLT_Ach_Add("corporate standarts|корпоративные стандарты|reach maximum trait bonus for the amsterdam|достичь максимального бонуса специализации «амстердама»|ach_cl_201", "ach_CL_201", "Corporate Standarts / Корпоративные стандарты", 0, 0);
	CLT_Ach_Add("hi, dad|привет, пап|complete the main quest|завершить основной сюжетный квест|ach_cl_202", "ach_CL_202", "Hi, Dad / Привет, пап", 0, 0);
	CLT_Ach_Add("caribbean legend|карибская легенда|stay in the caribbean|остаться на карибах|ach_cl_203", "ach_CL_203", "Caribbean Legend / Карибская легенда", 0, 0);
	CLT_Ach_Add("to each his own|каждому своё|stay in europe|остаться в европе|ach_cl_204", "ach_CL_204", "To Each His Own / Каждому своё", 0, 0);
	CLT_Ach_Add("someone paid for this|каждая монета — чья-то|amass 50 million pesos for the epilogue |собрать 50 миллионов песо в эпилоге|ach_cl_205", "ach_CL_205", "Someone Paid for This / Каждая монета — чья-то", 0, 0);
	CLT_Ach_Add("unsinkable|непотопляемый|enter the season of storm ready for anything|встретить сезон бури, будучи готовым ко всему|ach_cl_206", "ach_CL_206", "Unsinkable / Непотопляемый", 0, 0);
	CLT_Ach_Add("spiders in a jar|пауки в банке|enter the season of shade ready for anything|встретить сезон теней, будучи готовым ко всему|ach_cl_207", "ach_CL_207", "Spiders in a Jar / Пауки в банке", 0, 0);
	CLT_Ach_Add("even death may die|даже смерть может умереть|find the drowned one's hat|найти шляпу утопленника|ach_cl_208", "ach_CL_208", "Even Death May Die / Даже Смерть может умереть", 0, 0);
	CLT_Ach_Add("the last of the pirates|последний из пиратов|find the drowned one's sword|найти шпагу утопленника|ach_cl_209", "ach_CL_209", "The Last of the Pirates / Последний из пиратов", 0, 0);
	CLT_Ach_Add("cyclopean masonry|циклопическая кладка|answer the call in a dream|ответить на зов во сне|ach_cl_210", "ach_CL_210", "Cyclopean Masonry / Циклопическая кладка", 0, 0);
	CLT_Ach_Add("a pleasant surprise|приятный сюрприз|celebrate your birthday|отметить день рождения|ach_cl_211", "ach_CL_211", "A Pleasant Surprise / Приятный сюрприз", 0, 0);
	CLT_Ach_Add("all stick, no carrot|кнут и кнут|earn renown as a merciless captain who rules with an iron hand, in word and deed|словом и делом завоевать славу безжалостного капитана, правящего железной рукой|ach_cl_212", "ach_CL_212", "All Stick, No Carrot / Кнут и кнут", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_213", "ach_CL_213", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_214", "ach_CL_214", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_215", "ach_CL_215", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_216", "ach_CL_216", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_217", "ach_CL_217", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_218", "ach_CL_218", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_219", "ach_CL_219", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_220", "ach_CL_220", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_221", "ach_CL_221", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_222", "ach_CL_222", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_223", "ach_CL_223", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_224", "ach_CL_224", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_225", "ach_CL_225", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_226", "ach_CL_226", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_227", "ach_CL_227", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_228", "ach_CL_228", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_229", "ach_CL_229", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_230", "ach_CL_230", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_231", "ach_CL_231", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_232", "ach_CL_232", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_233", "ach_CL_233", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_234", "ach_CL_234", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_235", "ach_CL_235", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_236", "ach_CL_236", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_237", "ach_CL_237", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_238", "ach_CL_238", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_239", "ach_CL_239", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_240", "ach_CL_240", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_241", "ach_CL_241", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_242", "ach_CL_242", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_243", "ach_CL_243", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_244", "ach_CL_244", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_245", "ach_CL_245", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_246", "ach_CL_246", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_247", "ach_CL_247", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_248", "ach_CL_248", "Coming soon!", 0, 0);
	CLT_Ach_Add("coming soon!|ach_cl_249", "ach_CL_249", "Coming soon!", 0, 0);
}
// ==== ACH TABLE END ====

// --- инструмент ачивок: фильтр по названию + одна кнопка «Выполнить» ---
string clt_sAchFilter;		// текущий фильтр (ввод в CLT_EDIT, Enter)
int clt_nAchMatch;			// индекс единственного совпадения (-1 — нет/неоднозначно)
bool clt_achInited;			// таблица заполнена

#define CLT_MISC_FLAGS	12

string CLT_Misc_Flag(int i)
{
	switch (i)
	{
		case 0: return "cheats.ArcadeSailing"; break;	// ShipsUtilites.c:682, AIShip.c:3939 — скорость без учёта ветра
		case 1: return "Cheats.SeaTeleport"; break;		// BattleInterface.c:505,825… — выход на карту/переходы в море без ограничений
		case 2: return "Cheats.LocTeleport"; break;		// controls_func.c:890, landinterface.c:1209 — быстрый переход во враждебном городе
		case 3: return "cheats.dopgrus"; break;			// land_table.c:283 — +1000 к переносимому весу
		case 4: return "NoNavyPenalty"; break;			// RPG.c:36, helpers.c:39 — нет штрафа за класс корабля
		case 5: return "cheats.bladedamage"; break;		// melee.c:134 — урон клинком x10 (только у того, на ком флаг: у героя)
		case 6: return "cheats.energyupdate"; break;	// fightparams\common.c:619 — быстрое восстановление энергии
		case 7: return "cheats.hpupdate"; break;		// LAi_character.c:1012 — быстрое восстановление HP (герой)
		case 8: return "cheats.sliding"; break;			// LAi_events.c:213 — блок врага не останавливает удар героя (= «Навигатор» Pierce)
		case 9: return "Cheats.ImmediateDeath"; break;	// BattleInterface.c:1163
		case 10: return "CLToolkit.SeaCoordsOff"; break;	// инверт: атрибут есть = показ ВЫКЛ; по умолчанию показ включён.
		case 11: return "CLToolkit.LeaveBattleKeepFleet"; break;	// выход из боя без бросания компаньонов (если игрок вышел)
		case 12: return "Navigator.MapTP"; break;		// телепорт по атласу: читает наш interface\mapview.c («Навигатор» 1.9, Nav_CmdTowns)
															// Читает CLT_SeaCoordsUpdate в нашем battle_interface\BattleInterface.c (оверлей)
	}
	return "";
}

string CLT_Misc_Desc(int i)
{
	switch (i)
	{
		case 0: return "Аркадное плавание (без учёта ветра)"; break;
		case 1: return "Морской телепорт («Доплыть»)"; break;
		case 2: return "Быстрые переходы во враждебных городах"; break;
		case 3: return "+1000 к переносимому весу"; break;
		case 4: return "Без штрафа за класс корабля"; break;
		case 5: return "Урон клинком героя x10"; break;
		case 6: return "Быстрая регенерация энергии"; break;
		case 7: return "Быстрая регенерация здоровья"; break;
		case 8: return "Удары героя пробивают любой блок"; break;
		case 9: return "Мгновенная смерть врагов (море)"; break;
		case 10: return "Координаты корабля в море"; break;
		case 11: return "Выход из боя, сохраняя флот"; break;
		case 12: return "Телепорт по атласу"; break;
	}
	return "";
}

void CLT_Misc_Toggle(int i)
{
	string f = CLT_Misc_Flag(i);
	if (f == "") return;
	if (CheckAttribute(pchar, f))
	{
		DeleteAttribute(pchar, f); // строка с точкой — как DeleteAttribute(pchar, "cheats.dopgrus") в Cheat Menu
	}
	else
	{
		// Запись по сегментам: точку внутри .(x) не используем
		int dot = findsubstr(f, ".", 0);
		if (dot < 0) pchar.(f) = true;
		else
		{
			string grp = strcut(f, 0, dot - 1);
			string leaf = strcut(f, dot + 1, strlen(f) - 1);
			pchar.(grp).(leaf) = true;
		}
	}
	// Предел веса игра берёт из кэша ct.land.MaxWeight (weight.c GetMaxItemsWeight), а кэш пересчитывает
	// только при экипировке/перках (CT_UpdateCashTables) — без этого флаг не действовал до переодевания
	if (i == 3)
	{
		CT_UpdateCashTables(pchar);
		CheckAndSetOverloadMode(pchar);
	}
	// Флаг 12 (телепорт по атласу): как «Навигатор» 1.9 (nav_commands.c Nav_CmdTowns) — без карты региона
	// атлас пуст, поэтому при включении выдаём map_best и обновляем атлас
	if (i == 12 && CheckAttribute(pchar, f) && !CheckCharacterItem(pchar, "map_best"))
	{
		TakeNItems(pchar, "map_best", 1);
		RefreshEquippedMaps(pchar);
	}
	// Флаг 10 инвертный (SeaCoordsOff): галочка «показывать» = атрибута нет
	if (i == 10) CLT_Log(CLT_Misc_Desc(i) + ": " + CLT_OnOff(!CheckAttribute(pchar, f)));
	else CLT_Log(CLT_Misc_Desc(i) + ": " + CLT_OnOff(CheckAttribute(pchar, f)));
}

// Скорость своего корабля на глобальной карте x1/x2/x3 — «Навигатор» 1.9, раздел «Море», команды 5-6
// (nav_commands.c Nav_CmdSeaMap, Nav_MapSpeedReset): флаги pchar.Navigator.MapSpeed2 / MapSpeed3.
// Применяет их mod_inits\cltoolkit.c CLT_MapSpeedTick (событие карты EventCoordUpdate, каждые 200 мс):
// пишет множитель в TEV.worldMap.kPlayerMaxSpeed и worldMap.kPlayerMaxSpeed (база пересчитывает скорость
// из TEV, worldmap_coords.c UpdateWorldMapSpeed). Рецепт — nav_core.c Nav_ApplyAll.
int CLT_Misc_MapSpd()
{
	if (CheckAttribute(pchar, "Navigator.MapSpeed3")) return 3;
	if (CheckAttribute(pchar, "Navigator.MapSpeed2")) return 2;
	return 1;
}

void CLT_Misc_MapSpdStep(int dir)
{
	int k = CLT_Misc_MapSpd() + dir;
	if (k < 1) k = 3;
	if (k > 3) k = 1;
	DeleteAttribute(pchar, "Navigator.MapSpeed2");
	DeleteAttribute(pchar, "Navigator.MapSpeed3");
	if (k == 2) pchar.Navigator.MapSpeed2 = true;
	if (k == 3) pchar.Navigator.MapSpeed3 = true;
	TEV.worldMap.kPlayerMaxSpeed = k;
	if (IsEntity(&worldMap)) worldMap.kPlayerMaxSpeed = k;
	CLT_Log("Скорость корабля на глобальной карте: x" + k);
}

// Множитель урона героя — своя метка pchar.CLToolkit.Dmg. Читают наши копии
// cltoolkit\fight\melee.c и range.c (CLT_HeroDmgMtp): точный множитель итогового урона
// клинком и огнестрелом. От «Навигатора» 1.9 не зависит.
// Старое значение Navigator.dmg переносим при первом показе, чтобы не считать дважды.
float CLT_Misc_Dmg()
{
	if (!CheckAttribute(pchar, "CLToolkit.Dmg") && CheckAttribute(pchar, "Navigator.dmg"))
	{
		pchar.CLToolkit.Dmg = stf(pchar.Navigator.dmg);
		DeleteAttribute(pchar, "Navigator.dmg");
	}
	if (!CheckAttribute(pchar, "CLToolkit.Dmg")) return 1.0;
	return stf(pchar.CLToolkit.Dmg);
}

void CLT_Misc_DmgStep(int dir)
{
	float v[6];
	v[0] = 1.0; v[1] = 2.0; v[2] = 3.0; v[3] = 5.0; v[4] = 10.0; v[5] = 100.0;
	float cur = CLT_Misc_Dmg();
	int i;
	int k = 0;
	for (i = 0; i < 6; i++)
	{
		if (cur >= v[i]) k = i;
	}
	k = k + dir;
	if (k < 0) k = 5;
	if (k > 5) k = 0;
	if (k == 0) DeleteAttribute(pchar, "CLToolkit.Dmg");
	else pchar.CLToolkit.Dmg = v[k];
	DeleteAttribute(pchar, "Navigator.dmg");	// чтобы «Навигатор» не умножал ещё раз
	CLT_Log("Множитель урона героя: x" + int(v[k]));
}

// Лимит стоянки кораблей в порту (в базе — 3). Сама база проверяет только
// «int(NPChar.Portman) >= 3» в Common_Portman.c, счётчик перед каждым диалогом
// пересчитывает CLT_PortmanSync (mod_inits\cltoolkit.c). Здесь только выбор лимита.
// Метка — pchar.CLToolkit.PortStock (в сейве); атрибута нет = 3, штатное поведение.
int CLT_Misc_PortStock()
{
	if (!CheckAttribute(pchar, "CLToolkit.PortStock")) return 3;
	int v = int(pchar.CLToolkit.PortStock);
	if (v < 3) return 3;
	return v;
}

void CLT_Misc_PortStockStep(int dir)
{
	int v[4];
	v[0] = 3; v[1] = 5; v[2] = 8; v[3] = 12;
	int cur = CLT_Misc_PortStock();
	int i;
	int k = 0;
	for (i = 0; i < 4; i++)
	{
		if (cur >= v[i]) k = i;
	}
	k = k + dir;
	if (k < 0) k = 3;
	if (k > 3) k = 0;
	if (k == 0) DeleteAttribute(pchar, "CLToolkit.PortStock");
	else pchar.CLToolkit.PortStock = v[k];
	CLT_Log("Стоянка в порту: не больше " + v[k] + " кораблей");
}

void CLT_Misc_Build()
{
	int i;
	CLT_GCol(0, "Море и мир");
	CLT_GT("flag0", CLT_Misc_Desc(0), CheckAttribute(pchar, CLT_Misc_Flag(0)), true);
	// Два телепорта рядом: морской (флаг 1) — в пределах текущей морской локации, атлас (флаг 12) — по всему архипелагу
	CLT_GT("flag1", CLT_Misc_Desc(1), CheckAttribute(pchar, CLT_Misc_Flag(1)), true);
	CLT_GH("В море: «Доплыть» в порт, бухту, форт,");
	CLT_GH("к кораблю — с любой дистанции, и в бою.");
	CLT_GT("flag12", CLT_Misc_Desc(12), CheckAttribute(pchar, CLT_Misc_Flag(12)), true);
	CLT_GH("Атлас (M), карта региона: ПКМ по карте —");
	CLT_GH("список локаций, щелчок — перенос.");
	for (i = 2; i < 5; i++) CLT_GT("flag" + i, CLT_Misc_Desc(i), CheckAttribute(pchar, CLT_Misc_Flag(i)), true);
	CLT_GS("mapspd", "Скорость на карте", "x" + CLT_Misc_MapSpd(), true);
	CLT_GH("Свой корабль на глобальной карте: 1, 2, 3.");
	CLT_GCol(1, "");	// заголовок не рисуем: сверху в этой зоне поле фильтра ачивок (CLT_EDIT)
	for (i = 5; i < CLT_MISC_FLAGS; i++)
	{
		bool st = CheckAttribute(pchar, CLT_Misc_Flag(i));
		if (i == 10) st = !st;	// CL Toolkit: флаг 10 инвертный (SeaCoordsOff) — галочка = «показывать»
		CLT_GT("flag" + i, CLT_Misc_Desc(i), st, true);
	}
	CLT_GS("dmg", "Множитель урона", "x" + int(CLT_Misc_Dmg()), true);
	CLT_GH("Клинок и огнестрел героя: 1, 2, 3, 5, 10, 100.");
	CLT_GS("portstock", "Стоянка в порту", "до " + CLT_Misc_PortStock(), true);
	CLT_GH("Кораблей на стоянке у портмана: 3, 5, 8, 12.");
	CLT_GCol(2, "Ачивки и пояснения");
	if (!clt_achInited)
	{
		CLT_Ach_Init();
		clt_sAchFilter = "";
		clt_achInited = true;
	}
	CLT_Ach_Build();
	CLT_GSkip();
	CLT_GH("Флаги ставятся на главного героя (pchar)");
	CLT_GH("— те же атрибуты, что у штатного Cheat Menu.");
	CLT_GH("Сохраняются в сейве вместе с героем.");
	CLT_GH("Статистика читов (Statistic.Cheats) не пишется.");
	CLT_GH("Множитель урона — свои файлы мода;");
	CLT_GH("скорость на карте — событие глобальной карты.");
}

// Латиницу в нижний регистр (RU-ключи хранятся строчными; регистр ввода для RU — строчные)
string CLT_StrLower(string s)
{
	string up = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	string lo = "abcdefghijklmnopqrstuvwxyz";
	string r = "";
	string c;
	int p;
	int i;
	int n = strlen(s);
	for (i = 0; i < n; i++)
	{
		c = strcut(s, i, i);
		p = findsubstr(up, c, 0);
		if (p >= 0) c = strcut(lo, p, p);
		r = r + c;
	}
	return r;
}

// Строки инструмента ачивок (рисуются в текущей колонке — 2).
// Совпадений 0 или больше 1 -> кнопка выключена; ровно 1 -> «ВЫПОЛНИТЬ: имя».
void CLT_Ach_Build()
{
	string q = CLT_StrLower(clt_sAchFilter);
	string st;
	int n = 0;
	int first = -1;
	int i;
	int shown;
	if (q != "")
	{
		for (i = 0; i < clt_achCount; i++)
		{
			if (findsubstr(clt_achKey[i], q, 0) >= 0)
			{
				if (n == 0) first = i;
				n++;
			}
		}
	}
	clt_nAchMatch = first;
	if (clt_sAchFilter == "")
	{
		CLT_GH("Ачивки: введите часть названия (EN/RU, строчными)");
		CLT_GH("в поле сверху и нажмите Enter.");
		CLT_GA("achdo", "Выполнить", false);
		return;
	}
	if (n == 0)
	{
		CLT_GH("Ничего не найдено: «" + clt_sAchFilter + "».");
		CLT_GA("achdo", "Выполнить", false);
		return;
	}
	if (n == 1) CLT_GH("Найдено 1:");
	else CLT_GH("Найдено: " + n + " — уточните фильтр:");
	shown = 0;
	for (i = 0; i < clt_achCount && shown < 3; i++)
	{
		if (findsubstr(clt_achKey[i], q, 0) >= 0)
		{
			st = "";
			if (GetAchievement(clt_achApi[i])) st = " — получено";
			CLT_GH("• " + clt_achName[i] + st);
			shown++;
		}
	}
	if (n == 1) CLT_GA("achdo", "ВЫПОЛНИТЬ: " + clt_achName[first], !GetAchievement(clt_achApi[first]));
	else CLT_GA("achdo", "Выполнить (нужно одно совпадение)", false);
}

// Выполнить найденную ачивку: Achievment_Set(API) + синхронизация связанного стата (если есть).
void CLT_Ach_Do()
{
	int i = clt_nAchMatch;
	if (i < 0)
	{
		CLT_Log("Ачивка не выбрана: уточните фильтр");
		return;
	}
	if (GetAchievement(clt_achApi[i]))
	{
		CLT_Log("Уже получено: " + clt_achName[i]);
		return;
	}
	Achievment_Set(clt_achApi[i]);
	if (clt_achStatNum[i] > 0)
	{
		Achievment_SetStat(clt_achStatNum[i], clt_achStatMax[i]); // внутри есть StoreStats
		CLT_Log("Выполнено: " + clt_achName[i] + " (" + clt_achApi[i] + "), стат " + Stat_GetID(clt_achStatNum[i]) + " -> " + clt_achStatMax[i]);
		return;
	}
	StoreStats();
	CLT_Log("Выполнено: " + clt_achName[i] + " (" + clt_achApi[i] + ")");
}

// Enter в поле фильтра ачивок (CLT_EDIT) — вызывается из clt_window.c
void CLT_Misc_OnEdit(string s)
{
	clt_sAchFilter = s;
}

void CLT_Misc_Do(string cmd, int dir, bool big)
{
	if (cmd == "dmg") { CLT_Misc_DmgStep(dir); return; }
	if (cmd == "mapspd") { CLT_Misc_MapSpdStep(dir); return; }
	if (cmd == "portstock") { CLT_Misc_PortStockStep(dir); return; }
	if (cmd == "achdo") { CLT_Ach_Do(); return; }
	if (strlen(cmd) < 5 || strcut(cmd, 0, 3) != "flag") return;
	CLT_Misc_Toggle(sti(strcut(cmd, 4, strlen(cmd) - 1)));
}
