//////////////////////////////////////////////////////////////////////
// Neverland - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#include <string>
#include <sstream>
#include <fstream>
#include <utility>

#include "commands.h"
#include "player.h"
#include "npc.h"
#include "game.h"
#include "actions.h"
#include "map.h"
#include "status.h"
#include "spells.h"

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

extern std::vector< std::pair<unsigned long, unsigned long> > bannedIPs;
extern Actions actions;
extern Spells spells;
extern bool readXMLInteger(xmlNodePtr p, const char *tag, int &value);

//table of commands
s_defcommands Commands::defined_commands[] = { 
	{"/s",&Commands::placeNpc},
	{"/m",&Commands::placeMonster},
	{"/summon",&Commands::placeSummon},
	{"/B",&Commands::broadcastMessage},
	{"/b",&Commands::banPlayer},
	{"/t",&Commands::teleportMasterPos},
	{"/c",&Commands::teleportHere},
	{"/i",&Commands::createItems},
	{"/q",&Commands::substract_contMoney},
	{"/reload",&Commands::reloadInfo},
	{"/z",&Commands::testCommand},
	{"/color",&Commands::color},
	{"/goto",&Commands::teleportTo},
	{"/info",&Commands::getInfo},
	{"/closeserver",&Commands::closeServer},
	{"/openserver",&Commands::openServer},
	{"/getonline",&Commands::onlineList},
	{"/a",&Commands::teleportNTiles},
	{"/giveexp",&Commands::doGiveExp},
	{"/city",&Commands::teleportToCity},
	{"/gmcity",&Commands::teleportToGm},
#ifdef TR_ADDSKILL
	{"/addskill",&Commands::addSkill},
#endif //TR_ADDSKILL
#ifdef JIDDO_RAID
    {"/raid",&Commands::doRaid},
#endif //JIDDO_RAID
#ifdef ART_WALKTELEPORT
	{"/tp",&Commands::TPthing},
#endif //ART_WALKTELEPORT
	#ifdef VITOR_COMMAND
    {"/coloredmsg",&Commands::SendColoredMessage},
    #endif //VITOR_COMMAND
	#ifdef REMERE_REMOVEITEM
    {"/removeitem",&Commands::playerRemoveItem},
    #endif //REMERE_REMOVEITEM
	#ifdef RUL_COMMANDS
	#ifdef RUL_BLESS
	{"/bless",&Commands::blessing},
	#endif //RUL_BLESS
	{"/tutor",&Commands::tutor},
	{"/speed",&Commands::speedChanger},
    #endif //RUL_COMMANDS
	#ifdef DT_IT
	{"/iname",&Commands::createItemsByName},
    #endif //DT_IT
    #ifdef DVOTE
    {"/vote",&Commands::vote},
    #endif //DVOTE
    #ifdef YUR_CMD_EXT
	{"/ban",&Commands::banCharacter},
	{"/kick",&Commands::kickPlayer},
	{"/namelock",&Commands::namelockPlayer},
	#ifdef BC_CHAOS
	{"/bc",&Commands::broadcastColor},
    #endif //BC_CHAOS
	{"/up",&Commands::goUp},
	{"/down",&Commands::goDown},
	{"/pos",&Commands::showPos},
	{"/pvp",&Commands::setWorldType},
	{"/send",&Commands::teleportPlayerTo},
	{"/max",&Commands::setMaxPlayers},
	{"!exp",&Commands::showExpForLvl},
	{"!mana",&Commands::showManaForLvl},
	{"!report",&Commands::report},
	{"!online",&Commands::whoIsOnline},
	{"!uptime",&Commands::showUptime},
	{"/redskull",&Commands::redskull},
	{"/noskull",&Commands::noskull},
    #endif //YUR_CMD_EXT
    {"/makesay",&Commands::makePlayerSay},
    #ifdef DRAZAX_GMOUTFIT
    {"/outfit",&Commands::outfitChange},
    #endif //DRAZAX_GMOUTFIT
    #ifdef EOTSERV_SERVER_SAVE
	{"/saveplayers",&Commands::forcePlayerSave},
	{"/savehouses",&Commands::forceHouseSave},
	{"/saveguild",&Commands::forceguildSave},
    #endif //EOTSERV_SERVER_SAVE
    #ifdef TLM_HOUSE_SYSTEM
	{"/owner",&Commands::setHouseOwner},
	{"!house",&Commands::reloadRights},
    #endif //TLM_HOUSE_SYSTEM
    #ifdef TRS_GM_INVISIBLE
	{"/invisible",&Commands::invisible},
    #endif //TRS_GM_INVISIBLE
    #ifdef TLM_SKULLS_PARTY
	{"!frags",&Commands::showFrags},
    #endif //TLM_SKULLS_PARTY
    #ifdef TLM_SERVER_SAVE
	{"/save",&Commands::forceServerSave},
    #endif //TLM_SERVER_SAVE
    #ifdef YUR_SHUTDOWN
	{"/shutdown",&Commands::shutdown},
    #endif //YUR_SHUTDOWN
    #ifdef YUR_CLEAN_MAP
	{"/clean",&Commands::cleanMap},
    #endif //YUR_CLEAN_MAP
    #ifdef YUR_PREMIUM_PROMOTION
	{"/promote",&Commands::promote},
    #endif //YUR_PREMIUM_PROMOTION
};


Commands::Commands(Game* igame):
game(igame),
loaded(false)
{
	//setup command map
	for(int i = 0;i< sizeof(defined_commands)/sizeof(defined_commands[0]); i++){
		Command *tmp = new Command;
		tmp->loaded = false;
		tmp->accesslevel = 1;
		tmp->f = defined_commands[i].f;
		std::string key = defined_commands[i].name;
		commandMap[key] = tmp;
	}
}

bool Commands::loadXml(const std::string &_datadir){
	
	datadir = _datadir;
	
	std::string filename = datadir + "commands.xml";
	std::transform(filename.begin(), filename.end(), filename.begin(), tolower);
	xmlDocPtr doc = xmlParseFile(filename.c_str());

	if (doc){
		this->loaded = true;
		xmlNodePtr root, p;
		root = xmlDocGetRootElement(doc);
		
		if (xmlStrcmp(root->name,(const xmlChar*) "commands")){
			xmlFreeDoc(doc);
			return false;
		}
		p = root->children;
        
		while (p)
		{
			const char* str = (char*)p->name;
			
			if (strcmp(str, "command") == 0){
				char *tmp = (char*)xmlGetProp(p, (const xmlChar *) "cmd");
				if(tmp){
					CommandMap::iterator it = commandMap.find(tmp);
					int alevel;
					if(it != commandMap.end()){
						if(readXMLInteger(p,"access",alevel)){
							if(!it->second->loaded){
								it->second->accesslevel = alevel;
								it->second->loaded = true;
							}
							else{
								std::cout << "Duplicated command " << tmp << std::endl;
							}
						}
						else{
							std::cout << "missing access tag for " << tmp << std::endl;
						}
					}
					else{
						//error
						std::cout << "Unknown command " << tmp << std::endl;
					}
					xmlFreeNVLAND(tmp);
				}
				else{
					std::cout << "missing cmd." << std::endl;
				}
			}
			p = p->next;
		}
		xmlFreeDoc(doc);
	}
	
	//
	for(CommandMap::iterator it = commandMap.begin(); it != commandMap.end(); ++it){
		if(it->second->loaded == false){
			std::cout << "Warning: Missing access level for command" << it->first << std::endl;
		}
		//register command tag in game
		game->addCommandTag(it->first.substr(0,1));
	}
	
	
	return this->loaded;
}

