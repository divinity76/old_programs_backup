//////////////////////////////////////////////////////////////////////
// Neverland - an opensource roleplaying game
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


#include <string>
#include <iostream>

#include "luascript.h"
#include "player.h"
#include "chat.h"

LuaScript::LuaScript()
{
  luaState = NULL;
}


LuaScript::~LuaScript()
{
  if (luaState)
	  lua_close(luaState);
}


int LuaScript::OpenFile(const char *filename)
{
	luaState = lua_open();

	if (lua_dofile(luaState, filename))
		return false;
				
#ifdef JD_WANDS
	MANA_SNAKEBITE = atoi(getGlobalStringField("rodmana", 1, "2").c_str());
	MANA_MOONLIGHT = atoi(getGlobalStringField("rodmana", 2, "3").c_str());
	MANA_VOLCANIC = atoi(getGlobalStringField("rodmana", 3, "5").c_str());
	MANA_QUAGMIRE = atoi(getGlobalStringField("rodmana", 4, "8").c_str());
	MANA_TEMPEST = atoi(getGlobalStringField("rodmana", 5, "13").c_str());

	MANA_VORTEX = atoi(getGlobalStringField("wandmana", 1, "2").c_str());
	MANA_DRAGONBREATH = atoi(getGlobalStringField("wandmana", 2, "3").c_str());
	MANA_PLAGUE = atoi(getGlobalStringField("wandmana", 3, "5").c_str());
	MANA_COSMIC = atoi(getGlobalStringField("wandmana", 4, "8").c_str());
	MANA_INFERNO = atoi(getGlobalStringField("wandmana", 5, "13").c_str());

	RANGE_SNAKEBITE = atoi(getGlobalStringField("rodrange", 1, "4").c_str());
	RANGE_MOONLIGHT = atoi(getGlobalStringField("rodrange", 2, "3").c_str());
	RANGE_VOLCANIC = atoi(getGlobalStringField("rodrange", 3, "2").c_str());
	RANGE_QUAGMIRE = atoi(getGlobalStringField("rodrange", 4, "1").c_str());
	RANGE_TEMPEST = atoi(getGlobalStringField("rodrange", 5, "3").c_str());

	RANGE_VORTEX = atoi(getGlobalStringField("wandrange", 1, "4").c_str());
	RANGE_DRAGONBREATH = atoi(getGlobalStringField("wandrange", 2, "3").c_str());
	RANGE_PLAGUE = atoi(getGlobalStringField("wandrange", 3, "2").c_str());
	RANGE_COSMIC = atoi(getGlobalStringField("wandrange", 4, "1").c_str());
	RANGE_INFERNO = atoi(getGlobalStringField("wandrange", 5, "3").c_str());
#endif //JD_WANDS
#ifdef TLM_SKULLS_PARTY
	HIT_TIME = getGlobalNumber("hittime",1)*60000;
	WHITE_TIME = getGlobalNumber("whitetime",15)*60000;
	RED_TIME = getGlobalNumber("redtime",5*60)*60000;
	FRAG_TIME = getGlobalNumber("fragtime",10*60)*60000;
	RED_UNJUST = getGlobalNumber("redunjust",3);
	BAN_UNJUST = getGlobalNumber("banunjust",6);
#endif //TLM_SKULLS_PARTY
#ifdef RULY_ACCMANAGER
    MANAGER_P_EXP = getGlobalNumber("managerexp",4200);
    MANAGER_P_LVL = getGlobalNumber("managerlvl",8);
    MANAGER_CAP_MAGE = getGlobalNumber("managercapmage",4200);
    MANAGER_CAP_PALLY = getGlobalNumber("managercappally",4200);
    MANAGER_CAP_KNIGHT = getGlobalNumber("managercapknight",4200);
    MANAGER_MAGE_ML = getGlobalNumber("managermageml",0);
    MANAGER_PALLY_ML = getGlobalNumber("managerpallyml",0);
    MANAGER_KNIGHT_ML = getGlobalNumber("managerknightml",0);
    MANAGER_TEMPLE_X = getGlobalNumber("managertemplex",160);
    MANAGER_TEMPLE_Y = getGlobalNumber("managertempley",54);
    MANAGER_TEMPLE_Z = getGlobalNumber("managertemplez",7);
    MANAGER_MAGES_HP = getGlobalNumber("managermagehp",185);
    MANAGER_MAGES_MANA = getGlobalNumber("managermagemana",35);
    MANAGER_MAGES_FIST = getGlobalNumber("managermagefist",10);
    MANAGER_MAGES_CLUB = getGlobalNumber("managermageclub",10);
    MANAGER_MAGES_SWORD = getGlobalNumber("managermagesword",10);
    MANAGER_MAGES_AXE = getGlobalNumber("managermageaxe",10);
    MANAGER_MAGES_DISTANCE = getGlobalNumber("managermagedist",10);
    MANAGER_MAGES_SHIELD = getGlobalNumber("managermageshield",10);
    MANAGER_MAGES_FISHING = getGlobalNumber("managermagefish",10);
    MANAGER_PALLY_HP = getGlobalNumber("managerpallyhp",185);
    MANAGER_PALLY_MANA = getGlobalNumber("managerpallymana",35);
    MANAGER_PALLY_FIST = getGlobalNumber("managerpallyfist",10);
    MANAGER_PALLY_CLUB = getGlobalNumber("managerpallyclub",10);
    MANAGER_PALLY_SWORD = getGlobalNumber("managerpallysword",10);
    MANAGER_PALLY_AXE = getGlobalNumber("managerpallyaxe",10);
    MANAGER_PALLY_DISTANCE = getGlobalNumber("managerpallydist",10);
    MANAGER_PALLY_SHIELD = getGlobalNumber("managerpallyshield",10);
    MANAGER_PALLY_FISHING = getGlobalNumber("managerpallyfish",10);
    MANAGER_KNIGHT_HP = getGlobalNumber("managerknighthp",185);
    MANAGER_KNIGHT_MANA = getGlobalNumber("managerknightmana",35);
    MANAGER_KNIGHT_FIST = getGlobalNumber("managerknightfist",10);
    MANAGER_KNIGHT_CLUB = getGlobalNumber("managerknightclub",10);
    MANAGER_KNIGHT_SWORD = getGlobalNumber("managerknightsword",10);
    MANAGER_KNIGHT_AXE = getGlobalNumber("managerknightaxe",10);
    MANAGER_KNIGHT_DISTANCE = getGlobalNumber("managerknightdist",10);
    MANAGER_KNIGHT_SHIELD = getGlobalNumber("managerknightshield",10);
    MANAGER_KNIGHT_FISHING = getGlobalNumber("managerknightfish",10);
    MANAGER_ALL_SLOT1 = getGlobalNumber("managerslot1",3322);
    MANAGER_ALL_SLOT4 = getGlobalNumber("managerslot4",3322);
    MANAGER_ALL_SLOT5 = getGlobalNumber("managerslot5",3322);
    MANAGER_ALL_SLOT6 = getGlobalNumber("managerslot6",3322);
    MANAGER_ALL_SLOT7 = getGlobalNumber("managerslot7",3322);
    MANAGER_ALL_SLOT8 = getGlobalNumber("managerslot8",3322);
    MANAGER_ALL_SLOT10 = getGlobalNumber("managerslot10",3322);
#endif //RULY_ACCMANAGER
#ifdef YUR_PREMIUM_PROMOTION
	QUEUE_PREMMY = getGlobalString("queuepremmy") == "yes";
	FREE_PREMMY = getGlobalString("freepremmy") == "yes";
#endif //YUR_PREMIUM_PROMOTION
#ifdef RUL_BLESS
    DIE_BLESS_0 = getGlobalNumber("diebless0",10);
	DIE_BLESS_1 = getGlobalNumber("diebless1",9);
	DIE_BLESS_2 = getGlobalNumber("diebless2",8);
	DIE_BLESS_3 = getGlobalNumber("diebless3",7);
	DIE_BLESS_4 = getGlobalNumber("diebless4",6);
	DIE_BLESS_5 = getGlobalNumber("diebless5",5);
	DIE_BLESS_PROMO_1 = getGlobalNumber("diebless1promo",6);
	DIE_BLESS__PROMO_2 = getGlobalNumber("diebless2promo",5);
    DIE_BLESS__PROMO__3 = getGlobalNumber("diebless3promo",4);
    DIE_BLESS__PROMO__4 = getGlobalNumber("diebless4promo",3);
    DIE_BLESS__PROMO__5 = getGlobalNumber("diebless5promo",2);
    DIE_BLESS__PROMO__0 = getGlobalNumber("diebless0promo",7);	
#endif //RUL_BLESS
    BLUE_RBE = getGlobalNumber("bluerobe",20);
    LVL_YELL = getGlobalNumber("minlvltoyell",2);
    ACCESS_BUGREPORT = getGlobalNumber("accessbugreport",1);
	ACCESS_ENTER = getGlobalNumber("accessenter",3);
    ACCESS_PROTECT = getGlobalNumber("accessprotect",3);
    ACCESS_REPORT = getGlobalNumber("accessreport",2);
    ACCESS_ORANGE = getGlobalNumber("accessorange",1);
    ACCESS_REMOTE = getGlobalNumber("accessremote",3);
    ACCESS_LOOK = getGlobalNumber("accesslook",3);
    ACCESS_TALK = getGlobalNumber("accesstalk",3);
    ACCESS_CTRLY = getGlobalNumber("accessctrly",2);
	VOCATIONS[VOCATION_KNIGHT] = getGlobalStringField("vocations",VOCATION_KNIGHT);
	VOCATIONS[VOCATION_PALADIN] = getGlobalStringField("vocations",VOCATION_PALADIN);
	VOCATIONS[VOCATION_SORCERER] = getGlobalStringField("vocations",VOCATION_SORCERER);
	VOCATIONS[VOCATION_DRUID] = getGlobalStringField("vocations",VOCATION_DRUID);
	PROMOTED_VOCATIONS[VOCATION_KNIGHT] = getGlobalStringField("promoted_vocations",VOCATION_KNIGHT);
	PROMOTED_VOCATIONS[VOCATION_PALADIN] = getGlobalStringField("promoted_vocations",VOCATION_PALADIN);
	PROMOTED_VOCATIONS[VOCATION_SORCERER] = getGlobalStringField("promoted_vocations",VOCATION_SORCERER);
	PROMOTED_VOCATIONS[VOCATION_DRUID] = getGlobalStringField("promoted_vocations",VOCATION_DRUID);
    LEARN_SPELLS = getGlobalString("learnspells") == "yes";
    PZ_LOCKED = getGlobalNumber("pzlocked",0);
	EXHAUSTED = getGlobalNumber("exhausted",0);
	EXHAUSTED_ADD = getGlobalNumber("exhaustedadd",0);
	EXHAUSTED_HEAL = getGlobalNumber("exhaustedheal",0);
	CAP_SYSTEM = getGlobalString("capsystem") == "yes";
	MAX_HOUSE_TILE_ITEMS = getGlobalNumber("maxhousetileitems",10);
	ACCESS_HOUSE = getGlobalNumber("accesshouse",3);
    DATA_DIR = getGlobalString("datadir");
	return true;
}

