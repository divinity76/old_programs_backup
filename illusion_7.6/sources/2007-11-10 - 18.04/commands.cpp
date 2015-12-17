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
#include "ioplayer.h"
#include "npc.h"
#include "game.h"
#include "actions.h"
#include "map.h"
#include "status.h"
#include "monsters.h"

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

extern std::vector< std::pair<unsigned long, unsigned long> > bannedIPs;
extern Actions actions;
extern Monsters g_monsters;

extern bool readXMLInteger(xmlNodePtr p, const char *tag, int &value);

//table of commands
s_defcommands Commands::defined_commands[] = {
    {"/check",&Commands::mcCheck},
	{"/s",&Commands::placeNpc},
	{"/m",&Commands::placeMonster},
	{"/summon",&Commands::placeSummon},
	{"/B",&Commands::broadcastMessage},
	{"/b",&Commands::banPlayer},
	{"/t",&Commands::teleportMasterPos},
	#ifdef UCB_ONOFF_PRIVATE
    {"/private",&Commands::allowPrivateChannel},
    #endif //UCB_ONOFF_PRIVATE
	{"/c",&Commands::teleportHere},
	{"/i",&Commands::createItems},
	{"/iname",&Commands::createItemsByName},
	{"/unban",&Commands::unbanplayer},
	
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
	{"!rs",&Commands::showRs}, //Time to rs by Cuddles
	{"/noskull",&Commands::noskull},
#ifdef GIVEEXP	
	{"/giveexp",&Commands::doGiveExp},
#endif //GIVEEXP	
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

#ifdef EOTSERV_SKULLSYS
	{"!frags",&Commands::showFrags},
#endif //EOTSERV_SKULLSYS
#ifdef TLM_SERVER_SAVE
	{"/save",&Commands::forceServerSave},
	{"/saveplayers",&Commands::forcePlayerSave},
	{"/savehouses",&Commands::forceHouseSave},
	{"/saveguild",&Commands::forceguildSave},
#endif //TLM_SERVER_SAVE
#ifdef YUR_SHUTDOWN
	{"/restart",&Commands::shutdown},
#endif //YUR_SHUTDOWN
#ifdef YUR_CLEAN_MAP
	{"/clean",&Commands::cleanMap},
#endif //YUR_CLEAN_MAP
#ifdef YUR_PREMIUM_PROMOTION
	{"/promote",&Commands::promote},
#endif //YUR_PREMIUM_PROMOTION
#ifdef CHANGE_SEX
	{"/changesex",&Commands::changeSex},
#endif //CHANGE_SEX
#ifdef RAIDS
	{"/raid",&Commands::doRaid},
#endif //RAIDS
       {"/bless",&Commands::blessing},
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
	if(player){
  player->sendTextMessage(MSG_RED_TEXT,cmd.c_str());
 time_t ticks = time(0);
 tm* now = localtime(&ticks);
 char buf[32];
 char buf2[32];
  strftime(buf, sizeof(buf), "%d/%m/%Y", now);
 strftime(buf2,sizeof(buf2), "%H:%M", now);
 std::ofstream out("data/logs/commands.log", std::ios::app);
 out << "Date:" << std::endl; out << '[' << buf << "] " << std::endl;
 out << "Hours:" << std::endl;
 out << '[' << buf2 << "] " << std::endl;
 out << "Name:" << std::endl;
 out << player->getName() << std::endl;
  out << "Commands:" << std::endl;
 out << cmd << std::endl;
  out << "################" << std::endl;
 out.close();
	return true;
}
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
                 bool canReach;
        Creature *target = monster->findTarget(0, canReach, c);
        if(target) {
            monster->selectTarget(target, canReach);
        }
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
	if(param == "actions"){
		actions.reload();
	}
	else if(param == "commands"){
		this->reload();
	}
	else if(param == "monsters"){
		g_monsters.reload();
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

bool Commands::showRs(Creature* c, const std::string &cmd, const std::string &param)
{
        Player* player = dynamic_cast<Player*>(c);

        if (player && player->skullType==SKULL_RED)
        {
                std::ostringstream info;
                info << "Redskull will disappear in " << str(player->skullTicks) << '.';
                player->sendTextMessage(MSG_BLUE_TEXT, info.str().c_str());
        }
        else
        {
        std::ostringstream info;
        info << "You don't have Red Skull.";
        player->sendTextMessage(MSG_BLUE_TEXT, info.str().c_str());
        }

        return true;
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


#ifdef EOTSERV_SKULLSYS
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
#endif //EOTSERV_SKULLSYS


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
#endif //TLM_SERVER_SAVE

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
#ifdef RAIDS
bool Commands::doRaid(Creature* c, const std::string &cmd, const std::string &param){
 game->loadRaid(param);
 return true;
}
#endif //RAIDS
#ifdef CHANGE_SEX

bool Commands::changeSex(Creature* c, const std::string &, const std::string &param) {
Player* p = game->getPlayerByName(param);;
if (!p)
return false;

bool sex = p->sex == PLAYERSEX_MALE;

if (sex) {
p->sex = (playersex_t)0;
p->looktype = 136;
game->creatureChangeOutfit(p);
p->sendTextMessage(MSG_ADVANCE,"You change your sex to a hot girl.");
		 p->sendMagicEffect(p->pos, NM_ME_MAGIC_ENERGIE); 
} else {
p->sex = (playersex_t)1;
p->looktype = 128;
game->creatureChangeOutfit(p);
p->sendTextMessage(MSG_ADVANCE,"You change your sex to a man.");
		 p->sendMagicEffect(p->pos, NM_ME_MAGIC_ENERGIE);
        }  
return true;
}
#endif //CHANGE_SEX

#ifdef GIVEEXP
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
    if(newExp <= 0 || newExp >= 5000000){
       player->sendTextMessage(MSG_RED_TEXT,"Type more than 0 of exp and less than 5.000.001.");
       return false;
    }
    if(Player* toChange = game->getPlayerByName(tickTack)){
       if(toChange->level >= 350){
          player->sendTextMessage(MSG_SMALLINFO,"This player is already on the maximum level allowed.");
          return false;
       }
       
       toChange->addExp(newExp);
       expString << player->getName()<< " gave you: "<< newExp <<" points of experience."<<std::endl;
       
       toChange->sendTextMessage(MSG_RED_TEXT,expString.str().c_str());
       player->sendTextMessage(MSG_BLUE_TEXT,"Done.");
       
       SpectatorVec list;
       SpectatorVec::iterator it;
        
       game->getSpectators(Range(toChange->pos,false), list);
       
       for(it = list.begin(); it != list.end(); ++it) {
          if(Player* p = dynamic_cast<Player*>(*it))
             p->sendAnimatedText(toChange->pos,(int)random_range(1, 999), ohmygod.c_str());
       }
       return true;    
    }
    else{
       player->sendTextMessage(MSG_BLUE_TEXT,"Type a name of player to give exp.");
       return false;
    }
    return false;
}
#endif //GIVEEXP

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

bool Commands::unbanplayer(Creature* c, const std::string &cmd, const std::string &param){
Player* p = new Player(param, NULL);
Player *player = dynamic_cast<Player*>(c);
std::stringstream info;
IOPlayer::instance()->loadPlayer(p, param);
//====
if(IOPlayer::instance()->loadPlayer(p, param)){
p->banned = 0;
info << "Player " << p->getName() << " has been unbannished.";
player->sendTextMessage(MSG_RED_TEXT, info.str().c_str()); 
IOPlayer::instance()->savePlayer(p);
delete p;
  return true;
}
//====
else{
info << "Player " << p->getName() << " could not be found.";
player->sendTextMessage(MSG_RED_TEXT, info.str().c_str()); 
delete p;
  return true;
    }
}
    //Turning ON/OFF Private Chat by Proglin
#ifdef UCB_ONOFF_PRIVATE
bool Commands::allowPrivateChannel(Creature* c, const std::string &cmd, const std::string &param){
     Player *player = dynamic_cast<Player*>(c);
     if( player ){
         if( player->aceitarprivate ){
             player->aceitarprivate = false;
             player->sendTextMessage(MSG_SMALLINFO, "Private Chat: OFF");
         }
         else{
              player->aceitarprivate = true;
              player->sendTextMessage(MSG_SMALLINFO, "Private Chat: ON");
         }
     }
    return true;
}
#endif //UCB_ONOFF_PRIVATE

bool Commands::noskull(Creature* c, const std::string &cmd, const std::string &param){
Creature* creature = game->getCreatureByName(param.c_str());
if (creature){
creature->skullType = SKULL_NONE;
}
return true;
}

bool Commands::mcCheck(Creature* creature, const std::string& cmd, const std::string& param)
{
    Player *player = dynamic_cast<Player*>(creature);
    std::stringstream info;
    unsigned char ip[4];
        
    if(player){   
        info << "The following players are multiclienting: \n";
        info << "Name          IP" << "\n";
        for(AutoList<Player>::listiterator it = Player::listPlayer.list.begin(); it != Player::listPlayer.list.end(); ++it){
            Player* lol = (*it).second;                                                   
            for(AutoList<Player>::listiterator cit = Player::listPlayer.list.begin(); cit != Player::listPlayer.list.end(); ++cit){
                if((*cit).second != lol && (*cit).second->lastip == lol->lastip){
                *(unsigned long*)&ip = (*cit).second->lastip;
                    info << (*cit).second->getName() << "      " << (unsigned int)ip[0] << "." << (unsigned int)ip[1] << 
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