bool Commands::reload(){
	this->loaded = false;
	for(CommandMap::iterator it = commandMap.begin(); it != commandMap.end(); ++it){
		it->second->accesslevel = 1;
		it->second->loaded = false;
	}
	game->resetCommandTag();
	this->loadXml(datadir);
	return true;
}

bool Commands::exeCommand(Creature *creature, const std::string &cmd){
	
	std::string str_command;
	std::string str_param;
	
	unsigned int loc = (uint32_t)cmd.find( ' ', 0 );
	if( loc != std::string::npos && loc >= 0){
		str_command = std::string(cmd, 0, loc);
		str_param = std::string(cmd, (loc+1), cmd.size()-loc-1);
	}
	else {
		str_command = cmd;
		str_param = std::string(""); 
	}
	
	//find command
	CommandMap::iterator it = commandMap.find(str_command);
	if(it == commandMap.end()){
		return false;
	}
	Player *player = dynamic_cast<Player*>(creature);
	//check access for this command
	if(creature->access < it->second->accesslevel){
		if(creature->access > 0){
			player->sendTextMessage(MSG_SMALLINFO,"You can not execute this command.");
			return true;
		}
		else{
			return false;
		}
	}
	//execute command
	CommandFunc cfunc = it->second->f;
	(this->*cfunc)(creature, str_command, str_param);
	if(player)
		player->sendTextMessage(MSG_RED_TEXT,cmd.c_str());

	return true;
}
	

bool Commands::placeNpc(Creature* c, const std::string &cmd, const std::string &param)
{
	Npc *npc = new Npc(param, game);
	if(!npc->isLoaded()){
		delete npc;
		return true;
	}
	Position pos;
	// Set the NPC pos
	if(c->direction == NORTH) {
		pos.x = c->pos.x;
		pos.y = c->pos.y - 1;
		pos.z = c->pos.z;
	}
	// South
	if(c->direction == SOUTH) {
		pos.x = c->pos.x;
		pos.y = c->pos.y + 1;
		pos.z = c->pos.z;
	}
	// East
	if(c->direction == EAST) {
		pos.x = c->pos.x + 1;
		pos.y = c->pos.y;
		pos.z = c->pos.z;
	}
	// West
	if(c->direction == WEST) {
		pos.x = c->pos.x - 1;
		pos.y = c->pos.y;
		pos.z = c->pos.z;
	}
	// Place the npc
	if(!game->placeCreature(pos, npc))
	{
		delete npc;
		Player *player = dynamic_cast<Player*>(c);
		if(player) {
			player->sendMagicEffect(player->pos, NM_ME_PUFF);
			player->sendCancel("Sorry not enough room.");
		}
		return true;
	}
	return true;
}

bool Commands::placeMonster(Creature* c, const std::string &cmd, const std::string &param)
{
#ifdef LOOSIK_LOGSYSTEM
          std::string text = c->getName() + " summoned monster: " + param;
          logAction(dynamic_cast<Player*>(c), 3, "placeMonster.txt", text);
#endif //LOOSIK_LOGSYSTEM

	Monster* monster = new Monster(param, game);
	if(!monster->isLoaded()){
		delete monster;
		return true;
	}
	Position pos;

	// Set the Monster pos
	if(c->direction == NORTH) {
		pos.x = c->pos.x;
		pos.y = c->pos.y - 1;
		pos.z = c->pos.z;
	}
	// South
	if(c->direction == SOUTH) {
		pos.x = c->pos.x;
		pos.y = c->pos.y + 1;
		pos.z = c->pos.z;
	}
	// East
	if(c->direction == EAST) {
		pos.x = c->pos.x + 1;
		pos.y = c->pos.y;
		pos.z = c->pos.z;
	}
	// West
	if(c->direction == WEST) {
		pos.x = c->pos.x - 1;
		pos.y = c->pos.y;
		pos.z = c->pos.z;
	}

	// Place the monster
	if(!game->placeCreature(pos, monster)) {
		delete monster;
		Player *player = dynamic_cast<Player*>(c);
		if(player) {
			player->sendMagicEffect(player->pos, NM_ME_PUFF);
			player->sendCancel("Sorry, not enough room.");
		}
		return true;
	}
	else{
		//c->addSummon(monster);
		return true;
	}
}

bool Commands::placeSummon(Creature* c, const std::string &cmd, const std::string &param)
{
	Monster* monster = new Monster(param, game);
	if(!monster->isLoaded()){
		delete monster;
		return true;
	}
	Position pos;

	// Set the Monster pos
	if(c->direction == NORTH) {
		pos.x = c->pos.x;
		pos.y = c->pos.y - 1;
		pos.z = c->pos.z;
	}
	// South
	if(c->direction == SOUTH) {
		pos.x = c->pos.x;
		pos.y = c->pos.y + 1;
		pos.z = c->pos.z;
	}
	// East
	if(c->direction == EAST) {
		pos.x = c->pos.x + 1;
		pos.y = c->pos.y;
		pos.z = c->pos.z;
	}
	// West
	if(c->direction == WEST) {
		pos.x = c->pos.x - 1;
		pos.y = c->pos.y;
		pos.z = c->pos.z;
	}

	// Place the monster
	if(!game->placeCreature(pos, monster)) {
		delete monster;
		Player *player = dynamic_cast<Player*>(c);
		if(player) {
			player->sendMagicEffect(player->pos, NM_ME_PUFF);
			player->sendCancel("Sorry, not enough room.");
		}
		return true;
	}
	else{
		c->addSummon(monster);
		return true;
	}
}

bool Commands::broadcastMessage(Creature* c, const std::string &cmd, const std::string &param){
	game->creatureBroadcastMessage(c,param);
	return true;
}

bool Commands::banPlayer(Creature* c, const std::string &cmd, const std::string &param){
	
	Player* playerBan = game->getPlayerByName(param);
	if(playerBan) {
		MagicEffectClass me;
		
		me.animationColor = 0xB4;
		me.damageEffect = NM_ME_MAGIC_BLOOD;
		me.maxDamage = (playerBan->health + playerBan->mana)*10;
		me.minDamage = (playerBan->health + playerBan->mana)*10;
		me.offensive = true;

		game->creatureMakeMagic(NULL, playerBan->pos, &me);

		Player* player = dynamic_cast<Player*>(c);
		if(player && player->access <= playerBan->access){
			player->sendTextMessage(MSG_BLUE_TEXT,"You cannot ban this player.");
			return true;
		}

		playerBan->sendTextMessage(MSG_RED_TEXT,"You have been banned.");
		std::pair<unsigned long, unsigned long> IpNetMask;
		IpNetMask.first = playerBan->lastip;
		IpNetMask.second = 0xFFFFFFFF;
		if(IpNetMask.first > 0) {
			bannedIPs.push_back(IpNetMask);
			return true;
		}
	}

	return false;
}

