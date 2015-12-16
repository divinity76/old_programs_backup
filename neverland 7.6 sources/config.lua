---------------------------- OTServ configuration ------------------------------

-- datadir
datadir = "data/"

-- sets what map to load
map = "data/world/neverland.otbm"

-- OTBM for binary, SQL for SQL map, XML for OTX maps
mapkind = "OTBM"

-- SQL for acc/player info from SQL, XML for acc/player info from XML
sourcedata = "XML"

-- the message the player gets when he logs in
loginmsg = "Welcome to Neverland"

-- the port otserv listens on
port = "7171"

-- name of our server
servername = "Neverland"

-- name of the owner of our server
ownername = "Ruly"

-- email of the owner of our server
owneremail = ""

-- the url for more server info
url = ""

-- the location of the server
location = "Venezuela"

-- the ip the server should redirect too
ip = "200.35.212.31"

-- The messagebox you sometimes get before you choose characters
motd = "Welcome to Neverland"
motdnum="1"

-- use md5 passwords for accounts, yes/no
md5passwords = "no"

-- world type. options: pvp, no-pvp, pvp-enforced
worldtype = "pvp"

-- max number of players allowed
maxplayers = "30"

-- exhausted time in ms (1000 = 1sec)
exhausted = 1000

-- exhaustion time for healing spells (1000 = 1sec)
exhaustedheal = 1000

-- how many ms to add if the player is already exhausted and tries to cast a spell (1000 = 1sec)
exhaustedadd = 100

-- how long does the player has to stay out of fight to get pz unlocked in ms (1000 = 1sec)
pzlocked = 20*1000

-- allow multiple logins of the same char
allowclones = 0

-- vocation names
vocations = {"a sorcerer", "a druid", "a paladin", "a knight"}
promoted_vocations = {"a master sorcerer", "an elder druid", "a royal paladin", "an elite knight"}


--- SQL part
sql_host = "localhost"
sql_user = "root"
sql_pass = ""
sql_db   = "otserv"


--- SQL MAP part
sqlmap = "test_map"
map_host = "localhost"
map_user = "root"
map_pass = ""
map_db   = ""

---------------------------- Save configuration ------------------------------
-- how often do server saves it's state complete (0 = off, 5 = 5min, 60 = hour)
autosave = 300

-- Char saving.. yes/no
charSaving = "yes"

-- Guild saving.. yes/no
guildSaving = "yes"

-- House saving.. yes/no
houseSaving = "yes"

-- how often do server saves characters  (1 = 1min)  **Requires  Char saving..  enabled.
playersave = 30

-- how often do server saves houses (1 = 1min)  **Requires  House saving..  enabled.
housesave = 60

-- how often do server saves guilds  (1 = 1min)  **Requires  Guild saving..  enabled.
guildsave = 100

-------------------------Neverland configuration-----------------------
-- do you want to enable cap system (yes/no)
capsystem = "no"

-- do you want players to learn spells before they can use them (yes/no)
learnspells = "no"

-- how many summons player can have
maxsummons = 2

-- maximum number of items on a house tile (including those in containers)
maxhousetileitems = 10

-- do you want to give summons for all vocations (yes/no)
summonsallvoc = "no"

-- chance of losing a spear when shot (0 - none lost, 100000 - all lost)
spearlosechance = 50

-- anti-afk - maximum idle time to kick player (1 = 1min)
kicktime = 15

--Mc = 0 You cant use Mc. Mc = 1 You can use MC.
mc = "1"

-- "no" if you dont want thunders in the night, "yes" if you want thunders =)
thundersystem = "yes"

-- Name of City when depot id is 1 (for send parcels)
city1 = "neverland"
-- Name of City when depot id is 2 (for send parcels)
city2 = "iceland"
-- Name of City when depot id is 3 (for send parcels)
city3 = "yinyefon"
-- Name of City when depot id is 4 (for send parcels)
city4 = "sherwood"
-- Name of City when depot id is 5 (for send parcels)
city5 = "haley"
-- Name of City when depot id is 6 (for send parcels)
city6 = "ingenia"

