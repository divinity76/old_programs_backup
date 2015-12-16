
#ifdef DO_NOT_DEFINE_THIS

unfortunately, i did not write up all changes in the beginning, but the major
features are:

YurOTS HHB SV Edition
based on OTServ trunk and YurOTS 0.9.4F and code from various 8X servers
--Major code cleanups, reduced memory/CPU usage by several megabytes, and better cpu utilization.
--new Map loader, Huge speedups (like Yurots 094F map loader uses 1400 milliseconds to load default YurOTS map.
the new map loader uses 200-300 milliseconds...both compiled with: optimizations: -s -O2 -march=nocona -mtune=nocona mingw32-g++ 3.4.2 (dev-cpp 5.0 beta release mingw))
--Chase Monster Mode working! (knights will be happy)
--Follow Mode
--Safe Mode (no attack player on white hand) working
--House system. (also rent system, daily/weekly/monthly, if you dont have enough cash,
you loose your house and items teleported to DP, etc)
--Move items from floor/stackable etc fixed.
--Support for players.xml system (Major speedup for stuff like Nicaw Highscore list/guild list/etc)
--Support for Remeres Map Editor house system and (optional-name) houses.xml (while stil, if mapped with
                        simOnes or without houses.xml support, will default to houses.xml :D)
--Support for Remeres Map Editor "place NPC" system
_________________________________________________________________________________
--New skull system, fixed various bugs (like monster-attack bugs skull timeout), and yellow skull system working! __SKULLSYSTEM__
--Multi config.lua based on exe name. system -HHB_MULTI_CONFIG_LUA
Pvp.exe.config.lua, Mainserver.exe.config.lua, testserver.config.lua etc.
--new party system -- HHB_PARTY_SYSTEM
--new invisibility system, supposedly "perfect"
(no debug, no ill usion bug, etc, and cheat tools cannot detect it...)- __PB_GMINVISIBLE__
--all gms get Maximum speed supported by the client. -- HHB_GM_MAX_SPEED
--Fast Attack system!, speed in config.lua "attack_speed" (in milliseconds...) - HHB_ATTACK_SPEED
--GMs can move Anything from anywhere... --HHB_GM_MOVE - Hardcoded to access 3+
--Life rings/Ring of healing works --RULY_LIFE_RINGS


///DEFINES
BDB_UTEVO_LUX
YUR_RINGS_AMULETS
RULY_LIFE_RINGS
HHB_GM_MAX_SPEED
HHB_O2_CRASHFIX
__PB_GMINVISIBLE__
HHB_MULTI_CONFIG_LUA
__SKULLSYSTEM__
HHB_PARTY_SYS
TEM
HHB_NULL_GM (just press enter, and it defaults to account 111111 / tibia)
HHB_ATTACK_SPEED
YUR_CMD_EXT
YUR_READABLES
TLM_BUY_SELL
WOLV_LOAD_NPC
BD_ITEM_LOOK
///
RULY_LIFE_RINGS
YUR_RINGS_AMULETS
YUR_CMD_EXT
YUR_READABLES
TLM_BUY_SELL
WOLV_LOAD_NPC
BD_ITEM_LOOK
///
TODOS:
    Fix Light, utevo lux etc..
______HIGH PRIORITYS ^___________________________
YUR_READABLES isnt done
WOLV_LOAD_NPC isnt done..
YUR_CLEAN_MAP isnt done..

Commands extension?YUR_CMD_EXT has a few todos.
LOW PRIORITY TODOS:
fix the last remaining stuff in invisible? (namely walking in gms... :p)
#endif













#ifdef YUR_READABLES_DO_NOT_DEFINE_THIS
bool Readables::Load(Game *game)
{
	std::string file = g_config.getGlobalString("datadir") + "readables.xml";
	xmlDocPtr doc;
	xmlMutexLock(xmlmutex);

	doc = xmlParseFile(file.c_str());
	if (!doc)
		return false;

	xmlNodePtr root, readableNode;
	root = xmlDocGetRootElement(doc);
	if (xmlStrcmp(root->name, (const xmlChar*)"readables"))
	{
		xmlFreeDoc(doc);
		xmlMutexUnlock(xmlmutex);
		return false;
	}

	readableNode = root->children;
	while (readableNode)
	{
		if (strcmp((char*) readableNode->name, "readable") == 0)
		{
			int x = atoi((const char*)xmlGetProp(readableNode, (const xmlChar *) "x"));
			int y = atoi((const char*)xmlGetProp(readableNode, (const xmlChar *) "y"));
			int z = atoi((const char*)xmlGetProp(readableNode, (const xmlChar *) "z"));
			std::string text = (const char*)xmlGetProp(readableNode, (const xmlChar *) "text");
fix the last remaining stuff in invisible? (namely walking in gms... :p)

			for (size_t i = 0; i < text.length()-1; i++)	// make real newlines
				if (text.at(i) == '\\' && text.at(i+1) == 'n')
				{
					text[i] = ' ';
					text[i+1] = '\n';
				}

			Tile* tile = game->getTile(x, y, z);
			if (tile)
			{
				Thing* thing = tile->getTopThing();
				Item* item = thing? dynamic_cast<Item*>(thing) : NULL;

				if (item)
                {
                    std::cout << "setting text \""<<text<<"\" on tile:"<<
                    x<<","<<y<<","<<z<<std::endl;
					item->setReadable(text);
					item->
                }
				else
				{

					std::cout << "\nTop thing at " << Position(x,y,z) << " is not an item!";
					return false;
				}
			}
			else
			{
				std::cout << "\nTile " << Position(x,y,z) << " is not valid!";
				return false;
			}
		}
		readableNode = readableNode->next;
	}

	xmlFreeDoc(doc);
	xmlMutexUnlock(xmlmutex);
	return true;
}
#endif //YUR_READABLES