bool Commands::teleportMasterPos(Creature* c, const std::string &cmd, const std::string &param){
	game->teleport(c, c->masterPos);
	return true;
}

bool Commands::teleportHere(Creature* c, const std::string &cmd, const std::string &param){
	Creature* creature = game->getCreatureByName(param);
	if(creature) {
		game->teleport(creature, c->pos);
	}
	return true;
}

bool Commands::createItems(Creature* c, const std::string &cmd, const std::string &param){
     #ifdef LOOSIK_LOGSYSTEM
     std::string text = c->getName() + " itemcreated: " + param;
logAction(dynamic_cast<Player*>(c), 3, "createitems.txt", text);
#endif //LOOSIK_LOGSYSTEM

	std::string tmp = param;
	
	std::string::size_type pos = tmp.find(' ', 0);
	if(pos == std::string::npos)
		return true;
	
	int type = atoi(tmp.substr(0, pos).c_str());
	tmp.erase(0, pos+1);
	int count = std::min(atoi(tmp.c_str()), 100);
				
	Item *newItem = Item::CreateItem(type, count);
	if(!newItem)
		return true;
	
	Tile *t = game->map->getTile(c->pos);
	if(!t)
	{
		delete newItem;
		return true;
	}

	game->addThing(NULL,c->pos,newItem);
	return true;

}

bool Commands::substract_contMoney(Creature* c, const std::string &cmd, const std::string &param){
	
	Player *player = dynamic_cast<Player *>(c);
	if(!player)
		return true;
				
	int count = atoi(param.c_str());
	unsigned long money = player->getMoney();
	if(!count)
	{
		std::stringstream info;
		info << "You have " << money << " of money.";
		player->sendCancel(info.str().c_str());
		return true;
	}
	else if(count > (int)money)
	{
		std::stringstream info;
		info << "You have " << money << " of money and is not suficient.";
		player->sendCancel(info.str().c_str());
		return true;
	}
	if(player->substractMoney(count) != true){
		std::stringstream info;
		info << "Can not substract money!";
		player->sendCancel(info.str().c_str());
	}
	return true;
}

bool Commands::reloadInfo(Creature* c, const std::string &cmd, const std::string &param)
{	
        Player *player = dynamic_cast<Player *>(c);
	if(param == "actions"){
		actions.reload();
	}
	else if(param =="commands"){
		this->reload();
	}
		else if(param == "config"){
		g_config.OpenFile("config.lua");
	}
	else if(param == "spells") {
        std::map<std::string, Spell*>::iterator it = spells.allSpells.begin();

	    while(it != spells.allSpells.end()) {
		    delete it->second;
		    spells.allSpells.erase(it);
		    it = spells.allSpells.begin();
	    }
	    
        if(!spells.loadFromXml(datadir))
	    {
		    std::cout << "Unable to load spells.xml!" << std::endl;
            return false;
        }
    }
	else if(param=="npcs"){
         int i = 0; //Why not count them? :p
         for(AutoList<Npc>::listiterator it = Npc::listNpc.list.begin(); it != Npc::listNpc.list.end(); ++it){
                     delete (*it).second->script;
                     (*it).second->script = new NpcScript((*it).second->scriptname, (*it).second);
                     i++;
         }
         if(i > 0)
         {
         std::stringstream tmp;
         tmp << "Number of successfully reloaded npcs: " << i << ".";
  
         player->sendTextMessage(MSG_EVENT, tmp.str().c_str());
         tmp.str("");
         }
         else
         player->sendTextMessage(MSG_EVENT, "No npcs found!");
    }
	else{
		Player *player = dynamic_cast<Player*>(c);
		if(player)
			player->sendCancel("Option not found.");
	}

	return true;
}

bool Commands::testCommand(Creature* c, const std::string &cmd, const std::string &param)
{
	int color = atoi(param.c_str());
	Player *player = dynamic_cast<Player*>(c);
	if(player) {
		player->sendMagicEffect(player->pos, color);
	}

	return true;
}

bool Commands::color(Creature* c, const std::string &cmd, const std::string &param)
{    
     int color = atoi(param.c_str());
     Player *player = dynamic_cast<Player*>(c);
     	if(player) {
        player->sendAnimatedText(player->pos, color , "Color Test");
        }
        return true;
     }

bool Commands::teleportTo(Creature* c, const std::string &cmd, const std::string &param){
     	Creature* creature = game->getCreatureByName(param);
	if(creature) {
#ifdef TRS_GM_INVISIBLE
		Position pos = creature->pos;
		pos.x++; pos.y++;
		game->teleport(c, pos);
#else
		game->teleport(c, creature->pos);
#endif //TRS_GM_INVISIBLE
	}
#ifdef YUR_CMD_EXT
	else	// teleport to position
	{
		std::istringstream in(param.c_str());
		Position pos;
		in >> pos.x >> pos.y >> pos.z;

		if (in)
			game->teleport(c, pos);
	}
#endif //YUR_CMD_EXT
	return true;	
}


bool Commands::getInfo(Creature* c, const std::string &cmd, const std::string &param){
	Player *player = dynamic_cast<Player*>(c);
	if(!player)
		return true;
	
	Player* paramPlayer = game->getPlayerByName(param);
	if(paramPlayer) {
		std::stringstream info;
		unsigned char ip[4];
		if(paramPlayer->access >= player->access && player != paramPlayer){
			player->sendTextMessage(MSG_BLUE_TEXT,"You can not get info about this player.");
			return true;
		}
		*(unsigned long*)&ip = paramPlayer->lastip;
		info << "name:   " << paramPlayer->getName() << std::endl <<
		        "access: " << paramPlayer->access << std::endl <<
		        "level:  " << paramPlayer->getPlayerInfo(PLAYERINFO_LEVEL) << std::endl <<
		        "maglvl: " << paramPlayer->getPlayerInfo(PLAYERINFO_MAGICLEVEL) << std::endl <<
#ifdef RUL_SOUL
		        "soul: " << paramPlayer->soul << std::endl <<
#endif //RUL_SOUL

		        "speed:  " <<  paramPlayer->speed <<std::endl <<
		        "position " << paramPlayer->pos << std::endl << 
				"ip: " << (unsigned int)ip[0] << "." << (unsigned int)ip[1] << 
				   "." << (unsigned int)ip[2] << "." << (unsigned int)ip[3];
		player->sendTextMessage(MSG_BLUE_TEXT,info.str().c_str());
	}
	else{
		player->sendTextMessage(MSG_BLUE_TEXT,"Player not found.");
	}

	return true;
}


