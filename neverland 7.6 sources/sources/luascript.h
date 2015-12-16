//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
// class which takes care of all data which must get saved in the player
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


#ifndef __LUASCRIPT_H__
#define __LUASCRIPT_H__

#include <string>
extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

extern "C" struct lua_State;


class LuaScript
{
public:
	LuaScript();
	~LuaScript();
#ifdef JD_WANDS
	int MANA_SNAKEBITE;
	int MANA_MOONLIGHT;
	int MANA_VOLCANIC;
	int MANA_QUAGMIRE;
	int MANA_TEMPEST;

	int MANA_VORTEX;
	int MANA_DRAGONBREATH;
	int MANA_PLAGUE;
	int MANA_COSMIC;
	int MANA_INFERNO;

	int RANGE_SNAKEBITE;
	int RANGE_MOONLIGHT;
	int RANGE_VOLCANIC;
	int RANGE_QUAGMIRE;
	int RANGE_TEMPEST;

	int RANGE_VORTEX;
	int RANGE_DRAGONBREATH;
	int RANGE_PLAGUE;
	int RANGE_COSMIC;
	int RANGE_INFERNO;
#endif //JD_WANDS
#ifdef RULY_ACCMANAGER	
    int MANAGER_P_EXP;
    int MANAGER_P_LVL;
    int MANAGER_MAGE_ML;
    int MANAGER_PALLY_ML;
    int MANAGER_KNIGHT_ML;
    int MANAGER_TEMPLE_X;
    int MANAGER_TEMPLE_Y;
    int MANAGER_TEMPLE_Z;
    int MANAGER_MAGES_HP;
    int MANAGER_MAGES_MANA;
    int MANAGER_MAGES_FIST;
    int MANAGER_MAGES_CLUB;
    int MANAGER_MAGES_SWORD;
    int MANAGER_MAGES_AXE;
    int MANAGER_MAGES_DISTANCE;
    int MANAGER_MAGES_SHIELD;
    int MANAGER_MAGES_FISHING;
    int MANAGER_PALLY_HP;
    int MANAGER_PALLY_MANA;
    int MANAGER_PALLY_FIST;
    int MANAGER_PALLY_CLUB;
    int MANAGER_PALLY_SWORD;
    int MANAGER_PALLY_AXE;
    int MANAGER_PALLY_DISTANCE;
    int MANAGER_PALLY_SHIELD;
    int MANAGER_PALLY_FISHING;
    int MANAGER_KNIGHT_HP;
    int MANAGER_KNIGHT_MANA;
    int MANAGER_KNIGHT_FIST;
    int MANAGER_KNIGHT_CLUB;
    int MANAGER_KNIGHT_SWORD;
    int MANAGER_KNIGHT_AXE;
    int MANAGER_KNIGHT_DISTANCE;
    int MANAGER_KNIGHT_SHIELD;
    int MANAGER_KNIGHT_FISHING;
    int MANAGER_ALL_SLOT1;
    int MANAGER_ALL_SLOT4;
    int MANAGER_ALL_SLOT5;
    int MANAGER_ALL_SLOT6;
    int MANAGER_ALL_SLOT7;
    int MANAGER_ALL_SLOT8;
    int MANAGER_ALL_SLOT10;	
    int MANAGER_CAP_MAGE;
    int MANAGER_CAP_PALLY;
    int MANAGER_CAP_KNIGHT;
#endif //RULY_ACCMANAGER
int LVL_YELL;
	int ACCESS_PROTECT;
	int ACCESS_ORANGE;
	long PZ_LOCKED;
	long EXHAUSTED;
	long EXHAUSTED_ADD;
	long EXHAUSTED_HEAL;
	int ACCESS_HOUSE;
	int ACCESS_BUGREPORT;
	int ACCESS_TALK;
	int ACCESS_CTRLY;
	int ACCESS_REMOTE;
	int ACCESS_LOOK;
	int ACCESS_ENTER;
	int ACCESS_REPORT;
	bool LEARN_SPELLS;
	std::string PROMOTED_VOCATIONS[5];
	std::string VOCATIONS[5];
	std::string DATA_DIR;
	int MAX_HOUSE_TILE_ITEMS;
#ifdef RUL_BLESS
    int DIE_BLESS_0;
    int DIE_BLESS_1;
    int DIE_BLESS_2;
    int DIE_BLESS_3;
    int DIE_BLESS_4;
    int DIE_BLESS_5;
    int DIE_BLESS_PROMO_1;
    int DIE_BLESS__PROMO_2;
    int DIE_BLESS__PROMO__3;
    int DIE_BLESS__PROMO__4;
    int DIE_BLESS__PROMO__5;
    int DIE_BLESS__PROMO__0;
#endif //RUL_BLESS
    int BLUE_RBE;
#ifdef TLM_SKULLS_PARTY
	int HIT_TIME;
	int WHITE_TIME;
	int RED_TIME;
	int FRAG_TIME;
	int RED_UNJUST;
	int BAN_UNJUST;
#endif //TLM_SKULLS_PARTY
	bool CAP_SYSTEM;	
#ifdef YUR_PREMIUM_PROMOTION
    bool FREE_PREMMY;
	bool QUEUE_PREMMY;
#endif //YUR_PREMIUM_PROMOTION
  int OpenFile(const char* file);
  int getField (const char *key);
  void LuaScript::setField (const char *index, int value);
  //static version
  static int getField (lua_State *L , const char *key);
  static void setField (lua_State *L, const char *index, int val);
  // get a global string
  std::string getGlobalString(std::string var, const std::string &defString = "");
  int getGlobalNumber(std::string var, const int defNum = 0);
  std::string getGlobalStringField (std::string var, const int key, const std::string &defString = "");
  // set a var to a val
  int setGlobalString(std::string var, std::string val);
  int setGlobalNumber(std::string var, int val);

protected:
	std::string luaFile;   // the file we represent
	lua_State*  luaState;  // our lua state
};


#endif  // #ifndef __LUASCRIPT_H__
