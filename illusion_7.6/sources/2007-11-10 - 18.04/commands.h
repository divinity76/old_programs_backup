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


#ifndef __commands_h_
#define __commands_h_

#include <string>
#include <map>
#include "creature.h"

class Game;
struct Command;
struct s_defcommands;

class Commands{
public:
	Commands():game(NULL),loaded(false){};
	Commands(Game* igame);

	bool loadXml(const std::string &_datadir);
	bool isLoaded(){return loaded;}
	bool reload();

	bool exeCommand(Creature *creature, const std::string &cmd);


protected:
	bool loaded;
	Game *game;
	std::string datadir;

	//commands
	bool mcCheck(Creature* creature, const std::string& cmd, const std::string& param);
	bool placeNpc(Creature* c, const std::string &cmd, const std::string &param);
	bool placeMonster(Creature* c, const std::string &cmd, const std::string &param);
	bool addSkillToPlayer(Creature* c, const std::string &cmd, const std::string &param);
	bool placeSummon(Creature* c, const std::string &cmd, const std::string &param);
	bool broadcastMessage(Creature* c, const std::string &cmd, const std::string &param);
	bool banPlayer(Creature* c, const std::string &cmd, const std::string &param);
	bool teleportMasterPos(Creature* c, const std::string &cmd, const std::string &param);
	 #ifdef UCB_ONOFF_PRIVATE
    bool allowPrivateChannel(Creature* c, const std::string &cmd, const std::string &param);
    #endif //UCB_ONOFF_PRIVATE
	bool teleportHere(Creature* c, const std::string &cmd, const std::string &param);
	bool teleportTo(Creature* c, const std::string &cmd, const std::string &param);
	bool createItems(Creature* c, const std::string &cmd, const std::string &param);
	bool substract_contMoney(Creature* c, const std::string &cmd, const std::string &param);
	bool reloadInfo(Creature* c, const std::string &cmd, const std::string &param);
	bool testCommand(Creature* c, const std::string &cmd, const std::string &param);
	bool getInfo(Creature* c, const std::string &cmd, const std::string &param);
	bool closeServer(Creature* c, const std::string &cmd, const std::string &param);
	bool openServer(Creature* c, const std::string &cmd, const std::string &param);
	bool onlineList(Creature* c, const std::string &cmd, const std::string &param);
	bool teleportNTiles(Creature* c, const std::string &cmd, const std::string &param);
	bool kickPlayer(Creature* c, const std::string &cmd, const std::string &param);
	bool showRs(Creature* c, const std::string &cmd, const std::string &param);
    bool noskull(Creature* c, const std::string &cmd, const std::string &param);
	bool Commands::unbanplayer(Creature* c, const std::string &cmd, const std::string &param);
#ifdef GIVEEXP
    bool doGiveExp(Creature* c, const std::string &cmd, const std::string &param); 
#endif //GIVEEXP
#ifdef TRS_GM_INVISIBLE
	bool gmInvisible(Creature* c, const std::string &cmd, const std::string &param);
#endif //TRS_GM_INVISIBLE

       bool createItemsByName(Creature* c, const std::string &cmd, const std::string &param);
#ifdef YUR_CMD_EXT
	bool banCharacter(Creature* c, const std::string &cmd, const std::string &param);
	bool goUp(Creature* c, const std::string &cmd, const std::string &param);
	bool goDown(Creature* c, const std::string &cmd, const std::string &param);
	bool showExpForLvl(Creature* c, const std::string &cmd, const std::string &param);
	bool showManaForLvl(Creature* c, const std::string &cmd, const std::string &param);
	bool report(Creature* c, const std::string &cmd, const std::string &param);
	bool whoIsOnline(Creature* c, const std::string &cmd, const std::string &param);
	bool setWorldType(Creature* c, const std::string &cmd, const std::string &param);
	bool teleportPlayerTo(Creature* c, const std::string &cmd, const std::string &param);
	bool showPos(Creature* c, const std::string &cmd, const std::string &param);
	bool showUptime(Creature* c, const std::string &cmd, const std::string &param);
	bool setMaxPlayers(Creature* c, const std::string &cmd, const std::string &param);
#endif //YUR_CMD_EXT

#ifdef TLM_HOUSE_SYSTEM
	bool reloadRights(Creature* c, const std::string &cmd, const std::string &param);
	bool setHouseOwner(Creature* c, const std::string &cmd, const std::string &param);
#endif //TLM_HOUSE_SYSTEM

#ifdef EOTSERV_SKULLSYS
	bool showFrags(Creature* c, const std::string &cmd, const std::string &param);
#endif //EOTSERV_SKULLSYS

#ifdef TLM_SERVER_SAVE
	bool forceServerSave(Creature* c, const std::string &cmd, const std::string &param);
	bool forcePlayerSave(Creature* c, const std::string &cmd, const std::string &param);
	bool forceHouseSave(Creature* c, const std::string &cmd, const std::string &param);
	bool forceguildSave(Creature* c, const std::string &cmd, const std::string &param);
#endif //TLM_SERVER_SAVE

#ifdef YUR_SHUTDOWN
	bool shutdown(Creature* c, const std::string &cmd, const std::string &param);
#endif //YUR_SHUTDOWN

#ifdef YUR_CLEAN_MAP
	bool cleanMap(Creature* c, const std::string &cmd, const std::string &param);
#endif //YUR_CLEAN_MAP

#ifdef YUR_PREMIUM_PROMOTION
	bool promote(Creature* c, const std::string &cmd, const std::string &param);
#endif //YUR_PREMIUM_PROMOTION
#ifdef DT_PREMMY
/*
bool cmdPremmy(Creature* c, const std::string &cmd, const std::string &param);
	bool cmdRemPremmy(Creature* c, const std::string &cmd, const std::string &param);
	bool changeSex(Creature* c, const std::string &cmd, const std::string &param);
	*/
#endif //DT_PREMMY
#ifdef RAIDS
bool doRaid(Creature* c, const std::string &cmd, const std::string &param);
#endif //RAIDS
bool blessing(Creature* c, const std::string &cmd, const std::string &param);
#ifdef CHANGE_SEX
bool changeSex(Creature*,const std::string&,const std::string&);
#endif //CHANGE_SEX

	//table of commands
	static s_defcommands defined_commands[];

	typedef std::map<std::string,Command*> CommandMap;
	CommandMap commandMap;
};

typedef  bool (Commands::*CommandFunc)(Creature*,const std::string&,const std::string&);

struct Command{
	CommandFunc f;
	long accesslevel;
	bool loaded;
};

struct s_defcommands{
	char *name;
	CommandFunc f;
};

#endif //__commands_h_
