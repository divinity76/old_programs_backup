YurOTS 0.9.4d

OTS by Yurez pod Tibie 7.6 oparty na CVS z dnia 2006-01-14.

Mo�liwo�ci (i autorzy):
0.9.0
- bp/depo save (CVS)
- bezpieczny trade (CVS)
- buy/sell z kryszta�kami (TLM)
- house system (TLM, Black Demon, ja)
- skulle i party (TLM)
- server save (TLM)
- �wiat�o (Bomb Da Brat)
- vip lista (Elementaries)
- spawn npc (Wolv119)
- anty afk (Tibia Rules)
- runy na battle (Skulldiggers, Tibia Rules)
- summony (CVS, Tibia Rules, ja)
- niewidka dla gma (TRS)
- obracanie item�w (SuperGillis)
- ice rapier (Tibia Rules)
- construction kits (JTE)
- burst arrow (Skulldiggers)
- exiva (bryan007)
- amulety: wszystkie poza garlic i bronze (ja)
- ringi: skillowe, might, time, energy (ja)
- kolejka (ja)
- gildie z npcem (ja)
- pvp arena (ja)
- boh+hur+time ring (ja)
0.9.1
- burst arrow (Skulldiggers)
- destroy field (GriZzm0)
- exiva (Bryan)
- napisy na tabliczkach (ja)
- komendy: /pvp /owner /send
- wi�cej opcji w configu
- lepsze obliczanie obra�e�
- energy ring
- npc na ��dk�
- fieldy
0.9.2
- prostsze domki (DosMaster)
- nauka czar�w (ja)
- promocje (ja)
- komendy: /save /ban
- oddzielny distance mul
- wielolinijkowe signy
- leczenie summon�w
- poprawki obra�e�
- exhaust na uha
0.9.3
- system rooka (ja)
- czaty gildii (CVS)
- wygl�d itemu dla potwora/npca (Black Demon)
- config: expmulpvp, spearlosechance
- komendy: /pos /shutdown /max !uptime
- pochodnie, �wiece itp. - w miejscu na strza�y
- wszystkie czary �wiat�a
- mniejsze zu�ycie pami�ci
0.9.4
- protok� 7.6 (CVS)
- itemy OTB i mapy OTBM (CVS)
- dzie� i noc (CVS revmagsys)
- r�d�ki (Jiddo)
- system premium (ja)
- lista zgon�w z czasem (Jiddo)
- wysysanie �ycia i many (ja)
- garlic necklace i bronze amulet (ja)
- wysokie levele (Black Demon, ja)
- utana vid i stealth ring (ja)
- kolory krwi potwor�w (Tjin)
- prawdziwe potwory (KaM!oOoL, Shogun)
- komendy: /clean /premmy !premmy
- config: access*, max*, queuepremmy
- exani tera (CrazyToko)
- reload configa (Smygflik)
- drzwi levelowe przez actionId
- proste questy przez uniqueId
- gm widzi id i pozycj� itemu
- wiele aren pvp
0.9.4a
- config: rodrange, wandrange, freepremmy
0.9.4d
- tworzenie plik�w crashy (Andy Pennell)


Mapa by Aquisitor, update do 7.6 by Vide (na podstawie Ent-Online):
- 512x512, 1MB binarny
- ~1000 spawn�w
- 36 domk�w, 3 gh
- 12 quests�w (+anni)
- npc: sprzedaj�cy runy, jedzenie, meble; skupuj�cy loot
- temple x=160 y=54 z=7
- rook temple x=85 y=211 z=7


Komendy GMa:
/a x		skok do przodu o x kratek
/B msg		wiadomo�� do wszystkich
/b nick		ban na ip
/ban nick	ban postaci
/c nick		teleport gracza do siebie
/clean		usuwa �mieci le��ce na ziemi
/i id count	tworzenie item�w
/m name		postaw potwora
/summon name	postaw summona
/t		teleport do �wi�tyni
/goto nick	teleport obok gracza
/info nick	info o graczu
/closeserver	serwer tylko dla gm�w
/openserver	serwer dla wszystkich
/getonline	pokazuje graczy i levele
/kick nick	kickni�cie gracza
/up		teleport w g�r�
/down		teleport w d�
/invisible	prze��cz niewidk� gma
/max x		ustaw limit graczy
/pos		poka� moje po�o�enie
/premmy x nick	daj graczowi x godzin premii
/promote nick	daj graczowi promocj�
/pvp		ustaw pvp na brak=0, zwyk�e=1, enforced=2
/owner nick	zmie� w�a�ciciela domku w kt�rym stoisz
/owner		wyczy�� w�a�ciciela domku w kt�rym stoisz
/shutdown m	wy��cz serwer po up�ywie m minut
/s name		ustawia npca
/save		wymu� zapis stanu serwera
/send nick, x y z	przenie� gracza do podanych wsp�rz�dnych


