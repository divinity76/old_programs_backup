---------- Wanna make changes in game and reload config? Use /reload config ----------
----------------------------1337SOT CONFIGURATION---------------------------

-- house price for 1 sqm
priceforsqm = 1000

-- max houses for player
maxhouses = 1

-- required level to buy a house
houselevel = 30

-- Do you want to enable !buyhouse(yes/no)
buyhouseable = "no"

----- config animated text -----(Type the set command tomorow and a word. Example: ~HELLO!)
animatedtext = "~"

--Do you want the fancy skill advance animations?!
animations = "yes"
-- color of advance animations 
-- (darkblue = 2, red = 180, orange = 199, gray = 121, green = 84) 
-- (lime = 102, yellow = 169, light blue = 71, white = 983) 
anicolor = 102

--Auto Save time for each player.... 1 = 1min
saveTime = 2

-- How many % increase magic damage when u have blue robe? (2656)
bluerobe = 8

-- speed of attack in seconds (no-voc, sorc, druid, pally, knight)
speed = {"1.5", "2.5", "2.5", "1.0", "0.5"}

-- Life Ring Configuration
LifeRingHealth = "10"
LifeRingMana = "10"

-- ROH Configuration
RoHHealth = "15"
RoHMana = "15"

--Better Server saves, to prevent lagg... i usuly set players every 5 mins+the auto player save shown above, houses 20 mins and guids 30 then a real serversave every 1 hour
-- Char saving.. yes/no
charSaving = "no"

-- Guild saving.. yes/no
guildSaving = "no"

-- House saving.. yes/no
houseSaving = "no"

-- how often do server saves characters  (1 = 1min)  **Requires  Char saving..  enabled.
playersave = 5

-- how often do server saves houses (1 = 1min)  **Requires  House saving..  enabled.
housesave = 20
-- how often do server saves guilds  (1 = 1min)  **Requires  Guild saving..  enabled.
guildsave = 30


-- How many premmy days the player get when say !buypremmy
numpremmydays = 5

-- How many the player pay when say !buypremmy
pricepremmy = 5000


-- Cost to change gender using !gender
gendercost = 2000



-------Town Teleports

-- Teleport to town 1 (/tmp 1)
town1x = 141
town1y = 133
town1z = 7

-- Teleport to town 2 (/tmp 2)
town2x = 317
town2y = 311
town2z = 7

-- Teleport to town 3 (/tmp 3)
town3x = 122
town3y = 200
town3z = 7


