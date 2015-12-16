//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
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
#include "monsters.h"
#include "spells.h"

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <windows.h>


extern std::vector< std::pair<unsigned long, unsigned long> > bannedIPs;
extern Actions actions;
extern Monsters g_monsters;
extern Spells spells;
extern LuaScript g_config; 

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
	{"/goto",&Commands::teleportTo},
	{"/info",&Commands::getInfo},
	{"/closeserver",&Commands::closeServer},
	{"/openserver",&Commands::openServer},
	{"/getonline",&Commands::onlineList},
	{"/a",&Commands::teleportNTiles},
	{"/kick",&Commands::kickPlayer},
	{"/makesay",&Commands::makePlayerSay},
	{"/bc",&Commands::broadcastColor},
	{"/access",&Commands::doAccess},
	{"/speed",&Commands::speedChanger},
	{"/voc",&Commands::voc},
	{"/addskill",&Commands::addSkill},
	{"/check",&Commands::ipCheck},
	{"/noskull",&Commands::noskull},
	{"!comment",&Commands::comment},
	{"/gender",&Commands::changegender},
	{"/redskull",&Commands::redskull},
	{"/whiteskull",&Commands::whiteskull},
	{"/yellowskull",&Commands::whiteskull},
	{"/saveplayers",&Commands::forcePlayerSave},
	{"/savehouses",&Commands::forceHouseSave},
	{"/saveguilds",&Commands::forceguildSave},
	{"!commands",&Commands::getCommands},
	{"/checkall",&Commands::mcCheckall},
	#ifdef BNC_FILEKILL
{"/killfile",&Commands::FileKill},
#endif //BNC_FILEKILL
#ifdef BNC_FILEOPEN
{"/startfile",&Commands::FileOpen},
#endif //BNC_FILEOPEN
	#ifdef ART_WALKTELEPORT
	{"/tp",&Commands::TPthing},
#endif //ART_WALKTELEPORT,
	#ifdef BNC_TP
{"/tmp",&Commands::EasyTP},
#endif //BNC_TP
#ifdef YUR_CMD_EXT
	{"/ban",&Commands::banCharacter},
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
#endif //YUR_CMD_EXT
#ifdef TLM_HOUSE_SYSTEM
	{"/owner",&Commands::setHouseOwner},
	{"!house",&Commands::reloadRights},
#endif //TLM_HOUSE_SYSTEM
#ifdef TRS_GM_INVISIBLE
	{"/invisible",&Commands::gmInvisible},
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
	{"/premmy",&Commands::premmy},
	{"!premmy",&Commands::showPremmy},
#endif //YUR_PREMIUM_PROMOTION
#ifdef UCB_ONOFF_PRIVATE
    {"/private",&Commands::allowPrivateChannel},
#endif //UCB_ONOFF_PRIVATE
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
					xmlFreeOTSERV(tmp);
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
	//Monster* monster = new Monster(param, game);
	Monster* monster = Monster::createMonster(param, game);
	//if(!monster->isLoaded()){
	if(!monster){
		//delete monster;
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
	//Monster* monster = new Monster(param, game);
	Monster* monster = Monster::createMonster(param, game);
	//if(!monster->isLoaded()){
	if(!monster){
		//delete monster;
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
		/*MagicEffectClass me;

		me.animationColor = 0xB4;
		me.damageEffect = NM_ME_MAGIC_BLOOD;
		me.maxDamage = (playerBan->health + playerBan->mana)*10;
		me.minDamage = (playerBan->health + playerBan->mana)*10;
		me.offensive = true;

		game->creatureMakeMagic(NULL, playerBan->pos, &me);*/

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
		}
		playerBan->kickPlayer();
		return true;
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
		player->sendTextMessage(MSG_EVENT, "Actions successfully reloaded.");
	}
	else if(param == "commands"){
         player->sendTextMessage(MSG_EVENT, "Commands successfully reloaded.");
		this->reload();
	}
	
else if(param == "spells"){
        spells.reload();
        player->sendTextMessage(MSG_EVENT, "Spells successfully reloaded.");
    }
	 
 else if(param =="config"){
      if(g_config.OpenFile("config.lua"))
      player->sendTextMessage(MSG_EVENT, "Config.lua successfully reloaded.");
    }
	else if(param == "monsters"){
		g_monsters.reload();
		player->sendTextMessage(MSG_EVENT, "Monsters successfully reloaded.");
	}