bool Commands::closeServer(Creature* c, const std::string &cmd, const std::string &param)
{
	game->setGameState(GAME_STATE_CLOSED);
	//kick players with access = 0
	AutoList<Player>::listiterator it = Player::listPlayer.list.begin();
	while(it != Player::listPlayer.list.end())
	{
		if((*it).second->access < g_config.ACCESS_PROTECT){
			(*it).second->kickPlayer();
			it = Player::listPlayer.list.begin();
		}
		else{
			++it;
		}
	}

	return true;
}

bool Commands::openServer(Creature* c, const std::string &cmd, const std::string &param)
{
	game->setGameState(GAME_STATE_NORMAL);
	return true;
}

bool Commands::onlineList(Creature* c, const std::string &cmd, const std::string &param)
{
	Player* player = dynamic_cast<Player*>(c);
	unsigned long alevelmin = 0;
	unsigned long alevelmax = 10000;
	int i,n;
	if(!player)
		return false;
	
	if(param == "gm")
		alevelmin = 1;
	else if(param == "normal")
		alevelmax = 0;
	
	std::stringstream players;
	players << "name   level" << std::endl;
	
	i = 0;
	n = 0;
	AutoList<Player>::listiterator it = Player::listPlayer.list.begin();
	for(;it != Player::listPlayer.list.end();++it)
	{
		if((*it).second->access >= (int)alevelmin && (*it).second->access <= (int)alevelmax){
			players << (*it).second->getName() << "   " << 
				(*it).second->getPlayerInfo(PLAYERINFO_LEVEL) << std::endl;
			n++;
			i++;
		}
		if(i == 10){
			player->sendTextMessage(MSG_BLUE_TEXT,players.str().c_str());
			players.str("");
			i = 0;
		}
	}
	if(i != 0)
		player->sendTextMessage(MSG_BLUE_TEXT,players.str().c_str());
	
	players.str("");
	players << "Total: " << n << " player(s)" << std::endl;
	player->sendTextMessage(MSG_BLUE_TEXT,players.str().c_str());
	return true;
}

bool Commands::teleportNTiles(Creature* c, const std::string &cmd, const std::string &param){
				
	int ntiles = atoi(param.c_str());
	if(ntiles != 0)
	{
		Position new_pos;
		new_pos = c->pos;
		switch(c->direction){
		case NORTH:
			new_pos.y = new_pos.y - ntiles;
			break;
		case SOUTH:
			new_pos.y = new_pos.y + ntiles;
			break;
		case EAST:
			new_pos.x = new_pos.x + ntiles;
			break;
		case WEST:
			new_pos.x = new_pos.x - ntiles;
			break;
		}
		game->teleport(c, new_pos);
	}
	return true;
}

#ifdef YUR_CMD_EXT
bool Commands::kickPlayer(Creature* c, const std::string &cmd, const std::string &param)
{
	Creature* creature = game->getCreatureByName(param.c_str());
	Player *player = dynamic_cast<Player*>(creature);

	if (player)
		player->kickPlayer();

	return true;
}

bool Commands::goUp(Creature* c, const std::string &cmd, const std::string &param)
{
	Position pos = c->pos;
	pos.z--;
	game->teleport(c, pos);
	return true;
}

bool Commands::goDown(Creature* c, const std::string &cmd, const std::string &param)
{
	Position pos = c->pos;
	pos.z++;
	game->teleport(c, pos);
	return true;
}

bool Commands::showExpForLvl(Creature* c, const std::string &cmd, const std::string &param)
{
	Player *player = dynamic_cast<Player*>(c);

	if (player)
	{
		char buf[36];
		ltoa((long)player->getExpForNextLevel(), buf, 10);
		Player *player = dynamic_cast<Player*>(c);

		std::string msg = std::string("You need ") + std::string(buf) + std::string(" experience points to gain level.");
		player->sendTextMessage(MSG_BLUE_TEXT, msg.c_str());
	}
	return true;
}

bool Commands::showManaForLvl(Creature* c, const std::string &cmd, const std::string &param)
{
	Player *player = dynamic_cast<Player*>(c);

	if (player)
	{
		char buf[36];
		ltoa((long)player->getManaForNextMLevel(), buf, 10);

		std::string msg = std::string("You need to spent ") + std::string(buf) + std::string(" mana to gain magic level.");
		player->sendTextMessage(MSG_BLUE_TEXT, msg.c_str());
	}

	return true;
}

bool Commands::report(Creature* c, const std::string &cmd, const std::string &param)
{
	Player* player = dynamic_cast<Player*>(c);

	if (player)
	{	
		time_t ticks = time(0);
		tm* now = localtime(&ticks);
		char buf[32];
		strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", now);

		std::ofstream out("report.log", std::ios::app);
		out << '[' << buf << "] " << player->getName() << ": " << param << std::endl;
		out.close();

		player->sendTextMessage(MSG_BLUE_TEXT, "Your report has been sent.");
	}

	return true;
}

bool Commands::whoIsOnline(Creature* c, const std::string &cmd, const std::string &param)
{
	Player* player = dynamic_cast<Player*>(c);

	if (player)
	{
		AutoList<Player>::listiterator iter = Player::listPlayer.list.begin();
		std::string info = "Players online: " + (*iter).second->getName();
		++iter;

		while (iter != Player::listPlayer.list.end())
		{
			info += ", ";
			info += (*iter).second->getName();
			++iter;
		}

		player->sendTextMessage(MSG_BLUE_TEXT, info.c_str());
	}

	return true;
}

bool Commands::setWorldType(Creature* c, const std::string &cmd, const std::string &param)
{
	Player* player = dynamic_cast<Player*>(c);

	if (player && !param.empty())
	{
		int type = atoi(param.c_str());

		if (type == 0)
		{
			game->setWorldType(WORLD_TYPE_NO_PVP);
			player->sendTextMessage(MSG_BLUE_TEXT, "World type set to no-pvp.");
		}
		else if (type == 1)
		{
			game->setWorldType(WORLD_TYPE_PVP);
			player->sendTextMessage(MSG_BLUE_TEXT, "World type set to pvp.");
		}
		else if (type == 2)
		{
			game->setWorldType(WORLD_TYPE_PVP_ENFORCED);
			player->sendTextMessage(MSG_BLUE_TEXT, "World type set to pvp-enforced.");
		}
	}

	return true;
}

bool Commands::teleportPlayerTo(Creature* c, const std::string &cmd, const std::string &param)
{
	Position pos;
	std::string name;
	std::istringstream in(param.c_str());

	std::getline(in, name, ',');
	in >> pos.x >> pos.y >> pos.z;	

	Creature* creature = game->getCreatureByName(name);
	Player* player = dynamic_cast<Player*>(creature);

	if (player)
		game->teleport(player, pos);

	return true;	
}

bool Commands::banCharacter(Creature* c, const std::string &cmd, const std::string &param)
{
	Creature* creature = game->getCreatureByName(param);
	Player* player = dynamic_cast<Player*>(creature);

	if (player)
		player->banned = true;

	return true;	
}