---------------------- Soul System configuration ----------------------
-- how many soul pointss you gain when kill a monster
soulup = "1"

-------------------------House configuration---------------------------
-- house price for 1 sqm
priceforsqm = 1000

-- max houses for player
maxhouses = 2

-- Do you want to enable !buyhouse(yes/no)
buyhouseable = "yes"
-------------------------Deaths configuration---------------------------
-- how much % of exp/skills/stuff do you lose when dying if you dont have blessings and you dont have promotion
diepercent = 10
diebless0 = 10

-- how much % of exp/skills/stuff do you lose when dying if you dont have blessings and you have promotion
diebless0promo = 7

-- how much % of exp/skills/stuff do you lose when dying if you have 1 blessing and you dont have promotion
diebless1 = 9

-- how much % of exp/skills/stuff do you lose when dying if you have 2 blessings and you dont have promotion
diebless2 = 8

-- how much % of exp/skills/stuff do you lose when dying if you have 3 blessings and you dont have promotion
diebless3 = 7

-- how much % of exp/skills/stuff do you lose when dying if you have 4 blessings and you dont have promotion
diebless4 = 6

-- how much % of exp/skills/stuff do you lose when dying if you have 5 blessings and you dont have promotion
diebless5 = 5

-- how much % of exp/skills/stuff do you lose when dying if you have 1 blessing and you have promotion
diebless1promo = 6

-- how much % of exp/skills/stuff do you lose when dying if you have 2 blessings and you have promotion
diebless2promo = 5

-- how much % of exp/skills/stuff do you lose when dying if you have 3 blessings and you have promotion
diebless3promo = 4

-- how much % of exp/skills/stuff do you lose when dying if you have 4 blessings and you have promotion
diebless4promo = 3

-- how much % of exp/skills/stuff do you lose when dying if you have 5 blessings and you have promotion
diebless5promo = 2

--------------------------- Multipliers and gains ------------------------------

-- mana consumend by {snakebite, moonlight, volcanic, quagmire, tempest} rod
rodmana = {"2", "3", "5", "8", "13"}

-- mana consumed by wand of {vortex, dragonbreath, plague, cosmic energy, inferno}
wandmana = {"2", "3", "5", "8", "13"}

-- shooting range of {snakebite, moonlight, volcanic, quagmire, tempest} rod
rodrange = {"4", "3", "2", "1", "3"}

-- shooting range of  wand of {vortex, dragonbreath, plague, cosmic energy, inferno}
wandrange = {"4", "3", "2", "1", "3"}

-- experience multiplier (how much faster you got exp from monsters)
expmul = 33

-- experience multiplier for pvp-enforced (how much faster you got exp from players)
expmulpvp = 2

-- skill multipliers: 1 - rl tibia, 10 - ten times faster etc. {no-voc, sorcerer, druid, paladin, knight}
weaponmul = {"1", "1", "1", "1", "30"}
distmul = {"1", "1", "1", "30", "1"}
shieldmul = {"1", "1", "1", "20", "30"}
manamul = {"1", "3", "3", "3", "3"}

-- how much points of life,mana,cap do you get when advancing {no-voc, sorcerer, druid, paladin, knight}
capgain = {"10", "10", "10", "20", "25"}
managain = {"5", "30", "30", "15", "5"}
hpgain = {"5", "5", "5", "10", "15"}

-- how much faster do you recovery life,mana after eating food (1 - rl tibia)
healthtickmul = 10
manatickmul = 20

-- damage of burst arrows blast
-- default is from (1*lvl+5*mlvl)*0.24 to (1*lvl+5*mlvl)*0.55
burstarrowdmg = {"1.0", "5.0", "0.24", "0.55"}

-- How long work the utana vid in seconds
utanavidtime = 60