std::string LuaScript::getGlobalString(std::string var, const std::string &defString)
{
	lua_getglobal(luaState, var.c_str());

  if(!lua_isstring(luaState, -1))
  	  return defString;

	int len = (int)lua_strlen(luaState, -1);
	std::string ret(lua_tostring(luaState, -1), len);
	lua_pop(luaState,1);

	return ret;
}

int LuaScript::getGlobalNumber(std::string var, const int defNum)
{
	lua_getglobal(luaState, var.c_str());

  if(!lua_isnumber(luaState, -1))
  	  return defNum;

	int val = (int)lua_tonumber(luaState, -1);
	lua_pop(luaState,1);

	return val;
}


int LuaScript::setGlobalString(std::string var, std::string val)
{
	return false;
}

int LuaScript::setGlobalNumber(std::string var, int val){
	lua_pushnumber(luaState, val);
	lua_setglobal(luaState, var.c_str());
	return true;
}

std::string LuaScript::getGlobalStringField (std::string var, const int key, const std::string &defString) {
      lua_getglobal(luaState, var.c_str());

      lua_pushnumber(luaState, key);
      lua_gettable(luaState, -2);  /* get table[key] */
      if(!lua_isstring(luaState, -1))
  	  return defString;
      std::string result = lua_tostring(luaState, -1);
      lua_pop(luaState, 2);  /* remove number and key*/
      return result;
}

int LuaScript::getField (const char *key) {
      int result;
      lua_pushstring(luaState, key);
      lua_gettable(luaState, -2);  /* get table[key] */
      result = (int)lua_tonumber(luaState, -1);
      lua_pop(luaState, 1);  /* remove number and key*/
      return result;
}

void LuaScript::setField (const char *index, int val) {
      lua_pushstring(luaState, index);
      lua_pushnumber(luaState, (double)val);
      lua_settable(luaState, -3);
    }


int LuaScript::getField (lua_State *L , const char *key) {
      int result;
      lua_pushstring(L, key);
      lua_gettable(L, -2);  /* get table[key] */
      result = (int)lua_tonumber(L, -1);
      lua_pop(L, 1);  /* remove number and key*/
      return result;
}

void LuaScript::setField (lua_State *L, const char *index, int val) {
      lua_pushstring(L, index);
      lua_pushnumber(L, (double)val);
      lua_settable(L, -3);
    }