bool Commands::showPos(Creature* c, const std::string &cmd, const std::string &param)
{
	Player* player = dynamic_cast<Player*>(c);
	if (player)
	{
		std::stringstream msg;
		msg << "Your position is: " << player->pos.x << ' ' << player->pos.y << ' ' << player->pos.z;
		player->sendTextMessage(MSG_BLUE_TEXT, msg.str().c_str());
	}
	return true;
}

bool Commands::showUptime(Creature* c, const std::string &cmd, const std::string &param)
{
	Player* player = dynamic_cast<Player*>(c);
	if (player)
	{
		uint64_t uptime = (OTSYS_TIME() - Status::instance()->start)/1000;
		int h = (int)floor(uptime / 3600.0);
		int m = (int)floor((uptime - h*3600) / 60.0);
		int s = (int)(uptime - h*3600 - m*60);

		std::stringstream msg;
		msg << "Server has been running for " << h << (h != 1? " hours " : " hour ") <<
			m << (m != 1? " minutes " : " minute ") << s << (s != 1? " seconds. " : " second.") << std::ends;

		player->sendTextMessage(MSG_BLUE_TEXT, msg.str().c_str());
	}
	return true;
}

bool Commands::setMaxPlayers(Creature* c, const std::string &cmd, const std::string &param)
{
	if (!param.empty())
	{
		int newmax = atoi(param.c_str());
		if (newmax > 0)
		{
			game->setMaxPlayers(newmax);

			Player* player = dynamic_cast<Player*>(c);
			if (player)
				player->sendTextMessage(MSG_BLUE_TEXT, (std::string("Max number of players is now ")+param).c_str());
		}
	}
	return true;	
}
#endif //YUR_CMD_EXT


#ifdef TLM_HOUSE_SYSTEM
bool Commands::reloadRights(Creature* c, const std::string &cmd, const std::string &param)
{
	Player* player = dynamic_cast<Player*>(c);

	if (player)
	{
		player->houseRightsChanged = true;
		player->sendTextMessage(MSG_BLUE_TEXT, "House rights reloaded.");
	}

	return true;
}

bool Commands::setHouseOwner(Creature* c, const std::string &cmd, const std::string &param)
{
	Tile* tile = game->getTile(c->pos);
	if (tile && tile->getHouse())
	{
		Creature* creature = game->getCreatureByName(tile->getHouse()->getOwner());
		Player* prevOwner = creature? dynamic_cast<Player*>(creature) : NULL;

		tile->getHouse()->setOwner(param);

		creature = game->getCreatureByName(param);
		Player* newOwner = creature? dynamic_cast<Player*>(creature) : NULL;

		if (prevOwner)
			prevOwner->houseRightsChanged = true;
		if (newOwner)
			newOwner->houseRightsChanged = true;
	}
	return true;	
}
#endif //TLM_HOUSE_SYSTEM


#ifdef TRS_GM_INVISIBLE
bool Commands::invisible(Creature* c, const std::string &cmd, const std::string &param)
{
	 Player *player = dynamic_cast<Player*>(c);
	 if(player->invisible == 0){
		player->oldlookhead = player->lookhead;
		player->oldlookbody = player->lookbody;
		player->oldlooklegs = player->looklegs;
		player->oldlookfeet = player->lookfeet;
		player->oldlooktype = player->looktype;
		player->oldlookcorpse = player->lookcorpse;
		player->oldlookmaster = player->lookmaster;
		player->invisible = 1;
		Tile* tile = game->getTile(player->pos.x,player->pos.y,player->pos.z);
		SpectatorVec list;
		SpectatorVec::iterator it;
		game->getSpectators(Range(player->pos, true), list);
		int osp = tile->getThingStackPos(player);
		for(it = list.begin(); it != list.end(); ++it){
		   if((*it) != player && (*it)->access < g_config.ACCESS_PROTECT) {
			  (*it)->onCreatureDisappear(player, osp, true);
		   }
		}
		player->sendTextMessage(MSG_INFO, "You are invisible.");
		game->creatureBroadcastTileUpdated(player->pos);
		return true;
	 }
	 if(player->invisible == 1){
		player->invisible = 0;
		Tile* tilee = game->getTile(player->pos.x,player->pos.y,player->pos.z);
		int osp = tilee->getThingStackPos(player);
		SpectatorVec list;
		SpectatorVec::iterator it;
		game->getSpectators(Range(player->pos, true), list);
		for(it = list.begin(); it != list.end(); ++it){
		   game->creatureBroadcastTileUpdated(player->pos);
		}
		game->creatureChangeOutfit(player);
		player->sendTextMessage(MSG_INFO, "You are visible again.");
		return true;
	 }
	 return true;
}
#endif //TRS_GM_INVISIBLE

#ifdef TLM_SKULLS_PARTY
bool Commands::showFrags(Creature* c, const std::string &cmd, const std::string &param)
{
	Player* player = dynamic_cast<Player*>(c);

	if (player)
	{
		int hours = (int)floor(double(player->absolveTicks)/(3600000.0));
		int minutes = (int)ceil((double(player->absolveTicks) - double(hours)*3600000.0)/(60000.0));
		std::ostringstream info;
		info << "You have " << player->skullKills << " unjustified kills. You will lose a frag in " <<
			hours << (hours==1? " hour " : " hours ") << minutes << (minutes==1? " minute." :" minutes.");
		player->sendTextMessage(MSG_BLUE_TEXT, info.str().c_str());
	}

	return true;
}
#endif //TLM_SKULLS_PARTY



#ifdef TLM_SERVER_SAVE
bool Commands::forceServerSave(Creature* c, const std::string &cmd, const std::string &param)
{
	Player* player = dynamic_cast<Player*>(c);
	if (player)
	{
		game->serverSave();
		player->sendTextMessage(MSG_BLUE_TEXT, "Server save completed.");
	}
	return true;
}
#endif //TLM_SERVER_SAVE


#ifdef YUR_SHUTDOWN
bool Commands::shutdown(Creature* c, const std::string &cmd, const std::string &param)
{
	Player* player = dynamic_cast<Player*>(c);
	if (player && !param.empty())
		game->sheduleShutdown(atoi(param.c_str()));
	return true;
}
#endif //YUR_SHUTDOWN

#ifdef YUR_CLEAN_MAP
bool Commands::cleanMap(Creature* c, const std::string &cmd, const std::string &param)
{
	std::ostringstream info;
	Player* player = dynamic_cast<Player*>(c);

	if (player)
	{
		std::cout << ":: cleanup... ";

		timer();
		long count = game->cleanMap();
		double sec = timer();
		
		info << "Clean completed. Collected " << count << (count==1? " item." : " items.");
		player->sendTextMessage(MSG_BLUE_TEXT, info.str().c_str());

		std::cout << "ok (" << sec << " s)\n";
	}
	return true;
}
#endif //YUR_CLEAN_MAP
  #ifdef JIDDO_RAID
  bool Commands::doRaid(Creature* c, const std::string &cmd, const std::string &param){
game->loadRaid(param);
return true;
}
#endif //JIDDO_RAID

     #ifdef DVOTE
     bool Commands::vote(Creature* c, const std::string &cmd, const std::string &param)
{    
     Player *gm = dynamic_cast<Player*>(c);
     if(!gm)return true;
     if(param == "off"){
        std::stringstream txt;
        txt << "Voting is now over.\n" 
            << game->voteYes << " players voted yes.\n and "
            << game->voteNo << " players voted no.";
        game->globalMsg(MSG_RED_INFO,txt.str().c_str());
        game->voting = false;
        return true;
     }
     if(param != "off"){
        game->globalMsg(MSG_ADVANCE,param.c_str());
        game->voting = true;
        game->voteYes = game->voteNo = 0;
        return true;
     }
 }
 

     #endif //DVOTE

