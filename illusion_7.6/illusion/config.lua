-- map
-- sets what map to load, if sql, determines the sql map
mapstore = "data/world/map-mapstore.xml"
housestore = "data/world/houses.xml"
houserentperiod = "monthly"
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
loginmsg = "hi																																			qw2333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333356"

-- the port otserv listens on
port = "7171"

-- name of our server
servername = "Illusion"

-- name of the owner of our server
ownername = ""

-- email of the owner of our server
owneremail = ""

-- the url for more server info
url = ""

-- the location of the server
location = "Sweden"

-- the ip the server should redirect too
ip = ""

-- The messagebox you sometimes get before you choose characters
motd = ""
motdnum="1"

-- use md5 passwords for accounts, yes/no
md5passwords = "no"

-- world type. options: pvp, no-pvp, pvp-enforced
worldtype = "pvp"

-- max number of players allowed
maxplayers = "150"

-- exhausted time in ms (1000 = 1sec)
exhausted = 1300

-- exhausted time in ms (1000 = 1sec)
exhaustedheal = 1300

-- how many ms to add if the player is already exhausted and tries to cast a spell (1000 = 1sec)
exhaustedadd = 100

-- how long does the player has to stay out of fight to get pz unlocked in ms (1000 = 1sec)
pzlocked = 8*1000

-- allow multiple logins of the same char
allowclones = 0

-- vocation names
vocations = {"a wizard", "a mage", "a shooter", "a warrior"}
promoted_vocations = {"a dark wizard", "a holy mage", "a bowman", "a crusader"}


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


------------------------- YurOTS basic configuration ---------------------------

-- name of your world (seen on the character list when logging in)
worldname = "Illusion"

-- how often do server saves it's state (0 = off, 5 = 5min, 60 = hour)
autosave = 15

-- do you want to enable cap system? (yes/no)
capsystem = "no"

-- do you want players to learn spells before they can use them? (yes/no)
learnspells = "no"

-- do you want to give premium account to all players? (yes/no)
freepremmy = "yes"

-- do you want premium players to wait in queue as others? (yes/no)
queuepremmy = "yes"

-- how many summons player can have
maxsummons = 2

-- do you want to give summons for all vocations? (yes/no)
summonsallvoc = "no"

-- chance of losing a spear when shot (0 - none lost, 100000 - all lost)
spearlosechance = 0

-- anti-afk - maximum idle time to kick player (1 = 1min)
kicktime = 200

-- maximum number of items player can keep in his/her depot
maxdepotitems = 1000

-- maximum number of items premium player can keep in his/her depot
maxdepotitemspremium = 2000

-- maximum number of items on a house tile (including those in containers)
maxhousetileitems = 2000

-- how much death entries are saved in players file (old are deleted)
maxdeathentries = 10

-- mana consumend by {snakebite, moonlight, volcanic, quagmire, tempest} rod
rodmana = {"2", "3", "5", "8", "13"}

-- mana consumed by wand of {vortex, dragonbreath, plague, cosmic energy, inferno}
wandmana = {"2", "3", "5", "8", "13"}

-- shooting range of {snakebite, moonlight, volcanic, quagmire, tempest} rod
rodrange = {"10", "10", "10", "10", "10"}

-- shooting range of  wand of {vortex, dragonbreath, plague, cosmic energy, inferno}
wandrange = {"10", "10", "10", "10", "10"}

-- damage of burst arrows blast
-- default is from (1*lvl+5*mlvl)*0.24 to (1*lvl+5*mlvl)*0.55
burstarrowdmg = {"2.0", "5.0", "0.24", "0.55"}

--------------------------- DAMAGE PERCENTS ---------------------------------
-- Forms of attacks with any weapon
-- Full Attack,Balanced,Full Defense
attackforms = {"3.0", "2.0", "1.0"}

-- Forms of Defense of the Shield (NO IS INCLUDED ARMOR)
-- Full Attack,Balanced,Full Defense
defenseforms = {"0.2", "0.4", "0.7"}

-- Damage Form/x
-- Knight, Paladin
-- Default - {"2.0","1.5"}
dmg = {"2.0","1.5"}


--------------------------- DIE PERCENTS ---------------------------------

-- how much % of {exp, mana, skill, eq, bp} do you lose when dying
diepercent = {"3", "3", "0", "0", "0"}

-- how much % of {exp, mana, skill, eq, bp} do you lose when dying
diepercent_promotion = {"1", "1", "0", "0", "0"}

-- how much % of exp/skills/stuff do you lose when dying if you have 1 blessing and you dont have promotion
diebless1 = 1

-- how much % of exp/skills/stuff do you lose when dying if you have 2 blessings and you dont have promotion
diebless2 = 1

-- how much % of exp/skills/stuff do you lose when dying if you have 3 blessings and you dont have promotion
diebless3 = 1

-- how much % of exp/skills/stuff do you lose when dying if you have 4 blessings and you dont have promotion
diebless4 = 1

-- how much % of exp/skills/stuff do you lose when dying if you have 5 blessings and you dont have promotion
diebless5 = 1

