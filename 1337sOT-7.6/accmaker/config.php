<?
//---------------------------- OTS directories  --------------------------------
$diraccount = 'c:\\YurOTS\\data\\accounts';
$dirplayer = 'c:\\YurOTS\\data\\players';
$dirvip = 'c:\\YurOTS\\data\\vip';


//----------------------------- All vocations ----------------------------------
$lvl = '10'; 	// initial level
$exp = '9300';	// initial exp (must be correct with level)
$cap = '300';	// initial capacity

// spawn place
$spawnx = '160';
$spawny = '54';
$spawnz = '7';

// temple
$templex = '160';
$templey = '54';
$templez = '7';

// do not change!
if (!defined('MALE')) define('MALE', 1);
if (!defined('FEMALE')) define('FEMALE', 0);
if (!defined('SORC')) define('SORC', 1);
if (!defined('DRUID')) define('DRUID', 2);
if (!defined('PALADIN')) define('PALADIN', 3);
if (!defined('KNIGHT')) define('KNIGHT', 4);

//-------------------------------- Sorcerer ------------------------------------
// Looks$look[SORC][MALE] = '130';
$look[SORC][FEMALE] = '138';

// HP, mana, magic level
$health[SORC] = '195';
$mana[SORC] = '95';
$mlvl[SORC] = '30';

// Skills:          fist,club,sword,axe,dist,shld,fish
$skill[SORC] = array(10,  10,  10,  10,  10,  15,  10);

// Eq:               helm, amul, bp,  armor, rght, left, legs, boot, ring, ammo
$equip[SORC] = array(2490, 2129, 1988, 2463, 2423, 2525, 2468, 2643, 2164, 0);

// Backpack:
$bp[SORC] = '<item id="2152" count="2"/><item id="2311" count="10"/>';

//--------------------------------- Druid --------------------------------------// Looks
$look[DRUID][MALE] = '130';
$look[DRUID][FEMALE] = '138';

// HP, mana, magic level
$health[DRUID] = '195';
$mana[DRUID] = '95';
$mlvl[DRUID] = '30';

// Skills:           fist,club,sword,axe,dist,shld,fish
$skill[DRUID] = array(10,  10,  10,  10,  10,  15,  10);

// Eq:                helm, amul, bp,  armor, rght, left, legs, boot, ring, ammo$equip[DRUID] = array(2490, 2129, 1988, 2463, 2423, 2525, 2468, 2643, 2164, 0);

// Backpack:
$bp[DRUID] = '<item id="2152" count="2"/><item id="2311" count="10"/>';

//-------------------------------- Paladin -------------------------------------
// Looks$look[PALADIN][MALE] = '129';
$look[PALADIN][FEMALE] = '137';

// HP, mana, magic level
$health[PALADIN] = '205';
$mana[PALADIN] = '65';
$mlvl[PALADIN] = '12';

// Skills:             fist,club,sword,axe,dist,shld,fish
$skill[PALADIN] = array(10,  10,  10,  10,  71,  41,  10);

// Eq:                  helm, amul, bp,  armor, rght, left, legs, boot, ring, ammo
$equip[PALADIN] = array(2490, 2129, 1988, 2463, 2455,     0, 2468, 2643, 2164, 0);

// Backpack:
$bp[PALADIN] = '<item id="2152" count="2"/><item id="2543" count="10"/>';

//--------------------------------- Knight -------------------------------------
// Looks$look[KNIGHT][MALE] = '131';
$look[KNIGHT][FEMALE] = '139';

// HP, mana, magic level
$health[KNIGHT] = '215';
$mana[KNIGHT] = '45';
$mlvl[KNIGHT] = '4';

// Skills:            fist,club,sword,axe,dist,shld,fish
$skill[KNIGHT] = array(10,  71,  71,  71,  10,  71,  10);

// Eq:                 helm, amul, bp,  armor, rght, left, legs, boot, ring, ammo$equip[KNIGHT] = array(2490, 2129, 1988, 2463, 2423,  2525, 2468, 2643, 2164, 0);

// Backpack:
$bp[KNIGHT] = '<item id="2152" count="2"/><item id="2311" count="3"/>';
?>