-- Additional server comments (Viewed when a player types !info) (To start a new line type \n. Example: -- Hello\nMy\nName\nIs\nBncplix   (At each \n it will be on a seperate line, so 1 word per line in that -- case)
othercomments = "New Record, 69 people logged in.  =)"

-- Show blue energy ball when u click teleport after enabling /tp?
tpeffect = "no"




-- The average hit a gm does with melee (Default is 0)
gmdamage = 1


---- UCB Suspend System
-- Suspend time period (in minuts)
suspend_time_max = 0

-- Acc tries to be suspended in the period
suspend_acc_tries = 0

-- Same IP loggin tries to be suspended in the period
suspend_ip_tries = 0

-- Message to user when he got suspended
suspendmsg = "Ip block for 5 minutes for getting password wrong too many times."

-- Access lvl required to broadcast without a name with /bc (Twilite Aura Server Sources)
bcNoName = 4

-- When a player is baned, do u want the console to say :: >>PLAYERNAME<< Was just banned. ??
banmsg = "yes"

---------------------------END 1337SOT CONFIGURATION---------------------

---------------------------- OTServ configuration ------------------------------

-- datadir
datadir = "data/"

-- sets what map to load
map = "data/world/test.otbm"

-- OTBM for binary, SQL for SQL map, XML for OTX maps
mapkind = "OTBM"

-- SQL for acc/player info from SQL, XML for acc/player info from XML
sourcedata = "XML"

-- the message the player gets when he logs in
loginmsg = "Welcome to 1337sOT."

-- the port otserv listens on
port = "7171"

-- name of our server
servername = "1337sOT"

-- name of the owner of our server
ownername = "God 1337"

-- email of the owner of our server
owneremail = "God-1337@hotmail.com"

-- the url for more server info
url = "http://1337sOT.no-ip.org/index.php"

-- the location of the server
location = "Canada"

-- the ip the server should redirect to--type  auto   for auto ip detect
ip = "auto"

-- The messagebox you sometimes get before you choose characters
motd = "Welcome to 1337sOT 7.6! We are now 5000x exp rpg!"
motdnum="0"

-- use md5 passwords for accounts, yes/no
md5passwords = "no"

-- world type. options: pvp, no-pvp, pvp-enforced
worldtype = "no-pvp"

-- max number of players allowed
maxplayers = "70"

-- exhausted time in ms (1000 = 1sec)
exhausted = 1000

-- exhaustion time for healing spells (1000 = 1sec)
exhaustedheal = 1000

-- how many ms to add if the player is already exhausted and tries to cast a spell (1000 = 1sec)
exhaustedadd = 0

-- how long does the player has to stay out of fight to get pz unlocked in ms (1000 = 1sec)
pzlocked = 10*1000

-- allow multiple logins of the same char
allowclones = 0

-- vocation names
vocations = {"a 1337 sorcerer", "a 1337 druid", "a 1337 paladin", "a 1337 knight"}
promoted_vocations = {"a master sorcerer", "an elder druid", "a royal paladin", "an elite knight"}


--- SQL part
sql_host = "lo234sdfsdfsdftlhost"
sql_user = "ro324dfsdf"
sql_pass = ""
sql_db   = "otserv"


--- SQL MAP part
sqlmap = "test_map"
map_host = "loc234234fge4lhost"
map_user = "rosdfe234234"
map_pass = ""
map_db   = ""

------------------------- YurOTS basic configuration ---------------------------

-- name of your world (seen on the character list when logging in)
worldname = "1337sOT"

-- how often do server saves it's state (0 = off, 5 = 5min, 60 = hour)
autosave = 20

-- do you want to enable cap system? (yes/no)
capsystem = "no"

-- do you want players to learn spells before they can use them? (yes/no)
learnspells = "no"

-- do you want to give premium account to all players? (yes/no)
freepremmy = "yes"

-- do you want premium players to wait in queue as others? (yes/no)
queuepremmy = "yes"

-- how much % of {exp, mana, skill, eq, bp} do you lose when dying
diepercent = {"7", "7", "7", "7", "100"}

-- how many summons player can have
maxsummons = 2

-- do you want to give summons for all vocations? (yes/no)
summonsallvoc = "no"

-- chance of losing a spear when shot (0 - none lost, 100000 - all lost)
spearlosechance = 50

-- anti-afk - maximum idle time to kick player (1 = 1min)
kicktime = 15

-- maximum number of items player can keep in his/her depot
maxdepotitems = 3000

-- maximum number of items on a house tile (including those in containers)
maxhousetileitems = 300

-- how much death entries are saved in players file (old are deleted)
maxdeathentries = 10

-- mana consumend by {snakebite, moonlight, volcanic, quagmire, tempest} rod
rodmana = {"2", "3", "5", "8", "13"}

-- mana consumed by wand of {vortex, dragonbreath, plague, cosmic energy, inferno}
wandmana = {"2", "3", "5", "8", "13"}

-- shooting range of {snakebite, moonlight, volcanic, quagmire, tempest} rod
rodrange = {"6", "5", "4", "3", "2"}

-- shooting range of  wand of {vortex, dragonbreath, plague, cosmic energy, inferno}
wandrange = {"6", "5", "4", "3", "2"}

-- damage of burst arrows blast
-- default is from (1*lvl+5*mlvl)*0.24 to (1*lvl+5*mlvl)*0.55
burstarrowdmg = {"1.0", "5.0", "0.24", "0.55"}

--------------------------- Multipliers and gains ------------------------------

-- experience multiplier (how much faster you got exp from monsters)
expmul = 5000

-- experience multiplier for pvp-enforced (how much faster you got exp from players)
expmulpvp = 2

-- skill multipliers: 1 - rl tibia, 10 - ten times faster etc. {no-voc, sorcerer, druid, paladin, knight}
weaponmul = {"1", "1", "1", "1", "30"}
distmul = {"1", "1", "1", "30", "1"}
shieldmul = {"1", "1", "1", "30", "30"}
manamul = {"10","10", "10", "10", "10"}

-- how much points of life,mana,cap do you get when advancing {no-voc, sorcerer, druid, paladin, knight}
capgain = {"10", "10", "10", "20", "25"}
managain = {"5", "30", "30", "15", "5"}
hpgain = {"5", "5", "5", "10", "15"}

-- how much faster do you recovery life,mana after eating food (1 - rl tibia)
healthtickmul = 100
manatickmul = 100
-------------------------------- Skull system ----------------------------------

-- how many unjustified kills to get a red skull
redunjust = 3

-- how many unjustified kills to get banned
banunjust = 4

-- how long do you have white skull after attacking player (1 = 1min)
hittime = 1

-- how long do you have white skull after killing player (1 = 1min)
whitetime = 15

-- how long do you have red skull (1 = 1min)
redtime = 2*60

-- how long do you have to wait to lose 1 unjustified kill (1 = 1min)
fragtime = 1*60
------------------------------ GM access rights --------------------------------

-- access to walk into houses and open house doors
accesshouse = 4

-- access to login without waiting in the queue or when server is closed
accessenter = 3

-- access to ignore damage, exhaustion, cap limit and be ignored by monsters
accessprotect = 2

-- access to broadcast messages and talk in colors (#c blabla - in public channels)
accesstalk = 1

-- access to move distant items from/to distant locations
accessremote = 4

-- access to see id and position of the item you are looking at
accesslook = 2