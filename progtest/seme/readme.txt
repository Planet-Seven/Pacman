Semestrální práce na téma Pac-Man. Vojtěch Svoboda 2024, BI-PA2, FIT, CVUT.

V této semestrální práci budu implementovat věrohodný remake originálního pacmana. Budu ctít herní mechaniky původní hry, přidám však vlastní event, který hru trochu zpestří.

---HERNÍ MECHANIKY---
V originálním pacmanovi se hráč pohybuje po dvojrozměrné mapě. Cílem je sesbírat všechny žetony, zabránit tomu mají nepřatelé v podobě čtyř barevných duchů.
Ti hráče pronásledují, a pokud ho dostihnou, hráč přijde o život. Když ztratí všechny tři životy, hra končí. Pokud se ale hráči podaří všechny žetony posbírat,
postupuje do další úrovně.

Každý duch má svou "osobnost" a po mapě se pohybuje specifickým způsobem. Implementace tohoto chování je elegantně jednoduchá, o to působivější je výsledný efekt. 
Duchové se liši pouze v tom, které pole volí jako své cílové. Červený duch má svůj cíl téměř vždy na pozici hráče, ostatní duchové to mají trochu jinak.
Když duch dorazí na křižovatku, spočítá normu vektorů s počátkem v sousedních polích, které končí v poli cílovém, a vždy vstoupí na pole s nejmenší normou.
Pokud dvě pole mají stejnou normu, preferuje pole v tomto pořadí: nahoru-doleva-dolů-doprava.

V postupně zkracujících se intervalech se duchové přepínají do režimu hlídkování v sektorech, kdy se stáhnou do rohů, než zase začnou pronásledovat hráče. Ve vyšších úrovních
je doba trvání tohoto efektu nulová, stále se však projeví, protože při změně režimu se duch otočí o 180°.

Třetím herním režimem je pronásledování duchů hráčem. Když hráč sebere jeden z větších žetonů, duchové zmodrají, zpomalí a před hráčem začnou prchat. Když je hráč dostihne,
duch se vrátí na svou startovní pozici a hráč dostne bonusové skóre. 

Původní hra byla zamýšlená jako nekonečná, level 256 se kvůli chybě přetečení nenačetl správně a nešel dohrát. Tuto mechaniku (nekonečný postup, přetečení ideálně ne) uchovám i já.

Rozhodl jsem se však lehce pozměnit chování duchů. Využiji k tomu zajímavou (hurá!) aplikaci (HURÁ!) předmětu LA2. Každý duch bude mít své cílové pole přímo na pozici hráče,
mechanismus rozhodování na křižovatce zůstane také stejný. Rozdíl bude v tom, jakou normu budou duchové používat. Nabízí se tří p-normy, Manhattanská(p=1), "jednoduchá" Eukleidovská (p=2) a
maximová (p=inf). Očekávám, že tři duchové budou z pohledu obtížnosti dostačující, případně se dá přídat čtvrtý s libovolnou normou.

Druhou změnou oproti originálu bude režim "lights out". Ten se bude vyskytovat častěji a jeho trvání bude delší s přibývající úrovní. Obrazovka se zatemní a hráč uvidí jen na několik polí
dopředu, nebude tedy vědět, kde se duchové nachází.

Ještě stojí za zmínku portál. Ten je v originální hře implementován "průchodem" v pravé a levé půlce mapy. Moje verze bude obecnější, půjde o pár portálů, které se teoreticky
mohou nacházet kdekoliv na mapě.

---NÁVRH TŘÍD---
Viz diagram.png. Polymorfismus herních objektů se nabízí poměrně jasně. Do jednoho kontejneru si dáme duchy i všechny sesbíratelné předměty. Zredukujeme rozhraní na draw a update.
Update bude přijímat referenci na gamestate, což je kontext potřebný k provedeni všech dalších operací. Pro duchy je zásadní poloha hráče, gamemode, hrací plocha a konstanty pro rychlost.
Sesbíratelné předměty budou porovnávat polohu hráče se svou polohou, aby případně detekovaly sebrání.

Draw bude bezparametrické, vykreslí předmět na jeho pozici za použití knihovny SDL2.

---KONTROLNÍ BOD---
Pro kontrolní bod implementuji hlavičkové soubory většiny tříd a ucelený gameloop v souboru main.cpp. Gameloop je  framerate-independent a správně pracuje s časem mezi snímky (implementace delta time).
Soubor je zkompilovatelný, po spuštění vykreslí herní mapu a hráče. Umožňuje jednoduché ovládání bez kontroly omezení herního pole.