--How many people can have in the vip list
maxviplist = "30"

--Distance away from spawn to remove a monster - The Chaos
dist = "40"

-- How many poison hits from the poison arrow
poisonarrowhits = 5 

-- Max poison hit damage from the poison arrow
poisonarrowdamage = 10 

-- Minimun level for player can yell
minlvltoyell = 2

-- Muted System by Rex
mutedsystem = "yes"
mutedtime = "4"

-- How many % increase magic damage when u have blue robe?
bluerobe = 10 

--How many Hp/Mana you recover while Sleeping per second (60 seconds = [life +1 and mana +1])
bedregain = 60

-------------------------------- Skull system ----------------------------------
-- how many unjustified kills to get a red skull
redunjust = 3

-- how many unjustified kills to get banned
banunjust = 6

-- how long do you have white skull after attacking player (1 = 1min)
hittime = 1

-- how long do you have white skull after killing player (1 = 1min)
whitetime = 15

-- how long do you have red skull (1 = 1min)
redtime = 6*60

-- how long do you have to wait to lose 1 unjustified kill (1 = 1min)
fragtime = 12*60
------------------------------ GM System--------------------------------
-- access to ignore damage, exhaustion, cap limit and be ignored by monsters
accessprotect = 3

-- access to walk into houses and open house doors
accesshouse = 3

-- access to broadcast messages and talk in colors (#c blabla - in public channels)
accesstalk = 2

-- access to move distant items from/to distant locations
accessremote = 3

-- access to see id and position of the item you are looking at
accesslook = 2

-- access for report bugs with Ctrl+Z
accessbugreport = 1

-- access for talk orange in Help Channel
accessorange = 1

-- access for use ban window(aka ctrl+y)
accessctrly = 2

-- access for answer reports with Ctrl+R
accessreport = 2

-- only gamemaster can use mc system of tibia?
antigmmc = "yes"

-- Gamemaster cant attack players on melee or using runes? Yes/no
gmprotected = "no"
---------------------------- Premium configuration ------------------------------
-- do you want premium players to wait in queue as others? (yes/no)
queuepremmy = "yes"



-- How many premmy days the player get when say !buypremmy
manypremmydays = 1

-- How many the player pay when say !buypremmy
prricepremmy = 1000



-- How many days the player lost when say /sex (for change sex)
sexpremmydays = 5

-- Max Depot items for premmiums accounts
maxdepotpremmy = 2000

-- Max Depot items for free accounts
maxdepotfree = 1000



---------------------------- ACC MANAGER configuration ------------------------------
--Basic for all vocations
managerexp = 100000
managerlvl = 20

--Spawn and Temple when you create new char
managertemplex = 177
managertempley = 105
managertemplez = 8

--Capacity
managercapmage = 200
managercappally = 400
managercapknight = 400

--Sorc and Druid config
managermagehp = 230
managermagemana = 400
managermageml = 15
managermagefist = 12
managermageclub = 12
managermagesword = 12
managermageaxe = 12
managermagedist = 12
managermageshield = 12
managermagefish = 12

--Pally Config
managerpallyhp = 310
managerpallymana = 270
managerpallyml = 10
managerpallyfist = 12
managerpallyclub = 12
managerpallysword = 12
managerpallyaxe = 12
managerpallydist = 62
managerpallyshield = 42
managerpallyfish = 12

--Knight Config
managerknighthp = 450
managerknightmana = 100
managerknightml = 3
managerknightfist = 12
managerknightclub = 12
managerknightsword = 12
managerknightaxe = 50
managerknightdist = 12
managerknightshield = 50
managerknightfish  = 12


--Helmet
managerslot1 = 2457
--Armor
managerslot4 = 2463
--Shield
managerslot5 = 2516
--Weapon
managerslot6 = 2409
--legs
managerslot7 = 2647
--Boots
managerslot8 = 2643
--Ammo
managerslot10 = 2580