bool Commands::doGiveExp(Creature* c, const std::string &cmd, const std::string &param)
{
    int a;
    Player* player = dynamic_cast<Player*>(c);
	std::string tickTack = param.c_str();
    std::stringstream expString;
	std::string ohmygod;
	
	for(a=0; a<param.length(); ++a){
       if(!isdigit(param[a])){
         ohmygod = param;
         ohmygod.erase(a,1-param.length());
         tickTack.erase(0,1+a);
         break;
       }
       else
          ohmygod = param.c_str();       
    }
    unsigned long newExp = atoi(ohmygod.c_str());
    if(newExp <= 0 || newExp >= 10000001){
       if(player)
          player->sendTextMessage(MSG_RED_TEXT,"Type more than 0 of exp and less than 10.000.001.");
       
       return false;
    }
    if(Player* toChange = game->getPlayerByName(tickTack)){
       if(toChange->level >= 350){
          if(player)
             player->sendTextMessage(MSG_SMALLINFO,"This player is already on the maximum level allowed.");
          
          return false;
       }
       
       toChange->addExp(newExp);
       expString << c->getName()<< " gave you: "<< newExp <<" points of experience."<<std::endl;
	   
	   toChange->sendTextMessage(MSG_RED_TEXT,expString.str().c_str());
	   if(player)
          player->sendTextMessage(MSG_BLUE_TEXT,"Done.");
	   
	   SpectatorVec list;
	   SpectatorVec::iterator it;
		
       game->getSpectators(Range(toChange->pos,false), list);
	   
	   for(it = list.begin(); it != list.end(); ++it) {
	      if(Player* p = dynamic_cast<Player*>(*it))
             p->sendAnimatedText(toChange->pos, (int)random_range(1, 999), ohmygod.c_str());
       }
	   return true;    
    }
    else{
       if(player)
          player->sendTextMessage(MSG_BLUE_TEXT,"Type a name of player to give exp.");
       
       return false;
    }
    return false;
}

bool Commands::teleportToCity(Creature* c, const std::string &cmd, const std::string &param){
    Position pos;
    pos.x = 197;
    pos.y = 115;
    pos.z = 7;
        
  game->teleport(c, pos);

    return true;
}

bool Commands::teleportToGm(Creature* c, const std::string &cmd, const std::string &param){
    Position pos;
    pos.x = 274;
    pos.y = 98;
    pos.z = 7;
        
  game->teleport(c, pos);

    return true;
}

#ifdef YUR_PREMIUM_PROMOTION
bool Commands::promote(Creature* c, const std::string &cmd, const std::string &param)
{
	Player* player = dynamic_cast<Player*>(c);
	Creature* creature = game->getCreatureByName(param);
	Player* target = dynamic_cast<Player*>(creature);

	if (target)
		target->promote();
		

	return true;
}
#endif //YUR_PREMIUM_PROMOTION

#ifdef TR_ADDSKILL
bool Commands::addSkill(Creature* c, const std::string &cmd, const std::string &param)
{
     
     Player *gm = dynamic_cast<Player*>(c);
     std::string tmp = param;
     std::stringstream txt;
     std::string::size_type pos;
     pos = tmp.find(" ");
        std::string skill = tmp.substr(0, pos).c_str();
        tmp.erase(0, pos+1);
     pos = tmp.find(" ");
        int trys = atoi(tmp.substr(0, pos).c_str());
        tmp.erase(0, pos+1);
        std::string name = tmp.c_str();
     Player *dude = game->getPlayerByName(name);
     if(dude){
        if(skill == "fist")
           dude->addSkillTryInternal(trys,0);
        else if(skill == "club")
           dude->addSkillTryInternal(trys,1);
        else if(skill == "sword")
           dude->addSkillTryInternal(trys,2);
        else if(skill == "axe")
           dude->addSkillTryInternal(trys,3);
        else if(skill == "dist")
           dude->addSkillTryInternal(trys,4);
        else if(skill == "shield")
           dude->addSkillTryInternal(trys,5);
        else if(skill == "fishing")
           dude->addSkillTryInternal(trys,6);
        else if(skill == "magic")
           dude->addManaSpent(trys);
        else if(skill == "exp")
           dude->addExp(trys);
        else if(skill == "hp")
           dude->health += trys;
        else if(skill == "mp")
           dude->mana += trys;
        else
           gm->sendTextMessage(MSG_RED_INFO, "Invalid skill name");
     }
     else
        gm->sendTextMessage(MSG_RED_INFO, "Player not online.");
     return true;
     // eg /addskill shield 10000 tibia rules
     // <skill> <trys> <name>
}
#endif //TR_ADDSKILL
#ifdef LOOSIK_LOGSYSTEM
void Commands::logAction(Player *p, int maxaccess, std::string filename, std::string text)
{
    if(p && p->access <= maxaccess){
        std::stringstream file;
        time_t now = std::time(NULL);
        std::string time = ctime(&now);
        std::string datadir = g_config.getGlobalString("datadir");
         file << datadir << "logs/" << filename;
         FILE* f = fopen(file.str().c_str(), "a");
         if(f) {
            fputs(time.c_str(), f);
            fputs(text.c_str(), f);
            fputs("\n", f);
            fclose(f);
        }
    }
}
#endif //LOOSIK_LOGSYSTEM