Komendy gracza:
!exp		poka� exp brakuj�cy do levela
!mana		poka� man� brakuj�c� do mlevela
!online		lista graczy online
!house		prze�aduj prawa do domk�w
!frags		poka� ilo�� zabitych (unjust)
!report msg	bug report dla hostera
!uptime		wy�wietl czas dzia�ania serwera
!premmy		zobacz ilo�� godzin premii


Komendy Guild Mastera:
found		zak�adanie nowej gildii
invite		zapraszanie ludzi do gildii
kick, exclude	wyrzucanie z gildii
join		do�aczanie sie do gildii
leave		opuszczenie gildii,
pass		przekazanie przywodztwa
vice		awansowanie na vice-lidera
member		degradacja vice-lidera
nick, title	ustawienie opisu w nawiasie


Czary do domk�w:
aleta gom	edycja ownera (mo�e by� tylko jeden)
aleta som	edycja listy sub-ownerow
aleta grav	edycja listy door-ownerow (przy drzwiach)
aleta sio	edycja listy gosci
alana sio "Nick	wyrzucenie gracza z domku
alana sio	ucieczka z domku


Uwagi:
- gildie i domki zapisuj� si� podczas save servera, nie w momencie wylogowania gracza,
- stawiaj�c bossa u�ywaj /m, poniewa� /summon tworzy summona (nie ma loota),
- itemy 1740,1747,1748,1749,1770 s� przeznaczone na skrzynki questowe (nie mo�na ich podnosi� ani przesuwa� - znacznik questbox="1" w items.xml),
- ban na ip trwa do resetu serwa, ban na posta� trwa dop�ki nie ustawisz banned na 0 w pliku gracza,
- kiedy zamieniasz ring z ziemi z tym na r�ce, ten kt�ry wyl�duje na ziemi dalej b�yszczy (bez skutk�w ubocznych),
- kiedy niesiesz zapalon� pochodni�, �wiec� itp. czary �wiat�a s� ignorowane,
- plague-, magic-, flame- and poisonthrowers powoduj� debugi (nie u�ywaj ich).



Znane b��dy:
- kiedy b�d�c niewidzialnym znajdziesz si� w tym samym miejscu co gracz, on dostaje debuga,
- kiedy zmieniasz �wiat z pvp na no-pvp za pomoc� komendy /pvp, gracz kt�ry mia� zaznaczonego innego gracza mo�e nadal atakowa�,
- niekt�re kraw�dzie znikaj� pod plamami krwi.


Poprawione b��dy:
0.9.1
- bugi w skull systemie (zbyt wczesne znikanie red skulla i skull za uhanie kogo�),
0.9.2
- npce ko�cz� rozmow� gdy odejdziesz od nich zbyt daleko,
- kiedy u�yjesz komendy /owner, gracz nie musi u�ywa� !house aby wej�� do domku,
- skull system dzia�a tylo na serwerach z normalnym pvp,
- stone skin amulet daje tylko 95% ochrony przed obra�eniami,
- w�amywanie si� do domk�w za pomoc� "bugu z teleportem",
0.9.3
- nie musisz dodawa� frontowych drzwi w houses.xml,
- nie tracisz backpacka gdy diepercent jest r�wne 0,
- graczom z promocj� regeneruje si� �ycie,
- drzwi od domk�w wypychaj� ci� przy zamkni�ciu,
- mo�na tworzy� domki poza obszarem 512x512,
- w��cznie nie znikaj� (Beet Da Brat),
- burst arrowy nie powoduj� exhausta,
- nie mo�na strzela� przez drzwi,
- tusk table obraca si� poprawnie,
- dzia�aj� wszystkie dziury.
0.9.4
- boxy kupione w sklepie meblowym mo�na otwiera� od razu (Alreth),
- przed u�yciem construction kita musisz po�o�y� go na ziemi (Alreth),
- rzeczy w domkach s� zapisywane ze wszystkimi atrybutami (Kiper),
- po zabiciu gracza dostajesz pzlocka na <whitetime> minut,
- pkowanie summonami liczy si� do skulli,
- trzeba uczy� si� wbudowanych czar�w,
- npce nie powoduj� crashy (Tibia Rules),
0.9.4a
- nie dostajesz debuga przy otwieraniu nie-questowej skrzynki,
0.9.4b
- mo�na za�o�y� axe ring i dzia�a sword ring,
0.9.4c
- potwory trac� niewidk� po trafieniu magi�,
- kiedy potw�r staje si� niewidzialny atakuj�cy traci cel,
- tutorzy otrzymuj� obra�enia od potwor�w,
- potrzebujesz pacca �eby kupi� promocj�,
- b��dy przy konwersji mapy (Vide).
0.9.4d
- potwory atakuj� gracza w momencie przywo�ania (Smygflik),
- gm-owie z tym samym accessem widz� si� mimo niewidki.


Mi�ej zabawy!