-- how much % of exp/skills/stuff do you lose when dying if you have 1 blessing and you have promotion
diebless1promotion = 1

-- how much % of exp/skills/stuff do you lose when dying if you have 2 blessings and you have promotion
diebless2promotion = 1

-- how much % of exp/skills/stuff do you lose when dying if you have 3 blessings and you have promotion
diebless3promotion = 1

-- how much % of exp/skills/stuff do you lose when dying if you have 4 blessings and you have promotion
diebless4promotion = 1

-- how much % of exp/skills/stuff do you lose when dying if you have 5 blessings and you have promotion
diebless5promotion = 1

--------------------------- Multipliers and gains ------------------------------

-- experience multiplier (how much faster you got exp from monsters)
expmul = 100

-- experience multiplier for pvp-enforced (how much faster you got exp from players)
expmulpvp = 5

-- skill multipliers: 1 - rl tibia, 10 - ten times faster etc. {no-voc, sorcerer, druid, paladin, knight}
weaponmul = {"3", "20", "20", "3", "500"}
distmul = {"3", "3", "3", "500", "3"}
shieldmul = {"3", "3", "30", "500", "500"}
manamul = {"3", "40", "40", "15", "15"}

-- how much points of life,mana,cap do you get when advancing {no-voc, sorcerer, druid, paladin, knight}
capgain = {"10", "10", "10", "20", "35"}
managain = {"5", "25", "25", "20", "15"}
hpgain = {"5", "30", "30", "50", "80"}

-- how much faster do you recovery life,mana after eating food (1 - rl tibia)
healthtickmul = 300
manatickmul = 500

promotionhealthtickmul = 350
promotionmanatickmul = 550



-------------------------------- Skull system ----------------------------------

-- unjustified kill time (1 = 1min)
whiteskulltime = 1

-- redskull time (1 = 1min)
redskulltime = 60*1

-- unjustified kills to get a red skull
redskull = 3

-- unjustified kills to get banned
redskullban = 7

-- how many days the pk ban lasts (1=1day)
pkbandays = 1

-- how long do you have to wait to lose 1 unjustified kill (1 = 1min)
fragtime = 30*1

------------------------------ GM access rights --------------------------------

-- access to walk into houses and open house doors
accesshouse = 3

-- access to login without waiting in the queue or when server is closed
accessenter = 2

-- access to ignore damage, exhaustion, cap limit and be ignored by monsters
accessprotect = 3

-- access to broadcast messages and talk in colors (#c blabla - in public channels)
accesstalk = 1

-- access to move distant items from/to distant locations
accessremote = 3

-- access to see id and position of the item you are looking at
accesslook = 3
----------------------------------- DEVLAND OPTIONS ----------------------------

--Exp color
-- COLORS
-- (darkblue = 2, red = 180, orange = 199, gray = 121, green = 84) 
-- (lime = 102, yellow = 169, light blue = 71, white = 983) 
expcolor = 983

-- How many premmy days the player get when say !buypremmy
manypremmydays = 60

-- How many the player pay when say !buypremmy
prricepremmy = 1000

-- !buyhouse yes/no
buyhouse = "yes"

-- minimum level to buy house
levelhouse = 300

-- house price for 1 sqm
priceforsqm = 100000

-- max houses for player
maxhouses = 1

--How many Hp/Mana you recover while Sleeping per second (60 seconds = [life +1 and mana +1])
bedregain = 100

--Distance away from spawn to remove a monster - The Chaos
dist = 40

--Do you want the fancy animations?!
animations = "yes"
-- color of advance animations 
-- (darkblue = 2, red = 180, orange = 199, gray = 121, green = 84) 
-- (lime = 102, yellow = 169, light blue = 71, white = 983) 
anicolor = 84

--How many mana and Hp you recovery using Life Ring
liferingmana = "10"
liferinghealth = "10"

--How many mana and Hp you recovery using Ring of Healing
healingringmana = "5"
healingringhealth = "20"

--How many mana and Hp you recovery using Soft Boots
softmana = "10"
softhealth = "10"

-- How many percent increase magic damage when u have mystic turban?
mysticturban = 50

-- How many % increase magic damage when u have blue robe?
bluerobe = 100

dsmfire = 5

--Do you want Atack Animations (Fire AXE etc.)?!  yes/no
atackanimations = "yes"

-- Name of City when depot id is 1 (for send parcels)
city1 = "Northumbria"

-- Name of City when depot id is 2 (for send parcels)
city2 = "devland2"

-- Name of City when depot id is 3 (for send parcels)
city3 = "devland3"

-- hours to lost in sex change
sexchangehours = 10

-- speed of attack in seconds (no-voc, sorc, druid, pally, knight)
speed = {"0.1", "0.2", "0.2", "0.2", "0.2"}

-- drop rate multiplier (how much faster the itens will drop from monsters)
droprate = 10
--Auto Save time for each player.... 1 = 1min
saveTime = 30
----- config animated text -----
znaczek = "~"
--------------------------------------