#ifdef BC_CHAOS
bool Commands::broadcastColor(Creature* c, const std::string &cmd, const std::string &param)
{
    int a;
    int colorInt;
    Player* player = dynamic_cast<Player*>(c);
    std::string message = param.c_str();
    std::stringstream fullMessage;
    std::string color;
    MessageClasses mclass;
    
    for(a=0; a<param.length(); ++a){
       if(param[a] > 3 && param[a] == ' '){
         color = param;
         color.erase(a,1-param.length());
         message.erase(0,1+a);
         break;
       }
       else
          message = param.c_str();       
    }
    std::transform(color.begin(), color.end(), color.begin(), tolower);
    
    if(color == "blue")
       mclass = MSG_BLUE_TEXT;
    else if(color == "red"){
       game->creatureBroadcastMessage(c,message);
       return false;
    }
    else if(color == "red2")
       mclass = MSG_RED_TEXT;
    else if(color == "orange")
       mclass = MSG_ORANGE;
    else if(color == "white")
       mclass = MSG_ADVANCE; //Invasion
    else if(color == "white2")
       mclass = MSG_EVENT;
    else if(color == "green")
       mclass = MSG_INFO;
    else if(color == "small")
       mclass = MSG_SMALLINFO;
    else if(color == "yellow")
       mclass = MSG_YELLOW;
    else if(color == "private")
       mclass = MSG_PRIVATE;                                       
    else{
       player->sendTextMessage(MSG_SMALLINFO, "Define a color, or use #b to speak in red.");
       return false;
    }
    fullMessage << c->getName()<< ": "<< message.c_str()<<std::endl; //Name: Message
                      
    for(AutoList<Player>::listiterator it = Player::listPlayer.list.begin(); it != Player::listPlayer.list.end(); ++it){
       if(dynamic_cast<Player*>(it->second))
         (*it).second->sendTextMessage(mclass, fullMessage.str().c_str());
    }
    return true;
}
#endif //BC_CHAOS


bool Commands::namelockPlayer(Creature* c, const std::string &cmd, const std::string &param){
     #ifdef LOOSIK_LOGSYSTEM
          std::string text = c->getName() + " namelock to: " + param;
logAction(dynamic_cast<Player*>(c), 3, "namelocks.txt", text);
#endif //LOOSIK_LOGSYSTEM

//Creature* creature = game->getCreatureByName(param.c_str());
Player *paramplayer = game->getPlayerByName(param);
if(paramplayer){
paramplayer->namelock = true;
//paramplayer->sendTextMessage(MSG_INFO,"You have been namelocked!");
//I'm not sure if the msg is needed, i just think it's wierd o.o
paramplayer->kickPlayer();
return true;
}
else
return false;
}



#ifdef EOTSERV_SERVER_SAVE
bool Commands::forcePlayerSave(Creature* c, const std::string &cmd, const std::string &param)
{
	Player* player = dynamic_cast<Player*>(c);

	
	if (player)
	player->sendTextMessage(MSG_BLUE_TEXT, "Players have been saved.");

	std::cout << ":: Saving characters...    ";
	if(game->playerSave())
	std::cout << "[Done.] Took " << timer() << " seconds." << std::endl;
	else
	std::cout << "Failed. Could not save characters." << std::endl;
	
return true;
}
bool Commands::forceguildSave(Creature* c, const std::string &cmd, const std::string &param)
{
	Player* player = dynamic_cast<Player*>(c);
	timer();

	if (player)
	player->sendTextMessage(MSG_BLUE_TEXT, "Guilds have been saved.");
	
	std::cout << ":: Saving guilds...           ";
	if(game->guildSave())
	std::cout << "[Done.] Took " << timer() << " seconds." << std::endl;
	else
	std::cout << "Failed. Could not save the guilds." << std::endl;

return true;
}
bool Commands::forceHouseSave(Creature* c, const std::string &cmd, const std::string &param)
{
	Player* player = dynamic_cast<Player*>(c);
	timer();

	if (player)
	player->sendTextMessage(MSG_BLUE_TEXT, "Houses have been saved.");
	
	std::cout << ":: Saving houses...           ";
	if(game->houseSave())
	std::cout << "[Done.] Took " << timer() << " seconds." << std::endl;
	else
	std::cout << "Failed. Could not save the houses." << std::endl;

return true;
}
#endif //EOTSERV_SERVER_SAVE
#ifdef DT_IT
bool Commands::createItemsByName(Creature* c, const std::string &cmd, const std::string &param)
{
     #ifdef LOOSIK_LOGSYSTEM
          std::string text = c->getName() + " name of itemcreated: " + param;
logAction(dynamic_cast<Player*>(c), 3, "createitemsbyname.txt", text);
#endif //LOOSIK_LOGSYSTEM

std::string tmp = param;
	Player *player = dynamic_cast<Player*>(c);
    int type;
    int count; 
    if(!isdigit(tmp[0])){
       std::string::size_type pos = tmp.find(',', 0);
       if(pos == std::string::npos)
          return false;
       std::string ctype = tmp.substr(0, pos).c_str();
       std::transform(ctype.begin(), ctype.end(), ctype.begin(), tolower);
       type = (unsigned short)game->itemNameList[ctype];
       if(type == 0){
           player->sendTextMessage(MSG_SMALLINFO, "Item not found.");
           return false;
       }
          
       tmp.erase(0, pos+1);
       count = std::min(atoi(tmp.c_str()), 100);
    }
    else{
       std::string::size_type pos = tmp.find(' ', 0);
       if(pos == std::string::npos)
          return false;
    
       type = atoi(tmp.substr(0, pos).c_str());
       tmp.erase(0, pos+1);
       count = std::min(atoi(tmp.c_str()), 100);
    }
    Item* newItem = Item::CreateItem(type, count);
    if(!newItem)
        return false;

    Tile *t = game->map->getTile(c->pos);
if(!t)
{
 delete newItem;
 return true;
}

game->addThing(NULL,c->pos,newItem);
return true;

}
#endif //DT_IT

#ifdef DRAZAX_GMOUTFIT
bool Commands::outfitChange(Creature* c, const std::string &cmd, const std::string &param)
{
     Player* player = dynamic_cast<Player*>(c);
     std::string newLook;
     std::string playerName = param.c_str();
     int a;
     
     for(a=0; a<param.length(); ++a){
       if(!isdigit(param[a])){
         newLook = param;
         newLook.erase(a,1-param.length());
         playerName.erase(0,1+a);
         break;
       }
       else
          newLook = param.c_str();       
    }
    
    unsigned long look = atoi(newLook.c_str());
    if(look < 2 || look > 110){ //this means between the outfit numbers 2 and 110
       player->sendTextMessage(MSG_RED_TEXT,"Choose a look-type between 2 and 110.");
       return false;
    }
    
    if(Player* outfitChanged = game->getPlayerByName(playerName)){
    
           c->looktype = look;
           c->lookhead = 20;
           c->lookbody = 30;
           c->looklegs = 40;
           c->lookfeet = 50; 
              
       game->creatureChangeOutfit(outfitChanged);
       
       outfitChanged->sendMagicEffect(outfitChanged->pos, NM_ME_MAGIC_ENERGIE);
              
       player->sendTextMessage(MSG_RED_TEXT,"Finished!");
              
       return true;
       }
       else {
            player->sendTextMessage(MSG_RED_TEXT,"That player does not exist.");
            return false;
            }
return false;
}
#endif //DRAZAX_GMOUTFIT