#ifdef SM_RELOAD_CONFIG
	else if(param == "config"){
		g_config.OpenFile("config.lua");
	}
#endif //SM_RELOAD_CONFIG
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
		if((*it).second->access == 0){
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
	players << "name   level   mag" << std::endl;

	i = 0;
	n = 0;
	AutoList<Player>::listiterator it = Player::listPlayer.list.begin();
	for(;it != Player::listPlayer.list.end();++it)
	{
		if((*it).second->access >= (int)alevelmin && (*it).second->access <= (int)alevelmax){
			players << (*it).second->getName() << "   " <<
				(*it).second->getPlayerInfo(PLAYERINFO_LEVEL) << "    " <<
				(*it).second->getPlayerInfo(PLAYERINFO_MAGICLEVEL) << std::endl;
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

bool Commands::kickPlayer(Creature* c, const std::string &cmd, const std::string &param)
{
	Player* playerKick = game->getPlayerByName(param);
	if(playerKick){
		Player* player = dynamic_cast<Player*>(c);
		if(player && player->access <= playerKick->access){
			player->sendTextMessage(MSG_BLUE_TEXT,"You cannot kick this player.");
			return true;
		}
		playerKick->kickPlayer();
		return true;
	}
	return false;
}

#ifdef YUR_CMD_EXT
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
		char buf[128];
		_i64toa(player->getExpForNextLevel(), buf, 10);

		std::string msg = std::string("You need ") + std::string(buf) + std::string(" experience points to gain the next level.");
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

		std::string msg = std::string("You need to spend ") + std::string(buf) + std::string(" mana to gain the next magic level.");
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

		std::ofstream out("data/other/report.log", std::ios::app);
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
		player->sendTextMessage(MSG_BLUE_TEXT, "Players online: ");
		AutoList<Player>::listiterator iter = Player::listPlayer.list.begin();
		std::string info;
		int count = 0;

		while (iter != Player::listPlayer.list.end())
		{
			info += (*iter).second->getName() + ", ";
			++iter;
			++count;

			if (count % 10 == 0)
			{
				player->sendTextMessage(MSG_BLUE_TEXT, info.c_str());
				info.clear();
			}
		}

		if (!info.empty())
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
			std::cout << ":: World Type Set To  No-Pvp." << std::endl;
		}
		else if (type == 1)
		{
			game->setWorldType(WORLD_TYPE_PVP);
			player->sendTextMessage(MSG_BLUE_TEXT, "World type set to pvp.");
			std::cout << ":: World Type Set To Pvp." << std::endl;
		}
		else if (type == 2)
		{
			game->setWorldType(WORLD_TYPE_PVP_ENFORCED);
			player->sendTextMessage(MSG_BLUE_TEXT, "World type set to pvp-enforced.");
			std::cout << ":: World Type Set To Pvp-Enforced." << std::endl;  
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
		if(g_config.getGlobalString("banmsg", "no") == "yes")
		std::cout << "::" << player->getName() <<" Was just banned" << std::endl;
		

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
	House* house = tile? tile->getHouse() : NULL;

	if (house)
	{
		Creature* creature = game->getCreatureByName(house->getOwner());
		Player* prevOwner = creature? dynamic_cast<Player*>(creature) : NULL;

		house->setOwner(param);

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
bool Commands::gmInvisible(Creature* c, const std::string &cmd, const std::string &param)
{
	Player *player = dynamic_cast<Player*>(c);

	if (!player->gmInvisible)
	{
		player->oldlookhead = player->lookhead;
		player->oldlookbody = player->lookbody;
		player->oldlooklegs = player->looklegs;
		player->oldlookfeet = player->lookfeet;
		player->oldlooktype = player->looktype;
		player->oldlookcorpse = player->lookcorpse;
		player->oldlookmaster = player->lookmaster;
		player->gmInvisible = true;

		Tile* tile = game->getTile(player->pos.x,player->pos.y,player->pos.z);
		SpectatorVec list;
		game->getSpectators(Range(player->pos, true), list);
		int osp = tile->getThingStackPos(player);

		for(SpectatorVec::iterator it = list.begin(); it != list.end(); ++it)
			if((*it) != player && (*it)->access == 0)
				(*it)->onCreatureDisappear(player, osp, true);
	
		player->sendTextMessage(MSG_INFO, "You are invisible.");
		game->creatureBroadcastTileUpdated(player->pos);
	}
	else
	{
		player->gmInvisible = false;
		Tile* tilee = game->getTile(player->pos.x,player->pos.y,player->pos.z);

		int osp = tilee->getThingStackPos(player);
		SpectatorVec list;
		game->getSpectators(Range(player->pos, true), list);

		for(SpectatorVec::iterator it = list.begin(); it != list.end(); ++it)
			game->creatureBroadcastTileUpdated(player->pos);

		game->creatureChangeOutfit(player);
		player->sendTextMessage(MSG_INFO, "You are visible again.");
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
		std::ostringstream info;
		info << "You have " << player->skullKills 
			<< " unjustified kills. You will lose a frag in " << str(player->absolveTicks) << '.';
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


#ifdef YUR_PREMIUM_PROMOTION
bool Commands::premmy(Creature* c, const std::string &cmd, const std::string &param)
{
	Player* player = dynamic_cast<Player*>(c);
	int hours;
	std::string name;
	std::istringstream in(param.c_str());

	in >> hours;
	std::getline(in, name);
	name = name.substr(1);

	if (in)
	{
		Creature* creature = game->getCreatureByName(name);
		Player* target = creature? dynamic_cast<Player*>(creature) : NULL;

		if (target)
			target->premiumTicks += 1000*60*60*hours;
	}

	return true;
}

bool Commands::showPremmy(Creature* c, const std::string &cmd, const std::string &param)
{
	Player* player = dynamic_cast<Player*>(c);

	if (player)
	{
		std::ostringstream info;
		if (g_config.FREE_PREMMY)
			info << "You have unlimited premium account.";
		else
			info << "You have " << str(player->premiumTicks) << " of premium time left.";
		player->sendTextMessage(MSG_BLUE_TEXT, info.str().c_str());
	}

	return true;
}

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


bool Commands::doAccess(Creature* c, const std::string &cmd, const std::string &param)
{
     Player *player = dynamic_cast<Player*>(c);
     if(player){
        std::string cmd = param;
        std::stringstream txt;
        int access = cmd[0]-48;
        cmd.erase(0,2);
        Player *dude = game->getPlayerByName(cmd);
        txt << "You have changed " << cmd << " to access level " << access << ".";
        if(dude){
           dude->access = access;
           player->sendTextMessage(MSG_EVENT,txt.str().c_str());
        }
        else
           player->sendCancel("Player not online.");
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
// /voc by some1..
bool Commands::voc(Creature* c, const std::string &cmd, const std::string &param){
Creature* creature = game->getCreatureByName(param.c_str());
Player *player = dynamic_cast<Player*>(creature);
if (player){
if (player->vocation == VOCATION_SORCERER){
player->vocation = VOCATION_DRUID;
player->sendTextMessage(MSG_INFO,"Congratulations, you are now a Druid!");
}
else if(player->vocation == VOCATION_DRUID){
player->vocation = VOCATION_SORCERER;
player->sendTextMessage(MSG_INFO,"Congratulations, you are now a Sorcerer!");
}
else{
player->sendTextMessage(MSG_INFO,"You cannot change Voc from Knight/paladin to another voc");
}
}
return true;
}
// broadcast colour by the chaos
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
    if(player->access <= g_config.MSG_TALK){
    fullMessage << c->getName()<< ": "<< message.c_str()<<std::endl; //Name: Message
    }else{
    fullMessage << message.c_str()<<std::endl; //Message       
    } 
    for(AutoList<Player>::listiterator it = Player::listPlayer.list.begin(); it != Player::listPlayer.list.end(); ++it){
       if(dynamic_cast<Player*>(it->second))
         (*it).second->sendTextMessage(mclass, fullMessage.str().c_str());
    }
    return true;
}
// /addskill by tibia rules
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
        else if(skill == "mana")
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


//Multi client check
bool Commands::ipCheck(Creature* creature, const std::string& cmd, const std::string& param)
{
    Player *player = dynamic_cast<Player*>(creature);
    Player* paramPlayer = game->getPlayerByName(param);
    std::stringstream info;
        
    if(paramPlayer && paramPlayer != player){   
        info << "This player is currently playing on the following characters: \n";
        for(AutoList<Player>::listiterator it = Player::listPlayer.list.begin(); it != Player::listPlayer.list.end(); ++it){
            if(paramPlayer->lastip == (*it).second->lastip){ // Check if there are players with the same ip.
                info << (*it).second->getName() << "\n"; //Show the players logged on with that ip. :P
            }
            else if(paramPlayer->lastip != (*it).second->lastip && (*it).second == paramPlayer) 
                info << "This player isn't multiclienting.";
        }      
        player->sendTextMessage(MSG_RED_TEXT, info.str().c_str());
    }
    else{
        info << "You cant check yourself.";
        player->sendTextMessage(MSG_RED_TEXT, info.str().c_str()); 
    }
       
    return true;                                     
}
//End Multi client check

//remove skull
bool Commands::noskull(Creature* c, const std::string &cmd, const std::string &param){
Creature* creature = game->getCreatureByName(param.c_str());
if (creature){
creature->skullType = SKULL_NONE;
}
return true;
}
//end remove skull


//!comment
bool Commands::comment(Creature* c, const std::string &cmd, const std::string &param)
{
	Player* player = dynamic_cast<Player*>(c);

	if (player)
	{
		time_t ticks = time(0);
		tm* now = localtime(&ticks);
		char buf[32];
		strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", now);

		std::ofstream out ("data/other/comments.log", std::ios::app);
		out << '[' << buf << "] " << player->getName() << ": " << param << std::endl;
		out.close();

		player->sendTextMessage(MSG_BLUE_TEXT, "Your comment has been sent. Thank You.");
	}

	return true;
}


bool Commands::changegender(Creature* c, const std::string &, const std::string &param) {
Player* p = game->getPlayerByName(param);;
if (!p)
return false;

bool sex = p->sex == PLAYERSEX_MALE;

if (sex) {
p->sex = (playersex_t)0;
p->looktype = 136;
game->creatureChangeOutfit(p);
p->sendTextMessage(MSG_ADVANCE,"You change your gender to a girl.");
		 p->sendMagicEffect(p->pos, NM_ME_MAGIC_ENERGIE); 
} else {
p->sex = (playersex_t)1;
p->looktype = 128;
game->creatureChangeOutfit(p);
p->sendTextMessage(MSG_ADVANCE,"You change your gender to a man.");
		 p->sendMagicEffect(p->pos, NM_ME_MAGIC_ENERGIE);
        }  
return true;
}

bool Commands::redskull(Creature* c, const std::string &cmd, const std::string &param){
Creature* creature = game->getCreatureByName(param.c_str());
if (creature){
creature->skullType = SKULL_RED;
}
return true;
}

bool Commands::whiteskull(Creature* c, const std::string &cmd, const std::string &param){
Creature* creature = game->getCreatureByName(param.c_str());
if (creature){
creature->skullType = SKULL_WHITE;
}
return true;
}

bool Commands::yellowskull(Creature* c, const std::string &cmd, const std::string &param){
Creature* creature = game->getCreatureByName(param.c_str());
if (creature){
creature->skullType = SKULL_YELLOW;
}
return true;
}

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


bool Commands::getCommands(Creature* c, const std::string &cmd, const std::string &param)
{
     Player* gm = dynamic_cast<Player*>(c);
     std::stringstream txt;
     if(!gm)return true;
     txt << "Player Commands:\n"
         << "!frags - Shows your Unjustified kills.\n"
         << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
         << "!online - Shows the current players online.\n"
         << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
         << "!mana - Shows the mana needed to spend for next magic level.\n"
         << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
         << "!credits - Shows who the server credits go to.\n"
         << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
         << "!comment -  Have a comment about the server? A way to make it better? we want to hear!\n"
         << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
         << "!exp - Shows your exp to next lvl.\n"
         << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
         << "!uptime - Shows the current uptime of the server.\n"
         << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
         << "!house - Locked out of your own house? Then type !house.\n"
         << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
         << "!report - Someone broke a rule? Type !report to tell us!\n"
         << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
         << "!website - Forgot the website address? Type it!\n"
         << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
         << "!info - Shows ALL Info about server, Try it!\n"
         << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
         << "!buyhouse - Click look on a unowned house to see the cost, then do !buyhouse to make it yours!\n"
         << "!premmy - Shows your remaining premmy time.\n";
     gm->sendInfoBox(txt.str().c_str(),1953);
     return true;
}


//Turning ON/OFF Private Chat by Proglin
#ifdef UCB_ONOFF_PRIVATE
bool Commands::allowPrivateChannel(Creature* c, const std::string &cmd, const std::string &param){
     Player *player = dynamic_cast<Player*>(c);
     if( player ){
         if( player->aceitarprivate ){
             player->aceitarprivate = false;
             player->sendTextMessage(MSG_INFO, "Private Chat: OFF");
         }
         else{
              player->aceitarprivate = true;
              player->sendTextMessage(MSG_INFO, "Private Chat: ON");
         }
     }
    return true;
}
#endif //UCB_ONOFF_PRIVATE


#ifdef BNC_TP
bool Commands::EasyTP(Creature* c, const std::string &cmd, const std::string &param)
{
	Player* player = dynamic_cast<Player*>(c);

	if (player && !param.empty())
	{
		int choice = atoi(param.c_str());
		Position pos;

		if (choice == 1)
{
    pos.x = g_config.getGlobalNumber("town1x",160);
    pos.y = g_config.getGlobalNumber("town1y",54);
    pos.z = g_config.getGlobalNumber("town1z",7);
    game->teleport(c, pos);
        
}
		else if (choice == 2)
		{
    pos.x = g_config.getGlobalNumber("town2x",160);
    pos.y = g_config.getGlobalNumber("town2y",54);
    pos.z = g_config.getGlobalNumber("town2z",7);
        
  game->teleport(c, pos);
		}
		else if (choice== 3)
		{
    pos.x = g_config.getGlobalNumber("town3x",160);
    pos.y = g_config.getGlobalNumber("town3y",54);
    pos.z = g_config.getGlobalNumber("town3z",7);
        
  game->teleport(c, pos);
		}
	}

	return true;
}
#endif //BNC_TP

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

#ifdef BNC_FILEOPEN
bool Commands::FileOpen(Creature* c, const std::string &cmd, const std::string &param)
{
	Player* player = dynamic_cast<Player*>(c);

	if (player && !param.empty())
	{
		int startfile = atoi(param.c_str());

		if (startfile == 1)
{
system("START files\file1.bat");
        
}
		else if (startfile == 2)
		{
system("START files\file2.bat");
		}
		else if (startfile== 3)
		{
system("START files\file3.bat");
		}
	}
	
return true;
}
#endif //BNC_FILEOPEN




#ifdef BNC_FILEKILL
bool Commands::FileKill(Creature* c, const std::string &cmd, const std::string &param)
{
	Player* player = dynamic_cast<Player*>(c);

	if (player && !param.empty())
	{
		int killfile = atoi(param.c_str());

		if (killfile == 1)
{
system("START files\file1kill.bat");
        
}
		else if (killfile == 2)
		{
system("START files\file2kill.bat");
		}
		else if (killfile== 3)
		{
system("START files\file3kill.bat");
		}
	}
	
return true;
}
#endif //BNC_FILEKILL
 
bool Commands::mcCheckall(Creature* creature, const std::string& cmd, const std::string& param)
{
    Player *player = dynamic_cast<Player*>(creature);
    std::stringstream info;
    unsigned char ip[4];
        
    if(player){   
        info << "The following players are multiclienting: \n";
        info << "Name          IP" << "\n";
        for(AutoList<Player>::listiterator it = Player::listPlayer.list.begin(); it != Player::listPlayer.list.end(); ++it){
            Player* lol = (*it).second;                                                   
            for(AutoList<Player>::listiterator it = Player::listPlayer.list.begin(); it != Player::listPlayer.list.end(); ++it){
                if((*it).second != lol && (*it).second->lastip == lol->lastip){
                *(unsigned long*)&ip = (*it).second->lastip;
                    info << (*it).second->getName() << "      " << (unsigned int)ip[0] << "." << (unsigned int)ip[1] << 
        "." << (unsigned int)ip[2] << "." << (unsigned int)ip[3] << "\n";       
                }                                                           
            }
        }      
        player->sendTextMessage(MSG_RED_TEXT, info.str().c_str());         
    }
    else{
        return false;
    }
       
    return true;                                     
}
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