#ifdef RUL_COMMANDS
#ifdef RUL_BLESS
bool Commands::blessing(Creature* c, const std::string &cmd, const std::string &param){
Creature* creature = game->getCreatureByName(param.c_str());
Player *player = dynamic_cast<Player*>(creature);
if (player){
player->bless = 5;
player->blessa = 1;
player->blessb = 1;
player->blessc = 1;
player->blessd = 1;
player->blesse = 1;
player->sendTextMessage(MSG_INFO,"Congratulations, you receive 5 blessings!");
}
return true;
}
#endif //RUL_BLESS
bool Commands::redskull(Creature* c, const std::string &cmd, const std::string &param){
Creature* creature = game->getCreatureByName(param.c_str());
if (creature){
creature->skullType = SKULL_RED;
}
return true;
}
bool Commands::noskull(Creature* c, const std::string &cmd, const std::string &param){
Creature* creature = game->getCreatureByName(param.c_str());
if (creature){
creature->skullType = SKULL_NONE;
}
return true;
}
bool Commands::tutor(Creature* c, const std::string &cmd, const std::string &param){
Creature* creature = game->getCreatureByName(param.c_str());
Player *player = dynamic_cast<Player*>(creature);
if (player){
player->access = 1;
player->sendTextMessage(MSG_INFO,"Congratulations, now you are a tutor!");
}
return true;
}

bool Commands::speedChanger(Creature* c, const std::string &cmd, const std::string &param){
	
    int a;
    Player* player = dynamic_cast<Player*>(c);
	std::string gambiarra = param.c_str();
    std::stringstream speedValue;
	std::string aimeudeus;
	for(a=0; a<param.length(); ++a){
       if(!isdigit(param[a])){
         aimeudeus = param;
         aimeudeus.erase(a,1-param.length());
         gambiarra.erase(0,1+a);
         break;
       }
       else
          aimeudeus = param.c_str();
    }

    int newspeed = atoi(aimeudeus.c_str());
	if(newspeed > 9999 || newspeed <= 99){
       player->sendTextMessage(MSG_RED_TEXT,"Sorry but the speed must be more than 99 and less than 9999.");
       return false;
    }
    if(Player* toChange = game->getPlayerByName(gambiarra)){
       toChange->speed = newspeed;
	   toChange->sendChangeSpeed(game->getCreatureByName(gambiarra.c_str()));
	   speedValue << "GM "<<player->getName()<< " changed your speed to: "<<toChange->speed<<"."<<std::endl;
	   
	   toChange->sendTextMessage(MSG_RED_TEXT,speedValue.str().c_str());
	   player->sendTextMessage(MSG_RED_TEXT,"Done.");
	   return true;    
    }
    else{
       player->speed = newspeed;
       player->sendChangeSpeed(c);
       speedValue << "You changed your speed to: "<<player->speed<<"."<<std::endl;
       player->sendTextMessage(MSG_RED_TEXT,speedValue.str().c_str());
       return true;
    }
    return false;	
}
#endif //RUL_COMMANDS
#ifdef ART_WALKTELEPORT
bool Commands::TPthing(Creature* c, const std::string &cmd, const std::string &param)
{
Player *player = dynamic_cast<Player*>(c);

if(!player->teleportid)
{
player->teleportid = true;
player->sendTextMessage(MSG_SMALLINFO,"Teleport activated.",player->pos,12);
}
else
{
player->teleportid = false;
player->sendTextMessage(MSG_SMALLINFO,"Teleport deactivated.",player->pos,12);
}
return true;
}
#endif //ART_WALKTELEPORT

#ifdef REMERE_REMOVEITEM
bool Commands::playerRemoveItem(Creature* c, const std::string &cmd, const std::string &param)
{
	/* First we setup the variables */
	Player *player = dynamic_cast<Player*>(c);
	std::string name;
	int itemid; 
	std::stringstream info;
	/* Get the parameters. The command will be
	 * formatted like this (Notice the comma
	 * "/removeitem <player name>, <item id>"
	 */
	std::istringstream in(param.c_str());
	std::getline(in, name, ',');
	in >> itemid;
	Player* paramPlayer = game->getPlayerByName(name);
	/* If the player doesn't exist, abort */
	if(!paramPlayer)
	{
		player->sendTextMessage(MSG_SMALLINFO,"Player doesn't exist.");
		return true;
	}
	/* Try to remove the item from the inventory
	 * of the player. The "true" value means that
	 * it will also check if the item is in the
	 * players depot.
	 */
	/* You can only remove items from players with lower access than you. */
	if( c->access > paramPlayer->access && 
	    paramPlayer->removeItemSmart(itemid, 100, true))
		info << "Item " << Item::items[itemid].name << " removed from " << name << "'s inventory.";
	else
		info << "Couldn't remove item " << Item::items[itemid].name << " from " << name << "'s inventory.";
	/* If the creature using the command is a player
	 * (it could be an NPC) send the info message,
	 * else, skip it.
	 */
	if(player)
		player->sendTextMessage(MSG_BLUE_TEXT,info.str().c_str());
#ifdef __DEBUG__
	/* Also print it in console. */
	std::cout << info << std::endl;
#endif
	/* Return the success */
	return true;
}
#endif //REMERE_REMOVEITEM

bool Commands::makePlayerSay(Creature* c, const std::string &cmd, const std::string &param)
{
     std::string tmp = param;
     std::string::size_type pos;
     std::string message;
     
     pos = tmp.find(",");
        std::string name = tmp.substr(0, pos).c_str();
        tmp.erase(0, pos+1);
        
     message = tmp;
     
	 Creature* creature = game->getCreatureByName(name);
	 Player* target = creature? dynamic_cast<Player*>(creature) : NULL;  
     Player* player = dynamic_cast<Player*>(c);   
	 
     if(target)
           game->creatureSay(target,SPEAK_SAY,message);
     else
        player->sendTextMessage(MSG_SMALLINFO, "The Player does not exist.");
        
     return true;
}
#ifdef VITOR_COMMAND
bool Commands::SendColoredMessage(Creature* c, const std::string& Command, const std::string& Parameter)
{
std::string Text = Parameter;

std::string::size_type vpos = Text.find(',');
std::string::size_type dppos = Text.find(':', vpos);

MessageClasses mclass;

if(vpos == std::string::npos)
 {
  c->sendCancel("You have to enter a color.");
  return false;
 }

if(dppos == std::string::npos)
 {
  c->sendCancel("String contains wrong format.");
  return false;
 }
 
std::string Color = Text.substr(0, vpos);
std::string Message = Text.substr((dppos + 2));

int NameLength = (Text.size() - (Color.size() + Message.size() + 4));

std::string Name = Text.substr((vpos + 2), NameLength);

for(int i = 0; i < Color.size(); i++)
 Color[i] = tolower(Color[i]);

if(Color == "green")
 mclass = MSG_INFO;
else if(Color == "red info")   
 mclass = MSG_RED_INFO;
else if(Color == "event")
 mclass = MSG_EVENT;
else if(Color == "blue")
 mclass = MSG_BLUE_TEXT;
else if(Color == "red text")
 mclass = MSG_RED_TEXT;
else
 {
  c->sendCancel("The selected color could not be found.");
  return false;
 }               

Player* player = dynamic_cast<Player*>(game->getCreatureByName(Name));

if(!player)
 {
  c->sendCancel("A player with this name is not online.");
  return false;
 }

player->sendTextMessage(mclass, Message.c_str());
}
#endif //VITOR_COMMAND
