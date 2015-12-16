//////////////////////////////////////////////////////////////////////
// Neverland - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
// class representing the gamestate
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


#include "definitions.h"

#include <string>
#include <sstream>
#include <fstream>

#include <map>
//#include <algorithm>

#ifdef __DEBUG_CRITICALSECTION__
#include <iostream>
#include <fstream>
#endif

#include <boost/config.hpp>
#include <boost/bind.hpp>

using namespace std;

#include <stdio.h>
#include "nvsystem.h"
#include "items.h"
#include "commands.h"
#include "creature.h"
#include "player.h"
#include "monster.h"
#include "npc.h"
#include "game.h"
#include "tile.h"

#include "spells.h"
#include "actions.h"
#include "ioplayer.h"
#include "chat.h"
#include "status.h"
#ifdef VITOR_RVR_HANDLING

#include "networkmessage.h"

#endif
#include "luascript.h"
#include "templates.h"
#include "houses.h"
#include "summons.h"
#include "pvparena.h"
#include <ctype.h>
#include "ioaccount.h" //Acc Manager

#if defined __EXCEPTION_TRACER__
#include "exception.h"
extern OTSYS_THREAD_LOCKVAR maploadlock;
#endif



#define EVENT_CHECKCREATURE          123
#define EVENT_CHECKCREATUREATTACKING 124
extern LuaScript g_config;
extern Spells spells;
extern Actions actions;
extern Commands commands;
extern Chat g_chat;
extern xmlMutexPtr xmlmutex;
#ifdef CAYAN_SPELLBOOK
typedef std::vector<std::string> StringVector;
#endif //CAYAN_SPELLBOOK
extern std::vector< std::pair<unsigned long, unsigned long> > bannedIPs;

struct Teleports
{
   std::string keyword;
   int x,y,z;
   //Position pos; <-Bugged? wtf? always has 31,31,7?
};

GameState::GameState(Game *game, const Range &range)
{
	this->game = game;
	game->getSpectators(range, spectatorlist);
}

#ifdef YUR_PVP_ARENA
bool GameState::isPvpArena(Creature* c)
{
	if (!c)
		return false;
	Tile *tile = game->map->getTile(c->pos);
	return tile && tile->isPvpArena();
}
#endif //YUR_PVP_ARENA

#ifdef YUR_RINGS_AMULETS
int Game::applyAmulets(Player* player, int damage, attacktype_t atype, bool manadrain, bool lifedrain)
{
	if (!player || atype == ATTACK_NONE && !manadrain && !lifedrain)
		return damage;

	double newDamage = (double)damage;
	Item* necklace = player->getItem(SLOT_NECKLACE);
	Item* ring = player->getItem(SLOT_RING);

	if (necklace && necklace->getCharges() > 0)
	{
		if (necklace->getID() == ITEM_STONE_SKIN_AMULET)
		{
			newDamage *= 0.05;
			necklace->useCharge();
		}
		else if (necklace->getID() == ITEM_PROTECTION_AMULET)
		{
			newDamage *= 0.95;
			necklace->useCharge();
		}
		else if ((necklace->getID() == ITEM_DRAGON_NECKLACE && atype == ATTACK_FIRE) ||
			(necklace->getID() == ITEM_SILVER_AMULET && atype == ATTACK_POISON) ||
			(necklace->getID() == ITEM_STRANGE_TALISMAN && atype == ATTACK_ENERGY) ||
			(necklace->getID() == ITEM_ELVEN_AMULET))
		{
			newDamage *= 0.9;
			necklace->useCharge();
		}
		else if (necklace->getID() == BRONZE_AMULET && manadrain)
		{
			newDamage *= 0.5;
			necklace->useCharge();
		}
		else if (necklace->getID() == GARLIC_NECKLACE && lifedrain)
		{
			newDamage *= 0.05;
			necklace->useCharge();
		}

		if (necklace->getCharges() <= 0)
			player->removeItemInventory(SLOT_NECKLACE);
	}

	if (ring && ring->getCharges() > 0)
	{
		if (ring->getID() == ITEM_MIGHT_RING)
		{
			newDamage *= 0.9;
			ring->useCharge();
		}

		if (ring->getCharges() <= 0)
			player->removeItemInventory(SLOT_RING);
	}

	return (int)newDamage;
}
#endif //YUR_RINGS_AMULETS

void GameState::onAttack(Creature* attacker, const Position& pos, const MagicEffectClass* me)
{
	Tile *tile = game->map->getTile(pos);

	if(!tile)
		return;

#ifdef YUR_PVP_ARENA
	CreatureVector arenaLosers;
#endif //YUR_PVP_ARENA

	CreatureVector::iterator cit;
	Player* attackPlayer = dynamic_cast<Player*>(attacker);
	Creature *targetCreature = NULL;
	Player *targetPlayer = NULL;
	for(cit = tile->creatures.begin(); cit != tile->creatures.end(); ++cit) {
		targetCreature = (*cit);
		targetPlayer = dynamic_cast<Player*>(targetCreature);
		bool pvpArena = false;
#ifdef BD_PARALYZE
if(targetPlayer && !me->offensive){
Conditions& conditions = targetCreature->getConditions();
for(Conditions::iterator condIt = conditions.begin(); condIt != conditions.end(); ++condIt) {
if(condIt->first == ATTACK_PARALYZE) {
targetCreature->conditions.erase(condIt);
game->changeSpeed(targetPlayer->getID(), targetPlayer->getNormalSpeed());
if(targetPlayer)
targetPlayer->sendIcons();
}
}
}
#endif //BD_PARALYZE

#ifdef TR_SUMMONS
		bool targetIsSummon = (targetCreature && targetCreature->isPlayersSummon());
		bool summonVsPlayer = (attacker && attacker->isPlayersSummon() && targetPlayer);
#endif //TR_SUMMONS

		int damage = me->getDamage(targetCreature, attacker);
		int manaDamage = 0;

#ifdef YUR_RINGS_AMULETS
		damage = game->applyAmulets(targetPlayer, damage, me->attackType, false, false);
#endif //YUR_RINGS_AMULETS

		if (damage > 0) {
			if(attackPlayer && attackPlayer->access < g_config.ACCESS_PROTECT ) {
				if(targetPlayer && targetPlayer != attackPlayer && game->getWorldType() != WORLD_TYPE_NO_PVP)
					attackPlayer->pzLocked = true;
			}



			if(targetCreature->access < g_config.ACCESS_PROTECT && targetPlayer && game->getWorldType() != WORLD_TYPE_NO_PVP) {
				targetPlayer->inFightTicks = (long)g_config.getGlobalNumber("pzlocked", 0);
				targetPlayer->sendIcons();
			}
	//targetPlayer = dynamic_cast<Player*>(targetCreature);	
    #ifdef SURVIVAL_BLACKSQUARE
    if(damage > 0 && attackPlayer && targetPlayer && attackPlayer != targetPlayer){
NetworkMessage attackedpmsg;
attackedpmsg.AddPlayerAttacked(attacker);
targetPlayer->sendNetworkMessage(&attackedpmsg);
}
#endif //SURVIVAL_BLACKSQUARE	
#ifdef YUR_PVP_ARENA
			pvpArena = isPvpArena(attacker) && isPvpArena(targetCreature);
#endif //YUR_PVP_ARENA

#ifdef TR_SUMMONS
			if ((game->getWorldType() == WORLD_TYPE_NO_PVP && !pvpArena && summonVsPlayer) ||
				(game->getWorldType() == WORLD_TYPE_NO_PVP && !pvpArena && attackPlayer && (targetPlayer || targetIsSummon) && attackPlayer->access < g_config.ACCESS_PROTECT)) {
#else
			if(game->getWorldType() == WORLD_TYPE_NO_PVP && !pvpArena && attackPlayer && targetPlayer && attackPlayer->access < g_config.ACCESS_PROTECT){
#endif //TR_SUMMONS
				damage = 0;
			}
		}

		if(damage != 0) {

                  #ifdef YUR_PVP_ARENA
			game->creatureApplyDamage(targetCreature, damage, damage, manaDamage, (pvpArena? &arenaLosers : NULL));
#else
			game->creatureApplyDamage(targetCreature, damage, damage, manaDamage);
#endif //YUR_PVP_ARENA
		}
		
#ifdef TLM_SKULLS_PARTY
		if (me->offensive && game->getWorldType() == WORLD_TYPE_PVP)
		{
		    if(targetCreature->health <= 0)//!targetCreature->isDead - By that time the target isnt set as dead already. Then this is to avoid getting doubleunjust warnings.
		    game->onPvP(attacker, targetCreature,true);
            else
			game->onPvP(attacker, targetCreature,false);
		}
#endif //TLM_SKULLS_PARTY

if(attackPlayer && attackPlayer->getItem(SLOT_ARMOR) && attackPlayer->getItem(SLOT_ARMOR)->getID() == ITEM_BLUEROBE){
double newdamage = damage*g_config.BLUE_RBE/100.0; // 5% dmg adden
damage += (int)newdamage; // added to main damage
}

			if (targetCreature && !targetPlayer)
			{
				targetCreature->setInvisible(0);
				game->creatureChangeOutfit(targetCreature);
			}
			
		addCreatureState(tile, targetCreature, damage, manaDamage, me->drawblood);
	}

	//Solid ground items/Magic items (fire/poison/energy)
	MagicEffectItem *newmagicItem = me->getMagicItem(attacker, tile->isPz(),
		(tile->isBlocking(BLOCK_SOLID, true) != RET_NOERROR));

	if(newmagicItem) {

		MagicEffectItem *magicItem = tile->getFieldItem();

		if(magicItem) {
			//Replace existing magic field
			magicItem->transform(newmagicItem);

			int stackpos = tile->getThingStackPos(magicItem);
			if(tile->removeThing(magicItem)) {

				SpectatorVec list;
				SpectatorVec::iterator it;

				game->getSpectators(Range(pos, true), list);

				//players
				for(it = list.begin(); it != list.end(); ++it) {
					if(dynamic_cast<Player*>(*it)) {
						(*it)->onThingDisappear(magicItem, stackpos);
					}
				}

				//none-players
				for(it = list.begin(); it != list.end(); ++it) {
					if(!dynamic_cast<Player*>(*it)) {
						(*it)->onThingDisappear(magicItem, stackpos);
					}
				}

				tile->addThing(magicItem);

				//players
				for(it = list.begin(); it != list.end(); ++it) {
					if(dynamic_cast<Player*>(*it)) {
						(*it)->onThingAppear(magicItem);
					}
				}

				//none-players
				for(it = list.begin(); it != list.end(); ++it) {
					if(!dynamic_cast<Player*>(*it)) {
						(*it)->onThingAppear(magicItem);
					}
				}
			}
		}
		else {
			magicItem = new MagicEffectItem(*newmagicItem);
			magicItem->useThing();
			magicItem->pos = pos;

			tile->addThing(magicItem);

			SpectatorVec list;
			SpectatorVec::iterator it;

			game->getSpectators(Range(pos, true), list);

			//players
			for(it = list.begin(); it != list.end(); ++it) {
				if(dynamic_cast<Player*>(*it)) {
					(*it)->onThingAppear(magicItem);
				}
			}

			//none-players
			for(it = list.begin(); it != list.end(); ++it) {
				if(!dynamic_cast<Player*>(*it)) {
					(*it)->onThingAppear(magicItem);
				}
			}

			magicItem->isRemoved = false;
			game->startDecay(magicItem);
		}
	}

	//Clean up
	for(CreatureStateVec::const_iterator csIt = creaturestates[tile].begin(); csIt != creaturestates[tile].end(); ++csIt) {
		onAttackedCreature(tile, attacker, csIt->first, csIt->second.damage, csIt->second.drawBlood);
	}

if(attackPlayer && attackPlayer->access < g_config.ACCESS_PROTECT) {
		//Add exhaustion
		if(me->causeExhaustion(true) /*!areaTargetVec.empty())*/)
		{
#ifdef YUR_HEAL_EXHAUST
			if (!me->offensive && me->minDamage != 0)	// healing
				attackPlayer->exhaustedTicks = (long)g_config.getGlobalNumber("exhaustedheal", 0);
			else
#endif //YUR_HEAL_EXHAUST
				attackPlayer->exhaustedTicks = (long)g_config.getGlobalNumber("exhausted", 0);
		}

		//Fight symbol
		if(me->offensive /*&& !areaTargetVec.empty()*/)
			attackPlayer->inFightTicks = (long)g_config.getGlobalNumber("pzlocked", 0);
	}

#ifdef YUR_PVP_ARENA
	for (CreatureVector::iterator it = arenaLosers.begin(); it != arenaLosers.end(); ++it)
		game->teleport(*it, PvpArena::GetExit());	// kick losers
#endif //YUR_PVP_ARENA
}

void GameState::onAttack(Creature* attacker, const Position& pos, Creature* attackedCreature, fight_t damagetype /*=0*/)
{
	bool pvpArena = false;
#ifdef YUR_PVP_ARENA
	CreatureVector arenaLosers;
	pvpArena = isPvpArena(attacker) && isPvpArena(attackedCreature);
#endif //YUR_PVP_ARENA

	//TODO: Decent formulas and such...
	int damage = attacker->getWeaponDamage();
	int armor = attackedCreature->getArmor();
	int defense = attackedCreature->getDefense();

	Player* attackPlayer = dynamic_cast<Player*>(attacker);
	Player* attackedPlayer = dynamic_cast<Player*>(attackedCreature);

	if(attackedPlayer)
		attackedPlayer->addSkillShieldTry(1);

	int probability = rand() % 10000;


#ifdef YUR_CVS_MODS
	if (probability * damage < defense * 1000)
		damage = 0;
	else
		damage -= (int)(damage*(armor/100.0)*(rand()/(RAND_MAX+1.0))) + armor;

#else

	if(probability * damage < defense * 10000)
		damage = 0;
	else
	{
		damage -= (armor * (10000 + rand() % 10000)) / 10000;
	}
#endif //YUR_CVS_MODS

	int manaDamage = 0;

	if(attackPlayer && attackedPlayer){
		damage -= (int) damage / 2;
	}

	if (attacker->access >= g_config.ACCESS_PROTECT)
		damage += 1337;

if(damage < 0 || attackedCreature->access >= g_config.ACCESS_PROTECT)
        damage = 0;

	Tile* tile = game->map->getTile(pos);
	bool blood;
	if(damage != 0)
	{

#ifdef YUR_ICE_RAPIER
		if (attackPlayer)
			for (int slot = SLOT_RIGHT; slot <= SLOT_LEFT; slot++)
				if(attackPlayer->getItem(slot) && attackPlayer->getItem(slot)->getID() == ITEM_ICE_RAPIER)
					attackPlayer->removeItemInventory(slot);
#endif //YUR_ICE_RAPIER

#ifdef YUR_RINGS_AMULETS
		damage = game->applyAmulets(attackedPlayer, damage, ATTACK_PHYSICAL, false, false);
#endif //YUR_RINGS_AMULETS

#ifdef YUR_PVP_ARENA
		game->creatureApplyDamage(attackedCreature, damage, damage, manaDamage, (pvpArena? &arenaLosers : NULL));
#else
		game->creatureApplyDamage(attackedCreature, damage, damage, manaDamage);
#endif //YUR_PVP_ARENA

#ifdef TLM_SKULLS_PARTY
		if (game->getWorldType() == WORLD_TYPE_PVP)
		{
				if(attackedCreature->health <= 0)
				game->onPvP(attacker, attackedCreature,true);
				else
				game->onPvP(attacker, attackedCreature,false);
		}
#endif //TLM_SKULLS_PARTY

		blood = true;
	}
	else{//no draw blood
		blood = false;
	}

	addCreatureState(tile, attackedCreature, damage, manaDamage, blood);
	onAttackedCreature(tile, attacker, attackedCreature, damage,  true);

#ifdef BDB_REPLACE_SPEARS
	static const int SPEAR_LOSE_CHANCE = g_config.getGlobalNumber("spearlosechance", 0);

	if (attackPlayer && attackPlayer->isUsingSpears() && random_range(1,100000) > SPEAR_LOSE_CHANCE)
	{
		Item* spear = Item::CreateItem(ITEM_SPEAR, 1);
		spear->pos = attackedCreature->pos;
		game->addThing(attackPlayer, spear->pos, spear);
	}
#endif //BDB_REPLACE_SPEARS

#ifdef YUR_PVP_ARENA
	for (CreatureVector::iterator it = arenaLosers.begin(); it != arenaLosers.end(); ++it)
		game->teleport(*it, PvpArena::GetExit());
#endif //YUR_PVP_ARENA
}

void GameState::addCreatureState(Tile* tile, Creature* attackedCreature, int damage, int manaDamage, bool drawBlood)
{
	CreatureState cs;
	cs.damage = damage;
	cs.manaDamage = manaDamage;
	cs.drawBlood = drawBlood;

	creaturestates[tile].push_back( make_pair(attackedCreature, cs) );
}

void GameState::onAttackedCreature(Tile* tile, Creature *attacker, Creature* attackedCreature, int damage, bool drawBlood)
{
	Player *attackedplayer = dynamic_cast<Player*>(attackedCreature);
	Position CreaturePos = attackedCreature->pos;
#ifdef TJ_MONSTER_BLOOD
	bool dead = false;
#endif //TJ_MONSTER_BLOOD

#ifdef TR_SUMMONS
	Creature* attackerMaster = attacker? attacker->getMaster() : NULL;
	if (attackerMaster && dynamic_cast<Player*>(attackerMaster))	// attacker is players summon
	{
		attackedCreature->addInflictedDamage(attacker, damage/2);
		attackedCreature->addInflictedDamage(attackerMaster, damage/2);
	}
	else
		attackedCreature->addInflictedDamage(attacker, damage);
#else
	attackedCreature->addInflictedDamage(attacker, damage);
#endif //TR_SUMMONS

	if(attackedplayer){
		attackedplayer->sendStats();
	}
	//Remove player?
	if(attackedCreature->health <= 0 && attackedCreature->isRemoved == false) {
                                #ifdef TJ_MONSTER_BLOOD
		dead = true;
#endif //TJ_MONSTER_BLOOD
                                //DEATHLIST BY JIDDO START...
if(attackedplayer && attacker)  //Changed to prevent crash!
{
attackedplayer->addDeath(attacker->getName(), attackedplayer->level);
}
//DEATHLIST BY JIDDO END...
		unsigned char stackpos = tile->getThingStackPos(attackedCreature);

		//Prepare body
		Item *corpseitem = Item::CreateItem(attackedCreature->getLookCorpse());
		corpseitem->pos = CreaturePos;
		tile->addThing(corpseitem);

		//Add eventual loot
		Container *lootcontainer = dynamic_cast<Container*>(corpseitem);
		if(lootcontainer) {
			attackedCreature->dropLoot(lootcontainer);
		}

		if(attackedplayer){
			attackedplayer->onThingDisappear(attackedplayer,stackpos);
			attackedplayer->die();        //handles exp/skills/maglevel loss
		}
		//remove creature
		game->removeCreature(attackedCreature);
		// Update attackedCreature pos because contains
		//  temple position for players
		attackedCreature->pos = CreaturePos;

		//add body
		game->sendAddThing(NULL,corpseitem->pos,corpseitem);

		if(attackedplayer){
			std::stringstream ss;
			ss << corpseitem->getDescription(false);

			ss << "You recognize " << attackedplayer->getName() << ". ";
			if(attacker){
				ss << (attackedplayer->getSex() == PLAYERSEX_FEMALE ? "She" : "He") << " was killed by ";

				Player *attackerplayer = dynamic_cast<Player*>(attacker);
				if(attackerplayer) {
					ss << attacker->getName();
				}
				else {
					std::string creaturename = attacker->getName();
					std::transform(creaturename.begin(), creaturename.end(), creaturename.begin(), (int(*)(int))tolower);
					ss << "a " << creaturename;
				}
			}

			//set body special description
			corpseitem->setSpecialDescription(ss.str());
			//send corpse to the dead player. It is not in spectator list
			// because was removed
			attackedplayer->onThingAppear(corpseitem);
		}
		game->startDecay(corpseitem);

		//Get all creatures that will gain xp from this kill..
		CreatureState* attackedCreatureState = NULL;
		std::vector<long> creaturelist;
		if(!(dynamic_cast<Player*>(attackedCreature) && game->getWorldType() != WORLD_TYPE_PVP_ENFORCED)){
			creaturelist = attackedCreature->getInflicatedDamageCreatureList();
			CreatureStateVec& creatureStateVec = creaturestates[tile];
			for(CreatureStateVec::iterator csIt = creatureStateVec.begin(); csIt != creatureStateVec.end(); ++csIt) {
				if(csIt->first == attackedCreature) {
					attackedCreatureState = &csIt->second;
					break;
				}
			}
		}

		if(attackedCreatureState) { //should never be NULL..
			//Add experience
			for(std::vector<long>::const_iterator iit = creaturelist.begin(); iit != creaturelist.end(); ++iit) {
				Creature* gainExpCreature = game->getCreatureByID(*iit);
				if(gainExpCreature) {
					int gainedExperience = attackedCreature->getGainedExperience(gainExpCreature);
					if(gainedExperience <= 0)
						continue;

					Player *gainExpPlayer = dynamic_cast<Player*>(gainExpCreature);


#ifdef RUL_SOUL
				int maxval;

if (gainExpPlayer && gainExpPlayer->promoted)
{
maxval = 200;
} 
else {
maxval = 100; 
} 
if(gainExpPlayer) {
gainExpPlayer->addExp(gainedExperience);
if(gainedExperience && gainedExperience >= gainExpPlayer->level && gainExpPlayer->soul < maxval)
gainExpPlayer->soul += g_config.getGlobalNumber("soulup",1);
}
#endif //RUL_SOUL

					//Need to add this creature and all that can see it to spectators, unless they already added
					SpectatorVec creaturelist;
					game->getSpectators(Range(gainExpCreature->pos, true), creaturelist);

					for(SpectatorVec::const_iterator cit = creaturelist.begin(); cit != creaturelist.end(); ++cit) {
						if(std::find(spectatorlist.begin(), spectatorlist.end(), *cit) == spectatorlist.end()) {
							spectatorlist.push_back(*cit);
						}
					}

					//Add creature to attackerlist
					attackedCreatureState->attackerlist.push_back(gainExpCreature);
				}
			}
		}

		Player *player = dynamic_cast<Player*>(attacker);
		if(player){
			player->sendStats();
		}

		if(attackedCreature && attackedCreature->getMaster() != NULL) {
			attackedCreature->getMaster()->removeSummon(attackedCreature);
		}
	}


	//Add blood?
#ifdef TJ_MONSTER_BLOOD
	if((drawBlood || attackedCreature->health <= 0) && damage > 0 && attackedCreature->bloodsplash != 255) {
		Item* splash = Item::CreateItem(dead? ITEM_POOL : ITEM_SPLASH, attackedCreature->bloodsplash);
		game->addThing(NULL, CreaturePos, splash);
		game->startDecay(splash);
	} 
#else
	if((drawBlood || attackedCreature->health <= 0) && damage > 0 && monster->poisonmonster = false) {
		Item* splash = Item::CreateItem(ITEM_SPLASH, FLUID_BLOOD);
		game->addThing(NULL, CreaturePos, splash);
		game->startDecay(splash);
	}
	else 	if((drawBlood || attackedCreature->health <= 0) && damage > 0 && monster->poisonmonster = true) {
		Item* splash = Item::CreateItem(ITEM_SPLASH, FLUID_SLIME);
		game->addThing(NULL, CreaturePos, splash);
		game->startDecay(splash);
	}
	
#endif //TJ_MONSTER_BLOOD
}


Game::Game()
{
	eventIdCount = 1000;
	servstate = 0;
	this->game_state = GAME_STATE_NORMAL;
	this->map = NULL;
	this->worldType = WORLD_TYPE_PVP;
	OTSYS_THREAD_LOCKVARINIT(gameLock);
	OTSYS_THREAD_LOCKVARINIT(eventLock);
	OTSYS_THREAD_LOCKVARINIT(AutoID::autoIDLock);
#if defined __EXCEPTION_TRACER__
	OTSYS_THREAD_LOCKVARINIT(maploadlock);
#endif
	OTSYS_THREAD_SIGNALVARINIT(eventSignal);
	BufferedPlayers.clear();
	OTSYS_CREATE_THREAD(eventThread, this);

#ifdef __DEBUG_CRITICALSECTION__
	OTSYS_CREATE_THREAD(monitorThread, this);
#endif

	addEvent(makeTask(DECAY_INTERVAL, boost::bind(&Game::checkDecay,this,DECAY_INTERVAL)));
	#ifdef CVS_DAY_CYCLE
	int daycycle = 3600;
	light_hour_delta = 1440*10/daycycle;
	light_hour = 0;
	lightlevel = LIGHT_LEVEL_NIGHT;
	light_state = LIGHT_STATE_NIGHT;
	addEvent(makeTask(10000, boost::bind(&Game::checkLight, this, 10000)));
#endif //CVS_DAY_CYCLE
}


Game::~Game()
{
	if(map) {
		delete map;
	}
}

void Game::setWorldType(enum_world_type type)
{
	this->worldType = type;
}

enum_game_state Game::getGameState()
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::getGameState()");
	return game_state;
}

int Game::loadMap(std::string filename, std::string filekind) {
	if(!map)
		map = new Map;

	max_players = atoi(g_config.getGlobalString("maxplayers").c_str());
		    	distanceToKill = atoi(g_config.getGlobalString("dist", "50").c_str());

	return map->loadMap(filename, filekind);
}




/*****************************************************************************/

#ifdef __DEBUG_CRITICALSECTION__

OTSYS_THREAD_RETURN Game::monitorThread(void *p)
{
  Game* _this = (Game*)p;

	while (true) {
		OTSYS_SLEEP(6000);

		int ret = OTSYS_THREAD_LOCKEX(_this->gameLock, 60 * 2 * 1000);
		if(ret != OTSYS_THREAD_TIMEOUT) {
			OTSYS_THREAD_UNLOCK(_this->gameLock, NULL);
			continue;
		}

		bool file = false;
		std::ostream *outdriver;
		std::cout << "Error: generating critical section file..." <<std::endl;
		std::ofstream output("deadlock.txt",std::ios_base::app);
		if(output.fail()){
			outdriver = &std::cout;
			file = false;
		}
		else{
			file = true;
			outdriver = &output;
		}

		time_t rawtime;
		time(&rawtime);
		*outdriver << "*****************************************************" << std::endl;
		*outdriver << "Error report - " << std::ctime(&rawtime) << std::endl;

		OTSYS_THREAD_LOCK_CLASS::LogList::iterator it;
		for(it = OTSYS_THREAD_LOCK_CLASS::loglist.begin(); it != OTSYS_THREAD_LOCK_CLASS::loglist.end(); ++it) {
			*outdriver << (it->lock ? "lock - " : "unlock - ") << it->str
				<< " threadid: " << it->threadid
				<< " time: " << it->time
				<< " ptr: " << it->mutexaddr
				<< std::endl;
		}

		*outdriver << "*****************************************************" << std::endl;
		if(file)
			((std::ofstream*)outdriver)->close();

		std::cout << "Error report generated. Killing server." <<std::endl;
		exit(1); //force exit
	}
}
#endif

OTSYS_THREAD_RETURN Game::eventThread(void *p)
{
#if defined __EXCEPTION_TRACER__
	ExceptionHandler eventExceptionHandler;
	eventExceptionHandler.InstallHandler();
#endif

  Game* _this = (Game*)p;

  // basically what we do is, look at the first scheduled item,
  // and then sleep until it's due (or if there is none, sleep until we get an event)
  // of course this means we need to get a notification if there are new events added
  while (true)
  {
#ifdef __DEBUG__EVENTSCHEDULER__
    std::cout << "schedulercycle start..." << std::endl;
#endif

    SchedulerTask* task = NULL;
		bool runtask = false;

    // check if there are events waiting...
    OTSYS_THREAD_LOCK(_this->eventLock, "eventThread()")

		int ret;
    if (_this->eventList.size() == 0) {
      // unlock mutex and wait for signal
      ret = OTSYS_THREAD_WAITSIGNAL(_this->eventSignal, _this->eventLock);
    } else {
      // unlock mutex and wait for signal or timeout
      ret = OTSYS_THREAD_WAITSIGNAL_TIMED(_this->eventSignal, _this->eventLock, _this->eventList.top()->getCycle());
    }
    // the mutex is locked again now...
    if (ret == OTSYS_THREAD_TIMEOUT) {
      // ok we had a timeout, so there has to be an event we have to execute...
#ifdef __DEBUG__EVENTSCHEDULER__
      std::cout << "event found at " << OTSYS_TIME() << " which is to be scheduled at: " << _this->eventList.top()->getCycle() << std::endl;
#endif
      task = _this->eventList.top();
      _this->eventList.pop();
		}

		if(task) {
			std::map<unsigned long, SchedulerTask*>::iterator it = _this->eventIdMap.find(task->getEventId());
			if(it != _this->eventIdMap.end()) {
				_this->eventIdMap.erase(it);
				runtask = true;
			}
		}

		OTSYS_THREAD_UNLOCK(_this->eventLock, "eventThread()");
    if (task) {
			if(runtask) {
				(*task)(_this);
			}
			delete task;
    }
  }
#if defined __EXCEPTION_TRACER__
	eventExceptionHandler.RemoveHandler();
#endif

}

unsigned long Game::addEvent(SchedulerTask* event) {
  bool do_signal = false;
  OTSYS_THREAD_LOCK(eventLock, "addEvent()")

	if(event->getEventId() == 0) {
		++eventIdCount;
		event->setEventId(eventIdCount);
	}

#ifdef __DEBUG__EVENTSCHEDULER__
		std::cout << "addEvent - " << event->getEventId() << std::endl;
#endif

	eventIdMap[event->getEventId()] = event;

	/*
	if (eventList.empty() ||  *event < *eventList.top())
    do_signal = true;
	*/

	bool isEmpty = eventList.empty();
	eventList.push(event);

	if(isEmpty || *event < *eventList.top())
		do_signal = true;

	/*
	if (eventList.empty() ||  *event < *eventList.top())
    do_signal = true;
	*/

  OTSYS_THREAD_UNLOCK(eventLock, "addEvent()")

	if (do_signal)
		OTSYS_THREAD_SIGNAL_SEND(eventSignal);

	return event->getEventId();
}

bool Game::stopEvent(unsigned long eventid) {
	if(eventid == 0)
		return false;

  OTSYS_THREAD_LOCK(eventLock, "stopEvent()")

	std::map<unsigned long, SchedulerTask*>::iterator it = eventIdMap.find(eventid);
	if(it != eventIdMap.end()) {

#ifdef __DEBUG__EVENTSCHEDULER__
		std::cout << "stopEvent - eventid: " << eventid << "/" << it->second->getEventId() << std::endl;
#endif

		//it->second->setEventId(0); //invalidate the event
		eventIdMap.erase(it);

	  OTSYS_THREAD_UNLOCK(eventLock, "stopEvent()")
		return true;
	}

  OTSYS_THREAD_UNLOCK(eventLock, "stopEvent()")
	return false;
}

/*****************************************************************************/

uint32_t Game::getPlayersOnline() {return (uint32_t)Player::listPlayer.list.size();};
uint32_t Game::getMonstersOnline() {return (uint32_t)Monster::listMonster.list.size();};
uint32_t Game::getNpcsOnline() {return (uint32_t)Npc::listNpc.list.size();};
uint32_t Game::getCreaturesOnline() {return (uint32_t)listCreature.list.size();};

Tile* Game::getTile(unsigned short _x, unsigned short _y, unsigned char _z)
{
	return map->getTile(_x, _y, _z);
}

Tile* Game::getTile(const Position& pos)
{
	return map->getTile(pos);
}

void Game::setTile(unsigned short _x, unsigned short _y, unsigned char _z, unsigned short groundId)
{
	map->setTile(_x, _y, _z, groundId);
}

Creature* Game::getCreatureByID(unsigned long id)
{
	if(id == 0)
		return NULL;

	AutoList<Creature>::listiterator it = listCreature.list.find(id);
	if(it != listCreature.list.end()) {
		return (*it).second;
	}

	return NULL; //just in case the player doesnt exist
}

Player* Game::getPlayerByID(unsigned long id)
{
	if(id == 0)
		return NULL;

	AutoList<Player>::listiterator it = Player::listPlayer.list.find(id);
	if(it != Player::listPlayer.list.end()) {
		return (*it).second;
	}

	return NULL; //just in case the player doesnt exist
}

Creature* Game::getCreatureByName(const std::string &s)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::getCreatureByName()");

	std::string txt1 = s;
	std::transform(txt1.begin(), txt1.end(), txt1.begin(), upchar);
	for(AutoList<Creature>::listiterator it = listCreature.list.begin(); it != listCreature.list.end(); ++it){
		std::string txt2 = (*it).second->getName();
		std::transform(txt2.begin(), txt2.end(), txt2.begin(), upchar);
		if(txt1 == txt2)
			return it->second;
	}

	return NULL; //just in case the creature doesnt exist
}

Creature* Game::getCreatureByPosition(int x, int y, int z)
{
	for(AutoList<Creature>::listiterator it = listCreature.list.begin(); it != listCreature.list.end(); ++it){
		if(it->second->pos.x == x && it->second->pos.y == y && it->second->pos.z == z)
			return it->second;
	}

	return NULL;
}

Player* Game::getPlayerByName(const std::string &s)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::getPlayerByName()");

	std::string txt1 = s;
	std::transform(txt1.begin(), txt1.end(), txt1.begin(), upchar);
	for(AutoList<Player>::listiterator it = Player::listPlayer.list.begin(); it != Player::listPlayer.list.end(); ++it){
		std::string txt2 = (*it).second->getName();
		std::transform(txt2.begin(), txt2.end(), txt2.begin(), upchar);
		if(txt1 == txt2)
			return it->second;
	}

	return NULL; //just in case the player doesnt exist
}

bool Game::placeCreature(Position &pos, Creature* c
#ifdef YUR_LOGIN_QUEUE
						 , int* placeInQueue
#endif //YUR_LOGIN_QUEUE
						 )
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::placeCreature()");

	bool success = false;
	Player *p = dynamic_cast<Player*>(c);
Monster *monsterzin = dynamic_cast<Monster*>(c);

#ifdef YUR_LOGIN_QUEUE
	if (!p || c->access >= g_config.ACCESS_ENTER || 
 #ifdef YUR_PREMIUM_PROMOTION
		(p->premmium == true && !g_config.QUEUE_PREMMY) ||
 #endif //YUR_PREMIUM_PROMOTION
		loginQueue.login(p->accountNumber, getPlayersOnline(), max_players, placeInQueue)) {
#else //YUR_LOGIN_QUEUE
	if (!p || c->access >= g_config.ACCESS_ENTER || getPlayersOnline() < max_players 
#ifdef RULY_ACCMANAGER
	|| c->getName() == "Ruly The Acc Manager")
#endif //RULY_ACCMANAGER
     {
#endif //YUR_LOGIN_QUEUE
	    
		success = map->placeCreature(pos, c);
		if(success) {
			c->useThing();

			c->setID();
			//std::cout << "place: " << c << " " << c->getID() << std::endl;
			listCreature.addList(c);
			c->addList();
			c->isRemoved = false;

			sendAddThing(NULL,c->pos,c);

			if(p)
			{
                 #ifdef LOOSIK_RECORD
                                  checkRecord();
#endif //LOOSIK_RECORD

#ifdef __DEBUG_PLAYERS__
						std::cout << (uint32_t)getPlayersOnline() << " players online." << std::endl;
					//	std::cout << (uint32_t)getNpcsOnline() << " npcs online." << std::endl;
#ifdef TLM_BED
if (p->isSleeping()){
                     changeBed(getBedPos(p->getName()), getBedID(getBedPos(p->getName())), "Nobody");
int SET_TIME = (long)g_config.getGlobalNumber("bedregain",60); 
      int howmuch;
    time_t login;
    login = std::time(NULL);
    howmuch = login - p->lastlogout;
    howmuch = howmuch/SET_TIME;
    int maxval;
if (p && p->promoted)
{
maxval = 200;
} 
else {
maxval = 100; 
} 
    #ifdef _DEBUG_PLAYERS__
     std::cout << p->getName() << "regain" << howmuch << "after sleeping." <<std::endl; 
   #endif //__DEBUG_PLAYERS__
    if(p->healthmax - p->health > 0)
          p->health += min(howmuch, p->healthmax - p->health);
    if(p->manamax - p->mana > 0)
          p->mana += min(howmuch, p->manamax - p->mana);
          #ifdef RUL_SOUL
                        if(maxval - p->soul > 0)
    p->soul += min(howmuch, maxval - p->soul);
              }
#endif //RUL_SOUL



#endif //TLM_BED

#endif
#ifdef YUR_GUILD_SYSTEM
				Guilds::ReloadGuildInfo(p);
#endif //YUR_GUILD_SYSTEM
#ifdef ELEM_VIP_LIST
				vipLogin(p);
#endif //ELEM_VIP_LIST
			}

			if(p){
				c->eventCheck = addEvent(makeTask(1000, std::bind2nd(std::mem_fun(&Game::checkCreature), c->getID())));
			}
			else{
				c->eventCheck = addEvent(makeTask(500, std::bind2nd(std::mem_fun(&Game::checkCreature), c->getID())));
			}

			//c->eventCheckAttacking = addEvent(makeTask(2000, std::bind2nd(std::mem_fun(&Game::checkCreatureAttacking), c->getID())));
				}
	else {
		//we cant add the player, server is full
		success = false;
	}

if(monsterzin){
     c->masterPos.x = c->pos.x;
	 c->masterPos.y = c->pos.y;
	 c->masterPos.z = c->pos.z;  	   
  } 
  } 
  return success;
}

bool Game::removeCreature(Creature* c)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::removeCreature()");
	if(c->isRemoved == true)
		return false;
#ifdef __DEBUG__
	std::cout << "removing creature "<< std::endl;
#endif



	if(!removeThing(NULL,c->pos,c))
		return false;

	//std::cout << "remove: " << c << " " << c->getID() << std::endl;
	listCreature.removeList(c->getID());
	c->removeList();
	c->isRemoved = true;

	for(std::list<Creature*>::iterator cit = c->summons.begin(); cit != c->summons.end(); ++cit) {
		removeCreature(*cit);
	}

	stopEvent(c->eventCheck);
	stopEvent(c->eventCheckAttacking);
#ifdef BD_FOLLOW
stopEvent(c->eventCheckFollow);
#endif //BD_FOLLOW
	Player* player = dynamic_cast<Player*>(c);
	if(player){
       #ifdef TLM_SKULLS_PARTY
	    for(AutoList<Player>::listiterator it = Player::listPlayer.list.begin(); it != Player::listPlayer.list.end(); ++it){
           Player *on = dynamic_cast<Player*>(it->second);
		   if(on && player->isYellowTo(on)){
			/*#ifdef DEBUG_YELLOWSKULL
            std::cout << "Debugger: " << player->getName() << " isYellowTo " << on->getName() << ". Removing from list.. (Removing player.)" << std::endl;
            #endif //DEBUG_YELLOWSKULL*/
		   on->removeFromYellowList(player);
		   /*#ifdef DEBUG_YELLOWSKULL
		   std::cout << "Debugger: Erased " << player->getName() << " from " << on->getName() << "'s hasAsYellow list. (Removed player.)" << std::endl;
		   #endif //DEBUG_YELLOWSKULL*/
           }
		   if(on && on->hasAttacked(player)){
		   /*#ifdef DEBUG_YELLOWSKULL
		   std::cout << "Debugger: " << on->getName() << " hasAttacked " << player->getName() << ". Removing from list.. (Removing player.)" << std::endl;
		   #endif //DEBUG_YELLOWSKULL*/
		   on->removeFromAttakedList(player);
		   /*#ifdef DEBUG_YELLOWSKULL
		   std::cout << "Debugger: Erased " << player->getName() << " from " << on->getName() << "'s attakedPlayers list. (Removed player.)" << std::endl;
		   #endif //DEBUG_YELLOWSKULL*/
           }
           player->clearAttakedList();
           player->clearYellowList();
        }
        
		if(player->party != 0)
			LeaveParty(player);
#endif //TLM_SKULLS_PARTY        
#ifdef BRWOLF_EXETAVIS
        if(player->items[SLOT_RIGHT] && player->items[SLOT_RIGHT]->getID() == ITEM_MAGIC_STAFF)
            player->items[SLOT_RIGHT]->setID(ITEM_STAFF);
        else if(player->items[SLOT_LEFT] && player->items[SLOT_LEFT]->getID() == ITEM_MAGIC_STAFF)
            player->items[SLOT_LEFT]->setID(ITEM_STAFF);
#endif //BRWOLF_EXETAVIS
		if(player->tradePartner != 0) {
			playerCloseTrade(player);
		}
		if(player->eventAutoWalk)
			stopEvent(player->eventAutoWalk);

g_chat.removeUserFromAllChannels(player);

	if(!IOPlayer::instance()->savePlayer(player)){
			std::cout << "Error while saving player: " << player->getName() << std::endl;
		}
		
        #ifdef RULY_ACCMANAGER
        char accstring[16];
        sprintf(accstring, "%i",player->accountNumber);
        if(player->getName() != "Ruly the Acc Manager" && player->getName() != accstring)
		IOPlayer::instance()->savePlayer(player);
        #endif //RULY_ACCMANAGER
		#ifdef __DEBUG_PLAYERS__
		std::cout << (uint32_t)getPlayersOnline() << " players online." << std::endl;
	//	std::cout << (uint32_t)getNpcsOnline() << " npcs online." << std::endl;
		#endif
player->lastlogout = std::time(NULL);
#ifdef ELEM_VIP_LIST
		vipLogout(c->getName());
#endif //ELEM_VIP_LIST
	}

	this->FreeThing(c);
	return true;
}

void Game::thingMove(Creature *creature, Thing *thing,
	unsigned short to_x, unsigned short to_y, unsigned char to_z, unsigned char count)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::thingMove() - 1");

	Tile *fromTile = map->getTile(thing->pos);

	if (fromTile)
	{
		int oldstackpos = fromTile->getThingStackPos(thing);
		thingMoveInternal(creature, thing->pos.x, thing->pos.y, thing->pos.z, oldstackpos, 0, to_x, to_y, to_z, count);
	}
}

// Move Up
void Game::thingMove(Player *player, unsigned char from_cid, unsigned char from_slotid)
{
 OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::thingMove() - 0");
  
 thingMoveInternal(player, from_cid, from_slotid);
}

void Game::thingMove(Creature *creature, unsigned short from_x, unsigned short from_y, unsigned char from_z,
	unsigned char stackPos, unsigned short itemid, unsigned short to_x, unsigned short to_y, unsigned char to_z, unsigned char count)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::thingMove() - 2");

	Tile *fromTile = getTile(from_x, from_y, from_z);
	if(!fromTile)
		return;

	Thing* thing = fromTile->getThingByStackPos(stackPos);
	if(!thing)
		return;

	Item* item = dynamic_cast<Item*>(thing);

	if(item && (item->getID() != itemid || item != fromTile->getTopDownItem()))
		return;

	thingMoveInternal(creature, from_x, from_y, from_z, stackPos, itemid, to_x, to_y, to_z, count);
}

//container/inventory to container/inventory
void Game::thingMove(Player *player,
	unsigned char from_cid, unsigned char from_slotid, unsigned short itemid, bool fromInventory,
	unsigned char to_cid, unsigned char to_slotid, bool toInventory,
	unsigned char count)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::thingMove() - 3");

	thingMoveInternal(player, from_cid, from_slotid, itemid, fromInventory,
		to_cid, to_slotid, toInventory, count);
}

//container/inventory to ground
void Game::thingMove(Player *player,
	unsigned char from_cid, unsigned char from_slotid, unsigned short itemid, bool fromInventory,
	const Position& toPos, unsigned char count)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::thingMove() - 4");

	thingMoveInternal(player, from_cid, from_slotid, itemid, fromInventory, toPos, count);
}

//ground to container/inventory
void Game::thingMove(Player *player,
	const Position& fromPos, unsigned char stackPos, unsigned short itemid,
	unsigned char to_cid, unsigned char to_slotid, bool toInventory,
	unsigned char count)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::thingMove() - 5");
	thingMoveInternal(player, fromPos, stackPos, itemid, to_cid, to_slotid, toInventory, count);
}

/*ground -> ground*/
bool Game::onPrepareMoveThing(Creature* player, /*const*/ Thing* thing,
const Position& fromPos, const Position& toPos, int count)
{
     /*
	if ((player->access < g_config.ACCESS_REMOTE || dynamic_cast<const Player*>(thing)) && 
		((abs(player->pos.x - fromPos.x) > 1) || (abs(player->pos.y - fromPos.y) > 1) || (player->pos.z != fromPos.z))) 
	{
		player->sendCancel("Too far away...");
		return false;
 }
 */
 	const Creature* movingCreature = dynamic_cast<const Creature*>(thing);
 	/*
	if ((player->access < g_config.ACCESS_REMOTE || dynamic_cast<const Player*>(thing)) && abs(player->pos.x - fromPos.x) > 1 || abs(player->pos.y - fromPos.y) > 1 || player->pos.z != fromPos.z) {
		player->sendCancel("Too far away.");
 return false;
	}
	*/
		if ((player->access < g_config.ACCESS_REMOTE || dynamic_cast<const Player*>(thing)) && 
		((abs(player->pos.x - fromPos.x) > 1) || (abs(player->pos.y - fromPos.y) > 1) || (player->pos.z != fromPos.z))) 
	{
		player->sendCancel("Too far away...");
		return false;
 }
 else if( ((abs(fromPos.x - toPos.x) > thing->throwRange) || (abs(fromPos.y - toPos.y) > thing->throwRange)
  || (fromPos.z != toPos.z)) && player->access >= g_config.ACCESS_PROTECT) {
        if(player == thing)
           teleport(player,toPos);
        else
           teleport(thing,toPos);
    }	
 /*
 	if(!via_walkto) {
 if(player->walkTo.type != WALK_NONE) {
            cancelWalkTo(player);
        }
        else if(player->eventAutoWalk) {
            stopEvent(player->eventAutoWalk);
        }
        
        player->walkTo.type = WALK_THROWITEM;
        player->walkTo.to_pos = pos;
        player->walkTo.stack = stackpos;
        player->walkTo.index = index;
        player->walkTo.itemid = itemid;
        
        player->walkTo.dest_pos = getDestinationPos(player);
        
        int ticks = (int)player->getSleepTicks();
		player->eventAutoWalk = addEvent(makeTask(ticks, std::bind2nd(std::mem_fun(&Game::checkPlayerWalkTo), player->getID())));
    }
    */
    /*
 else if( ((abs(fromPos.x - toPos.x) > thing->throwRange) || (abs(fromPos.y - toPos.y) > thing->throwRange)
  || (fromPos.z != toPos.z)) && player->access >= g_config.ACCESS_PROTECT) {
        if(player == thing)
           teleport(player,toPos);
        else
           teleport(thing,toPos);
    }
    */

	else if ((player->access < g_config.ACCESS_REMOTE || dynamic_cast<const Player*>(thing)) && ((abs(fromPos.x - toPos.x) > thing->throwRange || abs(fromPos.y - toPos.y) > thing->throwRange || (abs(fromPos.z - toPos.z+1) > thing->throwRange)))) {
        player->sendCancel("Destination is out of reach.");
		return false;
	}

          
	    else if(player->access < g_config.ACCESS_REMOTE && movingCreature && fromPos.z != toPos.z){
    player->sendCancel("Too far away.");
    return false;
}

/*
else if( abs(fromPos.x - toPos.x) > thing->throwRange || abs(fromPos.y - 

toPos.y) > thing->throwRange) {
        player->sendCancel("Destination is out of reach.");
		return false;
	}
int z = std::max(std::abs(fromPos.z - toPos.z));
if(z >1){
player->sendCancel("Destination is out of reach.");
		return false;
}  
*/  
	else {
		const Item* item = dynamic_cast<const Item*>(thing);
		if(item) {
			int blockstate = 0;
			if(item->isBlocking())
				blockstate |= BLOCK_SOLID;

			if(item->isPickupable() || !item->isNotMoveable())
				blockstate |= BLOCK_PICKUPABLE;

			if(blockstate != 0) {
				switch(map->canThrowObjectTo(fromPos, toPos, blockstate)) {
					case RET_NOERROR:
						return true;
						break;

					case RET_CANNOTTHROW:
						player->sendCancel("You cannot throw there.");
						return false;
						break;

					case RET_CREATUREBLOCK:
					case RET_NOTENOUGHROOM:
						player->sendCancel("There is not enough room.");
						return false;
						break;

					default:
						player->sendCancel("Sorry not possible.");
						return false;
						break;
				}
			}
		}
	}

	return true;
}

/*ground -> ground*/
bool Game::onPrepareMoveThing(Creature* creature, const Thing* thing,
	const Tile* fromTile, const Tile *toTile, int count)
{
	const Player* player = dynamic_cast<const Player*>(creature);

	const Item *item = dynamic_cast<const Item*>(thing);
	const Creature* movingCreature = dynamic_cast<const Creature*>(thing);
	const Player* movingPlayer = dynamic_cast<const Player*>(thing);

	if(item && !item->canMovedTo(toTile)) {
	 	creature->sendCancel("Sorry, not possible.");
		return false;
	}
	else if(movingCreature && !movingCreature->canMovedTo(toTile)) {
    if(player) {
		  player->sendTextMessage(MSG_SMALLINFO, "Sorry, not possible.");
		  player->sendCancelWalk();
    }

		return false;
	}
	else if(!movingPlayer && toTile && toTile->floorChange()) {
		creature->sendCancel("Sorry, not possible.");
		return false;
	}
  else if(movingCreature && toTile && !toTile->ground) {
    if(player) {
      player->sendTextMessage(MSG_SMALLINFO, "Sorry, not possible.");
      player->sendCancelWalk();
    }

		return false;
	}

	if (fromTile && fromTile->splash == thing && fromTile->splash->isNotMoveable()) {
			creature->sendCancel("You cannot move this object.");
#ifdef __DEBUG__
		cout << creature->getName() << " is trying to move a splash item!" << std::endl;
#endif
		return false;
	}
	else if (item && item->isNotMoveable()) {
			creature->sendCancel("You cannot move this object.");
#ifdef __DEBUG__
		cout << creature->getName() << " is trying to move an unmoveable item!" << std::endl;
#endif
		return false;
	}
#ifdef TLM_HOUSE_SYSTEM
	if (item && toTile && toTile->isHouse())
	{
		const Container* container = dynamic_cast<const Container*>(item);
		int moving = container? container->getItemHoldingCount() : 1;

		if (moving + toTile->getItemHoldingCount() > g_config.MAX_HOUSE_TILE_ITEMS)
		{
			creature->sendCancel("You cannot put more items on a house tile.");
			return false;
		}
	}
#endif //TLM_HOUSE_SYSTEM
	return true; //return thing->canMovedTo(toTile);
}

/*inventory -> container*/
bool Game::onPrepareMoveThing(Player* player, const Item* fromItem, slots_t fromSlot,
	const Container* toContainer, const Item* toItem, int count)
{
if(!fromItem->isPickupable() && player->access < g_config.ACCESS_REMOTE) {  
  player->sendCancel("You cannot move this object.");
  return false;
 }
	else {
		const Container *itemContainer = dynamic_cast<const Container*>(fromItem);
		if(itemContainer) {
			if(itemContainer->isHoldingItem(toContainer) || (toContainer == itemContainer)) {
				player->sendCancel("This is impossible.");
				return false;
			}
		}

		if((!fromItem->isStackable() || !toItem || fromItem->getID() != toItem->getID() || toItem->getItemCountOrSubtype() >= 100) && toContainer->size() + 1 > toContainer->capacity()) {
			player->sendCancel("Sorry not enough room.");
			return false;
		}

		Container const *topContainer = toContainer->getTopParent();
		int itemsToadd;
		if(!topContainer)
			topContainer = toContainer;

		Container const *fromContainer = dynamic_cast<const Container*>(fromItem);
		if(fromContainer)
			itemsToadd = fromContainer->getItemHoldingCount() + 1;
		else
			itemsToadd = 1;

		if(topContainer->depot != 0 && player->max_depot_items != 0 && topContainer->getItemHoldingCount() + itemsToadd >= player->max_depot_items){
			player->sendCancel("You can not put more items in this depot.");
			return false;
		}
	}

	return true;
}

/*container -> container*/
/*ground -> container*/
bool Game::onPrepareMoveThing(Player* player,
	const Position& fromPos, const Container* fromContainer, const Item* fromItem,
	const Position& toPos, const Container* toContainer, const Item* toItem, int count)
{
	if (player->access < g_config.ACCESS_REMOTE && 
		((abs(player->pos.x - fromPos.x) > 1) || (abs(player->pos.y - fromPos.y) > 1) || (player->pos.z != fromPos.z))) 
	{
		player->sendCancel("Too far away...");
		return false;
	}
	else if (player->access < g_config.ACCESS_REMOTE && 
		((abs(fromPos.x - toPos.x) > fromItem->throwRange) || (abs(fromPos.y - toPos.y) > fromItem->throwRange)
		|| (fromPos.z != toPos.z))) 
	{
		player->sendCancel("Destination is out of reach.");
		return false;
	}

	if(!fromItem->isPickupable()) {
		player->sendCancel("You cannot move this object.");
		return false;
	}
	else {
		if((!fromItem->isStackable() || !toItem || fromItem->getID() != toItem->getID() || toItem->getItemCountOrSubtype() >= 100) && toContainer->size() + 1 > toContainer->capacity()) {
			player->sendCancel("Sorry, not enough room.");
			return false;
		}

		const Container *itemContainer = dynamic_cast<const Container*>(fromItem);
		if(itemContainer) {
			if(itemContainer->isHoldingItem(toContainer) || (toContainer == itemContainer) || (fromContainer && fromContainer == itemContainer)) {
				player->sendCancel("This is impossible.");
				return false;
			}
		}

		double itemWeight = (fromItem->isStackable() ? Item::items[fromItem->getID()].weight * std::max(1, count) : fromItem->getWeight());
		if((!fromContainer || !player->isHoldingContainer(fromContainer)) && player->isHoldingContainer(toContainer)) {
			if(player->access < g_config.ACCESS_PROTECT && player->getFreeCapacity() < itemWeight) {
				player->sendCancel("This object is too heavy.");
				return false;
			}
		}

		Container const *topContainer = toContainer->getTopParent();
		int itemsToadd;
		if(!topContainer)
			topContainer = toContainer;

		Container const *fromContainer = dynamic_cast<const Container*>(fromItem);
		if(fromContainer)
			itemsToadd = fromContainer->getItemHoldingCount() + 1;
		else
			itemsToadd = 1;

		if(topContainer->depot != 0 && player->max_depot_items != 0 && topContainer->getItemHoldingCount() >= player->max_depot_items){
			player->sendCancel("You can not put more items in this depot.");
			return false;
		}
	}

	return true;
}

/*ground -> ground*/
bool Game::onPrepareMoveCreature(Creature *creature, const Creature* creatureMoving,
	const Tile *fromTile, const Tile *toTile)
{
	const Player* playerMoving = dynamic_cast<const Player*>(creatureMoving);
	Player* player = dynamic_cast<Player*>(creature);

	if (creature->access < g_config.ACCESS_PROTECT && creature != creatureMoving && !creatureMoving->isPushable()) {
		creature->sendCancel("Sorry, not possible.");
    return false;
  }
	if(!toTile){
		if(creature == creatureMoving && player) {
			player->sendTextMessage(MSG_SMALLINFO, "Sorry, not possible.");
			player->sendCancelWalk();
		}

		return false;
	}
  else if (playerMoving && toTile->isPz() && playerMoving->pzLocked) {
		if (creature == creatureMoving && creature->pzLocked) {

			if(player) {
				player->sendTextMessage(MSG_SMALLINFO, "You can not enter a protection zone after attacking another player.");
				player->sendCancelWalk();
			}

			//player->sendCancelWalk("You can not enter a protection zone after attacking another player.");
			return false;
		}
		else if (playerMoving->pzLocked) {
			creature->sendCancel("Sorry, not possible.");
			return false;
		}
  }
  else if (playerMoving && fromTile->isPz() && !toTile->isPz() && creature != creatureMoving) {
		creature->sendCancel("Sorry, not possible.");
		return false;
  }
	else if(creature != creatureMoving && toTile->floorChange()){
		creature->sendCancel("Sorry, not possible.");
		return false;
	}
  else if(creature != creatureMoving && toTile->getTeleportItem()){
		creature->sendCancel("Sorry, not possible.");
		return false;
  }

	//Fields
if(playerMoving && player != playerMoving) {
MagicEffectItem* fieldItem = NULL;
for (ItemVector::const_iterator iit = toTile->downItems.begin(); iit != toTile->downItems.end(); ++iit)
{
fieldItem = dynamic_cast<MagicEffectItem*>(*iit);
if(fieldItem && (fieldItem->getID() == FIRE_FIELD || fieldItem->getID() == FIRE_FIELD2 || fieldItem->getID() == FIRE_FIELD3 || fieldItem->getID() == 1492 || fieldItem->getID() == 1493 || fieldItem->getID() == 1494 || fieldItem->getID() == 1490 || fieldItem->getID() == 1496 || fieldItem->getID() == 1491 || fieldItem->getID() == 1495)) {
creature->sendCancel("Sorry, not possible.");
return false;
}
}
}
     
	return true;
}

/*ground -> inventory*/
bool Game::onPrepareMoveThing(Player *player, const Position& fromPos, const Item *item,
	slots_t toSlot, int count)
{
	if (player->access < g_config.ACCESS_REMOTE &&  
		((abs(player->pos.x - fromPos.x) > 1) || (abs(player->pos.y - fromPos.y) > 1) || (player->pos.z != fromPos.z))) 
	{
		player->sendCancel("Too far away...");
		return false;
	}
	else if(!item->isPickupable() && player->access < g_config.ACCESS_REMOTE) {
		player->sendCancel("You cannot move this object.");
		return false;
	}

	double itemWeight = (item->isStackable() ? Item::items[item->getID()].weight * std::max(1, count) : item->getWeight());
	if(player->access < g_config.ACCESS_PROTECT && player->getFreeCapacity() < itemWeight) {
		player->sendCancel("This object is too heavy.");
		return false;
	}

	return true;
}

/*inventory -> inventory*/
bool Game::onPrepareMoveThing(Player *player, slots_t fromSlot, const Item *fromItem,
	slots_t toSlot, const Item *toItem, int count)
{
	if(toItem && (!toItem->isStackable() || toItem->getID() != fromItem->getID())) {
		player->sendCancel("Sorry, not enough room.");
		return false;
	}

	return true;
}

/*container -> inventory*/
bool Game::onPrepareMoveThing(Player *player, const Container *fromContainer, const Item *fromItem,
	slots_t toSlot, const Item *toItem, int count)
{
    /*
	if(toItem && (!toItem->isStackable() || toItem->getID() != fromItem->getID())) {
		player->sendCancel("Sorry, not enough room.");
		return false;
	}
	*/

	double itemWeight = (fromItem->isStackable() ? Item::items[fromItem->getID()].weight * std::max(1, count) : fromItem->getWeight());
	if(player->access < g_config.ACCESS_PROTECT && !player->isHoldingContainer(fromContainer) &&
		player->getFreeCapacity() < itemWeight) {
		player->sendCancel("This object is too heavy.");
		return false;
	}

	return true;
}

/*->inventory*/
bool Game::onPrepareMoveThing(Player *player, const Item *item,
	slots_t toSlot, int count)
{
	switch(toSlot)
	{
	case SLOT_HEAD:
		if(item->getSlotPosition() & SLOTP_HEAD)
			return true;
		break;
	case SLOT_NECKLACE:
		if(item->getSlotPosition() & SLOTP_NECKLACE)
			return true;
		break;
	case SLOT_BACKPACK:
		if(item->getSlotPosition() & SLOTP_BACKPACK)
			return true;
		break;
	case SLOT_ARMOR:
		if(item->getSlotPosition() & SLOTP_ARMOR)
			return true;
		break;
	case SLOT_RIGHT:
		if(item->getSlotPosition() & SLOTP_RIGHT){
			if(item->getSlotPosition() & SLOTP_TWO_HAND){
				if(player->items[SLOT_LEFT] != NULL){
					player->sendCancel("First remove the two-handed item.");
					return false;
				}
				return true	;
			}
			else{
				if(player->items[SLOT_LEFT]){
					if(player->items[SLOT_LEFT]->getSlotPosition() & SLOTP_TWO_HAND){
						player->sendCancel("First remove the two-handed item.");
						return false;
					}
					return true;
				}
				return true;
			}
		}
		break;
	case SLOT_LEFT:
		if(item->getSlotPosition() & SLOTP_LEFT){
			if(item->getSlotPosition() & SLOTP_TWO_HAND){
				if(player->items[SLOT_RIGHT] != NULL){
					player->sendCancel("First remove the two-handed item.");
					return false;
				}
				return true	;
			}
			else{
				if(player->items[SLOT_RIGHT]){
					if(player->items[SLOT_RIGHT]->getSlotPosition() & SLOTP_TWO_HAND){
						player->sendCancel("First remove the two-handed item.");
						return false;
					}
					return true;
				}
				return true;
			}
		}
		break;
	case SLOT_LEGS:
		if(item->getSlotPosition() & SLOTP_LEGS)
			return true;
		break;
	case SLOT_FEET:
		if(item->getSlotPosition() & SLOTP_FEET)
			return true;
		break;
	case SLOT_RING:
		if(item->getSlotPosition() & SLOTP_RING)
			return true;
		break;
	case SLOT_AMMO:
		if(item->getSlotPosition() & SLOTP_AMMO)
			return true;
		break;
	}

	player->sendCancel("You cannot put that object in that place.");
	return false;
}

//container/inventory to container/inventory
void Game::thingMoveInternal(Player *player,
	unsigned char from_cid, unsigned char from_slotid, unsigned short itemid,
	bool fromInventory,unsigned char to_cid, unsigned char to_slotid, bool toInventory,
	unsigned char count)
{
	Container *fromContainer = NULL;
	Container *toContainer = NULL;
	Item *fromItem = NULL;
	Item *toItem = NULL;

	if(fromInventory) {
		fromItem = player->getItem(from_cid);
		fromContainer = dynamic_cast<Container *>(fromItem);
	}
	else {
		fromContainer = player->getContainer(from_cid);

		if(fromContainer) {
			if(from_slotid >= fromContainer->size())
				return;

			fromItem = fromContainer->getItem(from_slotid);
		}
	}

	if(toInventory) {
		toItem = player->getItem(to_cid);
		toContainer = dynamic_cast<Container *>(toItem);
	}
	else {
		toContainer = player->getContainer(to_cid);

		if(toContainer) {
			if(to_slotid >= toContainer->capacity())
				return;

			toItem = toContainer->getItem(to_slotid);
			Container *toSlotContainer = dynamic_cast<Container*>(toItem);
			if(toSlotContainer) {
				toContainer = toSlotContainer;
				toItem = NULL;
			}
		}
	}

	if(!fromItem || (toItem == fromItem) || (fromItem->isStackable() && count > fromItem->getItemCountOrSubtype()) || fromItem->getID() != itemid)
		return;

	//Container to container
	if(!fromInventory && fromContainer && toContainer) {
		Position fromPos = (fromContainer->pos.x == 0xFFFF ? player->pos : fromContainer->pos);
		Position toPos = (toContainer->pos.x == 0xFFFF ? player->pos : toContainer->pos);

		if(onPrepareMoveThing(player, fromPos, fromContainer, fromItem, toPos, toContainer, toItem, count)) {

			autoCloseTrade(fromItem, true);
			int oldFromCount = fromItem->getItemCountOrSubtype();
			int oldToCount = 0;

			//move around an item in a container
			if(fromItem->isStackable()) {
				if(toItem && toItem != fromItem && toItem->getID() == fromItem->getID())
				{
					oldToCount = toItem->getItemCountOrSubtype();
					int newToCount = std::min(100, oldToCount + count);
					toItem->setItemCountOrSubtype(newToCount);

					if(oldToCount != newToCount) {
						autoCloseTrade(toItem);
					}

					int subcount = oldFromCount - count;
					fromItem->setItemCountOrSubtype(subcount);

					int surplusCount = oldToCount + count  - 100;
					if(surplusCount > 0) {
						Item *surplusItem = Item::CreateItem(fromItem->getID(), surplusCount);
						if(onPrepareMoveThing(player, fromPos, fromContainer, surplusItem, toPos, toContainer, NULL, count)) {
							autoCloseTrade(toContainer);
							toContainer->addItem(surplusItem);
						}
						else {
							delete surplusItem;
							count -= surplusCount; //re-define the actual amount we move.
							fromItem->setItemCountOrSubtype(fromItem->getItemCountOrSubtype() + surplusCount);
						}
					}
				}
				else if(count < oldFromCount) {
					int subcount = oldFromCount - count;
					fromItem->setItemCountOrSubtype(subcount);

					Item* moveItem = Item::CreateItem(fromItem->getID(), count);
					toContainer->addItem(moveItem);
					autoCloseTrade(toContainer);
				}
				else {
					if(fromContainer == toContainer) {
						fromContainer->moveItem(from_slotid, 0);
					}
					else if(fromContainer->removeItem(fromItem)) {
						autoCloseTrade(toContainer);
						toContainer->addItem(fromItem);
					}
				}

				if(fromItem->getItemCountOrSubtype() == 0) {
					fromContainer->removeItem(fromItem);
					this->FreeThing(fromItem);
				}
			}
			else {
				if(fromContainer == toContainer) {
					fromContainer->moveItem(from_slotid, 0);
				}
				else if(fromContainer->removeItem(fromItem)) {
					autoCloseTrade(toContainer);
					toContainer->addItem(fromItem);
				}
			}

			if(player->isHoldingContainer(fromContainer) != player->isHoldingContainer(toContainer)) {
				player->updateInventoryWeigth();
			}

			SpectatorVec list;
			SpectatorVec::iterator it;

			if(fromPos == toPos) {
				getSpectators(Range(fromPos, false), list);
			}
			else {
				getSpectators(Range(fromPos, toPos), list);
			}

			if(!list.empty()) {
				//players
				for(it = list.begin(); it != list.end(); ++it) {
					if(dynamic_cast<Player*>(*it)) {
						(*it)->onThingMove(player, fromContainer, from_slotid, fromItem, oldFromCount, toContainer, to_slotid, toItem, oldToCount, count);
					}
				}

				//none-players
				for(it = list.begin(); it != list.end(); ++it) {
					if(!dynamic_cast<Player*>(*it)) {
						(*it)->onThingMove(player, fromContainer, from_slotid, fromItem, oldFromCount, toContainer, to_slotid, toItem, oldToCount, count);
					}
				}
			}
			else
				player->onThingMove(player, fromContainer, from_slotid, fromItem, oldFromCount, toContainer, to_slotid, toItem, oldToCount, count);
		}
	}
	else {
		//inventory to inventory
		if(fromInventory && toInventory && !toContainer) {
			Position fromPos = player->pos;
			Position toPos = player->pos;
			if(onPrepareMoveThing(player, fromItem, (slots_t)to_cid, count) && onPrepareMoveThing(player, (slots_t)from_cid, fromItem, (slots_t)to_cid, toItem, count)) {

				autoCloseTrade(fromItem, true);
				int oldFromCount = fromItem->getItemCountOrSubtype();
				int oldToCount = 0;

				if(fromItem->isStackable()) {
					if(toItem && toItem != fromItem && toItem->getID() == fromItem->getID())
					{
						oldToCount = toItem->getItemCountOrSubtype();
						int newToCount = std::min(100, oldToCount + count);
						toItem->setItemCountOrSubtype(newToCount);

						if(oldToCount != newToCount) {
							autoCloseTrade(toItem);
						}

						int subcount = oldFromCount - count;
						fromItem->setItemCountOrSubtype(subcount);

						int surplusCount = oldToCount + count  - 100;
						if(surplusCount > 0) {
							fromItem->setItemCountOrSubtype(fromItem->getItemCountOrSubtype() + surplusCount);
							player->sendCancel("Sorry not enough room.");
						}

						if(fromItem->getItemCountOrSubtype() == 0) {
							player->removeItemInventory(from_cid, true);
							this->FreeThing(fromItem);
						}
					}
					else if(count < oldFromCount) {
						int subcount = oldFromCount - count;
						fromItem->setItemCountOrSubtype(subcount);

						autoCloseTrade(toItem);
						player->removeItemInventory(to_cid, true);
						player->addItemInventory(Item::CreateItem(fromItem->getID(), count), to_cid, true);

						if(fromItem->getItemCountOrSubtype() == 0) {
							player->removeItemInventory(from_cid, true);
							this->FreeThing(fromItem);
						}
					}
					else {
						if(player->removeItemInventory(from_cid, true)) {
							player->removeItemInventory(to_cid, true);
							player->addItemInventory(fromItem, to_cid, true);
						}
					}
				}
				else if(player->removeItemInventory(from_cid, true)) {
					player->removeItemInventory(to_cid, true);
					player->addItemInventory(fromItem, to_cid, true);
				}

				player->onThingMove(player, (slots_t)from_cid, fromItem, oldFromCount, (slots_t)to_cid, toItem, oldToCount, count);
			}
		}
		//container to inventory
		else if(!fromInventory && fromContainer && toInventory) {
			if(onPrepareMoveThing(player, fromItem, (slots_t)to_cid, count) && onPrepareMoveThing(player, fromContainer, fromItem, (slots_t)to_cid, toItem, count)) {
				autoCloseTrade(fromItem, true);
				int oldFromCount = fromItem->getItemCountOrSubtype();
				int oldToCount = 0;

				if(fromItem->isStackable()) {
					if(toItem && toItem != fromItem && toItem->getID() == fromItem->getID())
					{
						oldToCount = toItem->getItemCountOrSubtype();
						int newToCount = std::min(100, oldToCount + count);
						toItem->setItemCountOrSubtype(newToCount);

						if(oldToCount != newToCount) {
							autoCloseTrade(toItem);
						}

						int subcount = oldFromCount - count;
						fromItem->setItemCountOrSubtype(subcount);

						int surplusCount = oldToCount + count  - 100;
						if(surplusCount > 0) {
							fromItem->setItemCountOrSubtype(fromItem->getItemCountOrSubtype() + surplusCount);
							player->sendCancel("Sorry not enough room.");
						}

						if(fromItem->getItemCountOrSubtype() == 0) {
							fromContainer->removeItem(fromItem);
							this->FreeThing(fromItem);
						}
					}
					else if(count < oldFromCount) {
						int subcount = oldFromCount - count;
						fromItem->setItemCountOrSubtype(subcount);

						player->removeItemInventory(to_cid, true);
						player->addItemInventory(Item::CreateItem(fromItem->getID(), count), to_cid, true);

						if(toItem) {
							fromContainer->addItem(toItem);
						}

						if(fromItem->getItemCountOrSubtype() == 0) {
							fromContainer->removeItem(fromItem);
							this->FreeThing(fromItem);
						}
					}
					else {
						if(fromContainer->removeItem(fromItem)) {
							player->removeItemInventory(to_cid, true);
							player->addItemInventory(fromItem, to_cid, true);

							if(toItem) {
								fromContainer->addItem(toItem);
							}
						}
					}
				}
				else if(fromContainer->removeItem(fromItem)) {
					player->removeItemInventory(to_cid, true);
					player->addItemInventory(fromItem, to_cid, true);

					if(toItem) {
						fromContainer->addItem(toItem);
					}
				}

				if(!player->isHoldingContainer(fromContainer)) {
					player->updateInventoryWeigth();
				}

				//if(fromContainer->pos.x != 0xFFFF) {
				if(fromContainer->getTopParent()->pos.x != 0xFFFF) {
					SpectatorVec list;
					SpectatorVec::iterator it;

					getSpectators(Range(fromContainer->getTopParent()->pos, false), list);

					//players
					for(it = list.begin(); it != list.end(); ++it) {
						if(dynamic_cast<Player*>(*it)) {
							(*it)->onThingMove(player, fromContainer, from_slotid, fromItem, oldFromCount, (slots_t)to_cid, toItem, oldToCount, count);
						}
					}

					//none-players
					for(it = list.begin(); it != list.end(); ++it) {
						if(!dynamic_cast<Player*>(*it)) {
							(*it)->onThingMove(player, fromContainer, from_slotid, fromItem, oldFromCount, (slots_t)to_cid, toItem, oldToCount, count);
						}
					}
				}
				else
					player->onThingMove(player, fromContainer, from_slotid, fromItem, oldFromCount, (slots_t)to_cid, toItem, oldToCount, count);
			}
		}
		//inventory to container
		else if(fromInventory && toContainer) {
			Position fromPos = player->pos;
			Position toPos = (toContainer->pos.x == 0xFFFF ? player->pos : toContainer->pos);

			int oldFromCount = fromItem->getItemCountOrSubtype();
			int oldToCount = 0;

			if(onPrepareMoveThing(player, fromItem, (slots_t)from_cid, toContainer, toItem, count)) {
				autoCloseTrade(fromItem, true);
				if(fromItem->isStackable()) {
					if(toItem && toItem != fromItem && toItem->getID() == fromItem->getID())
					{
						oldToCount = toItem->getItemCountOrSubtype();
						int newToCount = std::min(100, oldToCount + count);
						toItem->setItemCountOrSubtype(newToCount);

						if(oldToCount != newToCount) {
							autoCloseTrade(toItem);
						}

						int subcount = oldFromCount - count;
						fromItem->setItemCountOrSubtype(subcount);

						int surplusCount = oldToCount + count  - 100;
						if(surplusCount > 0) {
							Item *surplusItem = Item::CreateItem(fromItem->getID(), surplusCount);

							if(onPrepareMoveThing(player, fromPos, NULL, surplusItem, toPos, toContainer, NULL, count)) {
								autoCloseTrade(toContainer);
								toContainer->addItem(surplusItem);
							}
							else {
								delete surplusItem;
								count -= surplusCount; //re-define the actual amount we move.
								fromItem->setItemCountOrSubtype(fromItem->getItemCountOrSubtype() + surplusCount);
							}
						}
					}
					else if(count < oldFromCount) {
						int subcount = oldFromCount - count;
						fromItem->setItemCountOrSubtype(subcount);
						autoCloseTrade(toContainer);
						toContainer->addItem(Item::CreateItem(fromItem->getID(), count));
					}
					else {
						if(player->removeItemInventory((slots_t)from_cid, true)) {
							autoCloseTrade(toContainer);
							toContainer->addItem(fromItem);

							if(player->isHoldingContainer(toContainer)) {
								player->updateInventoryWeigth();
							}
						}
					}

					if(fromItem->getItemCountOrSubtype() == 0) {
						player->removeItemInventory(from_cid, true);
						this->FreeThing(fromItem);
					}
				}
				else if(player->removeItemInventory(from_cid, true)) {
					autoCloseTrade(toContainer);
					toContainer->addItem(fromItem);

					if(player->isHoldingContainer(toContainer)) {
						player->updateInventoryWeigth();
					}
				}

				if(!player->isHoldingContainer(toContainer)) {
					player->updateInventoryWeigth();
				}

				if(toContainer->getTopParent()->pos.x != 0xFFFF) {
					SpectatorVec list;
					SpectatorVec::iterator it;

					getSpectators(Range(toContainer->getTopParent()->pos, false), list);

					//players
					for(it = list.begin(); it != list.end(); ++it) {
						if(dynamic_cast<Player*>(*it)) {
							(*it)->onThingMove(player, (slots_t)from_cid, fromItem, oldFromCount, toContainer, to_slotid, toItem, oldToCount, count);
						}
					}

					//none-players
					for(it = list.begin(); it != list.end(); ++it) {
						if(!dynamic_cast<Player*>(*it)) {
							(*it)->onThingMove(player, (slots_t)from_cid, fromItem, oldFromCount, toContainer, to_slotid, toItem, oldToCount, count);
						}
					}
				}
				else
					player->onThingMove(player, (slots_t)from_cid, fromItem, oldFromCount, toContainer, to_slotid, toItem, oldToCount, count);
			}
		}
	}
}

// Move Up
void Game::thingMoveInternal(Player *player, unsigned char from_cid, unsigned char from_slotid)
{
 Container *fromContainer = NULL;
 Container *toContainer = NULL;
 Item *fromItem = NULL;
    
    fromContainer = player->getContainer(from_cid);
    
    if(!fromContainer)
        return;
        
    if(fromContainer->getParent() == NULL)
        return;
    
 toContainer = fromContainer->getParent();
 fromItem = fromContainer->getItem(from_slotid);
 
 if(!toContainer || !fromItem)
     return;
 
 if(toContainer->size() + 1 > toContainer->capacity()) {  
  player->sendCancel("Sorry not enough room.");
  return;
 }
 
 fromContainer->removeItem(fromItem);
 toContainer->addItem(fromItem);
 
 Position fromPos = (fromContainer->pos.x == 0xFFFF ? player->pos : fromContainer->pos);
 Position toPos = (toContainer->pos.x == 0xFFFF ? player->pos : toContainer->pos);
 
 SpectatorVec list;
 SpectatorVec::iterator it;
    if(fromPos == toPos) {
  getSpectators(Range(fromPos, false), list);
 }
 else {
  getSpectators(Range(fromPos, toPos), list);
 }
 if(!list.empty()) {
     //players
     for(it = list.begin(); it != list.end(); ++it) {
      if(dynamic_cast<Player*>(*it)) {
       (*it)->onThingMove(player, fromContainer, from_slotid, fromItem, toContainer);
      }
     }
     //none-players
     for(it = list.begin(); it != list.end(); ++it) {
         if(!dynamic_cast<Player*>(*it)) {
              (*it)->onThingMove(player, fromContainer, from_slotid, fromItem, toContainer);
         }
     }
 }
 else
 player->onThingMove(player, fromContainer, from_slotid, fromItem, toContainer);
}

//container/inventory to ground
void Game::thingMoveInternal(Player *player,
	unsigned char from_cid, unsigned char from_slotid, unsigned short itemid, bool fromInventory,
	const Position& toPos, unsigned char count)
{
	Container *fromContainer = NULL;
	Tile *toTile = map->getTile(toPos);
	if(!toTile)
		return;

	/*container to ground*/
	if(!fromInventory) {
		fromContainer = player->getContainer(from_cid);
		if(!fromContainer)
			return;

		Position fromPos = (fromContainer->pos.x == 0xFFFF ? player->pos : fromContainer->pos);
		Item *fromItem = dynamic_cast<Item*>(fromContainer->getItem(from_slotid));
		Item* toItem = dynamic_cast<Item*>(toTile->getTopDownItem());

		if(!fromItem || (toItem == fromItem) || (fromItem->isStackable() && count > fromItem->getItemCountOrSubtype()) || fromItem->getID() != itemid)
			return;

		if(onPrepareMoveThing(player, fromItem, fromPos, toPos, count) && onPrepareMoveThing(player, fromItem, NULL, toTile, count)) {
			autoCloseTrade(fromItem, true);
			int oldFromCount = fromItem->getItemCountOrSubtype();
			int oldToCount = 0;

			//Do action...
#ifdef TP_TRASH_BINS
			if (toItem && toItem->isDeleter())
			{
				fromContainer->removeItem(fromItem);
				FreeThing(fromItem);
			}
			else if(fromItem->isStackable()) {
#else
			if(fromItem->isStackable()) {
#endif //TP_TRASH_BINS
				if(toItem && toItem->getID() == fromItem->getID())
				{
					oldToCount = toItem->getItemCountOrSubtype();
					int newToCount = std::min(100, oldToCount + count);
					toItem->setItemCountOrSubtype(newToCount);

					if(oldToCount != newToCount) {
						autoCloseTrade(toItem);
					}

					int subcount = oldFromCount - count;
					fromItem->setItemCountOrSubtype(subcount);

					int surplusCount = oldToCount + count  - 100;
					if(surplusCount > 0) {
						Item *surplusItem = Item::CreateItem(fromItem->getID(), surplusCount);
						surplusItem->pos = toPos;

						toTile->addThing(surplusItem);
					}

					if(fromItem->getItemCountOrSubtype() == 0) {
						fromContainer->removeItem(fromItem);
						this->FreeThing(fromItem);
					}
				}
				else if(count < oldFromCount) {
					int subcount = oldFromCount - count;
					fromItem->setItemCountOrSubtype(subcount);

					Item *moveItem = Item::CreateItem(fromItem->getID(), count);
					moveItem->pos = toPos;
					toTile->addThing(moveItem);

					if(fromItem->getItemCountOrSubtype() == 0) {
						fromContainer->removeItem(fromItem);
						this->FreeThing(fromItem);
					}
				}
				else if(fromContainer->removeItem(fromItem)) {
					fromItem->pos = toPos;
					toTile->addThing(fromItem);
				}
			}
			else if(fromContainer->removeItem(fromItem)) {
				fromItem->pos = toPos;
				toTile->addThing(fromItem);
			}

			if(player->isHoldingContainer(fromContainer)) {
				player->updateInventoryWeigth();
			}

			SpectatorVec list;
			SpectatorVec::iterator it;

			getSpectators(Range(fromPos, false), list);

			SpectatorVec tolist;
			getSpectators(Range(toPos, true), tolist);

			for(it = tolist.begin(); it != tolist.end(); ++it) {
				if(std::find(list.begin(), list.end(), *it) == list.end()) {
					list.push_back(*it);
				}
			}

			//players
			for(it = list.begin(); it != list.end(); ++it) {
				if(dynamic_cast<Player*>(*it)) {
					(*it)->onThingMove(player, fromContainer, from_slotid, fromItem, oldFromCount, toPos, toItem, oldToCount, count);
				}
			}

			//none-players
			for(it = list.begin(); it != list.end(); ++it) {
				if(!dynamic_cast<Player*>(*it)) {
					(*it)->onThingMove(player, fromContainer, from_slotid, fromItem, oldFromCount, toPos, toItem, oldToCount, count);
				}
			}
		}
	}
	else /*inventory to ground*/{
		Item *fromItem = player->getItem(from_cid);
		if(!fromItem || (fromItem->isStackable() && count > fromItem->getItemCountOrSubtype()) || fromItem->getID() != itemid)
			return;

		if(onPrepareMoveThing(player, fromItem, player->pos, toPos, count) && onPrepareMoveThing(player, fromItem, NULL, toTile, count)) {
			autoCloseTrade(fromItem, true);
			Item* toItem = dynamic_cast<Item*>(toTile->getTopDownItem());
			int oldFromCount = fromItem->getItemCountOrSubtype();
			int oldToCount = 0;

			//Do action...
#ifdef TP_TRASH_BINS
			if(toItem && toItem->isDeleter())
			{
				player->removeItemInventory(from_cid, true);
				FreeThing(fromItem);
			}
			else if(fromItem->isStackable()) {
#else
			if(fromItem->isStackable()) {
#endif //TP_TRASH_BINS
				if(toItem && toItem->getID() == fromItem->getID())
				{
					oldToCount = toItem->getItemCountOrSubtype();
					int newToCount = std::min(100, oldToCount + count);
					toItem->setItemCountOrSubtype(newToCount);

					if(oldToCount != newToCount) {
						autoCloseTrade(toItem);
					}

					int subcount = oldFromCount - count;
					fromItem->setItemCountOrSubtype(subcount);

					int surplusCount = oldToCount + count  - 100;
					if(surplusCount > 0) {
						Item *surplusItem = Item::CreateItem(fromItem->getID(), surplusCount);
						surplusItem->pos = toPos;

						toTile->addThing(surplusItem);
					}

					if(fromItem->getItemCountOrSubtype() == 0) {
						player->removeItemInventory(from_cid, true);
						this->FreeThing(fromItem);
					}
				}
				else if(count < oldFromCount) {
					int subcount = oldFromCount - count;
					fromItem->setItemCountOrSubtype(subcount);

					Item *moveItem = Item::CreateItem(fromItem->getID(), count);
					moveItem->pos = toPos;
					toTile->addThing(moveItem);

					if(fromItem->getItemCountOrSubtype() == 0) {
						player->removeItemInventory(from_cid, true);
						this->FreeThing(fromItem);
					}
				}
				else if(player->removeItemInventory(from_cid, true)) {
					fromItem->pos = toPos;
					toTile->addThing(fromItem);
				}
			}
			else if(player->removeItemInventory(from_cid, true)) {
				fromItem->pos = toPos;
				toTile->addThing(fromItem);
			}

			player->updateInventoryWeigth();

			SpectatorVec list;
			SpectatorVec::iterator it;

			getSpectators(Range(player->pos, false), list);

			SpectatorVec tolist;
			getSpectators(Range(toPos, true), tolist);

			for(it = tolist.begin(); it != tolist.end(); ++it) {
				if(std::find(list.begin(), list.end(), *it) == list.end()) {
					list.push_back(*it);
				}
			}

			//players
			for(it = list.begin(); it != list.end(); ++it) {
				if(dynamic_cast<Player*>(*it)) {
					(*it)->onThingMove(player, (slots_t)from_cid, fromItem, oldFromCount, toPos, toItem, oldToCount, count);
				}
			}

			//none-players
			for(it = list.begin(); it != list.end(); ++it) {
				if(!dynamic_cast<Player*>(*it)) {
					(*it)->onThingMove(player, (slots_t)from_cid, fromItem, oldFromCount, toPos, toItem, oldToCount, count);
				}
			}
		}
	}

#ifdef TP_TRASH_BINS
	//creatureBroadcastTileUpdated(toPos);
	//sendAddThing
#endif //TP_TRASH_BINS
}

//ground to container/inventory
void Game::thingMoveInternal(Player *player, const Position& fromPos, unsigned char stackPos,
	unsigned short itemid, unsigned char to_cid, unsigned char to_slotid, bool toInventory, unsigned char count)
{
	Tile *fromTile = map->getTile(fromPos);
	if(!fromTile)
		return;

	Container *toContainer = NULL;

	Item* fromItem = dynamic_cast<Item*>(fromTile->getTopDownItem());
	Item *toItem = NULL;

	if(!fromItem || (fromItem->getID() != itemid) || (fromItem != fromTile->getTopDownItem()))
		return;

	if(toInventory) {
		toItem = player->getItem(to_cid);
		toContainer = dynamic_cast<Container*>(toItem);
	}
	else {
		toContainer = player->getContainer(to_cid);
		if(!toContainer)
			return;

		toItem = toContainer->getItem(to_slotid);
		Container *toSlotContainer = dynamic_cast<Container*>(toItem);

		if(toSlotContainer) {
			toContainer = toSlotContainer;
			toItem = NULL;
		}
	}

	if((toItem == fromItem) || (fromItem->isStackable() && count > fromItem->getItemCountOrSubtype()))
		return;

	/*ground to container*/
	if(toContainer) {
		/*if(onPrepareMoveThing(player, fromItem, fromPos, player->pos, count) &&
				onPrepareMoveThing(player, fromItem, NULL, toContainer, toItem, count))*/

		Position toPos = (toContainer->pos.x == 0xFFFF ? player->pos : toContainer->pos);
		if(onPrepareMoveThing(player, fromPos, NULL, fromItem, toPos, toContainer, toItem, count)) {
			autoCloseTrade(fromItem, true);
			int oldFromCount = fromItem->getItemCountOrSubtype();
			int oldToCount = 0;
			int stackpos = fromTile->getThingStackPos(fromItem);

			if(fromItem->isStackable()) {
				if(toItem && toItem->getID() == fromItem->getID()) {
					oldToCount = toItem->getItemCountOrSubtype();
					int newToCount = std::min(100, oldToCount + count);
					toItem->setItemCountOrSubtype(newToCount);

					if(oldToCount != newToCount) {
						autoCloseTrade(toItem);
					}

					int subcount = oldFromCount - count;
					fromItem->setItemCountOrSubtype(subcount);

					int surplusCount = oldToCount + count  - 100;
					if(surplusCount > 0) {
						Item *surplusItem = Item::CreateItem(fromItem->getID(), surplusCount);

						if(onPrepareMoveThing(player, fromPos, NULL, surplusItem, toPos, toContainer, NULL, count)) {
							autoCloseTrade(toContainer);
							toContainer->addItem(surplusItem);
						}
						else {
							delete surplusItem;
							count -= surplusCount; //re-define the actual amount we move.
							fromItem->setItemCountOrSubtype(fromItem->getItemCountOrSubtype() + surplusCount);
						}
					}

					if(fromItem->getItemCountOrSubtype() == 0) {
						if(fromTile->removeThing(fromItem)){
							this->FreeThing(fromItem);
						}
					}
				}
				else if(count < oldFromCount) {
					int subcount = oldFromCount - count;
					fromItem->setItemCountOrSubtype(subcount);

					autoCloseTrade(toContainer);
					toContainer->addItem(Item::CreateItem(fromItem->getID(), count));

					if(fromItem->getItemCountOrSubtype() == 0) {
						if(fromTile->removeThing(fromItem)){
							this->FreeThing(fromItem);
						}
					}
				}
				else if(fromTile->removeThing(fromItem)) {
					autoCloseTrade(toContainer);
					toContainer->addItem(fromItem);
				}
			}
			else {
				if(fromTile->removeThing(fromItem)) {
					autoCloseTrade(toContainer);
					toContainer->addItem(fromItem);
				}
			}

			if(player->isHoldingContainer(toContainer)) {
				player->updateInventoryWeigth();
			}

			SpectatorVec list;
			SpectatorVec::iterator it;

			getSpectators(Range(fromPos, true), list);

			//players
			for(it = list.begin(); it != list.end(); ++it) {
				if(dynamic_cast<Player*>(*it)) {
					(*it)->onThingMove(player, fromPos, stackpos, fromItem, oldFromCount, toContainer, to_slotid, toItem, oldToCount, count);
				}
			}

			//none-players
			for(it = list.begin(); it != list.end(); ++it) {
				if(!dynamic_cast<Player*>(*it)) {
					(*it)->onThingMove(player, fromPos, stackpos, fromItem, oldFromCount, toContainer, to_slotid, toItem, oldToCount, count);
				}
			}
		}
	}
	//ground to inventory
	else if(toInventory) {
		if(onPrepareMoveThing(player, fromPos, fromItem, (slots_t)to_cid, count) && onPrepareMoveThing(player, fromItem, (slots_t)to_cid, count)) {
			autoCloseTrade(fromItem, true);
			int oldFromCount = fromItem->getItemCountOrSubtype();
			int oldToCount = 0;
			int stackpos = fromTile->getThingStackPos(fromItem);

			if(fromItem->isStackable()) {
				if(toItem && toItem->getID() == fromItem->getID()) {
					oldToCount = toItem->getItemCountOrSubtype();
					int newToCount = std::min(100, oldToCount + count);
					toItem->setItemCountOrSubtype(newToCount);

					if(oldToCount != newToCount) {
						autoCloseTrade(toItem);
					}

					int subcount = oldFromCount - count;
					fromItem->setItemCountOrSubtype(subcount);

					int surplusCount = oldToCount + count  - 100;
					if(surplusCount > 0) {
						fromItem->setItemCountOrSubtype(fromItem->getItemCountOrSubtype() + surplusCount);
						player->sendCancel("Sorry not enough room.");
					}

					if(fromItem->getItemCountOrSubtype() == 0) {
						if(fromTile->removeThing(fromItem)){
							this->FreeThing(fromItem);
						}
					}
				}
				else if(count < oldFromCount) {
					int subcount = oldFromCount - count;
					fromItem->setItemCountOrSubtype(subcount);

					player->removeItemInventory(to_cid, true);
					player->addItemInventory(Item::CreateItem(fromItem->getID(), count), to_cid, true);

					if(toItem) {
						autoCloseTrade(toItem);
						fromTile->addThing(toItem);
						toItem->pos = fromPos;
					}

					if(fromItem->getItemCountOrSubtype() == 0) {
						if(fromTile->removeThing(fromItem)){
							this->FreeThing(fromItem);
						}
					}
				}
				else {
					if(fromTile->removeThing(fromItem)) {
						player->removeItemInventory(to_cid, true);
						player->addItemInventory(fromItem, to_cid, true);

						if(toItem) {
							autoCloseTrade(toItem);
							fromTile->addThing(toItem);
							toItem->pos = fromPos;
						}
					}
				}
			}
			else {
				if(fromTile->removeThing(fromItem)) {
					player->removeItemInventory(to_cid, true);
					player->addItemInventory(fromItem, to_cid, true);

					if(toItem) {
						autoCloseTrade(toItem);
						fromTile->addThing(toItem);
						toItem->pos = fromPos;
					}
				}
			}

			player->updateInventoryWeigth();

			SpectatorVec list;
			SpectatorVec::iterator it;

			getSpectators(Range(fromPos, true), list);

			//players
			for(it = list.begin(); it != list.end(); ++it) {
				if(dynamic_cast<Player*>(*it)) {
					(*it)->onThingMove(player, fromPos, stackpos, fromItem, oldFromCount, (slots_t)to_cid, toItem, oldToCount, count);
				}
			}

			//none-players
			for(it = list.begin(); it != list.end(); ++it) {
				if(!dynamic_cast<Player*>(*it)) {
					(*it)->onThingMove(player, fromPos, stackpos, fromItem, oldFromCount, (slots_t)to_cid, toItem, oldToCount, count);
				}
			}
		}
	}
}

//ground to ground
void Game::thingMoveInternal(Creature *creature, unsigned short from_x, unsigned short from_y, unsigned char from_z,
	unsigned char stackPos, unsigned short itemid, unsigned short to_x, unsigned short to_y, unsigned char to_z, unsigned char count)
{
	Tile *fromTile = getTile(from_x, from_y, from_z);
	if(!fromTile)
		return;

	Tile *toTile   = getTile(to_x, to_y, to_z);
	/*
	if(!toTile){
		if(dynamic_cast<Player*>(player))
			dynamic_cast<Player*>(player)->sendCancelWalk("Sorry, not possible...");
		return;
	}
	*/

	Thing *thing = fromTile->getThingByStackPos(stackPos);

#ifdef __DEBUG__
				//				std::cout << "moving"
				/*
				<< ": from_x: "<< (int)from_x << ", from_y: "<< (int)from_y << ", from_z: "<< (int)from_z
				<< ", stackpos: "<< (int)stackPos
				<< ", to_x: "<< (int)to_x << ", to_y: "<< (int)to_y << ", to_z: "<< (int)to_z
				*/
				//<< std::endl;
#endif

	if (!thing)
		return;

	Item* item = dynamic_cast<Item*>(thing);
	Creature* creatureMoving = dynamic_cast<Creature*>(thing);
	Player* playerMoving = dynamic_cast<Player*>(creatureMoving);
	Player* player = dynamic_cast<Player*>(creature);


	Position oldPos;
	oldPos.x = from_x;
	oldPos.y = from_y;
	oldPos.z = from_z;

    #ifdef RULY_NOPUSHPARCEL
                                             if (toTile){
ItemVector::iterator brn;
for (brn = toTile->downItems.begin(); brn != toTile->downItems.end(); brn++)
{
if (playerMoving && (*brn)->getID() == 2595 && player != thing || playerMoving && (*brn)->getID() == 2596 && player != thing) 
{
player->sendCancel("Sorry not possible.");
return;
}
}  
}
#endif //RULY_NOPUSHPARCEL


    #ifdef RULY_ZITEM
    if(fromTile && playerMoving && player)//TODO: abs check
    {
        const Tile* down = getTile(to_x, to_y, to_z+1);
       if(down && down->zItem >= 3 && playerMoving->canMovedTo(down)) {
if(getTile(to_x, to_y, to_z) == NULL){
 teleport(playerMoving, Position(to_x, to_y, to_z+1));
            return;
}
}
        else if(toTile && toTile->zItem >= 3 && fromTile->zItem <= 1) {
            playerMoving->sendCancelWalk();
            player->sendCancel("Sorry, not possible.");
            return;
        }
        else if(toTile && toTile->zItem >= 2 && fromTile->zItem <= 0) {
             playerMoving->sendCancelWalk();
             player->sendCancel("Sorry, not possible.");
             return;
        }
        else if(fromTile->zItem >= 3 && player == playerMoving)
        {
                if(getTile(from_x, from_y, from_z-1) == NULL) //going up
                {
                    Tile* newTile = getTile(to_x, to_y, to_z-1);
                    if(newTile && newTile->ground && playerMoving->canMovedTo(newTile))
                    {
                        if(newTile->ground->getID() != 460 && newTile->ground->getID() != 459) {
                            teleport(playerMoving, Position(to_x, to_y, to_z-1));
                            return;
                        }
                    }
                }
        }
    }
#endif //RULY_ZITEM   

    
 if(toTile)
{
Thing *tothing = toTile->getThingByStackPos( stackPos );
Item *toItem = dynamic_cast<Item*>( tothing );
if( toItem && !playerMoving && !creatureMoving && toItem->getID() == 2494 )
{
switch( item->getID() )
{ 
//Undeleteable items
case 1774: //Barrel
case 1770: //Barrel
player->sendTextMessage(MSG_SMALLINFO, "Sorry, not possible.");
player->sendCancelWalk();
return;
break;
default:
removeThing( playerMoving, oldPos, item, true );
return;
break;
}
}
}
#ifdef TP_TRASH_BINS
	if(toTile)
	{
		Thing *tothing = toTile->getThingByStackPos(stackPos);
		Item *toItem = dynamic_cast<Item*>(tothing);

		if(item && toItem && !playerMoving && !creature && toItem->isDeleter())
		{
			fromTile->removeThing(item);
			this->FreeThing(item);
			//creatureBroadcastTileUpdated(oldPos);
			sendRemoveThing(player, item->pos, item, stackPos);
			return;
		}
    }
#endif //TP_TRASH_BINS

	// *** Creature moving itself to a non-tile
	if(!toTile && creatureMoving && creatureMoving == creature){
		//change level begin
		Tile* downTile = getTile(to_x, to_y, to_z+1);
		//diagonal begin
		if(!downTile)
		{
                         #ifdef RULY_NOPUSHFIELDS
                                                                  if (toTile){
ItemVector::iterator brn;
for (brn = toTile->downItems.begin(); brn != toTile->downItems.end(); brn++)
{
if (playerMoving && (*brn)->getID() == 1492 && player != thing || playerMoving && (*brn)->getID() == 1493 && player != thing || playerMoving && (*brn)->getID() == 1494 && player != thing || playerMoving && (*brn)->getID() == 1491 && player != thing || playerMoving && (*brn)->getID() == 1490 && player != thing || playerMoving && (*brn)->getID() == FIRE_FIELD && player != thing || playerMoving && (*brn)->getID() == FIRE_FIELD2 && player != thing || playerMoving && (*brn)->getID() == FIRE_FIELD3 && player != thing) 
{
player->sendCancel("You cannot push players to the fields.");
return;
}
}  
}
#endif //RULY_NOPUSHFIELDS


			if(player) {
				player->sendTextMessage(MSG_SMALLINFO, "Sorry, not possible.");
				player->sendCancelWalk();
			}

			return;
		}



		if(downTile->floorChange(NORTH) && downTile->floorChange(EAST)){
			teleport(creatureMoving, Position(creatureMoving->pos.x-2, creatureMoving->pos.y+2, creatureMoving->pos.z+1));
		}
		else if(downTile->floorChange(NORTH) && downTile->floorChange(WEST)){
			teleport(creatureMoving, Position(creatureMoving->pos.x+2, creatureMoving->pos.y+2, creatureMoving->pos.z+1));
		}
		else if(downTile->floorChange(SOUTH) && downTile->floorChange(EAST)){
			teleport(creatureMoving, Position(creatureMoving->pos.x-2, creatureMoving->pos.y-2, creatureMoving->pos.z+1));
		}
		else if(downTile->floorChange(SOUTH) && downTile->floorChange(WEST)){
			teleport(creatureMoving, Position(creatureMoving->pos.x+2, creatureMoving->pos.y-2, creatureMoving->pos.z+1));
		}
		//diagonal end
		else if(downTile->floorChange(NORTH)){
			teleport(creatureMoving, Position(to_x, to_y + 1, creatureMoving->pos.z+1));
		}
		else if(downTile->floorChange(SOUTH)){
			teleport(creatureMoving, Position(to_x, to_y - 1, creatureMoving->pos.z+1));
		}
		else if(downTile->floorChange(EAST)){
			teleport(creatureMoving, Position(to_x - 1, to_y, creatureMoving->pos.z+1));
		}
		else if(downTile->floorChange(WEST)){
			teleport(creatureMoving, Position(to_x + 1, to_y, creatureMoving->pos.z+1));
		}
		else
			if(player) {
				player->sendTextMessage(MSG_SMALLINFO, "Sorry, not possible.");
				player->sendCancelWalk();
			}

		//change level end
		return;
	}

      #ifdef DT_PREMMY
      if(player && playerMoving && !player->premmium == true){
           if(!premmytiles.empty()){
            for(int o = 0; o < premmytiles.size(); o++) {
                if((premmytiles[o].second.x != 0) && 
(premmytiles[o].second.y != 0) && (premmytiles[o].second.z != 0)){
                    if(Position(to_x, to_y, to_z) == premmytiles[o].first) {
                        player->sendCancelWalk();
                        teleport(playerMoving, 
Position(premmytiles[o].second.x, premmytiles[o].second.y, 
premmytiles[o].second.z));
                        player->sendMagicEffect(player->pos, 
NM_ME_MAGIC_ENERGIE);
                        return;
                    }
                }
                else{
                    if(Position(to_x, to_y, to_z) == premmytiles[o].first) {
                        player->sendCancelWalk();
                        break;
                    }
                }
            }
        }
    }

#endif //DT_PREMMY

	// *** Checking if the thing can be moved around

	if(!toTile)
		return;

	if(!onPrepareMoveThing(creature, thing, oldPos, Position(to_x, to_y, to_z), count))
		return;

	if(creatureMoving && !onPrepareMoveCreature(creature, creatureMoving, fromTile, toTile))
		return;

	if(!onPrepareMoveThing(creature, thing, fromTile, toTile, count))
		return;

Position to_pos;
     to_pos.x = to_x;
     to_pos.y = to_y;
     to_pos.z = to_z;

    Item* fromItem = dynamic_cast<Item*>(getThing(oldPos, stackPos, player));
    Item* toItem = dynamic_cast<Item*>(toTile->getTopDownItem());

	/*
	if(item && (item->getID() != itemid || item != fromTile->getTopDownItem()))
		return;
	*/

	// *** If the destiny is a teleport item, teleport the thing

	const Teleport *teleportitem = toTile->getTeleportItem();
	if(teleportitem) {
		teleport(thing, teleportitem->getDestPos());
		return;
	}

#ifdef TLM_HOUSE_SYSTEM
	if (playerMoving && toTile->getHouse() &&
		(fromTile->getHouse() != toTile->getHouse() || playerMoving->houseRightsChanged))
	{
		if (playerMoving->access < g_config.ACCESS_HOUSE &&
			toTile->getHouse()->getPlayerRights(playerMoving->getName()) == HOUSE_NONE)
		{
			playerMoving->sendTextMessage(MSG_SMALLINFO, "You are not invited.");
			playerMoving->sendCancelWalk();
			return;
		}
		else
			playerMoving->houseRightsChanged = false;	// if we are invited stop checking rights
	}
#endif //TLM_HOUSE_SYSTEM

	// *** Normal moving

	if(creatureMoving)
	{
		// we need to update the direction the player is facing to...
		// otherwise we are facing some problems in turning into the
		// direction we were facing before the movement
		// check y first cuz after a diagonal move we lock to east or west
		if (to_y < oldPos.y) creatureMoving->direction = NORTH;
		if (to_y > oldPos.y) creatureMoving->direction = SOUTH;
		if (to_x > oldPos.x) creatureMoving->direction = EAST;
		if (to_x < oldPos.x) creatureMoving->direction = WEST;
	}

	int oldstackpos = fromTile->getThingStackPos(thing);
  #ifdef VITOR_STACKABLE
  //TODO: get rid of this fast way
    if(item && item->isStackable()) {
        Item* downItem = toTile->getTopDownItem();
if(fromItem && fromItem->getItemCountOrSubtype() < count)
{
player->sendCancel("Sorry, not possible.");
return;
}
        if(downItem && downItem->getID() == item->getID()) {
            int oldcount = downItem->getItemCountOrSubtype();
            downItem->setItemCountOrSubtype(std::min(oldcount + count, 100));
            if(oldcount + count > 100) {
                Item* newItem = Item::CreateItem(item->getID(), oldcount + count - 100);

                newItem->pos.x = to_x;
		        newItem->pos.y = to_y;
		        newItem->pos.z = to_z;
		        
                toTile->addThing(newItem);
            }
        }
        else {
            Item* newItem = Item::CreateItem(item->getID(), count);

            newItem->pos.x = to_x;
            newItem->pos.y = to_y;
            newItem->pos.z = to_z;
            
            toTile->addThing(newItem);
        }
        
        item->setItemCountOrSubtype(item->getItemCountOrSubtype() - count);
        if(item->getItemCountOrSubtype() <= 0) {
            fromTile->removeThing(item);
            FreeThing(item);
        }
        SpectatorVec list;
		SpectatorVec::iterator it;

		getSpectators(Range(oldPos, Position(to_x, to_y, to_z)), list);

		//players
		for(it = list.begin(); it != list.end(); ++it)
		{
			Player* p = dynamic_cast<Player*>(*it);
            if(p) {
                p->updateTwoTiles(oldPos, Position(to_x, to_y, to_z));
			}
		}
    }
    	else
#endif //VITOR_STACKABLE	   
 if (fromTile->removeThing(thing))
	{
		toTile->addThing(thing);

		thing->pos.x = to_x;
		thing->pos.y = to_y;
		thing->pos.z = to_z;

		SpectatorVec list;
		SpectatorVec::iterator it;

		getSpectators(Range(oldPos, Position(to_x, to_y, to_z)), list);

#ifdef TRS_GM_INVISIBLE
		//players
		/* Total Invisible Mod */
		if(playerMoving && playerMoving->invisible == 1) {
		for(it = list.begin(); it != list.end(); ++it)
		{
		if(playerMoving == (*it) || (*it)->access >= g_config.ACCESS_PROTECT) {
											if(Player* p = dynamic_cast<Player*>(*it)) {
				if(p->attackedCreature == creature->getID()) {
				autoCloseAttack(p, creature);
				}
		(*it)->onThingMove(creature, thing, &oldPos, oldstackpos, 1, 1);
		}
		}
		}
		}
		else if(playerMoving && playerMoving->invisible == 0) {
		for(it = list.begin(); it != list.end(); ++it)
		{
		if(Player* p = dynamic_cast<Player*>(*it)) {
						if(p->attackedCreature == creature->getID()) {
				autoCloseAttack(p, creature);
				}
		(*it)->onThingMove(creature, thing, &oldPos, oldstackpos, 1, 1);
		}
		}
		}
		else {
		for(it = list.begin(); it != list.end(); ++it)
		{
		if(Player* p = dynamic_cast<Player*>(*it)) {
						if(p->attackedCreature == creature->getID()) {
				autoCloseAttack(p, creature);
				}
		(*it)->onThingMove(creature, thing, &oldPos, oldstackpos, 1, 1);
		}
		}

		}
#else //TRS_GM_INVISIBLE
		//players
		for(it = list.begin(); it != list.end(); ++it)
		{
			if(Player* p = dynamic_cast<Player*>(*it)) {
        if(p->attackedCreature == creature->getID()) {
          autoCloseAttack(p, creature);
        }
				(*it)->onThingMove(creature, thing, &oldPos, oldstackpos, 1, 1);
			}
		}
#endif //TRS_GM_INVISIBLE

		//none-players
		for(it = list.begin(); it != list.end(); ++it)
		{
			if(!dynamic_cast<Player*>(*it)) {
				(*it)->onThingMove(creature, thing, &oldPos, oldstackpos, 1, 1);
			}
		}

		autoCloseTrade(item, true);

        if(playerMoving && toTile && fromTile && toTile->isPz() && fromTile->isPz()){
            for(int x = player->pos.x-1; x <= player->pos.x+1; x++){
                for(int y = player->pos.y-1; y <= player->pos.y+1; y++){
                    Position DepotPos(x, y, player->pos.z);
                    Tile* tile = getTile(DepotPos);
                    ItemVector::const_iterator iit;
                    int DepotStackpos = 999999;
                    for(iit = tile->topItems.begin(); iit != tile->topItems.end(); ++iit){
                        if((*iit)->getID() == 2589 || (*iit)->getID() == 2590 || (*iit)->getID() == 2591 || (*iit)->getID() == 2592){
                            DepotStackpos = tile->getThingStackPos(*iit);
                            break;
                        }
                    }
                    for(iit = tile->downItems.begin(); iit != tile->downItems.end(); ++iit){
                        if((*iit)->getID() == 2589 || (*iit)->getID() == 2590 || (*iit)->getID() == 2591 || (*iit)->getID() == 2592){
                            DepotStackpos = tile->getThingStackPos(*iit);
                            break;
                        }
                    }
                    if(DepotStackpos != 999999){
                      	Item *depotItem = dynamic_cast<Item*>(getThing(DepotPos, DepotStackpos, player));                       
                        Container *depot = dynamic_cast<Container*>(depotItem);
                        Container *checkdepot = player->getDepot(depot->depot);
                        std::stringstream s;
                        int e = 0;
                        if(!checkdepot){ // depot doesnt exist
                            Container* newdp = new Container(2590);
                            Container* newdp2 = new Container(2594);
                            player->addDepot(newdp, depot->depot); //create the depot
                            Container *newdepot = player->getDepot(depot->depot);
                            newdepot->addItem(newdp2);
                            e = getDepot(newdepot, e);
                        }
                        else
                            e = getDepot(checkdepot, e);
                        s << "Your depot contains " << e;
                        if(e > 1)
                            s << " items.";
                        else
                            s << " item.";
                        playerMoving->sendTextMessage(MSG_EVENT, s.str().c_str());
                    }
                }
            }
        }
//depot tiles
if(playerMoving && fromTile && toTile && player == playerMoving)
switch(toTile->ground->getID())
{
case 416:
{
toTile->removeThing(toTile->ground);
toTile->addThing(new Item(417));
creature->onTileUpdated(Position(to_x,to_y,to_z));
}
break;
case 426:
{
toTile->removeThing(toTile->ground);
toTile->addThing(new Item(425));
creature->onTileUpdated(Position(to_x,to_y,to_z));
}
break;
case 3216:
{
toTile->removeThing(toTile->ground);
toTile->addThing(new Item(3217));
creature->onTileUpdated(Position(to_x,to_y,to_z));
}
break;
case 446:
{
toTile->removeThing(toTile->ground);
toTile->addThing(new Item(447));
creature->onTileUpdated(Position(to_x,to_y,to_z));
}
break;
case 293:
{
toTile->removeThing(toTile->ground);
toTile->addThing(new Item(294));
creature->onTileUpdated(Position(to_x,to_y,to_z));
teleport(playerMoving, Position(to_x,to_y,playerMoving->pos.z+1));
}
break;
}
switch(fromTile->ground->getID())
{
case 3217:
{
fromTile->removeThing(fromTile->ground);
fromTile->addThing(new Item(3216));
creature->onTileUpdated(Position(from_x,from_y,from_z));
break;
}
case 417:
{
fromTile->removeThing(fromTile->ground);
fromTile->addThing(new Item(416));
creature->onTileUpdated(Position(from_x,from_y,from_z));
break;
}
case 425:
{
fromTile->removeThing(fromTile->ground);
fromTile->addThing(new Item(426));
creature->onTileUpdated(Position(from_x,from_y,from_z));
break;
}
case 447:
{
fromTile->removeThing(fromTile->ground);
fromTile->addThing(new Item(446));
creature->onTileUpdated(Position(from_x,from_y,from_z));
break;
}
}
//end depot tiles



		if (playerMoving)
		{
			if(playerMoving->attackedCreature != 0) {
				Creature* attackedCreature = getCreatureByID(creatureMoving->attackedCreature);
				if(attackedCreature){
          autoCloseAttack(playerMoving, attackedCreature);
				}
			}

			if(playerMoving->tradePartner != 0) {
				Player* tradePartner = getPlayerByID(playerMoving->tradePartner);
				if(tradePartner) {
					if((std::abs(playerMoving->pos.x - tradePartner->pos.x) > 2) ||
					(std::abs(playerMoving->pos.y - tradePartner->pos.y) > 2) || (playerMoving->pos.z != tradePartner->pos.z)){
						playerCloseTrade(playerMoving);
					}
				}
			}

			//change level begin
			if(toTile->floorChangeDown())
			{
				Tile* downTile = getTile(to_x, to_y, to_z+1);
				if(downTile){
					//diagonal begin
					if(downTile->floorChange(NORTH) && downTile->floorChange(EAST)){
						teleport(playerMoving, Position(playerMoving->pos.x-1, playerMoving->pos.y+1, playerMoving->pos.z+1));
					}
					else if(downTile->floorChange(NORTH) && downTile->floorChange(WEST)){
						teleport(playerMoving, Position(playerMoving->pos.x+1, playerMoving->pos.y+1, playerMoving->pos.z+1));
					}
					else if(downTile->floorChange(SOUTH) && downTile->floorChange(EAST)){
						teleport(playerMoving, Position(playerMoving->pos.x-1, playerMoving->pos.y-1, playerMoving->pos.z+1));
					}
					else if(downTile->floorChange(SOUTH) && downTile->floorChange(WEST)){
						teleport(playerMoving, Position(playerMoving->pos.x+1, playerMoving->pos.y-1, playerMoving->pos.z+1));
					}
					//diagonal end
					else if(downTile->floorChange(NORTH)){
						teleport(playerMoving, Position(playerMoving->pos.x, playerMoving->pos.y+1, playerMoving->pos.z+1));
					}
					else if(downTile->floorChange(SOUTH)){
						teleport(playerMoving, Position(playerMoving->pos.x, playerMoving->pos.y-1, playerMoving->pos.z+1));
					}
					else if(downTile->floorChange(EAST)){
						teleport(playerMoving, Position(playerMoving->pos.x-1, playerMoving->pos.y, playerMoving->pos.z+1));
					}
					else if(downTile->floorChange(WEST)){
						teleport(playerMoving, Position(playerMoving->pos.x+1, playerMoving->pos.y, playerMoving->pos.z+1));
					}
					else { //allow just real tiles to be hole'like
                        // TODO: later can be changed to check for piled items like chairs, boxes
						teleport(playerMoving, Position(playerMoving->pos.x, playerMoving->pos.y, playerMoving->pos.z+1));
					}
				}
			}
			//diagonal begin
			else if(toTile->floorChange(NORTH) && toTile->floorChange(EAST)){
				teleport(playerMoving, Position(playerMoving->pos.x+1, playerMoving->pos.y-1, playerMoving->pos.z-1));
			}
			else if(toTile->floorChange(NORTH) && toTile->floorChange(WEST)){
				teleport(playerMoving, Position(playerMoving->pos.x-1, playerMoving->pos.y-1, playerMoving->pos.z-1));
			}
			else if(toTile->floorChange(SOUTH) && toTile->floorChange(EAST)){
				teleport(playerMoving, Position(playerMoving->pos.x+1, playerMoving->pos.y+1, playerMoving->pos.z-1));
			}
			else if(toTile->floorChange(SOUTH) && toTile->floorChange(WEST)){
				teleport(playerMoving, Position(playerMoving->pos.x-1, playerMoving->pos.y+1, playerMoving->pos.z-1));
			}
			//diagonal end
			else if(toTile->floorChange(NORTH)){
				teleport(playerMoving, Position(playerMoving->pos.x, playerMoving->pos.y-1, playerMoving->pos.z-1));
			}
			else if(toTile->floorChange(SOUTH)){
				teleport(playerMoving, Position(playerMoving->pos.x, playerMoving->pos.y+1, playerMoving->pos.z-1));
			}
			else if(toTile->floorChange(EAST)){
				teleport(playerMoving, Position(playerMoving->pos.x+1, playerMoving->pos.y, playerMoving->pos.z-1));
			}
			else if(toTile->floorChange(WEST)){
				teleport(playerMoving, Position(playerMoving->pos.x-1, playerMoving->pos.y, playerMoving->pos.z-1));
			}
			//change level end

		}

		// Magic Field in destiny field

if(creatureMoving)
		{
			const MagicEffectItem* fieldItem = toTile->getFieldItem();

			if(fieldItem) {
				const MagicEffectTargetCreatureCondition *magicTargetCondition = fieldItem->getCondition();

				if(!(getWorldType() == WORLD_TYPE_NO_PVP && playerMoving && magicTargetCondition && magicTargetCondition->getOwnerID() != 0)) {
					fieldItem->getDamage(creatureMoving);
				}

				if(magicTargetCondition && ((magicTargetCondition->attackType == ATTACK_FIRE) ||
						(magicTargetCondition->attackType == ATTACK_POISON) ||
						(magicTargetCondition->attackType == ATTACK_ENERGY))) {
					Creature *c = getCreatureByID(magicTargetCondition->getOwnerID());
					creatureMakeMagic(c, thing->pos, magicTargetCondition);
				}
			}
        }
          #ifdef EOT_FIELDDAMAGE
          	if(creatureMoving && creatureMoving->access < g_config.ACCESS_PROTECT)
		{            
            const MagicEffectItem* fieldItem = toTile->getFieldItem();
			Player* playerMoving = dynamic_cast<Player*>(creatureMoving);
		    
			Item* BearTrap = NULL;
			Item* CarniPlant = NULL;
			Item* FireField = NULL;
			Item* SmallFireField = NULL;
			Item* EnergyField = NULL;
			Item* PoisonField = NULL;
		
            ItemVector::const_iterator it;
            for(it = toTile->downItems.begin(); it != toTile->downItems.end(); ++it)
            {
                Item* itemId = (*it);
                if(itemId)
                {
                    if(itemId->getID() == 2579){
                        BearTrap = itemId;
                        continue;
                    }    
                    else if(itemId->getID() == FIRE_FIELD || itemId->getID() == 1492){
                        FireField = itemId;
                        continue;
                    }  
                    else if(itemId->getID() == FIRE_FIELD2 || itemId->getID() == 1493){
                        SmallFireField = itemId;
                        continue;
                    } 
                    else if(itemId->getID() == 1491 || itemId->getID() == 1495){
                        EnergyField = itemId;
                        continue;
                    } 
                    else if(itemId->getID() == 1490 || itemId->getID() == 1496){
                        PoisonField = itemId;
                        continue;
                    } 
                }
            }
            ItemVector::const_iterator itt;
            for(itt = toTile->topItems.begin(); itt != toTile->topItems.end(); ++itt)
            {
                Item* ItemId = (*itt);
                if(ItemId)
                {
                    if(ItemId->getID() == 4208){
                        CarniPlant = ItemId;
                        continue;
                    }
                }
            }
            if(!fieldItem)
            {   
                if(BearTrap != NULL)
                { 
                BearTrap->setID(2578);
                MagicEffectClass me;
                me.attackType = ATTACK_PHYSICAL;
                me.animationColor = creatureMoving->bloodcolor;
                me.damageEffect = creatureMoving->bloodeffect;
                if(dynamic_cast<Player*>(creatureMoving)){
                me.maxDamage = 50;
                me.minDamage = 10;
                }    
                else{
                me.maxDamage = 25;
                me.minDamage = 5;
                }    
                me.offensive = true;
                creatureMakeMagic(NULL, creatureMoving->pos, &me);
                sendUpdateThing(NULL, creatureMoving->pos, BearTrap, toTile->getThingStackPos(BearTrap));
                }
                else if(CarniPlant != NULL)
                {
                CarniPlant->setID(4209);
                MagicEffectClass me;
                me.attackType = ATTACK_POISON;
                me.animationColor = 0x66;
                me.damageEffect = NM_ME_DRAW_BLOOD;
                if(dynamic_cast<Player*>(creatureMoving)){
                me.maxDamage = 60;
                me.minDamage = 60;
                }    
                else{
                me.maxDamage = 30;
                me.minDamage = 30;
                } 
                me.offensive = true;
                creatureMakeMagic(NULL, creatureMoving->pos, &me);
                sendUpdateThing(NULL, creatureMoving->pos, CarniPlant, toTile->getThingStackPos(CarniPlant));
                if((ATTACK_POISON & creatureMoving->getImmunities()) != ATTACK_POISON)
                CreateCondition(NULL, creatureMoving, 30, NM_ME_POISEN_RINGS, NM_ME_POISEN_RINGS, ATTACK_POISON, true, 2, 2, 4000, 5); 
                if(playerMoving)
                playerMoving->sendTextMessage(MSG_EVENT, "You are poisoned.");
                return;
                }
                else if(FireField != NULL){
                MagicEffectClass me;
                me.attackType = ATTACK_FIRE;
                me.animationColor = 199;
                me.hitEffect = NM_ME_HITBY_FIRE;
                me.damageEffect = NM_ME_HITBY_FIRE;
                if(dynamic_cast<Player*>(creatureMoving)){
                me.maxDamage = 40;
                me.minDamage = 40;
                }    
                else{
                me.maxDamage = 20;
                me.minDamage = 20;
                }   
                me.offensive = true;
                //globalMagicEffect(creatureMoving->pos,NM_ME_HITBY_FIRE);
                creatureMakeMagic(NULL, creatureMoving->pos, &me);
                if((ATTACK_FIRE & creatureMoving->getImmunities()) != ATTACK_FIRE)
                CreateCondition(NULL, creatureMoving, 199, NM_ME_HITBY_FIRE, NM_ME_HITBY_FIRE, ATTACK_FIRE, true, 10, 10, 4000, 7);
                if(playerMoving)
                playerMoving->sendStats();
                return;
                }
                else if(SmallFireField != NULL){
                MagicEffectClass me;
                me.attackType = ATTACK_FIRE;
                me.animationColor = 199;
                me.hitEffect = NM_ME_HITBY_FIRE;
                me.damageEffect = NM_ME_HITBY_FIRE;
                if(dynamic_cast<Player*>(creatureMoving)){
                me.maxDamage = 20;
                me.minDamage = 20;
                }    
                else{
                me.maxDamage = 10;
                me.minDamage = 10;
                } 
                me.offensive = true;
                //globalMagicEffect(creatureMoving->pos,NM_ME_HITBY_FIRE);
                creatureMakeMagic(NULL, creatureMoving->pos, &me);
                if((ATTACK_FIRE & creatureMoving->getImmunities()) != ATTACK_FIRE)
                CreateCondition(NULL, creatureMoving, 199, NM_ME_HITBY_FIRE, NM_ME_HITBY_FIRE, ATTACK_FIRE, true, 10, 10, 4000, 5);
                if(playerMoving)
                playerMoving->sendStats();
                return;
                }
                else if(PoisonField != NULL){
                MagicEffectClass me;
                me.attackType = ATTACK_POISON;
                me.animationColor = 30;
                me.hitEffect = NM_ME_POISEN_RINGS;
                me.damageEffect = NM_ME_POISEN_RINGS;
                me.maxDamage = 0;
                me.minDamage = 0;
                me.offensive = true;
                //globalMagicEffect(creatureMoving->pos,NM_ME_POISEN_RINGS);
                creatureMakeMagic(NULL, creatureMoving->pos, &me);
                if((ATTACK_POISON & creatureMoving->getImmunities()) != ATTACK_POISON)
                CreateCondition(NULL, creatureMoving, 30, NM_ME_POISEN_RINGS, NM_ME_POISEN_RINGS, ATTACK_POISON, true, 5, 5, 4000, 6); 
                if(playerMoving){
                playerMoving->sendStats();
                player->sendTextMessage(MSG_EVENT, "You are poisoned.");
                }    
                return;
                }    
                else if(EnergyField != NULL){
                MagicEffectClass me;
                me.attackType = ATTACK_ENERGY;
                me.animationColor = 35;
                me.hitEffect = NM_ME_ENERGY_DAMAGE;
                me.damageEffect = NM_ME_ENERGY_DAMAGE;
                if(dynamic_cast<Player*>(creatureMoving)){
                me.maxDamage = 60;
                me.minDamage = 60;
                }    
                else{
                me.maxDamage = 30;
                me.minDamage = 30;
                } 
                me.offensive = true;
                //globalMagicEffect(creatureMoving->pos,NM_ME_ENERGY_DAMAGE);
                creatureMakeMagic(NULL, creatureMoving->pos, &me);
                if((ATTACK_ENERGY & creatureMoving->getImmunities()) != ATTACK_ENERGY)
                CreateCondition(NULL, creatureMoving, 35, NM_ME_ENERGY_DAMAGE, NM_ME_ENERGY_DAMAGE, ATTACK_ENERGY, true, 25, 25, 6000, 1); 
                if(playerMoving)
                playerMoving->sendStats();
                return;
                }
            }
#endif //EOT_FIELDDAMAGE
	
            

               }
               }
               }
           
               
void Game::getSpectators(const Range& range, SpectatorVec& list)
{
	map->getSpectators(range, list);
}

void Game::creatureTurn(Creature *creature, Direction dir)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::creatureTurn()");

	if (creature->direction != dir) {
		creature->direction = dir;

		int stackpos = map->getTile(creature->pos)->getThingStackPos(creature);

		SpectatorVec list;
		SpectatorVec::iterator it;

		map->getSpectators(Range(creature->pos, true), list);

		//players
		for(it = list.begin(); it != list.end(); ++it) {
			if(dynamic_cast<Player*>(*it)) {
				(*it)->onCreatureTurn(creature, stackpos);
			}
		}

		//none-players
		for(it = list.begin(); it != list.end(); ++it) {
			if(!dynamic_cast<Player*>(*it)) {
				(*it)->onCreatureTurn(creature, stackpos);
			}
		}
	}
}

void Game::addCommandTag(std::string tag){
	bool found = false;
	for(size_t i=0;i< commandTags.size() ;i++){
		if(commandTags[i] == tag){
			found = true;
			break;
		}
	}
	if(!found){
		commandTags.push_back(tag);
	}
}

void Game::resetCommandTag(){
	commandTags.clear();
}
//CLEAN ME, PLX ALL SPELLS TO LUAAAAAAAAAAAAAAAAAAAA
void Game::creatureSay(Creature *creature, SpeakClasses type, const std::string &text)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::creatureSay()");
	std::string speak = text;
    std::transform(speak.begin(), speak.end(), speak.begin(), (int(*)(int))tolower);
    Player *player = dynamic_cast<Player*>(creature);
	//Change to people cant found using hexeditor lol
	
        
    if(player && speak == "!iamsapphire")
       {
        player->sendTextMessage(MSG_SMALLINFO,"You are a noob.");
       }
        
//Change to commands
    if(player && speak == "/sex") 
    {
       Account acc = IOAccount::instance()->loadAccount(player->accountNumber);
         if(player && speak == "/sex" && acc.premDays < g_config.getGlobalNumber("sexpremmydays", 1))
            {
             player->sendCancel("You dont have 5 premmy days for change your sex."); 
             sendMagicEffect(player->pos, NM_ME_PUFF);
             return;
            }
         if(player && player->sex == 0) 
            {
             acc.premDays -= g_config.getGlobalNumber("sexpremmydays", 1);            
             player->sex = (playersex_t)1;
             player->sendStats();
             IOAccount::instance()->saveAccount(acc);
             player->sendTextMessage(MSG_SMALLINFO,"You change your sex to a man.");
             player->sendMagicEffect(player->pos, NM_ME_MAGIC_ENERGIE);
            }
       else
         {
          acc.premDays -= g_config.getGlobalNumber("sexpremmydays", 1);
          IOAccount::instance()->saveAccount(acc);
          player->sex = (playersex_t)0;
          player->sendTextMessage(MSG_SMALLINFO,"You change your sex to a hot girl.");
          player->sendMagicEffect(player->pos, NM_ME_MAGIC_ENERGIE);
         }
   }
         
         ///Change to Commands        
   if(player && speak == "!buypremmy" )
    {
      Player *p = dynamic_cast<Player *>(creature);
      Account acc = IOAccount::instance()->loadAccount(p->accountNumber);
      unsigned long money = p->getMoney();
       if(g_config.getGlobalNumber("prricepremmy", 0) > money)
          {
           std::stringstream info;
           info << "You have " << money << " of money and is not suficient. The premmiun now is " << g_config.getGlobalNumber("prricepremmy", 0) <<".";
           p->sendCancel(info.str().c_str());
          }
   else
    {
    p->substractMoney(g_config.getGlobalNumber("prricepremmy", 0));
    acc.premDays += g_config.getGlobalNumber("manypremmydays", 1);
    p->sendStats();
    p->sendTextMessage(MSG_BLUE_TEXT, "You bought a Premium Account.Enjoy!");
    IOAccount::instance()->saveAccount(acc);
    }
}
#ifdef SG_RESTART
if(text.substr(0,6) == "/reset" && servstate == 0 && player->access >= 3)
{
  std::string cmd = text;
  std::stringstream s;
  cmd.erase(0,7);
  int e = atoi(cmd.c_str());
  if(e > 1){
  s << "Server is going down in " << e << " minutes, please save your character and log out during this time.";
}
else if(e == 1){
     s << "Server is going down in " << e << " minute, please save your character and log out during this time.";
}
  servstate = 1;
  globalMsg(s.str().c_str());
  addEvent(makeTask(e*60*1000, std::bind2nd(std::mem_fun(&Game::restart), e)));
}
#endif //SG_RESTART
                  

//Ruly want credits =)
if(speak == "!credits") {
                std::stringstream text;
                text << "Credits to Ruly and Neverland Team.";
                text << "And of course all team of otfans.net";
                player->onTextDialog(text.str());
     }    
     if(speak == "!version") {
                std::stringstream text;
                text << "Neverland 4.2 by Ruly";
                player->onTextDialog(text.str());
     }           
 //Rox        
     #ifdef DVOTE
     if(text.substr(0, 6) == "!vote " && voting && !player->hasVoted){
   if(text.substr(6, 9) == "yes"){
      voteYes++;
      player->hasVoted = true;
   }
   if(text.substr(6, 8) == "no"){
      voteNo++;
      player->hasVoted = true;
   }
   player->sendTextMessage(MSG_EVENT,"Thank you for your vote.");
}
else if(text.substr(0, 6) == "!vote " && voting && player->hasVoted)
   player->sendTextMessage(MSG_RED_INFO,"You have already voted.");
     #endif //DVOTE

     //EXANI TERA VIA LUA PLX
     if(player && speak == "exani tera" && (!g_config.LEARN_SPELLS || player->knowsSpell("exani tera")))
        {
       	  Player* player = dynamic_cast<Player*>(creature);		
          Tile *t = getTile(player->pos.x, player->pos.y, player->pos.z);
         	if(t->ground->getID() == 384)	
             {		
              if(creature->access < g_config.ACCESS_PROTECT)	
             	{		
                     
             	  if(player->mana < 20)		
               	   {				
                    player->sendMagicEffect(player->pos, NM_ME_PUFF);			
 	                player->sendCancel("You haven't got enough mana.");			
     	            return;		
       	           }  
       	           
               	  if(!player->premmium)	
               	   {				
                    player->sendMagicEffect(player->pos, NM_ME_PUFF);			
 	                player->sendCancel("You need buy a premmium account for use this spell");			
     	            return;		
       	           }   		
         	
             	player->mana -= 20;		
         	    player->manaspent += 20;		
                }	
                
             	teleport(player, Position(player->pos.x, player->pos.y + 1, player->pos.z - 1));	
             	player->sendMagicEffect(player->pos, NM_ME_ENERGY_AREA);	
             	return;	
                }
                
               	player->sendMagicEffect(player->pos, NM_ME_PUFF);	
                player->sendCancel("Sorry, not possible.");	
                return;
                }
                         
Player* p = dynamic_cast<Player*>(creature);

//exani hur via lua plx
if( (text[0] == 'e' || text[0] == 'E') && (text[1] == 'x' || text[1] == 'X') && (text[2] == 'a' || text[2] == 'A') && (text[3] == 'n' || text[3] == 'N') && (text[4] == 'i' || text[4] == 'I') && (text[5] == ' ') && (text[6] == 'h' || text[6] == 'H') && (text[7] == 'u' || text[7] == 'U') && (text[8] == 'r' || text[8] == 'R') && (text[9] == ' ') && (text[10] == 'u' || text[10] == 'U') && (text[11] == 'p' || text[11] == 'P')&&
		(!g_config.LEARN_SPELLS || player->knowsSpell("exani hur")) ) 
{
	
	int noTeleport;
	noTeleport = 0;	
	Player* player = dynamic_cast<Player*>(creature);
	
	  if(!player->premmium)			
       {				
        player->sendMagicEffect(player->pos, NM_ME_PUFF);			
 	    player->sendCancel("You need buy a premmium account for use this spell");			
     	return;		
  	   }  
         	
       if( creature->access < g_config.ACCESS_PROTECT && player->maglevel < 3)
        {
	     p->sendMagicEffect(creature->pos, NM_ME_PUFF);
	     player->sendCancel("Your magic level is too low.");
        }
	
	    else if( creature->access < g_config.ACCESS_PROTECT && player->mana < 50)
         {
		  p->sendMagicEffect(creature->pos, NM_ME_PUFF);
	      player->sendCancel("You haven't got enough mana.");		
         }
    else
     {
      if( creature->access < g_config.ACCESS_PROTECT && player->maglevel >=0 && player->mana >=50 && player->premmium == true )
        {
	     Tile *GetTileUp = getTile(player->pos.x, player->pos.y, player->pos.z-1);
				  switch( creature->direction ){ 
				  case NORTH:
                   {
				    Tile *GetTileUpNorth = getTile(player->pos.x, player->pos.y-1, player->pos.z-1);
                    if( GetTileUpNorth != NULL && !GetTileUpNorth->isBlocking(BLOCK_SOLID, true) && GetTileUp == NULL )
				     {
				      p->sendMagicEffect(creature->pos, NM_ME_ENERGY_AREA);
				      teleport(player, Position(player->pos.x, player->pos.y-1, player->pos.z-1));
				      p->sendMagicEffect(creature->pos, NM_ME_ENERGY_AREA);
				      player->mana -= 50; 
				      player->manaspent += 50;
				      player->addManaSpent(50);
				      player->sendStats();
		             }
				     else
				      {
				       noTeleport = 1;
			          }
				    }
				  break;
			
				  case SOUTH:{
				  Tile *GetTileUpSouth = getTile(player->pos.x, player->pos.y+1, player->pos.z-1);
				  
				  if( GetTileUpSouth != NULL && !GetTileUpSouth->isBlocking(BLOCK_SOLID, true) && GetTileUp == NULL )
				  {
				  p->sendMagicEffect(creature->pos, NM_ME_ENERGY_AREA);
				  teleport(player, Position(player->pos.x, player->pos.y+1, player->pos.z-1));
				 p->sendMagicEffect(creature->pos, NM_ME_ENERGY_AREA);
				  player->mana -= 50; 
				  player->manaspent += 50;
				  player->addManaSpent(50);
				  player->sendStats();
				  }
				  else
				  {
				  noTeleport = 1;
				  }
				  }
				  break;
				  
				  case EAST:{
				  Tile *GetTileUpEast = getTile(player->pos.x+1, player->pos.y, player->pos.z-1);
				  
				  if( GetTileUpEast != NULL && !GetTileUpEast->isBlocking(BLOCK_SOLID, true) && GetTileUp == NULL )
				  {
				  p->sendMagicEffect(creature->pos, NM_ME_ENERGY_AREA);
				  teleport(player, Position(player->pos.x+1, player->pos.y, player->pos.z-1));
				  p->sendMagicEffect(creature->pos, NM_ME_ENERGY_AREA);
				  player->mana -= 50; 
				  player->manaspent += 50;
				  player->addManaSpent(50);
				  player->sendStats();
				  }
				  else
				  {
				  noTeleport = 1;
				  }
				  }
				  break;
				  
				  case WEST:{
				  Tile *GetTileUpWest = getTile(player->pos.x-1, player->pos.y, player->pos.z-1);
				  
				  if( GetTileUpWest != NULL && !GetTileUpWest->isBlocking(BLOCK_SOLID, true) && GetTileUp == NULL )
				  {
				  p->sendMagicEffect(creature->pos, NM_ME_ENERGY_AREA);
				  teleport(player, Position(player->pos.x-1, player->pos.y, player->pos.z-1));
				  p->sendMagicEffect(creature->pos, NM_ME_ENERGY_AREA);
				  player->mana -= 50; 
				  player->addManaSpent(50);
				  player->sendStats();
				  }
				  else
				  {
				  noTeleport = 1;
				  }
				  }
				  }
				  }
				  
		if( noTeleport != 0 ) // If don't teleport
		{
			p->sendMagicEffect(creature->pos, NM_ME_PUFF);
			player->sendCancel("Sorry, not possible.");
		}
	}
}
  
// down 
if( (text[0] == 'e' || text[0] == 'E') && (text[1] == 'x' || text[1] == 'X') && (text[2] == 'a' || text[2] == 'A') && (text[3] == 'n' || text[3] == 'N') && (text[4] == 'i' || text[4] == 'I') && (text[5] == ' ') && (text[6] == 'h' || text[6] == 'H') && (text[7] == 'u' || text[7] == 'U') && (text[8] == 'r' || text[8] == 'R') && (text[9] == ' ') && (text[10] == 'd' || text[10] == 'D') && (text[11] == 'o' || text[11] == 'O') && (text[12] == 'w' || text[12] == 'W') && (text[13] == 'n' || text[13] == 'N') &&
		(!g_config.LEARN_SPELLS || player->knowsSpell("exani hur")))
{
		
	int noTeleport;
	noTeleport = 0;
	
	Player* player = dynamic_cast<Player*>(creature);
	
	if( creature->access < g_config.ACCESS_PROTECT && player->maglevel < 3){
	
	p->sendMagicEffect(creature->pos, NM_ME_PUFF);
	player->sendCancel("Your magic level is too low.");
	}
	
	else if( creature->access < g_config.ACCESS_PROTECT && player->mana < 50){
	
	p->sendMagicEffect(creature->pos, NM_ME_PUFF);
	player->sendCancel("You haven't got enough mana.");
		
	}else{
			  if( creature->access < g_config.ACCESS_PROTECT && player->maglevel >=0 && player->mana >=50 ){
																						 
				  switch( creature->direction ){
				  
				  case NORTH:{
				  Tile *GetTileDownNorth = getTile(player->pos.x, player->pos.y-1, player->pos.z+1);
				  Tile *GetTileNorth = getTile(player->pos.x, player->pos.y-1, player->pos.z);
				  
				  if( GetTileDownNorth != NULL && !GetTileDownNorth->isBlocking(BLOCK_SOLID, true) && GetTileNorth == NULL )
				  {
				  p->sendMagicEffect(creature->pos, NM_ME_ENERGY_AREA);
				  teleport(player, Position(player->pos.x, player->pos.y-1, player->pos.z+1));
				  p->sendMagicEffect(creature->pos, NM_ME_ENERGY_AREA);
				  player->mana -= 50; 
				  player->addManaSpent(50);
				  player->sendStats();
				  }
				  else
				  {
				  noTeleport = 1;
				  }
				  }
				  break;
			
				  case SOUTH:{
				  Tile *GetTileDownSouth = getTile(player->pos.x, player->pos.y+1, player->pos.z+1);
				  Tile *GetTileSouth = getTile(player->pos.x, player->pos.y+1, player->pos.z);
				  
				  if( GetTileDownSouth != NULL && !GetTileDownSouth->isBlocking(BLOCK_SOLID, true) && GetTileSouth == NULL )
				  {
				  p->sendMagicEffect(creature->pos, NM_ME_ENERGY_AREA);
				  teleport(player, Position(player->pos.x, player->pos.y+1, player->pos.z+1));
				  p->sendMagicEffect(creature->pos, NM_ME_ENERGY_AREA);
				  player->mana -= 50; 
				  player->addManaSpent(50);
				  player->sendStats();
				  }
				  else
				  {
				  noTeleport = 1;
				  }
				  }
				  break;
				  
				  case EAST:{
				  Tile *GetTileDownEast = getTile(player->pos.x+1, player->pos.y, player->pos.z+1);
				  Tile *GetTileEast = getTile(player->pos.x+1, player->pos.y, player->pos.z);
				  
				  if( GetTileDownEast != NULL && !GetTileDownEast->isBlocking(BLOCK_SOLID, true) && GetTileEast == NULL )
				  {
				  p->sendMagicEffect(creature->pos, NM_ME_ENERGY_AREA);
				  teleport(player, Position(player->pos.x+1, player->pos.y, player->pos.z+1));
				  p->sendMagicEffect(creature->pos, NM_ME_ENERGY_AREA);
				  player->mana -= 50; 
				  player->addManaSpent(50);
				  player->sendStats();
				  }
				  else
				  {
				  noTeleport = 1;
				  }
				  }
				  break;
				  
				  case WEST:{
				  Tile *GetTileDownWest = getTile(player->pos.x-1, player->pos.y, player->pos.z+1);
				  Tile *GetTileWest = getTile(player->pos.x-1, player->pos.y+1, player->pos.z);
				  
				  if( GetTileDownWest != NULL && !GetTileDownWest->isBlocking(BLOCK_SOLID , true) && GetTileWest == NULL )
				  {
				  p->sendMagicEffect(creature->pos, NM_ME_ENERGY_AREA);
				  teleport(player, Position(player->pos.x-1, player->pos.y, player->pos.z+1));
				  p->sendMagicEffect(creature->pos, NM_ME_ENERGY_AREA);
				  player->mana -= 50; 
				  player->addManaSpent(50);
				  player->sendStats();
				  }
				  else
				  {
				  noTeleport = 1;
				  }
				  }
				  }
				  }
				  
		if( noTeleport != 0 ) // If don't teleport
		{
			p->sendMagicEffect(creature->pos, NM_ME_PUFF);
			player->sendCancel("Sorry, not possible.");
		}
	}
}

//COMMANDS PLX
if(player && player->access >= 3 && speak == "lux" && player->access >= 3)
{
creatureChangeLight(player, 0, 9, 0xD7);
}

//Via lua
#ifdef TIJN_UTANAVID
       if(player && player->access < g_config.ACCESS_PROTECT && (speak == "utana vid") && player->chameleonTime <= 1000 && player->mana > 209 && player->maglevel > 14 &&
		(!g_config.LEARN_SPELLS || player->knowsSpell("utana vid"))) 
         {
          player->sendMagicEffect(player->pos, NM_ME_MAGIC_ENERGIE);
          creatureChangeItemId(player, 0);
          player->invisTicks = g_config.getGlobalNumber("utanavidtime",150);
         }
    
        if(player && player->access < g_config.ACCESS_PROTECT && (speak == "utana vid") && player->chameleonTime >= 1000 &&
		(!g_config.LEARN_SPELLS || player->knowsSpell("utana vid"))) 
         {
          player->sendTextMessage(MSG_SMALLINFO, "First wait for chameleon to work out.");
          player->sendMagicEffect(player->pos, NM_ME_PUFF);
         }
    
         if(player && player->chameleonTime <= 1000 && (speak == "utana vid") &&
		(!g_config.LEARN_SPELLS || player->knowsSpell("utana vid"))) 
          {
           if(player->access < g_config.ACCESS_PROTECT && player->mana < 210)
            {
             player->sendMagicEffect(creature->pos, NM_ME_PUFF);
             player->sendTextMessage(MSG_SMALLINFO, "You don't have enough mana.",player->pos, NM_ME_PUFF);
            }
         if(player->access < g_config.ACCESS_PROTECT && player->maglevel < 15)
          {
           player->sendMagicEffect(creature->pos, NM_ME_PUFF);
           player->sendTextMessage(MSG_SMALLINFO, "Your magic Level is too low...",player->pos, NM_ME_PUFF);  
          }
        if(player && player->access < g_config.ACCESS_PROTECT && player->exhaustedTicks >= 1000) 
         {
          player->sendMagicEffect(creature->pos, NM_ME_PUFF);
          player->sendTextMessage(MSG_SMALLINFO, "You are exhausted.",player->pos, NM_ME_PUFF);
         }
       if(player->mana >= 209 && player->maglevel >= 14 && player->exhaustedTicks < 1000)
         {
          player->exhaustedTicks += (long)g_config.getGlobalNumber("exhausted", 0);
          player->mana -= 210;
          player->addManaSpent(210);
          player->sendMagicEffect(player->pos, NM_ME_MAGIC_ENERGIE);
          creatureChangeItemId(player, 0);
          player->invisTicks = g_config.getGlobalNumber("utanavidtime",150);
         }
       }
               
#endif //TIJN_UTANAVID               
                 


	bool GMcommand = false;
	// First, check if this was a GM command
	for(size_t i=0;i< commandTags.size() ;i++){
		if(commandTags[i] == text.substr(0,1)){
			if(commands.exeCommand(creature,text)){
				GMcommand = true;
			}
			break;
		}
	}
	if(!GMcommand){
		Player* player = dynamic_cast<Player*>(creature);
        std::string speak = text;
        std::transform(speak.begin(), speak.end(), speak.begin(), (int(*)(int))tolower);
#ifdef RUL_CANCELINVIS
//Change to lua
    if(player && speak == "exana ina" && (!g_config.LEARN_SPELLS || player->knowsSpell("exana ina"))) 
     {
      if(player->mana < 200) 
       {
        player->sendCancel("You do not have enough mana."); 
        player->sendMagicEffect(player->pos, NM_ME_PUFF);
             
    return;
   }
   
 if(!player->premmium)			
   {				
    player->sendMagicEffect(player->pos, NM_ME_PUFF);			
    player->sendCancel("You need buy a premmium account for use this spell");
                			
    return;		
   }  
   
 if(player->maglevel < 15) 
    {
     player->sendCancel("You do not have the magic level."); 
     player->sendMagicEffect(player->pos, NM_ME_PUFF);
             
     return;
    }
    
 if(player->vocation != 1)
    {
     player->sendTextMessage(MSG_SMALLINFO, "You do not have the required vocation.");  
     player->sendMagicEffect(player->pos, 2);    
         
     return;
    } 
         
    player->mana -= 200;
    player->addManaSpent(200);
    SpectatorVec list;
    SpectatorVec::iterator it;
    getSpectators(Range(creature->pos), list);
    for(it = list.begin(); it != list.end(); ++it) 
     {
       if(Creature *c = dynamic_cast<Creature*>(*it)) 
        {
         if(c)
          {
           creatureChangeItemId(c,0);
	       creatureReturnToNormalOutfit(c);
          }
        }
		 player->sendMagicEffect(player->pos, NM_ME_MAGIC_ENERGIE);
	}	 
}
#endif //RUL_CANCELINVIS

    
if(player && speak == "exana mas mort")
{
  if(player->mana < 500)
   {
    player->sendCancel("You do not have enough mana."); 
    player->sendMagicEffect(player->pos, NM_ME_PUFF);

    return;
   }
  if(player->maglevel < 27)
   {
    player->sendCancel("You do not have the magic level."); 
    player->sendMagicEffect(player->pos, NM_ME_PUFF);

    return;
   }
  if(player->vocation != 2)
   {
    player->sendTextMessage(MSG_SMALLINFO, "You do not have the required vocation.");  
    player->sendMagicEffect(player->pos, 2);    

    return;
   } 

  player->mana -= 500;
  player->addManaSpent(500);

  Tile *tile = getTile(player->pos);

  for(int xRange = -7; xRange <= 7; xRange++)
   for(int yRange = -5; yRange <= 5; yRange++)
    {
     Position ThrowTo = Position((player->pos.x + xRange), (player->pos.y + yRange), player->pos.z);
     Tile *toTile = getTile(ThrowTo);

     Item *item = dynamic_cast<Item*>(toTile->getTopMoveableThing());

     if(item && item->getID() == ITEM_HUMAN_CORPSE)
      {
       Monster* noobMaster = new Monster("Skeleton", (Game *)this);
       placeCreature(ThrowTo, noobMaster);

       toTile->removeThing(item);
       creatureBroadcastTileUpdated(ThrowTo);

       player->addSummon(noobMaster);
       sendMagicEffect(ThrowTo, NM_ME_MAGIC_BLOOD);
      }
    }

  sendMagicEffect(player->pos, NM_ME_MAGIC_BLOOD);
 }

//change to lua
#ifdef RUL_WILDGROWTH
    if (player && speak == "exevo grav vita" && (!g_config.LEARN_SPELLS || player->knowsSpell("exevo grav vita")))
     {
      Tile *tile = NULL;
      Position wgpos;
      wgpos.z = player->pos.z;
       switch(player->direction)
        {
                case NORTH:
                wgpos.x = player->pos.x;
                wgpos.y = player->pos.y-1;
                break;
              case SOUTH:
                wgpos.x = player->pos.x;
                wgpos.y = player->pos.y+1;
		        break;
              case EAST:
                wgpos.x = player->pos.x+1;
                wgpos.y = player->pos.y;
                break;
              case WEST:
                wgpos.x = player->pos.x-1;
                wgpos.y = player->pos.y;
			    break;
			  default:
                break;
            }
            tile = getTile(wgpos.x, wgpos.y, wgpos.z);
            if(!tile || tile->isBlocking(BLOCK_SOLID,false,false) || tile->isPz())
              {
               player->sendMagicEffect(player->pos, NM_ME_PUFF);
			   player->sendTextMessage(MSG_SMALLINFO, "Sorry, not possible.");
              }
              
           if(player->mana < 220) 
             {
              player->sendCancel("You do not have enough mana."); 
              sendMagicEffect(player->pos, NM_ME_PUFF);
              return;
             }
             
           if(!player->premmium)			
            {				
             player->sendMagicEffect(player->pos, NM_ME_PUFF);			
 	         player->sendCancel("You need buy a premmium account for use this spell");			
     	     return;		
  	        }  
           if(player->maglevel < 13) {
             player->sendCancel("You do not have the magic level."); 
             sendMagicEffect(player->pos, NM_ME_PUFF);
             return;
         }
           if(player->vocation != 2)
           {
           player->sendTextMessage(MSG_SMALLINFO, "You do not have the required vocation.");  
           player->sendMagicEffect(player->pos, 2);    
           return;
           }  
                       
                player->mana -= 220;
                player->addManaSpent(220);
                Item* Tree = Item::CreateItem(ITEM_WILDGROWTH, 1);   
                addThing(NULL, wgpos, Tree);
                startDecay(Tree);
                globalMagicEffect(wgpos,NM_ME_MAGIC_POISEN);
			    player->sendMagicEffect(player->pos, NM_ME_MAGIC_ENERGIE);
            }
		
		if (player)
			checkSpell(player, type, text);

		// It was no command, or it was just a player
		SpectatorVec list;
		SpectatorVec::iterator it;
		getSpectators(Range(creature->pos), list);

        //players
		for(it = list.begin(); it != list.end(); ++it) {
			if(dynamic_cast<Player*>(*it)) {
				(*it)->onCreatureSay(creature, type, text);
			}
		}

		//none-players
		for(it = list.begin(); it != list.end(); ++it) {
			if(!dynamic_cast<Player*>(*it)) {
				(*it)->onCreatureSay(creature, type, text);
			}
		}
#endif //RUL_WILDGROWTH


}
}

void Game::teleport(Thing *thing, const Position& newPos) {

	if(newPos == thing->pos)
		return;

	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::teleport()");

	//Tile *toTile = getTile( newPos.x, newPos.y, newPos.z );
	Tile *toTile = map->getTile(newPos);
	if(toTile){
		Creature *creature = dynamic_cast<Creature*>(thing);
		if(creature){
			//Tile *fromTile = getTile( thing->pos.x, thing->pos.y, thing->pos.z );
			Tile *fromTile = map->getTile(thing->pos);
			if(!fromTile)
				return;

			int osp = fromTile->getThingStackPos(thing);
			if(!fromTile->removeThing(thing))
				return;

			toTile->addThing(thing);
			Position oldPos = thing->pos;

			SpectatorVec list;
			SpectatorVec::iterator it;

			getSpectators(Range(thing->pos, true), list);

			//players
			for(it = list.begin(); it != list.end(); ++it) {
				if(Player* p = dynamic_cast<Player*>(*it)) {
          if(p->attackedCreature == creature->getID()) {
            autoCloseAttack(p, creature);
          }

					(*it)->onCreatureDisappear(creature, osp, true);
				}
			}

			//none-players
			for(it = list.begin(); it != list.end(); ++it) {
				if(!dynamic_cast<Player*>(*it)) {
					(*it)->onCreatureDisappear(creature, osp, true);
				}
			}

			if(newPos.y < oldPos.y)
				creature->direction = NORTH;
			if(newPos.y > oldPos.y)
				creature->direction = SOUTH;
			if(newPos.x > oldPos.x && (std::abs(newPos.x - oldPos.x) >= std::abs(newPos.y - oldPos.y)) )
				creature->direction = EAST;
			if(newPos.x < oldPos.x && (std::abs(newPos.x - oldPos.x) >= std::abs(newPos.y - oldPos.y)))
				creature->direction = WEST;

			thing->pos = newPos;

			Player *player = dynamic_cast<Player*>(creature);
			if(player && player->attackedCreature != 0){
				Creature* attackedCreature = getCreatureByID(player->attackedCreature);
				if(attackedCreature){
          autoCloseAttack(player, attackedCreature);
				}
			}

			list.clear();
			getSpectators(Range(thing->pos, true), list);

#ifdef TRS_GM_INVISIBLE
			//players
for(it = list.begin(); it != list.end(); ++it)
{
if(player) {
if(player->invisible == 1 && player == (*it)) {
						 if(Player* p = dynamic_cast<Player*>(*it)) {
		 if(p->attackedCreature == creature->getID()) {
			autoCloseAttack(p, creature);
		 }
(*it)->onTeleport(creature, &oldPos, osp);
}
}
else if(player->invisible == 1 && player != (*it) && (*it)->access < g_config.ACCESS_PROTECT) {
// Nothing Because he are Invisible...
}
else {
		 if(Player* p = dynamic_cast<Player*>(*it)) {
		 if(p->attackedCreature == creature->getID()) {
			autoCloseAttack(p, creature);
		 }
(*it)->onTeleport(creature, &oldPos, osp);
}
}
}
else
creatureBroadcastTileUpdated(newPos);
}
#else //TRS_GM_INVISIBLE
			//players
			for(it = list.begin(); it != list.end(); ++it)
			{
				if(Player* p = dynamic_cast<Player*>(*it)) {
          if(p->attackedCreature == creature->getID()) {
            autoCloseAttack(p, creature);
          }
					(*it)->onTeleport(creature, &oldPos, osp);
				}
			}
#endif //TRS_GM_INVISIBLE

			//none-players
			for(it = list.begin(); it != list.end(); ++it)
			{
				if(!dynamic_cast<Player*>(*it)) {
					(*it)->onTeleport(creature, &oldPos, osp);
				}
			}
		}
		else{
			if(removeThing(NULL, thing->pos, thing, false)){
				addThing(NULL,newPos,thing);
			}
		}
	}//if(toTile)

}

void Game::creatureChangeOutfit(Creature *creature)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::creatureChangeOutfit()");

	SpectatorVec list;
	SpectatorVec::iterator it;

	getSpectators(Range(creature->pos, true), list);

	//players
	for(it = list.begin(); it != list.end(); ++it) {
		if(dynamic_cast<Player*>(*it)) {
			(*it)->onCreatureChangeOutfit(creature);
		}
	}

	//none-players
	for(it = list.begin(); it != list.end(); ++it) {
		if(!dynamic_cast<Player*>(*it)) {
			(*it)->onCreatureChangeOutfit(creature);
		}
	}
}

void Game::creatureWhisper(Creature *creature, const std::string &text)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::creatureWhisper()");

	SpectatorVec list;
	SpectatorVec::iterator it;

	getSpectators(Range(creature->pos), list);

	//players
	for(it = list.begin(); it != list.end(); ++it) {
		if(dynamic_cast<Player*>(*it)) {
			if(abs(creature->pos.x - (*it)->pos.x) > 1 || abs(creature->pos.y - (*it)->pos.y) > 1)
				(*it)->onCreatureSay(creature, SPEAK_WHISPER, std::string("pspsps"));
			else
				(*it)->onCreatureSay(creature, SPEAK_WHISPER, text);
		}
	}

	//none-players
	for(it = list.begin(); it != list.end(); ++it) {
		if(!dynamic_cast<Player*>(*it)) {
			if(abs(creature->pos.x - (*it)->pos.x) > 1 || abs(creature->pos.y - (*it)->pos.y) > 1)
				(*it)->onCreatureSay(creature, SPEAK_WHISPER, std::string("pspsps"));
			else
				(*it)->onCreatureSay(creature, SPEAK_WHISPER, text);
		}
	}
}

void Game::creatureYell(Creature *creature, std::string &text)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::creatureYell()");
	Player* player = dynamic_cast<Player*>(creature);

	if (player && player->access < g_config.ACCESS_PROTECT && player->exhaustedTicks >= 1000) 
	{
		player->exhaustedTicks += g_config.EXHAUSTED_ADD;
		player->sendTextMessage(MSG_SMALLINFO, "You are exhausted.");
	}
	else {
		creature->exhaustedTicks = g_config.EXHAUSTED;
		std::transform(text.begin(), text.end(), text.begin(), upchar);

		SpectatorVec list;
		SpectatorVec::iterator it;

		getSpectators(Range(creature->pos, 18, 18, 14, 14), list);

		//players
		for(it = list.begin(); it != list.end(); ++it) {
			if(dynamic_cast<Player*>(*it)) {
				(*it)->onCreatureSay(creature, SPEAK_YELL, text);
			}
		}

		//none-players
		for(it = list.begin(); it != list.end(); ++it) {
			if(!dynamic_cast<Player*>(*it)) {
				(*it)->onCreatureSay(creature, SPEAK_YELL, text);
			}
		}
	}
}

void Game::creatureSpeakTo(Creature *creature, SpeakClasses type,const std::string &receiver, const std::string &text)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::creatureSpeakTo");

	Player* player = dynamic_cast<Player*>(creature);
	if(!player)
		return;

	Player* toPlayer = getPlayerByName(receiver);
	if(!toPlayer) {
		player->sendTextMessage(MSG_SMALLINFO, "A player with this name is not online.");
		return;
	}

	if(creature->access < g_config.ACCESS_TALK){
		type = SPEAK_PRIVATE;
	}

	toPlayer->onCreatureSay(creature, type, text);

	std::stringstream ss;
	ss << "Message sent to " << toPlayer->getName() << ".";
	player->sendTextMessage(MSG_SMALLINFO, ss.str().c_str());
}

 
   void Game::creatureTalkToChannel(Player *player, SpeakClasses type, std::string &text, unsigned short channelId) 
   { 
           OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::creatureTalkToChannel"); 
    
         	if(player->access < g_config.ACCESS_TALK )
		type = SPEAK_CHANNEL_Y;
	else if(player->access >= g_config.ACCESS_TALK)
         type = SPEAK_CHANNEL_R1;     
         
    g_chat.talkToChannel(player, type, text, channelId); 
   } 

void Game::creatureMonsterYell(Monster* monster, const std::string& text)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::creatureMonsterYell()");

	SpectatorVec list;
	SpectatorVec::iterator it;

	map->getSpectators(Range(monster->pos, 18, 18, 14, 14), list);

	//players
	for(it = list.begin(); it != list.end(); ++it) {
		if(dynamic_cast<Player*>(*it)) {
			(*it)->onCreatureSay(monster, SPEAK_MONSTER1, text);
		}
	}
}

void Game::creatureBroadcastMessage(Creature *creature, const std::string &text)
{
	if(creature->access < g_config.ACCESS_TALK)
		return;
		
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::creatureBroadcastMessage()");

	for(AutoList<Player>::listiterator it = Player::listPlayer.list.begin(); it != Player::listPlayer.list.end(); ++it)
	{
		(*it).second->onCreatureSay(creature, SPEAK_BROADCAST, text);
	}
}

/** \todo Someone _PLEASE_ clean up this mess */
bool Game::creatureMakeMagic(Creature *creature, const Position& centerpos, const MagicEffectClass* me)
{

	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::creatureMakeMagic()");

#ifdef __DEBUG__
	cout << "creatureMakeMagic: " << (creature ? creature->getName() : "No name") << ", x: " << centerpos.x << ", y: " << centerpos.y << ", z: " << centerpos.z << std::endl;
#endif

	Position frompos;

	if(creature) {
		frompos = creature->pos;

		if(!creatureOnPrepareMagicAttack(creature, centerpos, me))
		{

			return false;
		}
	}
	else {
		frompos = centerpos;
	}

	MagicAreaVec tmpMagicAreaVec;
	me->getArea(centerpos, tmpMagicAreaVec);

	std::vector<Position> poslist;

	Position topLeft(0xFFFF, 0xFFFF, frompos.z), bottomRight(0, 0, frompos.z);

	//Filter out the tiles we actually can work on
	for(MagicAreaVec::iterator maIt = tmpMagicAreaVec.begin(); maIt != tmpMagicAreaVec.end(); ++maIt) {
		Tile *t = map->getTile(maIt->x, maIt->y, maIt->z);
		if(t && (!creature || (creature->access >= g_config.ACCESS_PROTECT || !me->offensive || !t->isPz()) ) ) {
			if((t->isBlocking(BLOCK_PROJECTILE) == RET_NOERROR) && (me->isIndirect() ||
				//(map->canThrowItemTo(frompos, (*maIt), false, true) && !t->floorChange()))) {
				((map->canThrowObjectTo(centerpos, (*maIt), BLOCK_PROJECTILE) == RET_NOERROR) && !t->floorChange()))) {

				if(maIt->x < topLeft.x)
					topLeft.x = maIt->x;

				if(maIt->y < topLeft.y)
					topLeft.y = maIt->y;

				if(maIt->x > bottomRight.x)
					bottomRight.x = maIt->x;

				if(maIt->y > bottomRight.y)
					bottomRight.y = maIt->y;

				poslist.push_back(*maIt);
			}
		}
	}

	topLeft.z = frompos.z;
	bottomRight.z = frompos.z;

	if(topLeft.x == 0xFFFF || topLeft.y == 0xFFFF || bottomRight.x == 0 || bottomRight.y == 0){

    return false;
	}

#ifdef __DEBUG__
	printf("top left %d %d %d\n", topLeft.x, topLeft.y, topLeft.z);
	printf("bottom right %d %d %d\n", bottomRight.x, bottomRight.y, bottomRight.z);
#endif

	//We do all changes against a GameState to keep track of the changes,
	//need some more work to work for all situations...
	GameState gamestate(this, Range(topLeft, bottomRight));

	//Tile *targettile = getTile(centerpos.x, centerpos.y, centerpos.z);
	Tile *targettile = map->getTile(centerpos);
	bool bSuccess = false;
	bool hasTarget = false;
	bool isBlocking = true;
	if(targettile){
		hasTarget = !targettile->creatures.empty();
		isBlocking = (targettile->isBlocking(BLOCK_SOLID, true) != RET_NOERROR);
	}

	if(targettile && me->canCast(isBlocking, !targettile->creatures.empty())) {
		bSuccess = true;

		//Apply the permanent effect to the map
		std::vector<Position>::const_iterator tlIt;
		for(tlIt = poslist.begin(); tlIt != poslist.end(); ++tlIt) {
			gamestate.onAttack(creature, Position(*tlIt), me);
		}
	}

	SpectatorVec spectatorlist = gamestate.getSpectators();
	SpectatorVec::iterator it;

	for(it = spectatorlist.begin(); it != spectatorlist.end(); ++it) {
		Player* spectator = dynamic_cast<Player*>(*it);

		if(!spectator)
			continue;

		if(bSuccess) {
			me->getDistanceShoot(spectator, creature, centerpos, hasTarget);

			std::vector<Position>::const_iterator tlIt;
			for(tlIt = poslist.begin(); tlIt != poslist.end(); ++tlIt) {
				Position pos = *tlIt;
				//Tile *tile = getTile(pos.x, pos.y, pos.z);
				Tile *tile = map->getTile(pos);
				const CreatureStateVec& creatureStateVec = gamestate.getCreatureStateList(tile);

				if(creatureStateVec.empty()) { //no targets
					me->getMagicEffect(spectator, creature, NULL, pos, 0, targettile->isPz(), isBlocking);
				}
				else {
					for(CreatureStateVec::const_iterator csIt = creatureStateVec.begin(); csIt != creatureStateVec.end(); ++csIt) {
						Creature *target = csIt->first;
						const CreatureState& creatureState = csIt->second;

if(target->getItemId() == 0) {
                            if((me->attackType & target->getImmunities()) == me->attackType) {
                                //immune = true;
                            }
                            else {
                                if(dynamic_cast<Monster*>(target))
                                creatureReturnToNormalOutfit(target);
                            }
                        }
                        
						me->getMagicEffect(spectator, creature, target, target->pos, creatureState.damage, tile->isPz(), false);

						//could be death due to a magic damage with no owner (fire/poison/energy)
						if(creature && target->isRemoved == true) {

							for(std::vector<Creature*>::const_iterator cit = creatureState.attackerlist.begin(); cit != creatureState.attackerlist.end(); ++cit) {
								Creature* gainExpCreature = *cit;
								if(dynamic_cast<Player*>(gainExpCreature))
									dynamic_cast<Player*>(gainExpCreature)->sendStats();

								if(spectator->CanSee(gainExpCreature->pos.x, gainExpCreature->pos.y, gainExpCreature->pos.z)) {
									std::stringstream exp;
									exp << target->getGainedExperience(gainExpCreature);
									spectator->sendAnimatedText(gainExpCreature->pos, 0xD7, exp.str());
								}
							}

						}

						if(spectator->CanSee(target->pos.x, target->pos.y, target->pos.z))
						{
							if(creatureState.damage != 0) {
								std::stringstream dmg;
								dmg << std::abs(creatureState.damage);
#ifdef TJ_MONSTER_BLOOD
								if (me->attackType & ATTACK_PHYSICAL)
								    spectator->sendAnimatedText(target->pos, target->bloodcolor, dmg.str());
								else
#endif //TJ_MONSTER_BLOOD
									spectator->sendAnimatedText(target->pos, me->animationColor, dmg.str());
							}

							if(creatureState.manaDamage > 0){
								spectator->sendMagicEffect(target->pos, NM_ME_LOOSE_ENERGY);
								std::stringstream manaDmg;
								manaDmg << std::abs(creatureState.manaDamage);
								spectator->sendAnimatedText(target->pos, 2, manaDmg.str());
							}

							if (target->health > 0)
								spectator->sendCreatureHealth(target);

							if (spectator == target){
								CreateManaDamageUpdate(target, creature, creatureState.manaDamage);
								CreateDamageUpdate(target, creature, creatureState.damage);
							}
						}
					}
				}
			}
		}
		else {
			me->FailedToCast(spectator, creature, isBlocking, hasTarget);
		}

	}

	return bSuccess;
}

void Game::creatureApplyDamage(Creature *creature, int damage, int &outDamage, int &outManaDamage
#ifdef YUR_PVP_ARENA
							   , CreatureVector* arenaLosers
#endif //YUR_PVP_ARENA
							   )
{
	outDamage = damage;
	outManaDamage = 0;

	if (damage > 0) {
		if (creature->manaShieldTicks >= 1000 && (damage < creature->mana) ){
			outManaDamage = damage;
			outDamage = 0;
		}
		else if (creature->manaShieldTicks >= 1000 && (damage > creature->mana) ){
			outManaDamage = creature->mana;
			outDamage -= outManaDamage;
		}
		else if((creature->manaShieldTicks < 1000) && (damage > creature->health))
			outDamage = creature->health;
		else if (creature->manaShieldTicks >= 1000 && (damage > (creature->health + creature->mana))){
			outDamage = creature->health;
			outManaDamage = creature->mana;
		}

		if(creature->manaShieldTicks < 1000 || (creature->mana == 0))
#ifdef YUR_PVP_ARENA
			creature->drainHealth(outDamage, arenaLosers);
#else
			creature->drainHealth(outDamage);
#endif //YUR_PVP_ARENA
		else if(outManaDamage > 0)
		{
#ifdef YUR_PVP_ARENA
			creature->drainHealth(outDamage, arenaLosers);
#else
			creature->drainHealth(outDamage);
#endif //YUR_PVP_ARENA
			creature->drainMana(outManaDamage);
		}
		else
			creature->drainMana(outDamage);
	}
	else {
		int newhealth = creature->health - damage;
		if(newhealth > creature->healthmax)
			newhealth = creature->healthmax;

		creature->health = newhealth;

		outDamage = creature->health - newhealth;
		outManaDamage = 0;
	}
}

bool Game::creatureCastSpell(Creature *creature, const Position& centerpos, const MagicEffectClass& me) {

	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::creatureCastSpell()");
	#ifdef BD_PARALYZE
		if(me.offensive == false && me.damageEffect > 0 && creature->conditions.hasCondition(ATTACK_PARALYZE)){
creature->removeCondition(ATTACK_PARALYZE);
changeSpeed(creature->getID(), creature->getNormalSpeed()+creature->hasteSpeed);
Player *player = dynamic_cast<Player*>(creature);
if(player)
player->sendIcons();
}
#endif //BD_PARALYZE


	return creatureMakeMagic(creature, centerpos, &me);
}

bool Game::creatureThrowRune(Creature *creature, const Position& centerpos, const MagicEffectClass& me) {

	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::creatureThrowRune()");

	bool ret = false;
	if(creature->pos.z != centerpos.z) {
		creature->sendCancel("You need to be on the same floor.");
	}
	//else if(!map->canThrowItemTo(creature->pos, centerpos, false, true)) {
	else if(map->canThrowObjectTo(creature->pos, centerpos, BLOCK_PROJECTILE) != RET_NOERROR) {
		creature->sendCancel("You cannot throw there.");
	}
	else
		ret = creatureMakeMagic(creature, centerpos, &me);



	return ret;
}

bool Game::creatureOnPrepareAttack(Creature *creature, Position pos)
{
  if(creature){
		Player* player = dynamic_cast<Player*>(creature);

		//Tile* tile = (Tile*)getTile(creature->pos.x, creature->pos.y, creature->pos.z);
		Tile* tile = map->getTile(creature->pos);
		//Tile* targettile = getTile(pos.x, pos.y, pos.z);
		Tile* targettile = map->getTile(pos);

			if(creature->access < g_config.ACCESS_PROTECT) {
			if(tile && tile->isPz()) {
				if(player) {
					player->sendTextMessage(MSG_SMALLINFO, "You may not attack a person while you are in a protection zone.");
					playerSetAttackedCreature(player, 0);
				}

				return false;
			}
			else if(targettile && targettile->isPz()) {
				if(player) {
					player->sendTextMessage(MSG_SMALLINFO, "You may not attack a person in a protection zone.");
					playerSetAttackedCreature(player, 0);
				}

				return false;
			}
		}

		return true;
	}

	return false;
}

bool Game::creatureOnPrepareMagicAttack(Creature *creature, Position pos, const MagicEffectClass* me)
{
	if(!me->offensive || me->isIndirect() || creatureOnPrepareAttack(creature, pos)) {
		/*
			if(creature->access == 0) {
				if(!((std::abs(creature->pos.x-centerpos.x) <= 8) && (std::abs(creature->pos.y-centerpos.y) <= 6) &&
					(creature->pos.z == centerpos.z)))
					return false;
			}
		*/

		Player* player = dynamic_cast<Player*>(creature);
		if(player) {
				if(player->access < g_config.ACCESS_PROTECT) {
                              long manaCost = me->manaCost;
                              #ifdef TIJN_BERSERK
                              if(me->berserk) {
manaCost = (long)(player->manamax*0.80);
}
#endif //TIJN_BERSERK


				if(player->exhaustedTicks >= 1000 && me->causeExhaustion(true)) {
					if(me->offensive) {
						player->sendTextMessage(MSG_SMALLINFO, "You are exhausted.",player->pos, NM_ME_PUFF);
						player->exhaustedTicks += (long)g_config.getGlobalNumber("exhaustedadd", 0);
					}

					return false;
				}
				else if(player->mana < manaCost) {
					player->sendTextMessage(MSG_SMALLINFO, "You do not have enough mana.",player->pos, NM_ME_PUFF);
					return false;
				}
				else
				player->mana -= manaCost;
					//player->manaspent += me->manaCost;
					player->addManaSpent(manaCost);
			}
		}

		return true;
	}

	return false;
}

void Game::creatureMakeDamage(Creature *creature, Creature *attackedCreature, fight_t damagetype, attacktype_t attack /* = NONE*/)
{
	if(!creatureOnPrepareAttack(creature, attackedCreature->pos))
		return;


	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::creatureMakeDamage()");

	Player* player = dynamic_cast<Player*>(creature);
	Player* attackedPlayer = dynamic_cast<Player*>(attackedCreature);
	
#ifdef ANTI_MISS
if(player && player->atkMode == 1 && attackedPlayer && attackedPlayer->skullKills == 0) {
player->sendCancelAttacking();
        player->sendTextMessage(MSG_SMALLINFO, "Turn secure mode off if you really want to attack unmarked players.");
        return;
	    }
#endif //ANTI_MISS
if(player && player->access >= g_config.ACCESS_PROTECT && attackedPlayer && attackedPlayer->access < g_config.ACCESS_PROTECT && g_config.getGlobalString("gmprotected","yes") == "yes") {
          player->sendCancelAttacking();
        player->sendTextMessage(MSG_SMALLINFO, "Gms cant attack players.");
        return;
	    }
	    
	//Tile* targettile = getTile(attackedCreature->pos.x, attackedCreature->pos.y, attackedCreature->pos.z);
	Tile* targettile = map->getTile(attackedCreature->pos);

	//can the attacker reach the attacked?
	bool inReach = false;

	switch(damagetype){
                       
                       
                       #ifdef CAYAN_POISONMELEE
                       case FIGHT_POISON_MELEE:
			if((std::abs(creature->pos.x-attackedCreature->pos.x) <= 1) &&
				(std::abs(creature->pos.y-attackedCreature->pos.y) <= 1) &&
				(creature->pos.z == attackedCreature->pos.z))
					inReach = true;					
	    break;
		case FIGHT_ENERGY_MELEE:
			if((std::abs(creature->pos.x-attackedCreature->pos.x) <= 1) &&
				(std::abs(creature->pos.y-attackedCreature->pos.y) <= 1) &&
				(creature->pos.z == attackedCreature->pos.z))
					inReach = true;					
	    break;
		case FIGHT_FIRE_MELEE:
			if((std::abs(creature->pos.x-attackedCreature->pos.x) <= 1) &&
				(std::abs(creature->pos.y-attackedCreature->pos.y) <= 1) &&
				(creature->pos.z == attackedCreature->pos.z))
					inReach = true;					
	    break;
	    
	    		case FIGHT_MELEE:
			if((std::abs(creature->pos.x-attackedCreature->pos.x) <= 1) &&
				(std::abs(creature->pos.y-attackedCreature->pos.y) <= 1) &&
				(creature->pos.z == attackedCreature->pos.z))
					inReach = true;
					
		break;
#else
		case FIGHT_MELEE:
			if((std::abs(creature->pos.x-attackedCreature->pos.x) <= 1) &&
				(std::abs(creature->pos.y-attackedCreature->pos.y) <= 1) &&
				(creature->pos.z == attackedCreature->pos.z))
					inReach = true;
					
		break;
#endif //CAYAN_POISONMELEE

		
		case FIGHT_DIST:
			if((std::abs(creature->pos.x-attackedCreature->pos.x) <= 8) &&
				(std::abs(creature->pos.y-attackedCreature->pos.y) <= 5) &&
				(creature->pos.z == attackedCreature->pos.z)) {

					//if(map->canThrowItemTo(creature->pos, attackedCreature->pos, false, true))
					if(map->canThrowObjectTo(creature->pos, attackedCreature->pos, BLOCK_PROJECTILE) == RET_NOERROR)
						inReach = true;
				}
		break;
		case FIGHT_MAGICDIST:
			if((std::abs(creature->pos.x-attackedCreature->pos.x) <= 8) &&
				(std::abs(creature->pos.y-attackedCreature->pos.y) <= 5) &&
				(creature->pos.z == attackedCreature->pos.z)) {

					//if(map->canThrowItemTo(creature->pos, attackedCreature->pos, false, true))
					if(map->canThrowObjectTo(creature->pos, attackedCreature->pos, BLOCK_PROJECTILE) == RET_NOERROR)
						inReach = true;
				}
		break;

	}


	if (player && player->access < g_config.ACCESS_PROTECT) {
		player->inFightTicks = (long)g_config.getGlobalNumber("pzlocked", 0);
		player->sendIcons();

		if(attackedPlayer)
			player->pzLocked = true;
	}
	if(attackedPlayer && attackedPlayer->access < g_config.ACCESS_PROTECT){
	 attackedPlayer->inFightTicks = (long)g_config.getGlobalNumber("pzlocked", 0);
	 attackedPlayer->sendIcons();
  }

	if(!inReach){
		return;
	}

	//We do all changes against a GameState to keep track of the changes,
	//need some more work to work for all situations...
	GameState gamestate(this, Range(creature->pos, attackedCreature->pos));

	gamestate.onAttack(creature, attackedCreature->pos, attackedCreature);

	const CreatureStateVec& creatureStateVec = gamestate.getCreatureStateList(targettile);
	const CreatureState& creatureState = creatureStateVec[0].second;

	if(player && (creatureState.damage > 0 || creatureState.manaDamage > 0)) {
		player->addSkillTry(1);
	}
	else if(player)
		player->addSkillTry(1);
#ifdef SURVIVAL_BLACKSQUARE
//Add the black square
if(attackedPlayer){
NetworkMessage msg;
msg.AddByte(0x86);
msg.AddU32(creature->getID());
msg.AddByte(0x00);
attackedPlayer->sendNetworkMessage(&msg);
}
#endif //SURVIVAL_BLACKSQUARE		 


	SpectatorVec spectatorlist = gamestate.getSpectators();
	SpectatorVec::iterator it;


    
	for(it = spectatorlist.begin(); it != spectatorlist.end(); ++it) {
		Player* spectator = dynamic_cast<Player*>(*it);
		if(!spectator)
			continue;

#ifdef CAYAN_POISONMELEE
if((damagetype != FIGHT_MELEE) && (damagetype != FIGHT_POISON_MELEE) && (damagetype != FIGHT_FIRE_MELEE) && (damagetype != FIGHT_ENERGY_MELEE)){
			spectator->sendDistanceShoot(creature->pos, attackedCreature->pos, creature->getSubFightType());
		}
#else
		if(damagetype != FIGHT_MELEE){
			spectator->sendDistanceShoot(creature->pos, attackedCreature->pos, creature->getSubFightType());
		}
#endif //CAYAN_POISONMELEE



		if (attackedCreature->manaShieldTicks < 1000 && (creatureState.damage == 0) &&
			(spectator->CanSee(attackedCreature->pos.x, attackedCreature->pos.y, attackedCreature->pos.z))) {
				spectator->sendMagicEffect(attackedCreature->pos, NM_ME_PUFF);
		}
		else if (attackedCreature->manaShieldTicks < 1000 && (creatureState.damage < 0) &&
			(spectator->CanSee(attackedCreature->pos.x, attackedCreature->pos.y, attackedCreature->pos.z))) {
				spectator->sendMagicEffect(attackedCreature->pos, NM_ME_BLOCKHIT);
		}
		else {
			for(std::vector<Creature*>::const_iterator cit = creatureState.attackerlist.begin(); cit != creatureState.attackerlist.end(); ++cit) {
				Creature* gainexpCreature = *cit;
				if(dynamic_cast<Player*>(gainexpCreature))
					dynamic_cast<Player*>(gainexpCreature)->sendStats();

				if(spectator->CanSee(gainexpCreature->pos.x, gainexpCreature->pos.y, gainexpCreature->pos.z)) {
					std::stringstream exp;
					exp << attackedCreature->getGainedExperience(gainexpCreature);
					spectator->sendAnimatedText(gainexpCreature->pos, 0xD7, exp.str());
				}
			}

			if (spectator->CanSee(attackedCreature->pos.x, attackedCreature->pos.y, attackedCreature->pos.z))
			#ifdef BDB_BLOOD
						Monster* attackedMonster = dynamic_cast<Monster*>(attackedCreature);
#endif //BDB_BLOOD

			{
				if(creatureState.damage > 0) {
					std::stringstream dmg;
					dmg << std::abs(creatureState.damage);
#ifdef TJ_MONSTER_BLOOD
					spectator->sendAnimatedText(attackedCreature->pos, attackedCreature->bloodcolor, dmg.str());
					spectator->sendMagicEffect(attackedCreature->pos, attackedCreature->bloodeffect);
#else
					spectator->sendAnimatedText(attackedCreature->pos, 0xB4, dmg.str());
					spectator->sendMagicEffect(attackedCreature->pos, NM_ME_DRAW_BLOOD);
#endif //TJ_MONSTER_BLOOD
				}

				if(creatureState.manaDamage >0) {
					std::stringstream manaDmg;
					manaDmg << std::abs(creatureState.manaDamage);
					spectator->sendMagicEffect(attackedCreature->pos, NM_ME_LOOSE_ENERGY);
					spectator->sendAnimatedText(attackedCreature->pos, 2, manaDmg.str());
				}

				if (attackedCreature->health > 0)
					spectator->sendCreatureHealth(attackedCreature);

				if (spectator == attackedCreature) {
					CreateManaDamageUpdate(attackedCreature, creature, creatureState.manaDamage);
					CreateDamageUpdate(attackedCreature, creature, creatureState.damage);
				}
			}
		}
	}

#ifdef CAYAN_POISONMELEE
if(player && (damagetype != FIGHT_MELEE) && (damagetype != FIGHT_POISON_MELEE) && (damagetype != FIGHT_FIRE_MELEE) && (damagetype != FIGHT_ENERGY_MELEE)) {
		player->removeDistItem();
	}
#else
	if(damagetype != FIGHT_MELEE && player) {
		player->removeDistItem();
	}
#endif //CAYAN_POISONMELEE

}

std::list<Position> Game::getPathTo(Creature *creature, Position start, Position to, bool creaturesBlock){
	return map->getPathTo(creature, start, to, creaturesBlock);
}


void Game::checkPlayerWalk(unsigned long id)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::checkPlayerWalk");

	Player *player = getPlayerByID(id);

	if(!player)
		return;

	Position pos = player->pos;
	Direction dir = player->pathlist.front();
	player->pathlist.pop_front();

	switch (dir) {
		case NORTH:
			pos.y--;
			break;
		case EAST:
			pos.x++;
			break;
		case SOUTH:
			pos.y++;
			break;
		case WEST:
			pos.x--;
			break;
		case NORTHEAST:
			pos.x++;
			pos.y--;
			break;
		case NORTHWEST:
			pos.x--;
			pos.y--;
			break;
		case SOUTHWEST:
			pos.x--;
			pos.y++;
			break;
		case SOUTHEAST:
			pos.x++;
			pos.y++;
			break;
	}

/*
#ifdef __DEBUG__
	std::cout << "move to: " << dir << std::endl;
#endif
*/

	player->lastmove = OTSYS_TIME();
	this->thingMove(player, player, pos.x, pos.y, pos.z, 1);
	flushSendBuffers();

	if(!player->pathlist.empty()) {
		int ticks = (int)player->getSleepTicks();
/*
#ifdef __DEBUG__
		std::cout << "checkPlayerWalk - " << ticks << std::endl;
#endif
*/
		player->eventAutoWalk = addEvent(makeTask(ticks, std::bind2nd(std::mem_fun(&Game::checkPlayerWalk), id)));
	}
	else
		player->eventAutoWalk = 0;
}

void Game::checkCreature(unsigned long id)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::checkCreature()");

	Creature *creature = getCreatureByID(id);

	if (creature && creature->isRemoved == false)
	{
		int thinkTicks = 0;
		int oldThinkTicks = creature->onThink(thinkTicks);

		if(thinkTicks > 0) {
			creature->eventCheck = addEvent(makeTask(thinkTicks, std::bind2nd(std::mem_fun(&Game::checkCreature), id)));
		}
		else
			creature->eventCheck = 0;
        #ifdef TIJN_UTANAVID
        if(creature->invisTicks > 0) {
        creature->invisTicks--;
        if(creature->invisTicks == 0) {
        creatureReturnToNormalOutfit(creature);
        }
        }
#endif //TIJN_UTANAVID

       if(creature->chameleonTicks > 0) {
       creature->chameleonTicks--;
       if(creature->chameleonTicks == 0) {
       creatureReturnToNormalOutfit(creature);
       }
       }
        
       Player* player = dynamic_cast<Player*>(creature);
       if(player){
       //Tile *tile = getTile(player->pos.x, player->pos.y, player->pos.z);
       Tile *tile = map->getTile(player->pos);
       if(tile == NULL){
       std::cout << "CheckPlayer NULL tile: " << player->getName() << std::endl;
       return;
       }
	
	   #ifdef BRWOLF_EXETAVIS
	   if(player->items[SLOT_RIGHT] && player->items[SLOT_RIGHT]->getID() == ITEM_MAGIC_STAFF){
       int charid = (int)player->items[SLOT_RIGHT]->getTime();
       if(charid > 0){
       int actid = (int)player->items[SLOT_RIGHT]->getActionId();
       if(actid == 0 || actid == 100) {
       int newactida = (int)player->items[SLOT_RIGHT]->getTime();
       int newactidb = newactida + 100;
       player->items[SLOT_RIGHT]->setActionId(newactidb); 
       }
       if(actid > 101){
       int time = (int)player->items[SLOT_RIGHT]->getTime();
       int newactid = time - (thinkTicks/1000);
       player->items[SLOT_RIGHT]->setActionId(newactid);
       }
       if(actid == 101){
       player->items[SLOT_RIGHT]->setID(ITEM_STAFF);
       player->sendInventory(5);
       }
       }
       }
            
       if(player->items[SLOT_LEFT] && player->items[SLOT_LEFT]->getID() == ITEM_MAGIC_STAFF){
       int charid = (int)player->items[SLOT_LEFT]->getTime();
       if(charid > 0){
       int actid = (int)player->items[SLOT_LEFT]->getActionId();
       if(actid == 0 || actid == 100) {
       int newactida = (int)player->items[SLOT_LEFT]->getTime();
       int newactidb = newactida + 100;
       player->items[SLOT_LEFT]->setActionId(newactidb); 
       }
       if(actid > 101){
       int time = (int)player->items[SLOT_LEFT]->getTime();
       int newactid = time - (thinkTicks/1000);
       player->items[SLOT_LEFT]->setActionId(newactid);
       }
       if(actid == 101){
       player->items[SLOT_LEFT]->setID(ITEM_STAFF);
       player->sendInventory(6);
       }
       }
       }
#endif //BRWOLF_EXETAVIS

#ifdef REX_MUTED
       if (player->muted > 0)
       player->muted -= 1;
       if(player->tradeTicks >= 1000)
       player->tradeTicks -= thinkTicks;
#endif //REX_MUTED

#ifdef CVS_DAY_CYCLE
			player->sendWorldLightLevel(lightlevel, 0xD7);
#endif //CVS_DAY_CYCLE
#ifdef TR_ANTI_AFK
			player->checkAfk(thinkTicks);
#endif //TR_ANTI_AF
#ifdef YUR_BOH
			player->checkBoh();
#endif //YUR_BOH
#ifdef YUR_RINGS_AMULETS
			player->checkRing(thinkTicks);	
#endif //YUR_RINGS_AMULETS
#ifdef YUR_LIGHT_ITEM
			player->checkLightItem(thinkTicks);
#endif //YUR_LIGHT_ITEM
#ifdef TLM_SKULLS_PARTY
			if (player->checkSkull(thinkTicks))
				Skull(player);
#endif //TLM_SKULLS_PARTY
            if(player->chameleonTime >= 1000) {
            player->chameleonTime -= thinkTicks;
            if(player->chameleonTime < 1000 && player->itemid > 0)
 	        creature->itemID = 0;
 	        creatureChangeOutfit(player);
 	        }
					checkSkullTime(player); 	
			if(!tile->isPz()){
				if(player->food > 1000){
					//player->mana += min(5, player->manamax - player->mana);
					player->gainManaTick();
					player->food -= thinkTicks;
					if(player->healthmax - player->health > 0){
						//player->health += min(5, player->healthmax - player->health);
						if(player->gainHealthTick()){
							SpectatorVec list;
							SpectatorVec::iterator it;
							getSpectators(Range(creature->pos), list);
							for(it = list.begin(); it != list.end(); ++it) {
								Player* p = dynamic_cast<Player*>(*it);
								if(p)
									p->sendCreatureHealth(player);
							}
						}
					}
				}
			}

			//send stast only if have changed
			if(player->NeedUpdateStats()){
				player->sendStats();
			}

			player->sendPing();

			if(player->inFightTicks >= 1000) {
				player->inFightTicks -= thinkTicks;

            if(player->inFightTicks < 1000)
            player->pzLocked = false;
            player->sendIcons();
            }		
#ifdef RUL_DRUNK
           if(player->drunkTicks >= 1000 && player->candrunk) {
           int random = random_range(1,40);
           if(random <= 25){
           creatureSay(creature, SPEAK_SAY, "Hicks!");
           Position pos = player->pos;
           int randomwalk = random_range(1,4);
              switch(randomwalk){
                  case 1:pos.x++;break;
                  case 2:pos.x--;break;
                  case 3:pos.y++;break;
                  case 4:pos.y--;break;
              }
              thingMove(player, player, pos.x, pos.y, pos.z, 1);
              player->drunkTicks -= thinkTicks;
              player->sendIcons();
           }
        }
#endif //RUL_DRUNK

			if(player->exhaustedTicks >= 1000){
			player->exhaustedTicks -= thinkTicks;
            if(player->exhaustedTicks < 0)
            player->exhaustedTicks = 0;
			}

			if(player->manaShieldTicks >=1000){
			player->manaShieldTicks -= thinkTicks;
			if(player->manaShieldTicks  < 1000)
			player->sendIcons();
			}
			
			if(player->hasteTicks >=1000){
			player->hasteTicks -= thinkTicks;
			}
			
			if(player->stealthTicks > 0){
		    creatureChangeItemId(player, 0);
			}
			
			#ifdef RUL_DRUNK
			if(player->dwarvenTicks > 0){
            player->drunkTicks = 0;
            player->sendIcons();
			}
			if(player->dwarvenTicks = 0){
            player->drunkTicks = 4000;
            player->sendIcons();
			}
#endif //RUL_DRUNK
}
		else {
			if(creature->manaShieldTicks >=1000){
				creature->manaShieldTicks -= thinkTicks;
			}

			if(creature->hasteTicks >=1000){
				creature->hasteTicks -= thinkTicks;
			}		
			
            if(creature->stealthTicks = 0){
		        creatureChangeItemId(player, 99);
		        creatureReturnToNormalOutfit(player);
			}
		}
		
/*The life ring and ring of healing is now in player.cpp gainmanatick
if(player && player->items[SLOT_RING] && player->items[SLOT_RING]->getID() == 3027){
player->mana += min(g_config.getGlobalNumber("liferingmana", 1), player->manamax - player->mana);
player->health += min(g_config.getGlobalNumber("liferinghealth", 1), player->healthmax - player->health);
}

if(player && player->items[SLOT_RING] && player->items[SLOT_RING]->getID() == 3038){
player->mana += min(g_config.getGlobalNumber("healingringmana", 1), player->manamax - player->mana);
player->health += min(g_config.getGlobalNumber("healingringhealth", 1), player->healthmax - player->health);
}
*/

		Conditions& conditions = creature->getConditions();
		for(Conditions::iterator condIt = conditions.begin(); condIt != conditions.end(); ++condIt) {
			if(condIt->first == ATTACK_FIRE || condIt->first == ATTACK_ENERGY || condIt->first == ATTACK_POISON) {
				ConditionVec &condVec = condIt->second;

				if(condVec.empty())
					continue;

				CreatureCondition& condition = condVec[0];

				if(condition.onTick(oldThinkTicks)) {
					const MagicEffectTargetCreatureCondition* magicTargetCondition =  condition.getCondition();
					Creature* c = getCreatureByID(magicTargetCondition->getOwnerID());
					creatureMakeMagic(c, creature->pos, magicTargetCondition);

					if(condition.getCount() <= 0) {
						condVec.erase(condVec.begin());
                      if(dynamic_cast<Player*>(creature))
                      player->sendIcons();
					}
				}
			}
			#ifdef BD_PARALYZE
			else if(condIt->first == ATTACK_PARALYZE){
ConditionVec &condVec = condIt->second;
if(condVec.empty())
continue;
CreatureCondition& condition = condVec[0];
if(condition.onTick(oldThinkTicks)) {
if(creature->getImmunities() != ATTACK_PARALYZE){
changeSpeed(creature->getID(), 100);
if(dynamic_cast<Player*>(creature)){
player->sendTextMessage(MSG_SMALLINFO, "You are paralyzed.");
player->sendIcons();
}
if(condition.getCount() <= 0) {
condVec.erase(condVec.begin());
changeSpeed(creature->getID(), creature->getNormalSpeed()+creature->hasteSpeed);
if(dynamic_cast<Player*>(creature)){
player->sendIcons();
}
}
}
}
}
#endif //BD_PARALYZE
		
        }
		flushSendBuffers();
	}
}



void Game::changeOutfit(unsigned long id, int looktype){

	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::changeOutfit()");

	Creature *creature = getCreatureByID(id);
	if(creature){
		creature->looktype = looktype;
		creatureChangeOutfit(creature);
	}
}

void Game::changeOutfitAfter(unsigned long id, int looktype, long time)
{
	addEvent(makeTask(time, boost::bind(&Game::changeOutfit, this,id, looktype)));
}
#ifdef BD_PARALYZE
void Game::changeSpeed(unsigned long id, unsigned short speed)
{
OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::changeSpeed()");
Creature *creature = getCreatureByID(id);
if(creature && /*creature->hasteTicks < 1000 && */creature->speed != speed)
{
creature->speed = speed;
Player* player = dynamic_cast<Player*>(creature);
if(player){
player->sendChangeSpeed(creature);
player->sendIcons();
}
SpectatorVec list;
SpectatorVec::iterator it;
getSpectators(Range(creature->pos), list);
//for(unsigned int i = 0; i < list.size(); i++)
for(it = list.begin(); it != list.end(); ++it) {
Player* p = dynamic_cast<Player*>(*it);
if(p)
p->sendChangeSpeed(creature);
}
} 
}
#endif //BD_PARALYZE

void Game::checkCreatureAttacking(unsigned long id)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::checkCreatureAttacking()");

	Creature *creature = getCreatureByID(id);
	if (creature != NULL && creature->isRemoved == false)
	{
		creature->eventCheckAttacking = 0;
			Player *player = dynamic_cast<Player*>(creature);
		Monster *monster = dynamic_cast<Monster*>(creature);
		if (monster) {
			monster->onAttack();
		}
		else {
			if (creature->attackedCreature != 0)
			{
				Creature *attackedCreature = getCreatureByID(creature->attackedCreature);
				/*
				#ifdef SKI_WAND
				Player *attacker = dynamic_cast<Player*>(creature);
bool protection = false;
if(attacker)
{
for (int slot = SLOT_RIGHT; slot <= SLOT_LEFT; slot++)
//begin Wands
if(attacker->mana < 3 && attacker->getItem(slot) && (attacker->getItem(slot)->getID() == 3012)) {
attacker->mana = 0;
attacker->sendCancelAttacking();
attacker->sendCancel("You do not have enough mana!");
}
else if(attacker->mana < 5 && attacker->getItem(slot) && (attacker->getItem(slot)->getID() == 3010)) {
attacker->mana = 0;
attacker->sendCancelAttacking();
attacker->sendCancel("You do not have enough mana!");
}
else if(attacker->mana < 8 && attacker->getItem(slot) && (attacker->getItem(slot)->getID() == 3011)) {
attacker->mana = 0;
attacker->sendCancelAttacking();
attacker->sendCancel("You do not have enough mana!");
}
else if(attacker->mana < 10 && attacker->getItem(slot) && (attacker->getItem(slot)->getID() == 3009)) {
attacker->mana = 0;
attacker->sendCancelAttacking();
attacker->sendCancel("You do not have enough mana!");
}
else if(attacker->mana < 12 && attacker->getItem(slot) && (attacker->getItem(slot)->getID() == 3013)) {
attacker->mana = 0;
attacker->sendCancelAttacking();
attacker->sendCancel("You do not have enough mana!");
}
for (int slot = SLOT_RIGHT; slot <= SLOT_LEFT; slot++)
if(attacker->vocation > 1 && attacker->getItem(slot) && (attacker->getItem(slot)->getID() == 3010 || attacker->getItem(slot)->getID() == 3011 || attacker->getItem(slot)->getID() == 3012 || attacker->getItem(slot)->getID() == 3013 || attacker->getItem(slot)->getID() == 3009)) {
attacker->sendCancelAttacking();
attacker->sendCancel("Wands can only be used by sorcerers.");
}
// end Wands
//begin Rods
else if(attacker->mana < 3 && attacker->getItem(slot) && (attacker->getItem(slot)->getID() == 3004)) {
attacker->mana = 0;
attacker->sendCancelAttacking();
attacker->sendCancel("You do not have enough mana!");
}
else if(attacker->mana < 5 && attacker->getItem(slot) && (attacker->getItem(slot)->getID() == 3003)) {
attacker->mana = 0;
attacker->sendCancelAttacking();
attacker->sendCancel("You do not have enough mana!");
}
else if(attacker->mana < 8 && attacker->getItem(slot) && (attacker->getItem(slot)->getID() == 3006)) {
attacker->mana = 0;
attacker->sendCancelAttacking();
attacker->sendCancel("You do not have enough mana!");
}
else if(attacker->mana < 10 && attacker->getItem(slot) && (attacker->getItem(slot)->getID() == 3007)) {
attacker->mana = 0;
attacker->sendCancelAttacking();
attacker->sendCancel("You do not have enough mana!");
}
else if(attacker->mana < 12 && attacker->getItem(slot) && (attacker->getItem(slot)->getID() == 3005)) {
attacker->mana = 0;
attacker->sendCancelAttacking();
attacker->sendCancel("You do not have enough mana!");
}
for (int slot = SLOT_RIGHT; slot <= SLOT_LEFT; slot++)
if(attacker->vocation == 2 && attacker->getItem(slot) && (attacker->getItem(slot)->getID() == 3005 || attacker->getItem(slot)->getID() == 3007 || attacker->getItem(slot)->getID() == 3006 || attacker->getItem(slot)->getID() == 3003 || attacker->getItem(slot)->getID() == 3004)) {
}
else if(attacker->vocation == 1 && attacker->getItem(slot) && (attacker->getItem(slot)->getID() == 3005 || attacker->getItem(slot)->getID() == 3007 || attacker->getItem(slot)->getID() == 3006 || attacker->getItem(slot)->getID() == 3003 || attacker->getItem(slot)->getID() == 3004)) {
attacker->sendCancelAttacking();
attacker->sendCancel("Rods can only be used by druids.");
}
else if(attacker->vocation == 3 && attacker->getItem(slot) && (attacker->getItem(slot)->getID() == 3005 || attacker->getItem(slot)->getID() == 3007 || attacker->getItem(slot)->getID() == 3006 || attacker->getItem(slot)->getID() == 3003 || attacker->getItem(slot)->getID() == 3004)) {
attacker->sendCancelAttacking();
attacker->sendCancel("Rods can only be used by druids.");
}
else if(attacker->vocation == 4 && attacker->getItem(slot) && (attacker->getItem(slot)->getID() == 3005 || attacker->getItem(slot)->getID() == 3007 || attacker->getItem(slot)->getID() == 3006 || attacker->getItem(slot)->getID() == 3003 || attacker->getItem(slot)->getID() == 3004)) {
attacker->sendCancelAttacking();
attacker->sendCancel("Rods can only be used by druids.");
}
// end Rods
}

#endif //SKI_WAND
*/
				if (attackedCreature)
				{
					//Tile* fromtile = getTile(creature->pos.x, creature->pos.y, creature->pos.z);
					Tile* fromtile = map->getTile(creature->pos);
					if(fromtile == NULL) {
						std::cout << "checkCreatureAttacking NULL tile: " << creature->getName() << std::endl;
						//return;
					}
					if (!attackedCreature->isAttackable() == 0 && fromtile && fromtile->isPz() && creature->access < g_config.ACCESS_PROTECT)
					{
						Player* player = dynamic_cast<Player*>(creature);
						if (player) {
							player->sendTextMessage(MSG_SMALLINFO, "You may not attack a person in a protection zone.");
							//player->sendCancelAttacking();
							playerSetAttackedCreature(player, 0);
							return;
						}
					}
            		else
					{
						if (attackedCreature != NULL && attackedCreature->isRemoved == false)
						{
#ifdef SD_BURST_ARROW
							Player* player = dynamic_cast<Player*>(creature);
							if (player && player->isUsingBurstArrows())
									burstArrow(creature, attackedCreature->pos);
#endif //SD_BURST_ARROW
#ifdef JD_WANDS
								int wandid = player->getWandId();
								if (wandid > 0)
									useWand(player, attackedCreature, wandid);
#endif //JD_WANDS
#ifdef CAYAN_POISONARROW
else if(player->isUsingPoisonArrows()){
                                    if(player->items[SLOT_AMMO] && player->items[SLOT_AMMO]->getID() == 3386){
                                        Item* Arrows = player->getItem(SLOT_AMMO);
                                        if(Arrows->getItemCountOrSubtype() > 1)
                                            Arrows->setItemCountOrSubtype(Arrows->getItemCountOrSubtype()-1);
                                        else
    								        player->removeItemInventory(SLOT_AMMO);
                                        player->sendInventory(SLOT_AMMO);
                                        poisonArrow(creature, attackedCreature->pos);
                                    }
                                }
#endif //CAYAN_POISONARROW
							this->creatureMakeDamage(creature, attackedCreature, creature->getFightType());
						}
					}
#ifdef BD_FOLLOW
     if(player){
                       if(player->followMode != 0 && attackedCreature && attackedCreature != player->oldAttackedCreature){
                          player->oldAttackedCreature = attackedCreature;
                          playerSetFollowCreature(player, attackedCreature->getID());
                    }else{
                          if(player){
                             player->oldAttackedCreature = NULL;
                             playerSetFollowCreature(player, 0);
                          }
                       }
                    }
#endif //BD_FOLLOW
					creature->eventCheckAttacking = addEvent(makeTask(1600, std::bind2nd(std::mem_fun(&Game::checkCreatureAttacking), id)));
				}
			}
		}
		flushSendBuffers();
	}


}

void Game::checkDecay(int t)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::checkDecay()");

	addEvent(makeTask(DECAY_INTERVAL, boost::bind(&Game::checkDecay,this,DECAY_INTERVAL)));

	list<decayBlock*>::iterator it;
	for(it = decayVector.begin();it != decayVector.end();){
		(*it)->decayTime -= t;
		if((*it)->decayTime <= 0){
			list<Item*>::iterator it2;
			for(it2 = (*it)->decayItems.begin(); it2 != (*it)->decayItems.end(); it2++){
				/*todo: Decaying item could be in a  container carried by a player,
				should all items have a pointer to their parent (like containers)?*/
				Item* item = *it2;
				item->isDecaying = false;
				if(item->canDecay()){
					if(item->pos.x != 0xFFFF){
						Tile *tile = map->getTile(item->pos);
						if(tile){
							Position pos = item->pos;
							Item* newitem = item->decay();

							if(newitem){
								int stackpos = tile->getThingStackPos(item);
								if(newitem == item){
									sendUpdateThing(NULL,pos,newitem,stackpos);
								}
								else{
									if(tile->removeThing(item)){
										//autoclose containers
										if(dynamic_cast<Container*>(item)){
											SpectatorVec list;
											SpectatorVec::iterator it;

											getSpectators(Range(pos, true), list);

											for(it = list.begin(); it != list.end(); ++it) {
												Player* spectator = dynamic_cast<Player*>(*it);
												if(spectator)
													spectator->onThingRemove(item);
											}
										}

										tile->insertThing(newitem, stackpos);
										sendUpdateThing(NULL,pos,newitem,stackpos);
										FreeThing(item);
									}
								}
								startDecay(newitem);
							}
							else{
								if(removeThing(NULL,pos,item)){
									FreeThing(item);
								}
							}//newitem
						}//tile
					}//pos != 0xFFFF
				}//item->canDecay()
				FreeThing(item);
			}//for it2
			delete *it;
			it = decayVector.erase(it);
		}//(*it)->decayTime <= 0
		else{
			it++;
		}
	}//for it

	flushSendBuffers();
}

void Game::startDecay(Item* item){
	if(item->isDecaying)
		return;//dont add 2 times the same item
	//get decay time
	item->isDecaying = true;
	unsigned long dtime = item->getDecayTime();
	if(dtime == 0)
		return;
	//round time
	if(dtime < DECAY_INTERVAL)
		dtime = DECAY_INTERVAL;
	dtime = (dtime/DECAY_INTERVAL)*DECAY_INTERVAL;
	item->useThing();
	//search if there are any block with this time
	list<decayBlock*>::iterator it;
	for(it = decayVector.begin();it != decayVector.end();it++){
		if((*it)->decayTime == dtime){
			(*it)->decayItems.push_back(item);
			return;
		}
	}
	//we need a new decayBlock
	decayBlock* db = new decayBlock;
	db->decayTime = dtime;
	db->decayItems.clear();
	db->decayItems.push_back(item);
	decayVector.push_back(db);
}

void Game::checkSpawns(int t)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::checkSpawns()");

	SpawnManager::instance()->checkSpawns(t);
	this->addEvent(makeTask(t, std::bind2nd(std::mem_fun(&Game::checkSpawns), t)));
}

void Game::CreateDamageUpdate(Creature* creature, Creature* attackCreature, int damage)
{
	Player* player = dynamic_cast<Player*>(creature);
	Player* attackPlayer = dynamic_cast<Player*>(attackCreature);
	if(!player)
		return;
	//player->sendStats();
	//msg.AddPlayerStats(player);
	if (damage > 0) {
		std::stringstream dmgmesg;

		if(damage == 1) {
			dmgmesg << "You lose 1 hitpoint";
		}
		else
			dmgmesg << "You lose " << damage << " hitpoints";

		if(attackPlayer) {
			dmgmesg << " due to an attack by " << attackCreature->getName();
		}
		else if(attackCreature) {
			std::string strname = attackCreature->getName();
			std::transform(strname.begin(), strname.end(), strname.begin(), (int(*)(int))tolower);
			dmgmesg << " due to an attack by a " << strname;
		}
		dmgmesg <<".";

		player->sendTextMessage(MSG_STATUS, dmgmesg.str().c_str());
		//msg.AddTextMessage(MSG_EVENT, dmgmesg.str().c_str());
	}
	if (player->isRemoved == true){
		player->sendTextMessage(MSG_ADVANCE, "You are dead.");
	}
}

void Game::CreateManaDamageUpdate(Creature* creature, Creature* attackCreature, int damage)
{
	Player* player = dynamic_cast<Player*>(creature);
	if(!player)
		return;
	//player->sendStats();
	//msg.AddPlayerStats(player);
	if (damage > 0) {
		std::stringstream dmgmesg;
		dmgmesg << "You lose " << damage << " mana";
		if(attackCreature) {
			dmgmesg << " blocking an attack by " << attackCreature->getName();
		}
		dmgmesg <<".";

		player->sendTextMessage(MSG_EVENT, dmgmesg.str().c_str());
		//msg.AddTextMessage(MSG_EVENT, dmgmesg.str().c_str());
	}
}

bool Game::creatureSaySpell(Creature *creature, const std::string &text)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::creatureSaySpell()");

	bool ret = false;

	Player* player = dynamic_cast<Player*>(creature);
	std::string temp, var;
	unsigned int loc = (uint32_t)text.find( "\"", 0 );
	if( loc != string::npos && loc >= 0){
		temp = std::string(text, 0, loc-1);
		var = std::string(text, (loc+1), text.size()-loc-1);
	}
	else {
		temp = text;
		var = std::string("");
	}

	std::transform(temp.begin(), temp.end(), temp.begin(), (int(*)(int))tolower);
#ifdef BRWOLF_EXETAVIS
    if(temp == "exeta vis") {
             if(!EnchantedStaff(player))
                 return false;
      }
#endif //BRWOLF_EXETAVIS

#ifdef RUL_PREMMYSPELLS
if(temp == "utani hur" && player && !player->premmium) {
player->sendTextMessage(MSG_SMALLINFO,"You need a premmium account for use this spell.");
return false;
}
#endif //RUL_PREMMYSPELLS

		if(creature->access >= g_config.ACCESS_PROTECT || !player){
		std::map<std::string, Spell*>::iterator sit = spells.getAllSpells()->find(temp);
		if( sit != spells.getAllSpells()->end() ) {
			sit->second->getSpellScript()->castSpell(creature, creature->pos, var);
			ret = true;
		}
	}
	else if(player){
		std::map<std::string, Spell*>* tmp = spells.getVocSpells(player->vocation);
		if(tmp){
			std::map<std::string, Spell*>::iterator sit = tmp->find(temp);
			if(sit != tmp->end()) { 
				if(player->maglevel >= sit->second->getMagLv()){
#ifdef YUR_LEARN_SPELLS
					static const bool LEARN_SPELLS = (g_config.getGlobalString("learnspells","yes") == "yes");
					if (LEARN_SPELLS && !player->knowsSpell(temp))
						ret = false;
					else
#endif //YUR_LEARN_SPELLS
					{
						sit->second->getSpellScript()->castSpell(creature, creature->pos, var);
						ret = true;
					}
				}
			}
		}
	}


	return ret;
}

void Game::playerAutoWalk(Player* player, std::list<Direction>& path)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerAutoWalk()");
#ifdef RULY_WALKTO
if(player->walkTo.type != WALK_NONE) {
        cancelWalkTo(player);
    }
#endif //RULY_WALKTO
	
	stopEvent(player->eventAutoWalk);

	if(player->isRemoved)
		return;

	player->pathlist = path;
	int ticks = (int)player->getSleepTicks();
/*
#ifdef __DEBUG__
	std::cout << "playerAutoWalk - " << ticks << std::endl;
#endif
*/

	player->eventAutoWalk = addEvent(makeTask(ticks, std::bind2nd(std::mem_fun(&Game::checkPlayerWalk), player->getID())));

	// then we schedule the movement...
  // the interval seems to depend on the speed of the char?
	//player->eventAutoWalk = addEvent(makeTask<Direction>(0, MovePlayer(player->getID()), path, 400, StopMovePlayer(player->getID())));
	//player->pathlist = path;
}

bool Game::playerUseItemEx(Player *player, const Position& posFrom,const unsigned char  stack_from,
		const Position &posTo,const unsigned char stack_to, const unsigned short itemid, bool via_walkto /* = false*/)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerUseItemEx()");

	if(player->isRemoved)
		return false;

	bool ret = false;

	Position thingpos = getThingMapPos(player, posFrom);
	Item *item = dynamic_cast<Item*>(getThing(posFrom, stack_from, player));	
	               
	if(item) {
		
		if(item->getID() == RUNE_CHAMELEON) {
            if(posFrom.x != 0xFFFF && (abs(player->pos.x - posFrom.x) > 1 || abs(player->pos.y - posFrom.y) > 1 ||
                    player->pos.z != posFrom.z))
            {
                player->sendCancel("Too far away.");
                sendMagicEffect(player->pos, NM_ME_PUFF);
                return true;
            }
            else if(posTo.x != 0xFFFF && map->canThrowObjectTo(posFrom, posTo, BLOCK_PROJECTILE) != RET_NOERROR) {		
		        player->sendCancel("You cannot throw there.");
		        sendMagicEffect(player->pos, NM_ME_PUFF);
		        return true;
	        }
	        
            Item* targetItem = dynamic_cast<Item*>(getThing(posTo, stack_to, player));
            if(targetItem) {
                creatureChangeItemId(player, targetItem->getID());
                player->chameleonTicks = 300;
                sendMagicEffect(player->pos, NM_ME_MAGIC_ENERGIE);
                autoCloseTrade(item);
                item->setItemCharge(std::max((int)item->getItemCharge() - 1, 0));
                if(item->getItemCharge() == 0) {
                    if(removeThing(player,posFrom,item)){
                        FreeThing(item);
                    }
                }
            }
            else {
                player->sendCancel("Sorry, not possible.");
                sendMagicEffect(player->pos, NM_ME_PUFF);
            }
            return true;
        }
		
	if(item) {
		//Runes
		std::map<unsigned short, Spell*>::iterator sit = spells.getAllRuneSpells()->find(item->getID());
		if(sit != spells.getAllRuneSpells()->end()) {
			if( (abs(thingpos.x - player->pos.x) > 1) || (abs(thingpos.y - player->pos.y) > 1) ) {
				player->sendCancel("Too far away...");
				ret = false;
			}
			else {
				std::string var = std::string("");
			if(player->access >= g_config.ACCESS_PROTECT || sit->second->getMagLv() <= player->maglevel)
				{
					bool success = sit->second->getSpellScript()->castSpell(player, posTo, var);
					ret = success;
					if(success) {
						autoCloseTrade(item);
						item->setItemCharge(std::max((int)item->getItemCharge() - 1, 0) );
						if(item->getItemCharge() == 0) {
							if(removeThing(player,posFrom,item)){
								FreeThing(item);
							}
						}
					}
				}
				else
				{
					player->sendCancel("You don't have the required magic level to use that rune.");
				}
			}
		}
		else{
			actions.UseItemEx(player,posFrom,stack_from,posTo,stack_to,itemid);
#ifdef RULY_WALKTO
 if(!via_walkto) {
 if(player->walkTo.type != WALK_NONE) {
            cancelWalkTo(player);
        }
        else if(player->eventAutoWalk) {
            stopEvent(player->eventAutoWalk);
        }
        
        player->walkTo.type = WALK_USEITEMEX;
        player->walkTo.from_pos = posFrom;
        player->walkTo.stack = stack_from;
        player->walkTo.to_pos = posTo;
        player->walkTo.stack = stack_to;
        player->walkTo.itemid = itemid;
        
        player->walkTo.dest_pos = getDestinationPos(player);
        
        int ticks = (int)player->getSleepTicks();
		player->eventAutoWalk = addEvent(makeTask(ticks, std::bind2nd(std::mem_fun(&Game::checkPlayerWalkTo), player->getID())));
    }
#endif //RULY_WALKTO
			
				ret = true;
        }
}
     				}
	return ret;	
}

bool Game::playerUseItem(Player *player, const Position& pos, const unsigned char stackpos, const unsigned short itemid, unsigned char index, bool via_walkto /* = false*/)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerUseItem()");
	#ifdef ART_WALKTELEPORT
		if(actions.canUse(player,pos)== TOO_FAR){
	if(player->teleportid == 1){
teleport(player,pos);
sendMagicEffect(pos,NM_ME_ENERGY_AREA);
}
}
#endif //ART_WALKTELEPORT
	if(actions.canUse(player,pos) != TOO_FAR) {
                                  
     Item* item = dynamic_cast<Item*>(getThing(pos, stackpos, player));
     
 #ifdef TLM_BED
if(itemid == BED_ID1 || itemid == BED_ID2 || itemid == BED_ID3 || itemid == BED_ID4 || itemid == BED_ID5 || itemid == BED_ID6 /*|| itemid == 1762 || itemid == BED_ID9 || itemid == BED_ID8 || itemid == BED_ID10*/){
 
 if( (abs(player->pos.x - pos.x) > 1) || (abs(player->pos.y - pos.y) > 1) || (player->pos.z != pos.z)) {
		player->sendCancel("Too far away...");
		return false;
}

if(player && !player->premmium && itemid == BED_ID1 || itemid == BED_ID2 || itemid == BED_ID3 || itemid == BED_ID4 || itemid == BED_ID5 || itemid == BED_ID6 /*|| itemid == BED_ID7 || itemid == BED_ID9 || itemid == BED_ID8 || itemid == BED_ID10*/){
		player->sendCancel("Sorry, you need a premmium account for sleep in beds");
		return false;
}

if (changeBed(pos, itemid, player->getName())){
           teleport(player, pos);
           player->sendLogout();
           return true;
       }
       else{
           player->sendCancel("Sorry, not possible.");
           return false;

    }
  }
#endif //TLM_BED


  
	if(player->isRemoved)
		return false;
		#ifdef CAYAN_SPELLBOOK
		if(itemid == ITEM_SPELL_BOOK){
            if(((abs(player->pos.x - pos.x) > 1) || (abs(player->pos.y - pos.y) > 1) || (player->pos.z != pos.z)) && player->access < ACCESSLVL_GM){
                player->sendCancel("Sorry, not possible.");
                return false;
            }                  
            else if(setSpellbookText(player, item))
                return true;
        }
#endif //CAYAN_SPELLBOOK

	actions.UseItem(player,pos,stackpos,itemid,index);
	 }
	 #ifdef RULY_WALKTO
	else if(!via_walkto) {
 if(player->walkTo.type != WALK_NONE) {
            cancelWalkTo(player);
        }
        else if(player->eventAutoWalk) {
            stopEvent(player->eventAutoWalk);
        }
        
        player->walkTo.type = WALK_USEITEM;
        player->walkTo.to_pos = pos;
        player->walkTo.stack = stackpos;
        player->walkTo.index = index;
        player->walkTo.itemid = itemid;
        
        player->walkTo.dest_pos = getDestinationPos(player);
        
        int ticks = (int)player->getSleepTicks();
		player->eventAutoWalk = addEvent(makeTask(ticks, std::bind2nd(std::mem_fun(&Game::checkPlayerWalkTo), player->getID())));
    }
#endif //RULY_WALKTO

	return true;
}
#ifdef RULY_WALKTO
void Game::cancelWalkTo(Player* player)
{
    OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::cancelWalkTo()");
    
    player->sendCancelWalk();
    if(player->eventAutoWalk) {
        stopEvent(player->eventAutoWalk);
    }
    player->walkTo.type = WALK_NONE;
}

Position Game::getDestinationPos(Player* player)
{
     OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::getDestinationPos()");
     
     Position dest_pos;
     std::vector<Position> dest_list;
     Tile* tile = NULL;
     for(int dx = player->walkTo.to_pos.x - 1; dx <= player->walkTo.to_pos.x + 1; dx++) {
         for(int dy = player->walkTo.to_pos.y - 1; dy <= player->walkTo.to_pos.y + 1; dy++) {
             if(!(dx == player->walkTo.to_pos.x && dy == player->walkTo.to_pos.y)) {
                 tile = getTile(dx, dy, player->walkTo.to_pos.z);
                 if(tile) {
                     if(tile->isBlocking(BLOCK_SOLID, true) == RET_NOERROR) {
                         dest_list.push_back(Position(dx, dy, player->walkTo.to_pos.z));
                     }
                 }
             }      
         }
     }
     
     if(dest_list.empty()) {
         return Position(0xFFFF, 0xFFFF, 0xFF);
     }
     else {
         return dest_list.at(0);
         //TODO: get best pos
         /*std::list<Position>::iterator pos;
         for(pos = dest_list.begin(); pos != dest_list.end(); ++pos) {
             if(pos != dest_list.end()) {
                 return (*pos);
                 if(dest_pos == 0) {
                     dest_pos = (*pos);
                 }
                 
                 if(dest_pos != (*pos)) {
                     if(player->walkTo.to_pos.x < player->pos.x) {
                         //right
                         if((*pos)->x-1 ==
                     }
                 }
                 if(player->walkTo.to_pos.x < player->pos.x) if((*pos)->x-1 == player->walkTo.to_pos.x) dest_pos = (*pos);
                 else if(player->walkTo.to_pos.x > player->pos.x) if((*pos)->x+1 == player->walkTo.to_pos.x) dest_pos = (*pos);
                 else if((*pos)->x == player->walkTo.to_pos.x) dest_pos = (*pos);
     
                 if(player->walkTo.to_pos.y < player->pos.y) //down
                 else if(player->walkTo.to_pos.y > player->pos.y) //up
                 else //y
             }
         }*/
     }
}

void Game::checkPlayerWalkTo(unsigned long id)
{
    OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::checkPlayerWalkTo()");
    
    Player* player = getPlayerByID(id);
    if(!player)
        return;
    
    int ticks = 0, dy = 0, dx = 0;
    Position nextStep;
    std::list<Position> route;
        
    switch((int)player->walkTo.type)
    {
        case WALK_USEITEM:
             if(abs(player->walkTo.to_pos.x - player->pos.x) <= 1 && 
                     abs(player->walkTo.to_pos.y - player->pos.y) <= 1 &&
                     player->pos.z == player->walkTo.to_pos.z) {
                 playerUseItem(player, player->walkTo.to_pos, player->walkTo.stack, player->walkTo.itemid, player->walkTo.index, true);
                 player->walkTo.type = WALK_NONE;
                 flushSendBuffers();
                 return;
             }
             
             //TODO: save route
             route = getPathTo(player, player->pos, player->walkTo.dest_pos);
             if(route.size() == 0) {
                 cancelWalkTo(player);
                 player->sendCancel("There is no way.");
                 return;
             }
             else { 
                 route.pop_front();
             }
             
             nextStep = route.front();
             route.pop_front();
             
             dx = nextStep.x - player->pos.x;
             dy = nextStep.y - player->pos.y;
             
             player->lastmove = OTSYS_TIME();
             
             thingMove(player, player, player->pos.x + dx, player->pos.y + dy, player->pos.z, 1);
             flushSendBuffers();
             
            
             ticks = (int)player->getSleepTicks();
		     player->eventAutoWalk = addEvent(makeTask(ticks, std::bind2nd(std::mem_fun(&Game::checkPlayerWalkTo), id)));
             break;
       case WALK_USEITEMEX:
           if(abs(player->walkTo.to_pos.x - player->pos.x) <= 1 && 
                     abs(player->walkTo.to_pos.y - player->pos.y) <= 1 &&
                     player->pos.z == player->walkTo.to_pos.z) {
                 playerUseItemEx(player, player->walkTo.from_pos, player->walkTo.stack, player->walkTo.to_pos, player->walkTo.stack, player->walkTo.itemid, true);
                 player->walkTo.type = WALK_NONE;
                 flushSendBuffers();
                 return;
             }
             
             //TODO: save route
             route = getPathTo(player, player->pos, player->walkTo.dest_pos);
             if(route.size() == 0) {
                 cancelWalkTo(player);
                 //player->sendCancel("There is no way.");
                 return;
             }
             else { 
                 route.pop_front();
             }
             
             nextStep = route.front();
             route.pop_front();
             
             dx = nextStep.x - player->pos.x;
             dy = nextStep.y - player->pos.y;
             
             player->lastmove = OTSYS_TIME();
             
             thingMove(player, player, player->pos.x + dx, player->pos.y + dy, player->pos.z, 1);
             flushSendBuffers();
             
            
             ticks = (int)player->getSleepTicks();
		     player->eventAutoWalk = addEvent(makeTask(ticks, std::bind2nd(std::mem_fun(&Game::checkPlayerWalkTo), id)));
             break;
        case WALK_THROWITEM:
             //check ground to ground fuction
           break;
        case WALK_NONE:
             //lol wtf
             break;
        default:break;
    }
}
#endif //RULY_WALKTO


bool Game::playerUseBattleWindow(Player *player, Position &posFrom, unsigned char stackpos, unsigned short itemid, unsigned long creatureid)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerUseBattleWindow");

	if(player->isRemoved)
		return false;

	Creature *creature = getCreatureByID(creatureid);
	if(!creature || dynamic_cast<Player*>(creature))
		return false;

	if(std::abs(creature->pos.x - player->pos.x) > 7 || std::abs(creature->pos.y - player->pos.y) > 5 || creature->pos.z != player->pos.z)
		return false;

	bool ret = false;

	Position thingpos = getThingMapPos(player, posFrom);
	Item *item = dynamic_cast<Item*>(getThing(posFrom, stackpos, player));
	Position posTo = creature->pos;
	
	if(item) {
/*
if(item->getID() == RUNE_CONVINCE)
        {
            if(posTo.x == 0xFFFF)
            {
                player->sendCancel("You can only use this rune on creatures.");
                sendMagicEffect(player->pos, NM_ME_PUFF);
                return true;
            }
            else if(posFrom.x != 0xFFFF && (abs(player->pos.x - posFrom.x) > 1 || abs(player->pos.y - posFrom.y) > 1 ||
                    player->pos.z != posFrom.z))
            {
                player->sendCancel("Too far away.");
                sendMagicEffect(player->pos, NM_ME_PUFF);
                return true;
            }
            else if(map->canThrowObjectTo(posFrom, posTo, BLOCK_PROJECTILE) != RET_NOERROR) {		
		        player->sendCancel("You cannot throw there.");
		        sendMagicEffect(player->pos, NM_ME_PUFF);
		        return true;
	        }
	        
            Tile *t = getTile(posTo.x, posTo.y, posTo.z);
            if(t)
            {
                Monster* m = dynamic_cast<Monster*>(t->getTopCreature());
                if(m)
                {
                    if(m->pos.z != player->pos.z) {	
		                player->sendCancel("You need to be on the same floor.");
		                sendMagicEffect(player->pos, NM_ME_PUFF);
		                return true;
	                }
	                
                        if(player->getSummonCount() >= 2 && player->access == 0)
                        {
                            player->sendCancel("You already have two summons.");
                            sendMagicEffect(player->pos, NM_ME_PUFF);
                            return true;
                        }
                        else if(m->mana != 0)
                        {
                            if(m->isSummon() && m->getMaster() && m->getMaster() == player) {
                                player->sendCancel("You are already a master of that creature.");
                                sendMagicEffect(player->pos, NM_ME_PUFF);
                                return true;
                            }
                            
                                m->setAttackedCreature(NULL);
                                autoCloseTrade(item);
                                player->addSummon(m);
                                sendMagicEffect(m->pos, NM_ME_MAGIC_BLOOD);
                                item->setItemCharge(std::max((int)item->getItemCharge() - 1, 0));
    	                        if(item->getItemCharge() == 0) {
                                    if(removeThing(player,posFrom,item)){
    								    FreeThing(item);
    							    }
                                }
                                return true;
                            }
                        }
                        else {
                            player->sendCancel("You cannot convince this creature.");
                            sendMagicEffect(player->pos, NM_ME_PUFF);
                            return true;
                        }
                }
                else {
                    player->sendCancel("You can only use this rune on creatures.");
                    sendMagicEffect(player->pos, NM_ME_PUFF);
                    return true;
                }
            }
            else {
                return true;
            }
            return true;
        }
        */
        //Runes
		std::map<unsigned short, Spell*>::iterator sit = spells.getAllRuneSpells()->find(item->getID());
		if(sit != spells.getAllRuneSpells()->end()) {
			if( (abs(thingpos.x - player->pos.x) > 1) || (abs(thingpos.y - player->pos.y) > 1) ) {
				player->sendCancel("Too far away.");
			}
			else {
				std::string var = std::string("");
				if(player->access >= g_config.ACCESS_PROTECT || sit->second->getMagLv() <= player->maglevel)
				{
					bool success = sit->second->getSpellScript()->castSpell(player, creature->pos, var);
					ret = success;
					if(success){
						autoCloseTrade(item);
						item->setItemCharge(std::max((int)item->getItemCharge() - 1, 0) );
						if(item->getItemCharge() == 0){
							if(removeThing(player,posFrom,item)){
								FreeThing(item);
							}
						}
					}
				}
				else
				{
					player->sendCancel("You don't have the required magic level to use that rune.");
				}
			}
}
}
	return ret;
}



bool Game::playerRotateItem(Player *player, const Position& pos, const unsigned char stackpos, const unsigned short itemid)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerRotateItem()");

	if(player->isRemoved)
		return false;

	Item *item = dynamic_cast<Item*>(getThing(pos, stackpos, player));
	if(item && item->rotate()){
		sendUpdateThing(player, pos, item, stackpos);
	}

	return false;
}

void Game::playerRequestTrade(Player* player, const Position& pos,
	const unsigned char stackpos, const unsigned short itemid, unsigned long playerid)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerRequestTrade()");

	if(player->isRemoved)
		return;

	Player *tradePartner = getPlayerByID(playerid);
	if(!tradePartner || tradePartner == player) {
		player->sendTextMessage(MSG_INFO, "Sorry, not possible.");
		return;
	}

	if(player->tradeState != TRADE_NONE && !(player->tradeState == TRADE_ACKNOWLEDGE && player->tradePartner == playerid)) {
		player->sendCancel("You are already trading.");
		return;
	}
	else if(tradePartner->tradeState != TRADE_NONE && tradePartner->tradePartner != player->getID()) {
		player->sendCancel("This player is already trading.");
		return;
	}

	Item *tradeItem = dynamic_cast<Item*>(getThing(pos, stackpos, player));
	if(!tradeItem || tradeItem->getID() != itemid || !tradeItem->isPickupable()) {
		player->sendCancel("Sorry, not possible.");
		return;
	}

	if(!player->removeItem(tradeItem, true)) {
		/*if( (abs(player->pos.x - pos.x) > 1) || (abs(player->pos.y - pos.y) > 1) ) {
			player->sendCancel("To far away...");
			return;
		}*/
		player->sendCancel("Sorry, not possible.");
		return;
	}

	std::map<Item*, unsigned long>::const_iterator it;
	const Container* container = NULL;
	for(it = tradeItems.begin(); it != tradeItems.end(); it++) {
		if(tradeItem == it->first ||
			((container = dynamic_cast<const Container*>(tradeItem)) && container->isHoldingItem(it->first)) ||
			((container = dynamic_cast<const Container*>(it->first)) && container->isHoldingItem(tradeItem)))
		{
			player->sendTextMessage(MSG_INFO, "This item is already beeing traded.");
			return;
		}
	}

	Container* tradeContainer = dynamic_cast<Container*>(tradeItem);
	if(tradeContainer && tradeContainer->getItemHoldingCount() + 1 > 100){
		player->sendTextMessage(MSG_INFO, "You can not trade more than 100 items.");
		return;
	}

	player->tradePartner = playerid;
	player->tradeItem = tradeItem;
	player->tradeState = TRADE_INITIATED;
	tradeItem->useThing();
	tradeItems[tradeItem] = player->getID();

	player->sendTradeItemRequest(player, tradeItem, true);

	if(tradePartner->tradeState == TRADE_NONE){
		std::stringstream trademsg;
		trademsg << player->getName() <<" wants to trade with you.";
		tradePartner->sendTextMessage(MSG_INFO, trademsg.str().c_str());
		tradePartner->tradeState = TRADE_ACKNOWLEDGE;
		tradePartner->tradePartner = player->getID();
	}
	else {
		Item* counterOfferItem = tradePartner->tradeItem;
		player->sendTradeItemRequest(tradePartner, counterOfferItem, false);
		tradePartner->sendTradeItemRequest(player, tradeItem, false);
	}
}

void Game::playerAcceptTrade(Player* player)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerAcceptTrade()");

	if(player->isRemoved)
		return;

	player->setAcceptTrade(true);
	Player *tradePartner = getPlayerByID(player->tradePartner);
	if(tradePartner && tradePartner->getAcceptTrade()) {
		Item *tradeItem1 = player->tradeItem;
		Item *tradeItem2 = tradePartner->tradeItem;

		std::map<Item*, unsigned long>::iterator it;

		it = tradeItems.find(tradeItem1);
		if(it != tradeItems.end()) {
			FreeThing(it->first);
			tradeItems.erase(it);
		}

		it = tradeItems.find(tradeItem2);
		if(it != tradeItems.end()) {
			FreeThing(it->first);
			tradeItems.erase(it);
		}

		player->setAcceptTrade(false);
		tradePartner->setAcceptTrade(false);
		player->sendCloseTrade();
		tradePartner->sendCloseTrade();

		if(player->addItem(tradeItem2, true) && tradePartner->addItem(tradeItem1, true) &&
			player->removeItem(tradeItem1, true) && tradePartner->removeItem(tradeItem2, true)){

			player->removeItem(tradeItem1);
			tradePartner->removeItem(tradeItem2);

			player->onThingRemove(tradeItem1);
			tradePartner->onThingRemove(tradeItem2);

			player->addItem(tradeItem2);
			tradePartner->addItem(tradeItem1);
		}
		else{
			player->sendTextMessage(MSG_SMALLINFO, "Sorry not possible.");
			tradePartner->sendTextMessage(MSG_SMALLINFO, "Sorry not possible.");
		}
	}
}

void Game::playerLookInTrade(Player* player, bool lookAtCounterOffer, int index)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerLookInTrade()");

	Player *tradePartner = getPlayerByID(player->tradePartner);
	if(!tradePartner)
		return;

	Item *tradeItem = NULL;

	if(lookAtCounterOffer)
		tradeItem = tradePartner->getTradeItem();
	else
		tradeItem = player->getTradeItem();

	if(!tradeItem)
		return;

	if(index == 0) {
		stringstream ss;
		ss << "You see " << tradeItem->getDescription(true);
		player->sendTextMessage(MSG_INFO, ss.str().c_str());
		return;
	}

	Container *tradeContainer = dynamic_cast<Container*>(tradeItem);
	if(!tradeContainer || index > tradeContainer->getItemHoldingCount())
		return;

	bool foundItem = false;
	std::list<const Container*> stack;
	stack.push_back(tradeContainer);

	ContainerList::const_iterator it;

	while(!foundItem && stack.size() > 0) {
		const Container *container = stack.front();
		stack.pop_front();

		for (it = container->getItems(); it != container->getEnd(); ++it) {
			Container *container = dynamic_cast<Container*>(*it);
			if(container) {
				stack.push_back(container);
			}

			--index;
			if(index == 0) {
				tradeItem = *it;
				foundItem = true;
				break;
			}
		}
	}

	if(foundItem) {
		stringstream ss;
		ss << "You see " << tradeItem->getDescription(true);
		player->sendTextMessage(MSG_INFO, ss.str().c_str());
	}
}

void Game::playerCloseTrade(Player* player)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerCloseTrade()");

	Player* tradePartner = getPlayerByID(player->tradePartner);

	std::vector<Item*>::iterator it;
	if(player->getTradeItem()) {
		std::map<Item*, unsigned long>::iterator it = tradeItems.find(player->getTradeItem());
		if(it != tradeItems.end()) {
			FreeThing(it->first);
			tradeItems.erase(it);
		}
	}

	player->setAcceptTrade(false);
	player->sendTextMessage(MSG_SMALLINFO, "Trade cancelled.");
	player->sendCloseTrade();

	if(tradePartner) {
		if(tradePartner->getTradeItem()) {
			std::map<Item*, unsigned long>::iterator it = tradeItems.find(tradePartner->getTradeItem());
			if(it != tradeItems.end()) {
				FreeThing(it->first);
				tradeItems.erase(it);
			}
		}

		tradePartner->setAcceptTrade(false);
		tradePartner->sendTextMessage(MSG_SMALLINFO, "Trade cancelled.");
		tradePartner->sendCloseTrade();
	}
}

void Game::autoCloseTrade(const Item* item, bool itemMoved /*= false*/)
{
	if(!item)
		return;

	std::map<Item*, unsigned long>::const_iterator it;
	const Container* container = NULL;
	for(it = tradeItems.begin(); it != tradeItems.end(); it++) {
		if(item == it->first ||
			(itemMoved && (container = dynamic_cast<const Container*>(item)) && container->isHoldingItem(it->first)) ||
			((container = dynamic_cast<const Container*>(it->first)) && container->isHoldingItem(item)))
		{
			Player* player = getPlayerByID(it->second);
			if(player){
				playerCloseTrade(player);
			}

			break;
		}
	}
}

void Game::autoCloseAttack(Player* player, Creature* target)
{
  if((std::abs(player->pos.x - target->pos.x) > 7) ||
  (std::abs(player->pos.y - target->pos.y) > 5) || (player->pos.z != target->pos.z)){
	  player->sendTextMessage(MSG_SMALLINFO, "Target lost.");
	  playerSetAttackedCreature(player, 0);
  }
}

void Game::playerSetAttackedCreature(Player* player, unsigned long creatureid)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerSetAttackedCreature()");

	if(player->isRemoved)
		return;

	if(player->attackedCreature != 0 && creatureid == 0) {
		player->sendCancelAttacking();
	}


	Creature* attackedCreature = NULL;
	if(creatureid != 0) {
		attackedCreature = getCreatureByID(creatureid);
	}

	Player* attackedPlayer = dynamic_cast<Player*>(attackedCreature);
	bool pvpArena = false, rook = false, attackedIsSummon = false;

#ifdef YUR_PVP_ARENA
	if (player && attackedCreature)
	{
		Tile *t1 = map->getTile(player->pos), *t2 = map->getTile(attackedCreature->pos);
		pvpArena = t1 && t2 && t1->isPvpArena() && t2->isPvpArena();
	}
#endif //YUR_PVP_ARENA

#ifdef YUR_ROOKGARD
	rook = player && player->isRookie() && attackedPlayer && attackedPlayer->isRookie();
#endif //YUR_ROOKGARD

#ifdef TR_SUMMONS
	attackedIsSummon = (attackedCreature && attackedCreature->isPlayersSummon() && attackedCreature->getMaster() != player);
#endif //TR_SUMMONS

	if(!attackedCreature || (attackedCreature->access >= g_config.ACCESS_PROTECT || ((getWorldType() == WORLD_TYPE_NO_PVP || rook) &&
		!pvpArena && player->access < g_config.ACCESS_PROTECT && (dynamic_cast<Player*>(attackedCreature) || attackedIsSummon)))) {
	if(attackedCreature) {
		  player->sendTextMessage(MSG_SMALLINFO, "You may not attack this player.");
    }
		player->sendCancelAttacking();
		player->setAttackedCreature(NULL);
		stopEvent(player->eventCheckAttacking);
		player->eventCheckAttacking = 0;
	}
	else if(attackedCreature) {
		player->setAttackedCreature(attackedCreature);
		stopEvent(player->eventCheckAttacking);
		player->eventCheckAttacking = addEvent(makeTask(1600, std::bind2nd(std::mem_fun(&Game::checkCreatureAttacking), player->getID())));
	}

}

bool Game::requestAddVip(Player* player, const std::string &vip_name)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::requestAddVip");
	std::string real_name;
	real_name = vip_name;
	unsigned long guid;
	unsigned long access_lvl;

	if(!IOPlayer::instance()->getGuidByName(guid, access_lvl, real_name)){
		player->sendTextMessage(MSG_SMALLINFO, "A player with that name doesn't exist.");
		return false;
	}
	if(access_lvl > (unsigned long)player->access){
		player->sendTextMessage(MSG_SMALLINFO, "You can not add this player.");
		return false;
	}
	bool online = (getPlayerByName(real_name) != NULL);
	return player->addVIP(guid, real_name, online);
}

void Game::flushSendBuffers()
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::flushSendBuffers()");

	for(std::vector<Player*>::iterator it = BufferedPlayers.begin(); it != BufferedPlayers.end(); ++it) {
		(*it)->flushMsg();
		(*it)->SendBuffer = false;
		(*it)->releaseThing();
/*
#ifdef __DEBUG__
		std::cout << "flushSendBuffers() - releaseThing()" << std::endl;
#endif
*/
		}
	BufferedPlayers.clear();

	//free memory
	for(std::vector<Thing*>::iterator it = ToReleaseThings.begin(); it != ToReleaseThings.end(); ++it){
		(*it)->releaseThing();
	}
	ToReleaseThings.clear();


	return;
}

void Game::addPlayerBuffer(Player* p)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::addPlayerBuffer()");

/*
#ifdef __DEBUG__
	std::cout << "addPlayerBuffer() - useThing()" << std::endl;
#endif
*/
	if(p->SendBuffer == false){
		p->useThing();
		BufferedPlayers.push_back(p);
		p->SendBuffer = true;
	}

	return;
}

void Game::FreeThing(Thing* thing){

	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::FreeThing()");
	//std::cout << "freeThing() " << thing <<std::endl;
	ToReleaseThings.push_back(thing);

	return;
}
/*
ADD
container(player,pos-cid,thing)
inventory(player,pos-i,[ignored])
ground([ignored],postion,thing)

REMOVE
container(player,pos-cid,thing,autoclose?)
inventory(player,pos-i,thing,autoclose?)
ground([ignored],postion,thing,autoclose?,stackpos)

UPDATE
container(player,pos-cid,thing)
inventory(player,pos-i,[ignored])
ground([ignored],postion,thing,stackpos)
*/
void Game::sendAddThing(Player* player,const Position &pos,const Thing* thing){
	if(pos.x == 0xFFFF) {
		if(!player)
			return;
		if(pos.y & 0x40) { //container
			if(!thing)
				return;

			const Item *item = dynamic_cast<const Item*>(thing);
			if(!item)
				return;

			unsigned char containerid = pos.y & 0x0F;
			Container* container = player->getContainer(containerid);
			if(!container)
				return;

			SpectatorVec list;
			SpectatorVec::iterator it;

			Position centerpos = (container->pos.x == 0xFFFF ? player->pos : container->pos);
			getSpectators(Range(centerpos,2,2,2,2,false), list);

			if(!list.empty()) {
				for(it = list.begin(); it != list.end(); ++it) {
					Player *spectator = dynamic_cast<Player*>(*it);
					if(spectator)
						spectator->onItemAddContainer(container,item);
				}
			}
			else
				player->onItemAddContainer(container,item);

		}
		else //inventory
		{
			player->sendInventory(pos.y);
		}
	}
	else //ground
	{
		if(!thing)
			return;

		SpectatorVec list;
		SpectatorVec::iterator it;

		getSpectators(Range(pos,true), list);

		//players
		for(it = list.begin(); it != list.end(); ++it) {
			if(dynamic_cast<Player*>(*it)) {
				(*it)->onThingAppear(thing);
			}
		}

		//none-players
		for(it = list.begin(); it != list.end(); ++it) {
			if(!dynamic_cast<Player*>(*it)) {
				(*it)->onThingAppear(thing);
			}
		}
	}
}

void Game::sendRemoveThing(Player* player,const Position &pos,const Thing* thing,const unsigned char stackpos /*=1*/ ,const bool autoclose/* =false*/){
	if(!thing)
		return;

	const Item *item = dynamic_cast<const Item*>(thing);
	bool perform_autoclose = false;
	if(autoclose && item){
		const Container *container = dynamic_cast<const Container*>(item);
		if(container)
			perform_autoclose = true;
	}

	if(pos.x == 0xFFFF) {
		if(!player)
			return;
		if(pos.y & 0x40) { //container
			if(!item)
				return;

			unsigned char containerid = pos.y & 0x0F;
			Container* container = player->getContainer(containerid);
			if(!container)
				return;

			//check that item is in the container
			unsigned char slot = container->getSlotNumberByItem(item);

			SpectatorVec list;
			SpectatorVec::iterator it;

			Position centerpos = (container->pos.x == 0xFFFF ? player->pos : container->pos);
			getSpectators(Range(centerpos,2,2,2,2,false), list);

			if(!list.empty()) {
				for(it = list.begin(); it != list.end(); ++it) {
					Player *spectator = dynamic_cast<Player*>(*it);
					if(spectator){
						spectator->onItemRemoveContainer(container,slot);
						if(perform_autoclose){
							spectator->onThingRemove(thing);
						}
					}
				}
			}
			else{
				player->onItemRemoveContainer(container,slot);
				if(perform_autoclose){
					player->onThingRemove(thing);
				}
			}

		}
		else //inventory
		{
			player->removeItemInventory(pos.y);
			if(perform_autoclose){
				player->onThingRemove(thing);
			}
		}
	}
	else //ground
	{
		SpectatorVec list;
		SpectatorVec::iterator it;

		getSpectators(Range(pos,true), list);

		//players
		for(it = list.begin(); it != list.end(); ++it) {
			Player *spectator = dynamic_cast<Player*>(*it);
			if(spectator) {
				spectator->onThingDisappear(thing,stackpos);

				if(perform_autoclose){
					spectator->onThingRemove(thing);
				}
			}
		}

		//none-players
		for(it = list.begin(); it != list.end(); ++it) {
			if(!dynamic_cast<Player*>(*it)) {
				(*it)->onThingDisappear(thing,stackpos);
			}
		}
	}
}

void Game::sendUpdateThing(Player* player,const Position &pos,const Thing* thing,const unsigned char stackpos/*=1*/){

	if(pos.x == 0xFFFF) {
		if(!player)
			return;
		if(pos.y & 0x40) { //container
			if(!thing)
				return;

			const Item *item = dynamic_cast<const Item*>(thing);
			if(!item)
				return;

			unsigned char containerid = pos.y & 0x0F;
			Container* container = player->getContainer(containerid);
			if(!container)
				return;
			//check that item is in the container
			unsigned char slot = container->getSlotNumberByItem(item);

			SpectatorVec list;
			SpectatorVec::iterator it;

			Position centerpos = (container->pos.x == 0xFFFF ? player->pos : container->pos);
			getSpectators(Range(centerpos,2,2,2,2,false), list);

			if(!list.empty()) {
				for(it = list.begin(); it != list.end(); ++it) {
					Player *spectator = dynamic_cast<Player*>(*it);
					if(spectator)
						spectator->onItemUpdateContainer(container,item,slot);
				}
			}
			else{
				//never should be here
				std::cout << "Error: sendUpdateThing" << std::endl;
				//player->onItemUpdateContainer(container,item,slot);
			}

		}
		else //inventory
		{
			player->sendInventory(pos.y);
		}
	}
	else //ground
	{
		if(!thing)
			return;

		SpectatorVec list;
		SpectatorVec::iterator it;

		getSpectators(Range(pos,true), list);

		//players
		for(it = list.begin(); it != list.end(); ++it) {
			if(dynamic_cast<Player*>(*it)) {
				(*it)->onThingTransform(thing,stackpos);
			}
		}

		//none-players
		for(it = list.begin(); it != list.end(); ++it) {
			if(!dynamic_cast<Player*>(*it)) {
				(*it)->onThingTransform(thing,stackpos);
			}
		}
	}
}
void Game::addThing(Player* player,const Position &pos,Thing* thing)
{
	if(!thing)
		return;
	Item *item = dynamic_cast<Item*>(thing);

	if(pos.x == 0xFFFF) {
		if(!player || !item)
			return;

		if(pos.y & 0x40) { //container
			unsigned char containerid = pos.y & 0x0F;
			Container* container = player->getContainer(containerid);
			if(!container)
				return;

			container->addItem(item);
			sendAddThing(player,pos,thing);
		}
		else //inventory
		{
			player->addItemInventory(item,pos.y,true);
			sendAddThing(player,pos,thing);
		}
	}
	else //ground
	{
		if(!thing)
			return;
		//Tile *tile = map->getTile(pos.x, pos.y, pos.z);
		Tile *tile = map->getTile(pos);
		if(tile){
			thing->pos = pos;
			if(item && item->isSplash()){
				if(tile->splash){
					int oldstackpos = tile->getThingStackPos(tile->splash);
					Item *oldsplash = tile->splash;

					oldsplash->isRemoved = true;
					FreeThing(oldsplash);

					tile->splash = item;

					sendUpdateThing(NULL, pos, item, oldstackpos);
				}
				else{
					tile->splash = item;
					sendAddThing(NULL,pos,tile->splash);
				}
			}
			else if(item && item->isGroundTile()){
				tile->ground = item;

				SpectatorVec list;
				SpectatorVec::iterator it;

				getSpectators(Range(thing->pos, true), list);

				//players
				for(it = list.begin(); it != list.end(); ++it) {
					if(dynamic_cast<Player*>(*it)) {
						(*it)->onTileUpdated(pos);
					}
				}

				//none-players
				for(it = list.begin(); it != list.end(); ++it) {
					if(!dynamic_cast<Player*>(*it)) {
						(*it)->onTileUpdated(pos);
					}
				}

				//Game::creatureBroadcastTileUpdated(thing->pos);
			}
			else if(item && item->isStackable()){
				Item *topitem = tile->getTopDownItem();
				if(topitem && topitem->getID() == item->getID() &&
				  topitem->getItemCountOrSubtype() + item->getItemCountOrSubtype() <= 100){
					topitem->setItemCountOrSubtype(topitem->getItemCountOrSubtype() + item->getItemCountOrSubtype());
					int stackpos = tile->getThingStackPos(topitem);
					sendUpdateThing(NULL,topitem->pos,topitem,stackpos);
					item->pos.x = 0xFFFF;
					FreeThing(item);
				}
				else{
					tile->addThing(thing);
					sendAddThing(player,pos,thing);
				}
			}
			else{
				tile->addThing(thing);
				sendAddThing(player,pos,thing);
			}
		}
	}
}

bool Game::removeThing(Player* player,const Position &pos,Thing* thing,  bool setRemoved /*= true*/)
{
	if(!thing)
		return false;
	Item *item = dynamic_cast<Item*>(thing);

	if(pos.x == 0xFFFF) {
		if(!player || !item)
			return false;

		if(pos.y & 0x40) { //container
			unsigned char containerid = pos.y & 0x0F;
			Container* container = player->getContainer(containerid);
			if(!container)
				return false;

			sendRemoveThing(player,pos,thing,0,true);
			if(!container->removeItem(item))
				return false;

			if(player && player->isHoldingContainer(container)) {
				player->updateInventoryWeigth();
				player->sendStats();
			}
		}
		else //inventory
		{
			//sendRemoveThing(player,pos,thing,0,true);
			if(!player->removeItemInventory(pos.y))
				return false;
			player->onThingRemove(thing);
			//player->removeItemInventory(pos.y,true);
		}
		if(setRemoved)
			item->isRemoved = true;
		return true;
	}
	else //ground
	{
		//Tile *tile = map->getTile(pos.x, pos.y, pos.z);
		Tile *tile = map->getTile(pos);
		if(tile){
			unsigned char stackpos = tile->getThingStackPos(thing);
			if(!tile->removeThing(thing))
				return false;
			sendRemoveThing(NULL,pos,thing,stackpos,true);
		}
		else{
			return false;
		}
		if(item && setRemoved){
			item->isRemoved = true;
		}
		return true;
	}
}

Position Game::getThingMapPos(Player *player, const Position &pos)
{
	if(pos.x == 0xFFFF){
		Position dummyPos(0,0,0);
		if(!player)
			return dummyPos;
		if(pos.y & 0x40) { //from container
			unsigned char containerid = pos.y & 0x0F;
			const Container* container = player->getContainer(containerid);
			if(!container){
				return dummyPos;
			}
			while(container->getParent() != NULL) {
				container = container->getParent();
			}
			if(container->pos.x == 0xFFFF)
				return player->pos;
			else
				return container->pos;
		}
		else //from inventory
		{
			return player->pos;
		}
	}
	else{
		return pos;
	}
}

Thing* Game::getThing(const Position &pos,unsigned char stack, Player* player /*=NULL*/)
{
	if(pos.x == 0xFFFF) {
		if(!player)
			return NULL;
		if(pos.y & 0x40) { //from container
			unsigned char containerid = pos.y & 0x0F;
			Container* container = player->getContainer(containerid);
			if(!container)
				return NULL;

			return container->getItem(pos.z);
		}
		else //from inventory
		{
			return player->getItem(pos.y);
		}
	}
	else //from ground
	{
		//Tile *t = getTile(pos.x, pos.y, pos.z);
		Tile *t = map->getTile(pos);
		if(!t)
			return NULL;

		return t->getThingByStackPos(stack);
	}
}

int Game::getDepot(Container* c, int e)
{
for(int a = 0; a < c->size(); a++)
{
Container* x = dynamic_cast<Container*>(dynamic_cast<Item*>(c->getItem(a)));
Item* i = dynamic_cast<Item*>(c->getItem(a));
if(i)
e++;
if(x)
e = getDepot(x, e);
}
return e;
}

#ifdef WOLV_LOAD_NPC
bool Game::loadNpcs()
{
	xmlDocPtr doc;
	doc = xmlParseFile((g_config.DATA_DIR + "world/npc.xml").c_str());
	if (!doc)
		return false; 

	xmlNodePtr root, npcNode;
	root = xmlDocGetRootElement(doc);

	if (xmlStrcmp(root->name, (const xmlChar*)"npclist")) 
	{
		xmlFreeDoc(doc);
		return false;
	}

	npcNode = root->children;
	while (npcNode)
	{
		if (strcmp((const char*) npcNode->name, "npc") == 0)
		{
			std::string name = (const char*)xmlGetProp(npcNode, (const xmlChar *) "name");
			int x = atoi((const char*) xmlGetProp(npcNode, (const xmlChar*) "x"));
			int y = atoi((const char*) xmlGetProp(npcNode, (const xmlChar*) "y"));
			int z = atoi((const char*) xmlGetProp(npcNode, (const xmlChar*) "z"));
	
			Npc* mynpc = new Npc(name, this);
			mynpc->pos = Position(x, y, z);

			if (!placeCreature(mynpc->pos, mynpc))
			{
				std::cout << "Could not place " << name << "!" << std::endl;
				xmlFreeDoc(doc);
				return false;
			}

			const char* tmp = (const char*)xmlGetProp(npcNode, (const xmlChar*) "dir");
			if (tmp)
				mynpc->setDirection((Direction)atoi(tmp));
		}  
		npcNode = npcNode->next;
	}

	xmlFreeDoc(doc);
	return true;
}

void Game::addNpc(std::string name, int x, int y, int z)
{
	Npc* mynpc = new Npc(name.c_str(), (Game *)this);
	mynpc->pos.x=x;
	mynpc->pos.y=y;
	mynpc->pos.z=z;
	placeCreature(mynpc->pos, mynpc);
}
#endif //WOLV_LOAD_NPC


#ifdef TLM_SERVER_SAVE
void Game::serverSave()
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::serverSave()");
	std::cout << ":: server save... ";
	timer();

	AutoList<Player>::listiterator it = Player::listPlayer.list.begin();
	while (it != Player::listPlayer.list.end())
	{
		IOPlayer::instance()->savePlayer(it->second);
		++it;
	}

	Guilds::Save();
	Houses::Save(this);
	loginQueue.save();
	std::cout << "ok (" << timer() << "s)" << std::endl;
}

void Game::autoServerSave()
{
	serverSave();
	addEvent(makeTask(g_config.getGlobalNumber("autosave", 1)*60000, std::mem_fun(&Game::autoServerSave)));
}
#endif //TLM_SERVER_SAVE


#ifdef ELEM_VIP_LIST
void Game::vipLogin(Player* player)
{
	std::string vipname = player->getName();
	for(AutoList<Creature>::listiterator cit = listCreature.list.begin(); cit != listCreature.list.end(); ++cit)
	{
		Player* player = dynamic_cast<Player*>((*cit).second);
		if (player)
			player->sendVipLogin(vipname);
	}
}

void Game::vipLogout(std::string vipname)
{
	for(AutoList<Creature>::listiterator cit = listCreature.list.begin(); cit != listCreature.list.end(); ++cit)
	{
		Player* player = dynamic_cast<Player*>((*cit).second);
		if (player)
			player->sendVipLogout(vipname);
	}
}

bool Game::isPlayer(std::string name)
{
	extern xmlMutexPtr xmlmutex;
	std::string datadir = g_config.getGlobalString("datadir");
	std::string filenamecheck = datadir + "players/" + name + ".xml";
	std::transform(filenamecheck.begin(),filenamecheck.end(), filenamecheck.begin(), (int(*)(int))tolower);

	xmlDocPtr doc;
	xmlMutexLock(xmlmutex);
	doc = xmlParseFile(filenamecheck.c_str());

	if (doc)
	{
		xmlMutexUnlock(xmlmutex);
		xmlFreeDoc(doc);
		return true;
	}
	else
	{
		xmlMutexUnlock(xmlmutex);
		xmlFreeDoc(doc);
		return false;
	}
}
#endif //ELEM_VIP_LIST

void Game::checkSpell(Player* player, SpeakClasses type, std::string text)
{
	if (player->vocation == VOCATION_NONE && player->access < g_config.ACCESS_PROTECT)	// rookies
	{
		return;
	}
#ifdef TLM_HOUSE_SYSTEM
	else if (text == "aleta gom")		// edit owner
	{
		Tile* tile = getTile(player->pos);
		House* house = tile? tile->getHouse() : NULL;

		if (house && house->getPlayerRights(player->getName()) == HOUSE_OWNER)
		{
			player->sendHouseWindow(house, player->pos, HOUSE_OWNER);
		}
		else
		{
			player->sendMagicEffect(player->pos, NM_ME_PUFF);
			player->sendTextMessage(MSG_SMALLINFO, "Sorry, not possible.");
		}
	}
	else if (text == "aleta grav")		// edit door owners
	{
		bool last = false;
		for (int x = player->pos.x-1; x <= player->pos.x+1 && !last; x++)
		{
			for(int y = player->pos.y-1; y <= player->pos.y+1 && !last; y++)
			{
				Position doorPos(x, y, player->pos.z);
				Tile *tile = getTile(doorPos);
				House* house = tile? tile->getHouse() : NULL;

				if (house && house->getPlayerRights(doorPos, player->getName()) == HOUSE_OWNER)
				{
					Item *item = dynamic_cast<Item*>(tile->getThingByStackPos(tile->getThingCount()-1));
					if (item && Item::items[item->getID()].isDoor)
					{
						player->sendHouseWindow(house, doorPos, HOUSE_DOOROWNER);
						last = true;
					}
				}
			}
		}
	}
	else if (text == "!pricehouse") //COMMAND for see price houses
	{
      bool last = false;
        for (int x = player->pos.x-1; x <= player->pos.x+1 && !last; x++)
        {
         for(int y = player->pos.y-1; y <= player->pos.y+1 && !last; y++)
         {
    Position doorPos(x, y, player->pos.z);
    Tile* tile = getTile(doorPos);
    House* house = tile? tile->getHouse() : NULL;
    Item *item = dynamic_cast<Item*>(tile->getThingByStackPos(tile->getThingCount()-1));
     if (house)
     {         
       if (item && Item::items[item->getID()].isDoor)
        {     
     long price = g_config.getGlobalNumber("priceforsqm", 0) * house->getHouseSQM(house->getName());
     std::stringstream textmsg;
     textmsg << " The price of this house is " << price << " pieces of gold ";
     player->sendTextMessage(MSG_ADVANCE, textmsg.str().c_str());
        }
      }
    }
  }
}
    
  else if ((text == "!buyhouse" || text == "!buyhome") && player->premmium == true && g_config.getGlobalString("buyhouseable") == "yes") //command for buy houses 
 {
  unsigned long money = player->getMoney(); 
  bool last = false;
  for (int x = player->pos.x-1; x <= player->pos.x+1 && !last; x++)
  {
   for(int y = player->pos.y-1; y <= player->pos.y+1 && !last; y++)
   {
    Position doorPos(x, y, player->pos.z);
    Tile* tile = getTile(doorPos);
    House* house = tile? tile->getHouse() : NULL;
    
    if (house && house->getPlayerRights(player->getName()) == HOUSE_OWNER){
              player->sendTextMessage(MSG_ADVANCE, "You own this house.");
              return;
    }
            if (player && !player->premmium){
              player->sendTextMessage(MSG_ADVANCE, "You dont have a premium account.");
              return;
    }
    if (house && house->isBought()){
              player->sendTextMessage(MSG_ADVANCE, "This house already has an owner.");
              return;
    }
    if(house && house->checkHouseCount(player) >= g_config.getGlobalNumber("maxhouses", 0)){
              std::stringstream textmsg;
              textmsg << " You cant have more than " << g_config.getGlobalNumber("maxhouses", 1) << " houses ";
              player->sendTextMessage(MSG_ADVANCE, textmsg.str().c_str());
              return;
    }
    if (house && house->getPlayerRights(doorPos, player->getName()) == HOUSE_NONE && !house->isBought() && house->checkHouseCount(player) < g_config.getGlobalNumber("maxhouses", 1))
    {
     Item *item = dynamic_cast<Item*>(tile->getThingByStackPos(tile->getThingCount()-1));
     long price = g_config.getGlobalNumber("priceforsqm", 0) * house->getHouseSQM(house->getName());
     if (item && Item::items[item->getID()].isDoor && price <= money)
     {
      player->substractMoney(price);
      house->setOwner(player->getName());
      house->save();
      player->sendTextMessage(MSG_ADVANCE, "You bought a house.");
      last = true;
     }
     else
     {
     player->sendMagicEffect(player->pos, NM_ME_PUFF);
     player->sendTextMessage(MSG_SMALLINFO, "You dont have enough money to buy this house.");
     }
   }
  }
 }
}

	else if (text == "aleta sio")		// edit guests
	{
		Tile *tile = getTile(player->pos);
		House* house = tile? tile->getHouse() : NULL;

		if (house && house->getPlayerRights(player->getName()) >= HOUSE_SUBOWNER)
		{
			player->sendHouseWindow(house, player->pos, HOUSE_GUEST);
		}
		else
		{
			player->sendMagicEffect(player->pos, NM_ME_PUFF);
			player->sendTextMessage(MSG_SMALLINFO, "Sorry, not possible.");
		}
	}
	else if (text == "aleta som")		// edit subowners
	{
		Tile *tile = getTile(player->pos);
		House* house = tile? tile->getHouse() : NULL;

		if (house && house->getPlayerRights(player->getName()) == HOUSE_OWNER)
		{
			player->sendHouseWindow(house, player->pos, HOUSE_SUBOWNER);
		}
		else
		{
			player->sendMagicEffect(player->pos, NM_ME_PUFF);
			player->sendTextMessage(MSG_SMALLINFO, "Sorry, not possible.");
		}
	}
	
	#ifdef ZYX_RESETS
	else if(player && text == "!reset"){
         if(player->getResets() <= 4) {
         if(player->getLevel() >= 1000){
                       player->level = 1;
                       player->resets += 1;
                       player->experience = 0; 
                       player->sendTextMessage(MSG_ADVANCE, "Reseted!");
                       return;
                       }
         if(player->getLevel() <= 999){
             player->sendCancel("You need to be at least lvl 1000 to reset!"); 
             player->sendMagicEffect(player->pos, NM_ME_PUFF);
             return;
             }
             }
             else
             {
			     player->sendTextMessage(MSG_ADVANCE, "You cant reset more! You already have 5 resets!");
                 player->sendMagicEffect(player->pos, NM_ME_PUFF);
                 return;
             }
             }
#endif //ZYX_RESETS
	else if (text == "alana sio")	// kick me
	{
		Tile *tile = getTile(player->pos);
		House* house = tile? tile->getHouse() : NULL;

		if (house)
		{
			teleport(player, house->getFrontDoor());
			player->sendMagicEffect(player->pos, NM_ME_ENERGY_AREA);
		}
		else
		{
			player->sendMagicEffect(player->pos, NM_ME_PUFF);
			player->sendTextMessage(MSG_SMALLINFO, "You are not in a house.");
		}
	}
	else if (text.substr(0, 11) == "alana sio \"")	// kick someone
	{
		Creature* c = getCreatureByName(text.substr(11).c_str());
		Player *target = c? dynamic_cast<Player*>(c) : NULL;

		if (target)
		{
			Tile* tile = getTile(player->pos);
			Tile* targetTile = getTile(target->pos);
			House* house = tile? tile->getHouse() : NULL;
			House* targetHouse = targetTile? targetTile->getHouse() : NULL;

			if (house && targetHouse && house == targetHouse &&
				house->getPlayerRights(player->getName()) >= HOUSE_SUBOWNER)
			{
				Position pos = house->getFrontDoor();
				if (pos.x != 0xFFFF && pos.y != 0xFFFF && pos.z != 0xFF)
				{
					teleport(target, pos);
					player->sendMagicEffect(target->pos, NM_ME_ENERGY_AREA);
				}
				else
				{
					player->sendMagicEffect(player->pos, NM_ME_PUFF);
					player->sendTextMessage(MSG_SMALLINFO, "Sorry, not possible.");
				}
			}
			else
				player->sendTextMessage(MSG_SMALLINFO, "Sorry, not possible.");
		}
		else
		{
			player->sendMagicEffect(player->pos, NM_ME_PUFF);
			player->sendTextMessage(MSG_SMALLINFO, "This player does not exist.");
		}
	}
#endif //TLM_HOUSE_SYSTEM

#ifdef TR_SUMMONS
	else if (text.substr(0, 11) == "utevo res \"")
	{
		static const bool SUMMONS_ALL_VOC = (g_config.getGlobalString("summonsallvoc","no") == "yes");
		static const size_t MAX_SUMMONS = g_config.getGlobalNumber("maxsummons",2);

		if (player->vocation == VOCATION_DRUID || player->vocation == VOCATION_SORCERER ||
			(SUMMONS_ALL_VOC && player->vocation != VOCATION_NONE))
		{
			std::string name = text.substr(11);
			int reqMana = Summons::getRequiredMana(name);
			Tile* tile = getTile(player->pos);

			if (!tile)
			{
				player->sendMagicEffect(player->pos, NM_ME_PUFF);
				player->sendCancel("Sorry, not possible.");
			}
			else if (reqMana < 0)
			{
				player->sendMagicEffect(player->pos, NM_ME_PUFF);
				player->sendCancel("You cannot summon this creature.");
			}
			else if (tile->isPz())
			{
				player->sendMagicEffect(player->pos, NM_ME_PUFF);
				player->sendCancel("You cannot summon creatures in protection zone.");
			}
#ifdef YUR_PVP_ARENA
			else if (tile->isPvpArena())
			{
				player->sendMagicEffect(player->pos, NM_ME_PUFF);
				player->sendCancel("You cannot summon creatures on arena.");
			}
#endif //YUR_PVP_ARENA
			else if (player->getSummonCount() >= MAX_SUMMONS)
			{
				player->sendMagicEffect(player->pos, NM_ME_PUFF);
				player->sendCancel("You cannot have more summons.");
			}
			else if (player->getMana() < reqMana)
			{
				player->sendMagicEffect(player->pos, NM_ME_PUFF);
				player->sendCancel("Not enough mana.");
			}
			else if (!placeSummon(player, name))
			{
				player->sendMagicEffect(player->pos, NM_ME_PUFF);
				player->sendCancel("Not enough room");
			}
			else
			{
				player->mana -= reqMana;
				player->addManaSpent(reqMana);
			}
		}
	}
#endif //TR_SUMMONS

#ifdef BRN_EXIVA
	else if (text.substr(0,7) == "exiva \"" &&
		(!g_config.LEARN_SPELLS || player->knowsSpell("exiva")))
	{
    
		std::string name = text.substr(7);
        Creature *c;
        #ifdef TIJN_WILDCARD
                std::string namecard;
        if(hasWildcard(name)) {//Testing if name have ~ inside him
namecard = getNameByWildcard(name);
if(namecard != "") {
c = getCreatureByName(namecard);
}
else if(namecard == ".ambiguous") {
//ambiguous
std::cout << "Player not found!!!" << std::endl;
return;
}
else {
//name doesn't exist
player->sendCancel("Sorry, this player ist not online");
return;
}
}

else {
//not a wildcard - continue normally
c = getCreatureByName(name);
}
#endif //TIJN_WILDCARD

		if (dynamic_cast<Player*>(c))
		{
			if(player->mana >= 20)
			{
				player->mana -= 20;
				player->addManaSpent(20);
			}
			else
			{
				player->sendMagicEffect(player->pos, NM_ME_PUFF);
				player->sendTextMessage(MSG_SMALLINFO, "Not enough mana.");
				return;
			}

			int x = c->pos.x - player->pos.x;
			int y = c->pos.y - player->pos.y;
			int z = c->pos.z - player->pos.z;
			std::stringstream position;
			if(namecard != "")
			position << namecard;
			else
			position << name;
if(c->access < 3) {
			if((x > 48 && y > 24) || (x > 24 && y > 48) || (x > 36 && y > 36))
				position << " is far to the south-east.";
			else if((x > 48 && y < -24) || (x > 24 && y < -48) || (x > 36 && y < -36))
				position << " is far to the north-east.";
			else if((x < -48 && y > 24) || (x < -24 && y > 48) || (x < -36 && y > 36))
				position << " is far to the south-west.";
			else if((x < -48 && y < -24) || (x < -24 && y < -48) || (x < -36 && y < -36))
				position << " is far to the north-west."; 
          
			else if((x > 6 && y > 12 && z > 0) || (x > 12 && y > 6 && z > 0) || (x > 9 && y > 9 && z > 0))
				position << " is on a lower lvl to the south-east.";
			else if((x > 6 && y < -12 && z > 0) || (x > 12 && y < -6 && z > 0) || (x > 9 && y < -9 && z > 0))
				position << " is on a lower lvl to the north-east.";
			else if((x < -6 && y > 12 && z > 0) || (x < -12 && y > 6 && z > 0) || (x < -9 && y > 9 && z > 0))
				position << " is on a lower lvl to the south-west.";
			else if((x < -6 && y < -12 && z > 0) || (x < -12 && y < -6 && z > 0) || (x < -9 && y < -9 && z > 0))
				position << " is on a lower lvl to the north-west."; 
          
			else if((x > 6 && y > 12 && z < 0) || (x > 12 && y > 6 && z < 0) || (x > 9 && y > 9 && z < 0))
				position << " is on a higher lvl to the south-east.";
			else if((x > 6 && y < -12 && z < 0) || (x > 12 && y < -6 && z < 0) || (x > 9 && y < -9 && z < 0))
				position << " is on a higher lvl to the north-east.";
			else if((x < -6 && y > 12 && z < 0) || (x < -12 && y > 6 && z < 0) || (x < -9 && y > 9 && z < 0))
				position << " is on a higher lvl to the south-west.";
			else if((x < -6 && y < -12 && z < 0) || (x < -12 && y < -6 && z < 0) || (x < -9 && y < -9 && z < 0))
				position << " is on a higher lvl to the north-west."; 
           
			else if((x > 6 && y > 12 && z == 0) || (x > 12 && y > 6 && z == 0) || (x > 9 && y > 9 && z == 0))
				position << " is to the south-east.";
			else if((x > 6 && y < -12 && z == 0) || (x > 12 && y < -6 && z == 0) || (x > 9 && y < -9 && z == 0))
				position << " is to the north-east.";
			else if((x < -6 && y > 12 && z == 0) || (x < -12 && y > 6 && z == 0) || (x < -9 && y > 9 && z == 0))
				position << " is to the south-west.";
			else if((x < -6 && y < -12 && z == 0) || (x < -12 && y < -6 && z == 0) || (x < -9 && y < -9 && z == 0))
				position << " is to the north-west.";  
               
			else if(x > 36)
				position << " is far to the east.";
			else if(x < -36)
				position << " is far to the west.";
			else if(y > 36)
				position << " is far to the south.";
			else if(y < -36)
				position << " is far to the north.";
          
			else if(x > 3 && z < 0)
				position << " is on a higher lvl to the east.";
			else if(x < -3 && z < 0)
				position << " is on a higher lvl to the west.";
			else if(y > 3 && z < 0)
				position << " is on a higher lvl to the south.";
			else if(y < -3 && z < 0)
				position << " is on a higher lvl to the north.";
          
			else if(x > 3 && z > 0)
				position << " is on a lower lvl to the east.";
			else if(x < -3 && z > 0)
				position << " is on a lower lvl to the west.";
			else if(y > 3 && z > 0)
				position << " is on a lower lvl to the south.";
			else if(y < -3 && z > 0)
				position << " is on a lower lvl to the north.";
          
			else if(x > 3 && z == 0)
				position << " is to the east.";
			else if(x < -3 && z == 0)
				position << " is to the west.";
			else if(y > 3 && z == 0)
				position << " is to the south.";
			else if(y < -3 && z == 0)
				position << " is to the north.";
          
			else if(x < 4 && y < 4 && z > 0)
				position << " is below you.";
			else if(x < 4 && y < 4 && z < 0)
				position << " is above you.";     
			else
				position << " is just beside you.";

			player->sendTextMessage(MSG_INFO, position.str().c_str());
			player->sendMagicEffect(player->pos, NM_ME_MAGIC_ENERGIE);
		}
				else
		player->sendTextMessage(MSG_SMALLINFO, "Sorry you cant exiva Gamemasters");
    }
		else
			player->sendTextMessage(MSG_SMALLINFO, "This player is not online.");
	}
	#endif //BRN_EXIVA
}

#ifdef TR_SUMMONS
bool Game::placeSummon(Player* p, const std::string& name)
{
	Monster* monster = new Monster(name, this);
	if(!monster->isLoaded())
	{
		delete monster;
		return false;
	}

	Position pos = p->pos;
	switch (p->direction)
	{
		case NORTH: pos.y--; break;
		case SOUTH: pos.y++; break;
		case EAST: pos.x++; break;
		case WEST: pos.x--; break;
	}

	Tile* tile = getTile(pos);
#ifdef YUR_PVP_ARENA
	if (!tile || tile->isPz() || tile->isPvpArena() || !placeCreature(pos, monster))
#else
	if (!tile || tile->isPz() || !placeCreature(pos, monster))
#endif //YUR_PVP_ARENA
	{
		delete monster;
		return false;
	}
	else
	{
		p->addSummon(monster);
		return true;
	}
}
#endif //TR_SUMMONS


#ifdef TRS_GM_INVISIBLE
void Game::creatureBroadcastTileUpdated(const Position& pos)
{
	SpectatorVec list;
	SpectatorVec::iterator it;
	getSpectators(Range(pos, true), list);

	//players
	for(it = list.begin(); it != list.end(); ++it) {
		if(dynamic_cast<Player*>(*it)) {
			(*it)->onTileUpdated(pos);
		}
	}

	//none-players
	for(it = list.begin(); it != list.end(); ++it) {
		if(!dynamic_cast<Player*>(*it)) {
			(*it)->onTileUpdated(pos);
		}
	}
}
#endif //TRS_GM_INVISIBLE



#ifdef TLM_SKULLS_PARTY
void Game::Skull(Player* player) 
{
	if (player) 
	{
		SpectatorVec list;
		SpectatorVec::iterator it;
		getSpectators(Range(player->pos, true), list);

		for(it = list.begin(); it != list.end(); ++it) 
		{
			Player* spectator = dynamic_cast<Player*>(*it);
			if(spectator)
			    if(player->skullType == SKULL_NONE || 
                player->skullType == SKULL_WHITE || 
                player->skullType == SKULL_RED || 
                player->skullType == SKULL_YELLOW && player->isYellowTo(spectator))
				spectator->onSkull(player);
		}
	}
}
 
void Game::onPvP(Creature* creature, Creature* attacked, bool murder)
{
	Player* player = dynamic_cast<Player*>(creature);
	Player* attackedPlayer = dynamic_cast<Player*>(attacked);
 
	if (player == attackedPlayer)
		return;
	if(!player || !attackedPlayer)	
		return;
	if (player && player->access != 0 || attackedPlayer && attackedPlayer->access != 0)
		return;
	if (player->party != 0 && attackedPlayer->party != 0 && player->party == attackedPlayer->party)
		return;

	player->pzLocked = true;
 
	if (!murder) 
	{
        if(!player->hasAttacked(attackedPlayer)){
        /*#ifdef DEBUG_YELLOWSKULL 
        std::cout << "Debugger: " << player->getName() << " has not attacked " << attackedPlayer->getName() << " before. Adding to attackedPlayers list.. (onPvP, No murder.)" << std::endl;
        #endif //DEBUG_YELLOWSKULL*/
        player->attackedPlayers.push_back(attackedPlayer);
        /*#ifdef DEBUG_YELLOWSKULL
        std::cout << "Debugger: " << attackedPlayer->getName() << " has been added to " << player->getName() << "'s attackedPlayers list. (onPvP, No murder.)" << std::endl;
        #endif //DEBUG_YELLOWSKULL*/
        }
        
		if (attackedPlayer->skullType == SKULL_NONE || attackedPlayer->skullType == SKULL_YELLOW && !attackedPlayer->isYellowTo(player)) 
		{	 
			if (player->skullType != SKULL_RED && player->skullType != SKULL_WHITE)
			{
				player->skullType = SKULL_WHITE;
				Skull(player);
			}	
		} 
		else if(attackedPlayer->skullType == SKULL_WHITE || attackedPlayer->skullType == SKULL_RED) 
		{ 
		    if(player->skullType == SKULL_NONE && !player->isYellowTo(attackedPlayer))//si no tiene skull y no es yellow, tenemos que ponerle yellow.
		    {
                    if(!attackedPlayer->hasAttacked(player))
                    {
                    /*#ifdef DEBUG_YELLOWSKULL
                    std::cout << "Debugger: " << attackedPlayer->getName() << " has not attacked " << player->getName() << " before. Setting yellow skull.. (onPvP, No murder.)" << std::endl;
                    #endif //DEBUG_YELLOWSKULL*/
                    player->skullType = SKULL_YELLOW;
                    attackedPlayer->hasAsYellow.push_back(player);
                    /*#ifdef DEBUG_YELLOWSKULL
                    if(player->isYellowTo(attackedPlayer))
                    std::cout << "Debugger: " << player->getName() << " has been succesfully set up as yellow to " << attackedPlayer->getName() << ". (onPvP, No murder.)" << std::endl;
                    #endif //DEBUG_YELLOWSKULL*/
                    attackedPlayer->onSkull(player);
                    }
            }
        }
        if(player->inFightTicks < (long)g_config.getGlobalNumber("pzlocked", 0))
		player->inFightTicks = (long)g_config.getGlobalNumber("pzlocked", 0);
		if(player->skullTicks < (long)g_config.getGlobalNumber("pzlocked", 0))
		player->skullTicks = (long)g_config.getGlobalNumber("pzlocked", 0);
    }
	else	// attacked died
	{   
		if (attackedPlayer->skullType == SKULL_NONE)//victim was inocent (unjust)
		{
			player->skullKills++;	 
			std::string justice(std::string("Warning! The murder of ") + attackedPlayer->getName() + " was not justified!");
			player->sendTextMessage(MSG_RED_INFO, justice.c_str());
		 
			if (player->skullKills >= g_config.getGlobalNumber("redskullban", 6)) 
            {

                banPlayer(player, "Excessive unjustifed player killing", "AccountBan+FinalWarning", "Talaturens Autoban for Playerkilling", false);

              //  player->banned = true;

            }
			else if (player->skullKills >= g_config.getGlobalNumber("redskull", 3)) 
			{
				player->skullType = SKULL_RED;
				if(player->skullTicks < g_config.getGlobalNumber("redskulltime",24*60)*60000)
				player->skullTicks = g_config.getGlobalNumber("redskulltime",24*60)*60000;
				if(player->inFightTicks < g_config.getGlobalNumber("whiteskulltime",15)*60000)
                player->inFightTicks = g_config.getGlobalNumber("whiteskulltime",15)*60000;
				Skull(player); 
			}
			else
			{
				player->skullType = SKULL_WHITE; 
				if(player->skullTicks < g_config.getGlobalNumber("whiteskulltime",15)*60000)
				player->skullTicks = g_config.getGlobalNumber("whiteskulltime",15)*60000;
				if(player->inFightTicks < g_config.getGlobalNumber("whiteskulltime",15)*60000)
				player->inFightTicks = g_config.getGlobalNumber("whiteskulltime",15)*60000;
				Skull(player);
			}
		}
		else if (attackedPlayer->skullType == SKULL_RED)//victim had red skull..(fair kill)
		{
		          //we aren't removin his skull..are we?
		          if(player->inFightTicks < g_config.getGlobalNumber("whiteskulltime",15)*60000)
		          player->inFightTicks = g_config.getGlobalNumber("whiteskulltime",15)*60000;//not giving him a skull.. just setting the murder time.
		}
		else if (attackedPlayer->skullType == SKULL_WHITE) //victim had white skull.. (fair kill)
		{
				attackedPlayer->skullType = SKULL_NONE; 
				attackedPlayer->skullTicks = 0;
				Skull(attackedPlayer);
				if(player->inFightTicks < g_config.getGlobalNumber("whiteskulltime",15)*60000)
				player->inFightTicks = g_config.getGlobalNumber("whiteskulltime",15)*60000;//not giving him a skull.. just setting the murder time.
		}
       else	if (attackedPlayer->skullType == SKULL_YELLOW && attackedPlayer->isYellowTo(player))//el que murio era yellow skull para el que lo mato.
       {
            /*#ifdef DEBUG_YELLOWSKULL
            std::cout << "Debugger: " << attackedPlayer->getName() << " isYellowTo " << player->getName() << ". Removing skull.. (onPvP, Murder.)" << std::endl;
            #endif //DEBUG_YELLOWSKULL*/
   			attackedPlayer->skullType = SKULL_NONE; 
			attackedPlayer->skullTicks = 0;
			Skull(attackedPlayer);
			if(player->inFightTicks < g_config.getGlobalNumber("whiteskulltime",15)*60000)
			player->inFightTicks = g_config.getGlobalNumber("whiteskulltime",15)*60000;//not giving him a skull.. just setting the murder time.
			player->removeFromAttakedList(attackedPlayer);//moved to removeCreature.
			player->removeFromYellowList(attackedPlayer);//moved to removeCreature.
	  }	
	  attackedPlayer->clearAttakedList();//moved to removeCreature.
	  attackedPlayer->clearYellowList();//moved to removeCreature.
	}
}
 
void Game::LeaveParty(Player *player)
{ 
	int members = 0;
	std::stringstream bericht1; 
	bericht1 << player->getName() << " has left the party";
	if(player->getID() == player->party)
	{
		disbandParty(player->party);
		return;
	}
	for(AutoList<Player>::listiterator it = Player::listPlayer.list.begin(); it != Player::listPlayer.list.end(); ++it)
	{
		if((*it).second->party == player->party)
		{
			members++;
			if((*it).second->getID() != player->getID())
				(*it).second->sendTextMessage(MSG_INFO, bericht1.str().c_str());
			(*it).second->onPartyIcons(player, 0, false, true);
			player->onPartyIcons((*it).second, 0, false, true);
		}
	}
	if(members <= 2)
	{
		disbandParty(player->party);
		return;
	}
	player->sendTextMessage(MSG_INFO, "You have left the party.");
	player->party = 0;
}
 
void Game::disbandParty(unsigned long partyID)
{
	for(AutoList<Player>::listiterator cit = Player::listPlayer.list.begin(); cit != Player::listPlayer.list.end(); ++cit)
	{
		if((*cit).second->party == partyID)
		{
			(*cit).second->party = 0;
			for(AutoList<Player>::listiterator it = Player::listPlayer.list.begin(); it != Player::listPlayer.list.end(); ++it)
			{
				(*cit).second->onPartyIcons((*it).second, 0, false, true);
				 if((*it).second->skullType == SKULL_NONE || 
                 (*it).second->skullType == SKULL_WHITE || 
                 (*it).second->skullType == SKULL_RED || 
                 (*it).second->skullType == SKULL_YELLOW && 
                 (*it).second->isYellowTo((*cit).second))
				(*cit).second->onSkull((*it).second);
			}
			(*cit).second->sendTextMessage(MSG_INFO, "Your party has been disbanded.");
		}
	}
}
void Game::checkSkullTime(Player* player)
{
    if(player->skullType == SKULL_NONE)//just in case
    return;
    
    if(player->skullTicks < player->inFightTicks)
    player->skullTicks = player->inFightTicks;
    
    if(player->skullType != SKULL_RED && player->skullTicks > player->inFightTicks) //we don't want to do that if the player has a red skull...
    player->inFightTicks = player->skullTicks;

}    
#endif //TLM_SKULLS_PARTY

#ifdef SD_BURST_ARROW
class MagicEffectAreaNoExhaustionClass: public MagicEffectAreaClass {
public:
	bool causeExhaustion(bool hasTarget) const { return false; }
};

void Game::burstArrow(Creature* c, const Position& pos)
{
	std::vector<unsigned char> col;
	MagicEffectAreaNoExhaustionClass runeAreaSpell;

	runeAreaSpell.attackType = ATTACK_PHYSICAL;
	runeAreaSpell.animationEffect = NM_ANI_BURSTARROW;
	runeAreaSpell.hitEffect = NM_ME_EXPLOSION_DAMAGE;
	runeAreaSpell.areaEffect = NM_ME_EXPLOSION_AREA;
	runeAreaSpell.animationColor = 198; //DAMAGE_FIRE;
	runeAreaSpell.drawblood = true;
	runeAreaSpell.offensive = true;

	/* Area of Spell */
	col.push_back(1);
	col.push_back(1);
	col.push_back(1);
	runeAreaSpell.areaVec.push_back(col);
	col.clear();
	col.push_back(1);
	col.push_back(1);
	col.push_back(1);
	runeAreaSpell.areaVec.push_back(col);
	col.clear();
	col.push_back(1);
	col.push_back(1);
	col.push_back(1);
	runeAreaSpell.areaVec.push_back(col);

	static const double BURST_DMG_LVL = atof(g_config.getGlobalStringField("burstarrowdmg", 1, "2.0").c_str()),
		BURST_DMG_MLVL = atof(g_config.getGlobalStringField("burstarrowdmg", 2, "3.0").c_str()),
		BURST_DMG_LO = atof(g_config.getGlobalStringField("burstarrowdmg", 3, "0.24").c_str()),
		BURST_DMG_HI = atof(g_config.getGlobalStringField("burstarrowdmg", 4, "0.55").c_str());

	/* hard no ? */
	runeAreaSpell.direction = 1;
	runeAreaSpell.minDamage = int(((c->level*BURST_DMG_LVL)+(c->maglevel*BURST_DMG_MLVL))*BURST_DMG_LO);
	runeAreaSpell.maxDamage = int(((c->level*BURST_DMG_LVL)+(c->maglevel*BURST_DMG_MLVL))*BURST_DMG_HI);
	creatureThrowRune(c, pos, runeAreaSpell);
}
#endif //SD_BURST_ARROW


#ifdef YUR_SHUTDOWN
void Game::sheduleShutdown(int minutes)
{
	if (minutes > 0)
		checkShutdown(minutes);
}

void Game::checkShutdown(int minutes)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::checkShutdown()");
	if (minutes == 0)
	{
		setGameState(GAME_STATE_CLOSED);
		while (!Player::listPlayer.list.empty())
			Player::listPlayer.list.begin()->second->kickPlayer();

		serverSave();
		//setGameState(GAME_STATE_SHUTDOWN); 
		std::cout << ":: shutdown..." << std::endl;
		OTSYS_SLEEP(1000);
		exit(1);
	}
	else
	{
		std::stringstream msg;
		msg << "SERVER SHUTDOWN IN " << minutes << (minutes>1? " MINUTES" : " MINUTE") << std::ends;

		AutoList<Player>::listiterator it = Player::listPlayer.list.begin();
		while (it != Player::listPlayer.list.end())
		{
			(*it).second->sendTextMessage(MSG_RED_INFO, msg.str().c_str());
			++it;
		}

		addEvent(makeTask(60000, boost::bind(&Game::checkShutdown, this, minutes - 1)));
	}
}
#endif //YUR_SHUTDOWN


#ifdef YUR_CMD_EXT
void Game::setMaxPlayers(uint32_t newmax)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::setMaxPlayers()");
	max_players = newmax;
	Status::instance()->playersmax = newmax;
}
#endif //YUR_CMD_EXT


#ifdef YUR_CLEAN_MAP
long Game::cleanMap()
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::cleanMap()");
	return map->clean();
}
#endif //YUR_CLEAN_MAP
  #ifdef JIDDO_RAID
  bool Game::loadRaid(std::string name) 
{ 
  xmlDocPtr doc;
  std::cout << "Executing raid named " << name << "." << std::endl;
  std::string file = "data/world/raids.xml";
  doc = xmlParseFile(file.c_str());
  if(doc){
  xmlNodePtr root, raid, command;
  root = xmlDocGetRootElement(doc);
  if(xmlStrcmp(root->name, (const xmlChar*)"raids")) {
     xmlFreeDoc(doc);
     return -1;
  }
   raid = root->children; 
   while(raid){ 
      if(strcmp((char*) raid->name, "raid")==0){
 
         std::string nameIN = (const char*)xmlGetProp(raid, (const xmlChar *) "name");
         if(nameIN == name) {
         std::string messageIN = (const char*)xmlGetProp(raid, (const xmlChar *) "message");
         std::string brodcasterIN = (const char*)xmlGetProp(raid, (const xmlChar *) "brodcaster");
 
         Creature *c = getCreatureByName(brodcasterIN);
         if(c) {
            creatureBroadcastEvent(messageIN);
         } else {
            std::cout << "Could not send news msg! Brodcaster does not exist" << std::endl;
         }
 
         if(nameIN == name) {
            command = raid->children;
 
            while(command) {
 
               if(strcmp((char*) command->name, "monster")==0){
                  std::string monstername = (const char*)xmlGetProp(command, (const xmlChar *) "name");
                  int x = atoi((const char*)xmlGetProp(command, (const xmlChar *) "x"));
                  int y = atoi((const char*)xmlGetProp(command, (const xmlChar *) "y"));
                  int z = atoi((const char*)xmlGetProp(command, (const xmlChar *) "z"));
 
                  int loot = atoi((const char*)xmlGetProp(command, (const xmlChar *) "lootid"));  //Not yet implemented!
                  int chance = atoi((const char*)xmlGetProp(command, (const xmlChar *) "chance"));  //Not yet implemented!
                  placeRaidMonster(monstername, x, y, z);
               }
 
               if(strcmp((char*) command->name, "area")==0){
                  std::string monstername = (const char*)xmlGetProp(command, (const xmlChar *) "monster");
                  int count = atoi((const char*)xmlGetProp(command, (const xmlChar *) "count"));
                  int xf = atoi((const char*)xmlGetProp(command, (const xmlChar *) "posxfrom"));
                  int yf = atoi((const char*)xmlGetProp(command, (const xmlChar *) "posyfrom"));
                  int zf = atoi((const char*)xmlGetProp(command, (const xmlChar *) "poszfrom"));
 
                  int xt = atoi((const char*)xmlGetProp(command, (const xmlChar *) "posxto"));
                  int yt = atoi((const char*)xmlGetProp(command, (const xmlChar *) "posyto"));
                  int zt = atoi((const char*)xmlGetProp(command, (const xmlChar *) "poszto"));
 
                  int i = 0;
                  int tries = 0;
                  while (i<=count && tries<=(count*10)) {
                      int x = (int)((xt-xf) * (rand()/(RAND_MAX+1.0)) + xf);
                      int y = (int)((yt-yf) * (rand()/(RAND_MAX+1.0)) + yf);
                      int z = (int)((zt-zf) * (rand()/(RAND_MAX+1.0)) + zf);
                      Tile* t = map->getTile(x,y,z);
                      if(t && t->isPz() == false) {
                         placeRaidMonster(monstername, x, y, z);
                         i++;
                      }
                      tries++;
                  }
               }
               if(strcmp((char*) command->name, "message")==0){
                  std::string msg = (const char*)xmlGetProp(command, (const xmlChar *) "text");
                  std::string brodcaster = (const char*)xmlGetProp(command, (const xmlChar *) "brodcaster");
                  Creature *c = getCreatureByName(brodcaster);
                  if(c) {
                     creatureBroadcastEvent(msg);
                  } else {
                     std::cout << "Could not send news msg! Brodcaster does not exist." << std::endl;
                  }
               } 
               command = command->next;
            }
         }
      }
   }
   raid = raid->next;
   }     
   xmlFreeDoc(doc);
   return 0; 
  }
  return -1;
}

bool Game::placeRaidMonster(std::string name, int x, int y, int z)
{
Monster* monster = new Monster(name, this);
if(!monster->isLoaded()){
delete monster;
return false;
}
Position pos;
pos.x = x;
pos.y = y;
pos.z = z;
// Place the monster
if(!placeCreature(pos, monster)) {
delete monster;
return false;
}
return true;
}
#endif //JIDDO_RAID

  #ifdef EOT_FIELDDAMAGE
  void Game::CreateCondition(Creature* creature, Creature* target, 
        unsigned char animationColor, unsigned char damageEffect, unsigned char hitEffect, 
        attacktype_t attackType, bool offensive, int maxDamage, int minDamage, long ticks, 
        long count)
{
    unsigned long attackerID = 0;
    if(creature)
    attackerID = creature->getID();

    MagicEffectTargetCreatureCondition magicCondition = MagicEffectTargetCreatureCondition(attackerID);
    magicCondition.animationColor = animationColor;
    magicCondition.damageEffect = damageEffect;
    magicCondition.hitEffect = hitEffect;
    magicCondition.attackType = attackType;
    magicCondition.maxDamage = maxDamage;
    magicCondition.minDamage = minDamage;
    magicCondition.offensive = offensive;
    magicCondition.ground = true;
    CreatureCondition condition = CreatureCondition(ticks, count, magicCondition);
    target->addCondition(condition, true);
    
    Player *player = dynamic_cast<Player*>(target);
    if(player)
    player->sendIcons();
    
}
void Game::doFieldDamage(Creature* creature, unsigned char animationColor, unsigned char damageEffect,
       unsigned char hitEffect, attacktype_t attackType, bool offensive, int damage)
{
   MagicEffectClass cd;
   cd.animationColor = animationColor;
   cd.damageEffect = damageEffect;
   cd.hitEffect = hitEffect;
   cd.attackType = attackType;
   cd.offensive = offensive;
   Player* itsHim = dynamic_cast<Player*>(getCreatureByID(creature->getID()));
   if(itsHim){ //Since that was causing damage/2 against player, here its my solution =)
      cd.maxDamage = damage*2;
      cd.minDamage = damage*2;
   }
   else{
      cd.maxDamage = damage;
      cd.minDamage = damage;
   }
   creatureMakeMagic(NULL, creature->pos, &cd);
}
#endif //EOT_FIELDDAMAGE

#ifdef BD_ITEMTOSTAIRHOLE
bool Game::canDelete(Player *player, unsigned short itemid, Position toPos, Position fromPos, int from_stack, unsigned char count)
{
OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::canDelete()");
if(itemid == 99 || (itemid >= 4329 && itemid <= 4555))
return false;
else if(fromPos.x != 0xFFFF && ((abs(player->pos.x - fromPos.x) > 1) || (abs(player->pos.y - fromPos.y) > 1) || (player->pos.z != fromPos.z)))
return false;
else if(map->canThrowObjectTo(player->pos, toPos, BLOCK_PROJECTILE) != RET_NOERROR)
return false;
else if(toPos.x == 0xFFFF)
return false;
Item* trash = dynamic_cast<Item*>(getThing(fromPos, from_stack, player));
if(trash){
trash->pos = fromPos;
if((abs(player->pos.x - toPos.x) > trash->throwRange) || (abs(player->pos.y - toPos.y) > trash->throwRange)) {        
return false;
}
Tile *toTile = map->getTile(toPos);
if(toTile){
if((toTile->ground->getID() >= GROUND_WATER1 && toTile->ground->getID() <= GROUND_WATER3) || (toTile->ground->getID() >= GROUND_LAVA1 && toTile->ground->getID() <= GROUND_SWAMP4)){
if(!trash->isNotMoveable() && trash->isBlocking())
return false;
else if(trashObjects(player, toTile, trash, toPos, fromPos, from_stack, count))
return true;
}
Item *toItem = dynamic_cast<Item*>(toTile->getTopTopItem());
if(toItem && toItem->getID() == ITEM_DUSTBIN){
if(!trash->isNotMoveable() && trash->isBlocking())
return false;
else if(trashObjects(player, toTile, trash, toPos, fromPos, from_stack, count))
return true;
}
}
}
return false;
}     
bool Game::trashObjects(Player *player, Tile *toTile, Item *trash, Position toPos, Position fromPos, int from_stack, unsigned char count)
{
//OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::trashObjects()");
if(toTile){
switch(toTile->ground->getID()){
case GROUND_WATER1:
spectatorEffect(toPos, NM_ME_LOOSE_ENERGY);
if(trashItems(player, trash, fromPos, from_stack, count))
return true;     
break;
case GROUND_WATER2:
spectatorEffect(toPos, NM_ME_LOOSE_ENERGY);
if(trashItems(player, trash, fromPos, from_stack, count))
return true;
break;
case GROUND_WATER3:
spectatorEffect(toPos, NM_ME_LOOSE_ENERGY);
if(trashItems(player, trash, fromPos, from_stack, count))
return true;
break;
case GROUND_LAVA1:
spectatorEffect(toPos, NM_ME_HITBY_FIRE);
if(trashItems(player, trash, fromPos, from_stack, count))
return true;
break;
case GROUND_LAVA2:
spectatorEffect(toPos, NM_ME_HITBY_FIRE);
if(trashItems(player, trash, fromPos, from_stack, count))
return true;
break;
case GROUND_LAVA3:
spectatorEffect(toPos, NM_ME_HITBY_FIRE);
if(trashItems(player, trash, fromPos, from_stack, count))
return true;
break;
case GROUND_LAVA4:
spectatorEffect(toPos, NM_ME_HITBY_FIRE);
if(trashItems(player, trash, fromPos, from_stack, count))
return true;
break;
case GROUND_SWAMP1:
spectatorEffect(toPos, NM_ME_POISEN_RINGS);
if(trashItems(player, trash, fromPos, from_stack, count))
return true;
break;
case GROUND_SWAMP2:
spectatorEffect(toPos, NM_ME_POISEN_RINGS);
if(trashItems(player, trash, fromPos, from_stack, count))
return true;
break;
case GROUND_SWAMP3:
spectatorEffect(toPos, NM_ME_POISEN_RINGS);
if(trashItems(player, trash, fromPos, from_stack, count))
return true;
break;
case GROUND_SWAMP4:
spectatorEffect(toPos, NM_ME_POISEN_RINGS);
if(trashItems(player, trash, fromPos, from_stack, count))
return true;
break;
}     
Item *toItem = dynamic_cast<Item*>(toTile->getTopTopItem());
if(toItem && toItem->getID() == ITEM_DUSTBIN){
if(trashItems(player, trash, fromPos, from_stack, count))
return true;
}
}
return false;
}
bool Game::trashItems(Player *player, Item *trash, Position fromPos, int from_stack, unsigned char count)
{
//OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::trashItems()");
if(trash && player){
if(trash->isStackable()){
if(trash->getItemCountOrSubtype() > count){
trash->setItemCountOrSubtype(trash->getItemCountOrSubtype() - count);
sendUpdateThing(player,fromPos,trash,from_stack);
player->updateInventoryWeigth();
return true;
}
else{
if(removeThing(player, fromPos, trash)){
player->updateInventoryWeigth();
return true;
}
}
}
else{
if(removeThing(player, fromPos, trash)){
player->updateInventoryWeigth();
return true;
}
}
}
return false;
}
bool Game::canTeleportItem(Player *player, unsigned short itemid, Position toPos, Position fromPos, int from_stack, unsigned char count)
{
OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::canTeleportItem()");
if(itemid == 99 || (itemid >= 4329 && itemid <= 4555))
return false;
else if(fromPos.x != 0xFFFF && ((abs(player->pos.x - fromPos.x) > 1) || (abs(player->pos.y - fromPos.y) > 1) || (player->pos.z != fromPos.z)))
return false;
else if(map->canThrowObjectTo(player->pos, toPos, BLOCK_PROJECTILE) != RET_NOERROR)
return false;
else if(toPos.x == 0xFFFF)
return false;
else if(!checkChangeFloor(map->getTile(toPos), getTile(toPos.x,toPos.y,toPos.z+1)))
return false;
Item* tpItem = dynamic_cast<Item*>(getThing(fromPos, from_stack, player));
if(tpItem){
tpItem->pos = fromPos;
if((abs(player->pos.x - toPos.x) > tpItem->throwRange) || (abs(player->pos.y - toPos.y) > tpItem->throwRange)) {        
return false;
}
if(tpItem->isStackable()){
if(tpItem->getItemCountOrSubtype() > count){
tpItem->setItemCountOrSubtype(tpItem->getItemCountOrSubtype() - count);
Item *newitem = Item::CreateItem(tpItem->getID(), count);
addThing(player,getTeleportPos(toPos),newitem);
sendUpdateThing(player,fromPos,tpItem,from_stack);
player->updateInventoryWeigth();
return true;
}
else{
if(removeThing(player, fromPos, tpItem)){
addThing(player,getTeleportPos(toPos),tpItem);
player->updateInventoryWeigth();
return true;
}
}
}
else{
if(removeThing(player, fromPos, tpItem)){
addThing(player,getTeleportPos(toPos),tpItem);
player->updateInventoryWeigth();
return true;
}
}
}
return false;
}
Position Game::getTeleportPos(Position to)
{
Tile *toTile = map->getTile(to);
if(toTile->ground && toTile->ground->floorChangeDown())
{
Tile* downTile = getTile(to.x, to.y, to.z+1);
if(downTile){
//diagonal begin
if(downTile->floorChange(NORTH) && downTile->floorChange(EAST)){
return Position(to.x-1, to.y+1, to.z+1);
}
else if(downTile->floorChange(NORTH) && downTile->floorChange(WEST)){
return Position(to.x+1, to.y+1, to.z+1);
}
else if(downTile->floorChange(SOUTH) && downTile->floorChange(EAST)){
return Position(to.x-1, to.y-1, to.z+1);
}
else if(downTile->floorChange(SOUTH) && downTile->floorChange(WEST)){
return Position(to.x+1, to.y-1, to.z+1);
}
//diagonal end
else if(downTile->floorChange(NORTH)){
return Position(to.x, to.y+1, to.z+1);
}
else if(downTile->floorChange(SOUTH)){
return Position(to.x, to.y-1, to.z+1);
}
else if(downTile->floorChange(EAST)){
return Position(to.x-1, to.y, to.z+1);
}
else if(downTile->floorChange(WEST)){
return Position(to.x+1, to.y, to.z+1);
}
//floor change down
else if(Item::items[toTile->ground->getID()].floorChangeDown){
return Position(to.x, to.y, to.z+1);
}
else {
return Position(to.x, to.y, to.z+1);
}
}
}
//diagonal begin
else if(toTile->floorChange(NORTH) && toTile->floorChange(EAST)){
return Position(to.x+1, to.y-1, to.z-1);
}
else if(toTile->floorChange(NORTH) && toTile->floorChange(WEST)){
return Position(to.x-1, to.y-1, to.z-1);
}
else if(toTile->floorChange(SOUTH) && toTile->floorChange(EAST)){
return Position(to.x+1, to.y+1, to.z-1);
}
else if(toTile->floorChange(SOUTH) && toTile->floorChange(WEST)){
return Position(to.x-1, to.y+1, to.z-1);
}                  
else if(toTile->floorChange(NORTH)){
return Position(to.x, to.y-1, to.z-1);
}
else if(toTile->floorChange(SOUTH)){
return Position(to.x, to.y+1, to.z-1);
}
else if(toTile->floorChange(EAST)){
return Position(to.x+1, to.y, to.z-1);
}
else if(toTile->floorChange(WEST)){
return Position(to.x-1, to.y, to.z-1);
}                                      
if(!toTile){
Tile* downTile = getTile(to.x, to.y, to.z+1);
if(!downTile)
{
return Position(0,0,0);
}
else if(downTile->floorChange(NORTH) && downTile->floorChange(EAST)){
return Position(to.x-2, to.y+2, to.z+1);
}
else if(downTile->floorChange(NORTH) && downTile->floorChange(WEST)){
return Position(to.x+2, to.y+2, to.z+1);
}
else if(downTile->floorChange(SOUTH) && downTile->floorChange(EAST)){
return Position(to.x-2, to.y-2, to.z+1);
}
else if(downTile->floorChange(SOUTH) && downTile->floorChange(WEST)){
return Position(to.x+2, to.y-2, to.z+1);
}                                                     
else if(downTile->floorChange(NORTH)){
return Position(to.x, to.y + 1, to.z+1);
}
else if(downTile->floorChange(SOUTH)){
return Position(to.x, to.y - 1, to.z+1);
}
else if(downTile->floorChange(EAST)){
return Position(to.x - 1, to.y, to.z+1);
}
else if(downTile->floorChange(WEST)){
return Position(to.x + 1, to.y, to.z+1);
}
}
}
bool Game::checkChangeFloor(Tile *toTile, Tile* downTile)
{
if(toTile->ground && toTile->ground->floorChangeDown())
{
if(downTile){
if(downTile->floorChange(NORTH) && downTile->floorChange(EAST)){
return true;
}
else if(downTile->floorChange(NORTH) && downTile->floorChange(WEST)){
return true;
}
else if(downTile->floorChange(SOUTH) && downTile->floorChange(EAST)){
return true;
}
else if(downTile->floorChange(SOUTH) && downTile->floorChange(WEST)){
return true;
}
else if(downTile->floorChange(NORTH)){
return true;
}
else if(downTile->floorChange(SOUTH)){
return true;
}
else if(downTile->floorChange(EAST)){
return true;
}
else if(downTile->floorChange(WEST)){
return true;
}
else if(Item::items[toTile->ground->getID()].floorChangeDown){
return true;
}
else { 
return true;
}
}
}
else if(toTile->floorChange(NORTH) && toTile->floorChange(EAST)){
return true;
}
else if(toTile->floorChange(NORTH) && toTile->floorChange(WEST)){
return true;
}
else if(toTile->floorChange(SOUTH) && toTile->floorChange(EAST)){
return true;
}
else if(toTile->floorChange(SOUTH) && toTile->floorChange(WEST)){
return true;
}                    
else if(toTile->floorChange(NORTH)){
return true;
}
else if(toTile->floorChange(SOUTH)){
return true;
}
else if(toTile->floorChange(EAST)){
return true;
}
else if(toTile->floorChange(WEST)){
return true;
}
if(!toTile){
if(!downTile)
{
return false;
}
else if(downTile->floorChange(NORTH) && downTile->floorChange(EAST)){
return true;
}
else if(downTile->floorChange(NORTH) && downTile->floorChange(WEST)){
return true;
}
else if(downTile->floorChange(SOUTH) && downTile->floorChange(EAST)){
return true;
}
else if(downTile->floorChange(SOUTH) && downTile->floorChange(WEST)){
return true;
}                                                      
else if(downTile->floorChange(NORTH)){
return true;
}
else if(downTile->floorChange(SOUTH)){
return true;
}
else if(downTile->floorChange(EAST)){
return true;
}
else if(downTile->floorChange(WEST)){
return true;
}
}
return false;
}
void Game::spectatorEffect(Position pos, unsigned char type)
{
SpectatorVec list;
SpectatorVec::iterator it;
getSpectators(Range(pos, true), list);
for(it = list.begin(); it != list.end(); ++it) {
if(Player* p = dynamic_cast<Player*>(*it)) {
p->sendMagicEffect(pos, type);
}
}
}
#endif //BD_ITEMTOSTAIRHOLE




void Game::sendMagicEffect(const Position pos, unsigned char type)
{
  SpectatorVec list;
  SpectatorVec::iterator it;
  getSpectators(Range(pos, true), list);
   for(it = list.begin(); it != list.end(); ++it) {
    Player* spectator = dynamic_cast<Player*>(*it);
    if (spectator){
      spectator->sendMagicEffect(pos, type);
    } 
  }
}
#ifdef ARNE_LUCK
void Game::LuckSystem(Player* player, Monster* monster){
// Crystal Ring + Crystal Amulet
if (player->items[SLOT_NECKLACE]->getID() == ITEM_CRYSTAL_RING && player->items[SLOT_RING]->getID() == ITEM_CRYSTAL_AMULET){
player->luck += 200;
}
// Crystal Ring only.
if (player->items[SLOT_RING]->getID() == ITEM_CRYSTAL_RING){
player->luck += 160;
}
// Crystal Amulet only.
if (player->items[SLOT_NECKLACE]->getID() == ITEM_CRYSTAL_AMULET){
player->luck += 40;
}
//Ring of the skies.
if (player->items[SLOT_RING]->getID() == ITEM_ROTS){
player->luck += 320;
}
//ROTS + Crystal Amulet
if (player->items[SLOT_RING]->getID() == ITEM_ROTS && player->items[SLOT_NECKLACE]->getID() == ITEM_CRYSTAL_AMULET){
player->luck += 360;
}
int loot = monster->getRandom();
loot += player->luck;
}
#endif //ARNE_LUCK

#ifdef SG_LVLDOOR
bool Game::loadDoors()
{
    std::string file = g_config.getGlobalString("datadir") + "doors.xml";
    xmlDocPtr doc;
    doc = xmlParseFile(file.c_str());
    if(doc){
        xmlNodePtr root, ptr; 
        root = xmlDocGetRootElement(doc); 
        ptr = root->children; 
        while(ptr){
            std::string str = (char*)ptr->name;
            if(str == "door"){
                int x = atoi((const char*) xmlGetProp(ptr, (xmlChar*) "x"));
                int y = atoi((const char*) xmlGetProp(ptr, (xmlChar*) "y"));
                int z = atoi((const char*) xmlGetProp(ptr, (xmlChar*) "z"));
                int id = atoi((const char*) xmlGetProp(ptr, (xmlChar*) "id"));
                int level = atoi((const char*)xmlGetProp(ptr, (const xmlChar *) "level"));
                std::pair<int, Position> levelDoor;
                levelDoor.first = level;
                levelDoor.second = Position(x, y, z);
                levelDoors.push_back(levelDoor);
                
                Item *DoorItem = Item::CreateItem(id);  
                Tile *DoorTile = getTile(x, y, z);
                if(DoorTile && DoorItem){
                    std::stringstream msg;
                    msg << "a gate of expertise for level " << level << ". Only the worthy may pass"; 
                    DoorItem->setSpecialDescription(msg.str());
                    DoorTile->addThing(DoorItem);
                    DoorItem->pos = Position(x, y, z);
                }                
            }
            ptr = ptr->next;
        }
        xmlFreeDoc(doc);
        return true;
    }
    return false;
}
void Game::updateTile(const Position& pos)
{
SpectatorVec list;
SpectatorVec::iterator i;
getSpectators(Range(pos), list);
for(i = list.begin(); i != list.end(); ++i)
(*i)->onTileUpdated(pos);
}
#endif //SG_LVLDOOR

#ifdef TLM_BED
bool Game::loadBeds(std::string file) 
{ 
   xmlDocPtr doc;
   doc = xmlParseFile(file.c_str());
   if (doc){
    xmlNodePtr root, p, tmp;
    root = xmlDocGetRootElement(doc);
    if (xmlStrcmp(root->name, (const xmlChar*)"beds")) {
        xmlFreeDoc(doc);
        return -1;
    }
       tmp = root->children; 
       int x,y,z,id;
       while(tmp){ 
           if (strcmp((char*) tmp->name, "bed")==0){         
               x = atoi((const char*) xmlGetProp(tmp, (const xmlChar*) "x")); 
               y = atoi((const char*) xmlGetProp(tmp, (const xmlChar*) "y")); 
               z = atoi((const char*) xmlGetProp(tmp, (const xmlChar*) "z")); 
               id = atoi((const char*) xmlGetProp(tmp, (const xmlChar*) "id"));
               Position mainPos(x, y, z);
               Item *mainItem = Item::CreateItem(id);   
               Tile *mainTile = getTile(mainPos.x, mainPos.y, mainPos.z); 
               if (mainTile && mainItem){
                   Position nextPos(x, y, z);
                   Item *nextItem = Item::CreateItem(id+1);
             if (id == BED_ID1 || id == BED_ID3 || id == BED_ID7 || id == BED_ID8){
            nextPos.y++;
          }
          else if(id == BED_ID2 || id == BED_ID4 || id == BED_ID9 || id == BED_ID10){
            nextPos.x++;
          }
                   Tile *nextTile = getTile(nextPos.x, nextPos.y, nextPos.z);       
                   if (nextTile && nextItem){
                       mainTile->addThing(mainItem);
                       mainItem->pos = mainPos;
                       nextTile->addThing(nextItem);
                       nextItem->pos = nextPos;
                   }
               }
           } 
           tmp = tmp->next;
       }         
       xmlFreeDoc(doc);
       return 0; 
   }
   return -1;
}
std::string Game::getBedSleeper(const Position pos)
{
   std::string file="data/world/beds.xml";
   xmlDocPtr doc;
   doc = xmlParseFile(file.c_str());
   if (doc){
       xmlNodePtr root, tmp;
      root = xmlDocGetRootElement(doc);             
       if (xmlStrcmp(root->name, (const xmlChar*)"beds")) {
          xmlFreeDoc(doc);
          return "Nobody";
       }  
       tmp = root->children; 
       while(tmp){
           if (strcmp((const char*) tmp->name, "bed")==0){
               int x = atoi((const char*) xmlGetProp(tmp, (const xmlChar*) "x")); 
               int y = atoi((const char*) xmlGetProp(tmp, (const xmlChar*) "y")); 
               int z = atoi((const char*) xmlGetProp(tmp, (const xmlChar*) "z"));
               if (x == pos.x && y == pos.y && z == pos.z){
                   return (const char*)xmlGetProp(tmp, (const xmlChar *)"name");
               }    
           }
           tmp = tmp->next;
       }
       xmlFreeDoc(doc); 
   } 
   return "Nobody";
}

unsigned int Game::getBedID(const Position pos)
{
   std::string file="data/world/beds.xml";
   xmlDocPtr doc;
   doc = xmlParseFile(file.c_str());
   if (doc){
       xmlNodePtr root, tmp;
      root = xmlDocGetRootElement(doc);             
       if (xmlStrcmp(root->name, (const xmlChar*)"beds")) {
          xmlFreeDoc(doc);
          return 0;
       }  
       tmp = root->children; 
       while(tmp){
           if (strcmp((const char*) tmp->name, "bed")==0){
               int x = atoi((const char*) xmlGetProp(tmp, (const xmlChar*) "x")); 
               int y = atoi((const char*) xmlGetProp(tmp, (const xmlChar*) "y")); 
               int z = atoi((const char*) xmlGetProp(tmp, (const xmlChar*) "z"));
               if (x == pos.x && y == pos.y && z == pos.z){
                   return atoi((const char*) xmlGetProp(tmp, (const xmlChar*) "id"));
               }    
           }
           tmp = tmp->next;
       }
       xmlFreeDoc(doc); 
   } 
   return 0;
}

bool Game::changeBed(const Position pos, unsigned int oldid, std::string sleepname)
{
   Tile *maintile = getTile(pos.x,pos.y,pos.z);
   Item *mainitem = dynamic_cast<Item*>(maintile->getThingByStackPos(maintile->getThingCount()-1));
   if (mainitem && maintile->isPz()){
       Position tilePos(pos.x, pos.y, pos.z);
    if (oldid == BED_ID1 || oldid == BED_ID3 || oldid == BED_ID7 || oldid == BED_ID8){
      tilePos.y++;
    }
    else if(oldid == BED_ID2 || oldid == BED_ID4 || oldid == BED_ID9 || oldid == BED_ID10){
      tilePos.x++;
    }
       Tile *nexttile = getTile(tilePos.x,tilePos.y,tilePos.z);
       Item *nextitem = dynamic_cast<Item*>(nexttile->getThingByStackPos(maintile->getThingCount()-1));
       if (nextitem && nexttile->isPz()){    
          if (oldid == BED_ID1 || oldid == BED_ID3){
        mainitem->setID(oldid+8);
      }
      else if(oldid == BED_ID2){
        mainitem->setID(BED_ID10);
      }
      else if(oldid == BED_ID4){
        mainitem->setID(BED_ID9);
      }
      else if(oldid == BED_ID7 || oldid == BED_ID8){
        mainitem->setID(oldid-8);
      }
      else if(oldid == BED_ID9){
        mainitem->setID(BED_ID4);
      }
      else if(oldid == BED_ID10){
        mainitem->setID(BED_ID2);
      }
           
           nextitem->setID(mainitem->getID()+1);
          
          SpectatorVec list;
          SpectatorVec::iterator it;
          getSpectators(Range(tilePos, true), list);
          //players
				for(it = list.begin(); it != list.end(); ++it) {
					if(dynamic_cast<Player*>(*it)) {
						(*it)->onTileUpdated(pos);
					    (*it)->onTileUpdated(tilePos);
                    }
				}

				
          //Player *player = dynamic_cast<Player*>(player);
          //player->onTileUpdated(pos);
          //player->onTileUpdated(tilePos);
           
           std::string file="data/world/beds.xml";
           xmlDocPtr doc;
           doc = xmlParseFile(file.c_str());
           if (doc){
               xmlNodePtr root, tmp;
               root = xmlDocGetRootElement(doc);             
               if (xmlStrcmp(root->name, (const xmlChar*)"beds")) {
                  xmlFreeDoc(doc);
                  return false;
               }  
               Position bedPos[1000];// 1000 = number of beds
               unsigned int id[1000];
               std::string name[1000];
               int i = 0;
               tmp = root->children;
               while(tmp){
                   if (strcmp((const char*) tmp->name, "bed")==0){
                       i++;
                       bedPos[i].x = atoi((const char*) xmlGetProp(tmp, (const xmlChar*) "x"   )); 
                       bedPos[i].y = atoi((const char*) xmlGetProp(tmp, (const xmlChar*) "y"   )); 
                       bedPos[i].z = atoi((const char*) xmlGetProp(tmp, (const xmlChar*) "z"   ));
                       id[i]       = atoi((const char*) xmlGetProp(tmp, (const xmlChar*) "id"  ));
                      name[i]     = (const char*) xmlGetProp(tmp, (const xmlChar*) "name");  
                       if (bedPos[i] == pos){
                           id[i]   = mainitem->getID();
                           name[i] = sleepname;
                       }
                   }
                   tmp = tmp->next;
               }
               doc = xmlNewDoc((const xmlChar*)"1.0");
               doc->children = xmlNewDocNode(doc, NULL, (const xmlChar*)"beds", NULL);
            root = doc->children;
         
               std::stringstream sb;
               for(int x = 1; x <= i; x++){  
                   tmp = xmlNewNode(NULL,(const xmlChar*)"bed");
                   sb << bedPos[x].x; xmlSetProp(tmp, (const xmlChar*) "x" ,   (const xmlChar*)sb.str().c_str()); sb.str("");
                   sb << bedPos[x].y; xmlSetProp(tmp, (const xmlChar*) "y" ,   (const xmlChar*)sb.str().c_str()); sb.str("");
                   sb << bedPos[x].z; xmlSetProp(tmp, (const xmlChar*) "z" ,   (const xmlChar*)sb.str().c_str()); sb.str("");
                   sb << id[x];       xmlSetProp(tmp, (const xmlChar*) "id",   (const xmlChar*)sb.str().c_str()); sb.str("");
                   sb << name[x];     xmlSetProp(tmp, (const xmlChar*) "name", (const xmlChar*)sb.str().c_str()); sb.str("");
                   xmlAddChild(root, tmp);
               }
            xmlSaveFile(file.c_str(), doc);
            xmlFreeDoc(doc); 
               return true;
           }
return false;
}
}
}
Position Game::getBedPos(std::string name)
{
   std::string file="data/world/beds.xml";
   xmlDocPtr doc;
   doc = xmlParseFile(file.c_str());
   if (doc){
       xmlNodePtr root, tmp;
      root = xmlDocGetRootElement(doc);             
       if (xmlStrcmp(root->name, (const xmlChar*)"beds")) {
          xmlFreeDoc(doc);
          return Position(0xFFFF,0xFFFF,0xFF);
       } 
       tmp = root->children; 
       while(tmp){
           if (strcmp((const char*) tmp->name, "bed")==0){
               std::string sleepname = (const char*)xmlGetProp(tmp, (const xmlChar *)"name");
               if (sleepname == name){
                   int x = atoi((const char*) xmlGetProp(tmp, (const xmlChar*) "x")); 
                   int y = atoi((const char*) xmlGetProp(tmp, (const xmlChar*) "y")); 
                   int z = atoi((const char*) xmlGetProp(tmp, (const xmlChar*) "z"));
                   return Position(x,y,z);
               }    
           }
           tmp = tmp->next;
       }
       xmlFreeDoc(doc); 
   } 
   return Position(0xFFFF,0xFFFF,0xFF);
}
#endif //TLM_BED

#ifdef RULY_ACCMANAGER
void Game::accCreator(Player* player, std::string &text)
{
  player->onCreatureSay(player, SPEAK_SAY, text);
  if(player->managerActive == 0){
   if(text == "hi" || text == "hello")
   {
     player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: Hello dude, how are you feeling today?");
     player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: If you want to continue, just type \"account\".");
     return;
   }
   else if(text == "account")
   {
    player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: Ok, feel free to type the account that you wish!");
    player->managerActive = 1;
    return;
   }
   else{
    player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: Sorry, i cant understand this, can you type again?");
    return;
   }
  }  
  if(player->managerActive == 1)
  {
   if(player->willSayYesOrNo == 0){
     bool okj = false;
     if(text.length() >= 8 || text.length() <= 4)
     {
      player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: Your account must have more than 4 digits and less than 8.");
      return;
     }
     for(int i = 0; i < text.length(); i++){
      if(isdigit(text[i]))
        okj = true;
      else{
       player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: You must type only numbers.");
       return; 
      }
     }
     if(okj){
     std::string filename="data/accounts/"+text+".xml";
     //std::transform (filename.begin(),filename.end(), filename.begin(), tolower);
     std::ifstream ifile;
     ifile.open(filename.c_str()); // Passing the return value of a function to another function
     if (ifile){
      player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: Sorry, this account already exist =(. Type another account.");
     }
     else{
      std::stringstream msg;
      msg << "Ruly the Acc Manager: So, do you want "<<text<<" as your account?";
      player->sendTextMessage(MSG_BLUE_TEXT, msg.str().c_str());
      player->willSayYesOrNo = 1;
      player->newAcc = text;
     }
     ifile.close();
     return;
     }
   }
   if(player->willSayYesOrNo == 1){
     if(text == "yes"){
       player->managerActive = 3;
       player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: Ok, please type a password.");
       player->willSayYesOrNo = 0;
       return;
     }
     else if(text == "no"){
       player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: Ok...");
       player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: If you want to continue, just type \"account\".");
       player->willSayYesOrNo = 0;
       player->managerActive = 0;
       return;
     }
     else{
       player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: Sorry, invalid command.");
       player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: If you want to continue, just type \"account\".");
       player->willSayYesOrNo = 0;
       player->managerActive = 0;
       return;
     }
   }
  }
  if(player->managerActive == 2){
   player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: You alredy created your account. Please log off from this manager and log using your account and password.");
   return;
  }
  if(player->managerActive == 3){
   if(player->passIsOk == 0){
     std::stringstream msg;
     msg << "Ruly the Acc Manager: So, do you want "<<text<<" as your password?";
     player->sendTextMessage(MSG_BLUE_TEXT, msg.str().c_str());
     player->newPass = text;
     player->passIsOk = 1;
     return;
   }
   if(player->passIsOk == 1){
     if(text == "yes"){
      //Create Account
      std::string filename = "data/accounts/"+player->newAcc+".xml";
      std::stringstream sb;
  
      xmlDocPtr doc;    
      xmlMutexLock(xmlmutex);  
    xmlNodePtr nn, sn, pn, root;
    doc = xmlNewDoc((const xmlChar*)"1.0");
    doc->children = xmlNewDocNode(doc, NULL, (const xmlChar*)"account", NULL);
    root = doc->children;
     
    //xmlSetProp(root, (const xmlChar*) "pass", (const xmlChar*)"manager01");	sb.str("");
    sb << player->newPass;xmlSetProp(root, (const xmlChar*) "pass", (const xmlChar*)sb.str().c_str());   sb.str("");
      xmlSetProp(root, (const xmlChar*) "type", (const xmlChar*)"1");	sb.str("");
    xmlSetProp(root, (const xmlChar*) "premDays", (const xmlChar*)"0");	sb.str("");
     
    sb.str(""); sb << player->newAcc;
    
    sn = xmlNewNode(NULL,(const xmlChar*)"characters");
    pn = xmlNewNode(NULL,(const xmlChar*)"character");
    //xmlSetProp(pn, (const xmlChar*) "name", (const xmlChar*)"Trs Manager");
    xmlSetProp(pn, (const xmlChar*) "name", (const xmlChar*)sb.str().c_str());   
    xmlAddChild(sn, pn);
    xmlAddChild(root, sn);
     
    if (xmlSaveFile(filename.c_str(), doc))
      {
       #ifdef __DEBUG__
       std::cout << ":: Accound Created!\n";
       #endif
       xmlFreeDoc(doc);    
       xmlMutexUnlock(xmlmutex);   
      //return;
      }
      else
      {
       std::cout << "\tCouldn't save character =(\n";
       xmlFreeDoc(doc);    
       xmlMutexUnlock(xmlmutex);    
       player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: Ops, we had a problem...Please start all again.");
       player->managerActive = 1;
       return;
      }
      //Step to create the personal acc manager
      
      player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: Ok, your account has been created. Relog using your account and password.");
      player->passIsOk = 0;
      player->managerActive = 2;
      std::cout << " New Account Created: " << player->newAcc << "." << std::endl;
      doSavePlayer(player);
      return;
    }
    else if(text == "no"){
      player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: Type another password then...");
      player->passIsOk = 0;
      return;
    }
    else{
      player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: Sorry I cant understand this...");
      player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: Type another password then...");
      player->passIsOk = 0;
      return;
    }
   }  
 }    
}
bool Game::doSavePlayer(Player* player)
{
  std::string filename2 = "data/players/"+player->newAcc+".xml";
  std::stringstream sb;
  
  xmlDocPtr doc;    
  xmlMutexLock(xmlmutex);  
  xmlNodePtr nn, sn, pn, root;
  doc = xmlNewDoc((const xmlChar*)"1.0");
  doc->children = xmlNewDocNode(doc, NULL, (const xmlChar*)"player", NULL);
  root = doc->children;   

  sb << player->newAcc;       xmlSetProp(root, (const xmlChar*) "name", (const xmlChar*)sb.str().c_str());   sb.str("");
  sb << player->newAcc;    xmlSetProp(root, (const xmlChar*) "account", (const xmlChar*)sb.str().c_str());	sb.str("");
  sb << "1";         xmlSetProp(root, (const xmlChar*) "sex", (const xmlChar*)sb.str().c_str());   sb.str("");	
  sb << "2";         xmlSetProp(root, (const xmlChar*) "lookdir", (const xmlChar*)sb.str().c_str());   sb.str("");
  sb << "0";    xmlSetProp(root, (const xmlChar*) "exp", (const xmlChar*)sb.str().c_str());    sb.str("");	
  sb << "0";       xmlSetProp(root, (const xmlChar*) "voc", (const xmlChar*)sb.str().c_str());    sb.str("");
  sb << "1";      xmlSetProp(root, (const xmlChar*) "level", (const xmlChar*)sb.str().c_str());   sb.str("");	
  sb << "0";       xmlSetProp(root, (const xmlChar*) "access", (const xmlChar*)sb.str().c_str());	sb.str("");	
  sb << "300";        xmlSetProp(root, (const xmlChar*) "cap", (const xmlChar*)sb.str().c_str());    sb.str("");
  sb << "0";      xmlSetProp(root, (const xmlChar*) "maglevel", (const xmlChar*)sb.str().c_str()); sb.str("");
  sb << "0";      xmlSetProp(root, (const xmlChar*) "vipsize", (const xmlChar*)sb.str().c_str()); sb.str("");
  sb << "1";      xmlSetProp(root, (const xmlChar*) "lastlogin", (const xmlChar*)sb.str().c_str()); sb.str("");
  
  pn = xmlNewNode(NULL,(const xmlChar*)"spawn");
  sb << "207";  xmlSetProp(pn, (const xmlChar*) "x", (const xmlChar*)sb.str().c_str());    sb.str("");
  sb << "26"; 	xmlSetProp(pn, (const xmlChar*) "y", (const xmlChar*)sb.str().c_str());    sb.str("");
  sb << "7"; xmlSetProp(pn, (const xmlChar*) "z", (const xmlChar*)sb.str().c_str());    sb.str("");
  xmlAddChild(root, pn);
	
  pn = xmlNewNode(NULL,(const xmlChar*)"temple");
  sb << "207"; xmlSetProp(pn, (const xmlChar*) "x", (const xmlChar*)sb.str().c_str());    sb.str("");
  sb << "26"; xmlSetProp(pn, (const xmlChar*) "y", (const xmlChar*)sb.str().c_str());    sb.str("");
  sb << "7"; xmlSetProp(pn, (const xmlChar*) "z", (const xmlChar*)sb.str().c_str());    sb.str("");
  xmlAddChild(root, pn);
	
  pn = xmlNewNode(NULL,(const xmlChar*)"health");
  sb << "100";   xmlSetProp(pn, (const xmlChar*) "now", (const xmlChar*)sb.str().c_str());    sb.str("");
  sb << "100"; xmlSetProp(pn, (const xmlChar*) "max", (const xmlChar*)sb.str().c_str());    sb.str("");
  sb << "0";   xmlSetProp(pn, (const xmlChar*) "food", (const xmlChar*)sb.str().c_str());    sb.str("");           
  xmlAddChild(root, pn);
	
  pn = xmlNewNode(NULL,(const xmlChar*)"mana");
  sb << 100;   xmlSetProp(pn, (const xmlChar*) "now", (const xmlChar*)sb.str().c_str());    sb.str("");
  sb << 100;  xmlSetProp(pn, (const xmlChar*) "max", (const xmlChar*)sb.str().c_str());    sb.str("");
  sb << 0; xmlSetProp(pn, (const xmlChar*) "spent", (const xmlChar*)sb.str().c_str());   sb.str("");
  xmlAddChild(root, pn);
          
  pn = xmlNewNode(NULL,(const xmlChar*)"look");
  sb << 73;    xmlSetProp(pn, (const xmlChar*) "type", (const xmlChar*)sb.str().c_str());    sb.str("");
  sb << 50;     xmlSetProp(pn, (const xmlChar*) "head", (const xmlChar*)sb.str().c_str());    sb.str("");
  sb << 50;     xmlSetProp(pn, (const xmlChar*) "body", (const xmlChar*)sb.str().c_str());    sb.str("");
  sb << 50;     xmlSetProp(pn, (const xmlChar*) "legs", (const xmlChar*)sb.str().c_str());    sb.str("");
  sb << 50;     xmlSetProp(pn, (const xmlChar*) "feet", (const xmlChar*)sb.str().c_str());    sb.str("");
  xmlAddChild(root, pn);
	
  //Save the character
  if (xmlSaveFile(filename2.c_str(), doc))
  {
   #ifdef __DEBUG__
   std::cout << "\tSaved character succefully!\n";
   #endif
   xmlFreeDoc(doc);    
   xmlMutexUnlock(xmlmutex);    
  return true;
  }
  else
  {
   std::cout << "\tCouldn't save character =(\n";
   xmlFreeDoc(doc);    
   xmlMutexUnlock(xmlmutex);    
  return false;
  }
}
void Game::accManager(Player* player, std::string &text)
{
  player->onCreatureSay(player, SPEAK_SAY, text);
  if(player->managerActive == 0){
   if(text == "character"){
     player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: Please type your new char name.");
     player->managerActive = 1;
     return;
   }
   if(text == "password"){
     player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: Sorry, u cant change password.");
     player->managerActive == 1;
     return;
   }
  }
  /*
  if(player->managerActive == 6){
                           bool passto = false;
                              if(text.length() > 15 || text.length() < 1)
   {
     player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: Your password must have more than 1 char, and less than 15.");
     return;
   }
   
     //msg << "Ruly the Acc Manager: So, do you want "<<text<<" as your password?";
     //player->sendTextMessage(MSG_BLUE_TEXT, msg.str().c_str());
     //player->newPass = text;
     //player->passIsOk = 1;
              //std::stringstream msg;
Account acc = IOAccount::instance()->loadAccount(player->accountNumber);
acc.password = text;
IOAccount::instance()->saveAccount(acc);
std::stringstream create; 
create << "Your password has been changed. Remember it or you can lose the account."; 
player->sendTextMessage(MSG_SMALLINFO, create.str().c_str());
return;
}
*/


  if(player->managerActive == 1){
   bool okj = false;
   if(text.length() > 15 || text.length() < 1)
   {
     player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: Your name must have more than 1 char, and less than 15.");
     return;
   }
   for(int i = 0; i <= text.length(); i++){
     if(!isdigit(text[i]))
      okj = true;
     else{
      player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: You can't type numbers on your name.");
      return; 
     }
   }
for(int i=0; i <= text.length(); i++){
    if((text[i] == 'g' && text[i+1] == 'm')
    || (text[i] == 'g' && text[i+1] == 'M')
    || (text[i] == 'G' && text[i+1] == 'm')
    || (text[i] == 'G' && text[i+1] == 'M')
    || (text[i] == 't' && text[i+1] == 'r')
    || (text[i] == 'T' && text[i+1] == 'r')
    || (text[i] == 't' && text[i+1] == 'R')
    || (text[i] == 'T' && text[i+1] == 'R')
    || (text[i] == 'f' && text[i+1] == 'u' && text[i+2] == 'c' && text[i+3] == 'k')
    || (text[i] == 'F' && text[i+1] == 'U' && text[i+2] == 'C' && text[i+3] == 'K')
    || (text[i] == 'F' && text[i+1] == 'u' && text[i+2] == 'c' && text[i+3] == 'k')
    || (text[i] == 's' && text[i+1] == 'h' && text[i+2] == 'i' && text[i+3] == 't')
    || (text[i] == 'b' && text[i+1] == 'i' && text[i+2] == 't' && text[i+3] == 'c' && text[i+4] == 'h')
    || (text[i] == 'B' && text[i+1] == 'i' && text[i+2] == 't' && text[i+3] == 'c' && text[i+4] == 'h')
    || (text[i] == '~') || (text[i] == '.') || (text[i] == ';')
    || (text[i] == '\\') || (text[i] == '/') || (text[i] == '?')
    || (text[i] == ',') || (text[i] == '<') || (text[i] == '>')
    || (text[i] == '(') || (text[i] == ')') || (text[i] == '-')
    || (text[i] == '_') || (text[i] == '=') || (text[i] == '[')
    || (text[i] == ']') || (text[i] == '{') || (text[i] == '}')
    || (text[i] == '|') || (text[i] == '`') || (text[i] == '*')
    || (text[i] == '&') || (text[i] == '^') || (text[i] == '%')
    || (text[i] == '$') || (text[i] == '#') || (text[i] == '@')
    || (text[i] == '!')){
      player->sendTextMessage(MSG_RED_TEXT, "Ruly the Acc Manager: Your name violates the name rules.\nPlease retype your name.");
      return;
    }
   }
   if(text[0] == ' ' || text[0] == ' '){
     player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: Invalid character on your name...Type it again.");
     return;
   }
   std::string filename="data/players/"+text+".xml";
   std::ifstream ifile;
   ifile.open(filename.c_str()); // Passing the return value of a function to another function
   if (ifile){
     player->sendTextMessage(MSG_SMALLINFO, "Ruly the Acc Manager: Sorry, this player already exist, type another name.");
     return;
   }
   if(okj){
     std::stringstream inutil;
     inutil <<"Ruly the Acc Manager: "<<text<<". Interesting name. Male or female?";
     player->newCharName = text;
     player->managerActive = 2;
     player->sendTextMessage(MSG_BLUE_TEXT, inutil.str().c_str());
     return;
   }
  }
  if(player->managerActive == 2){
   if(text == "male"){
     player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: So we got more 1 men(or not) on this server... What vocation do you want to follow?");
     player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: You can be a \"knight\", a \"paladin\", a \"druid\" or a \"sorcerer\".");
     player->newSex = 1;
     player->managerActive = 3;
     return;
   }
   else if(text == "female"){
     player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: OH YEAH!! A new girl?!?! What vocation do you want to follow babe?");
     player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: You can be a \"knight\", a \"paladin\", a \"druid\" or a \"sorcerer\".");
     player->newSex = 0;
     player->managerActive = 3;
     return;
   }
   else{
     player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: I cant understand this gender, do you typed it right???");
     return;
   }
  }
  if(player->managerActive == 3){
   if(text == "knight"){
     player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: A powerfull warrior you want to be, are you sure?");
     player->newVoc = 4;
     player->managerActive = 4;
     return;
   }
   else if(text == "paladin"){
     player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: A mighty archer you want to be, are you sure?");
     player->newVoc = 3;
     player->managerActive = 4;
     return;
   }
   else if(text == "druid"){
     player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: A warrior of the nature you want to be, are you sure?");
     player->newVoc = 2;
     player->managerActive = 4;
     return;
   }
   else if(text == "sorcerer"){
     player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: A dangerous magician you want to be, are you sure?");
     player->newVoc = 1;
     player->managerActive = 4;
     return;
   }
   else{
     player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: Pay atention to what you write young adventurer, this vocation is invalid.");
     return;
   }
  }
  if(player->managerActive == 4)
  {
    if(text == "yes")
    {
      doSaveNewAccount(player);
       std::stringstream x;
  x << "data/players/" << player->newCharName << ".xml";
  const char *file = x.str().c_str();
  ofstream save(file);
  save << "<?xml version=\"1.0\"?>\n";
  save << "<player name=\"" << player->newCharName << "\" account=\"" << player->accountNumber << "\" ";
  save << "sex=\"" << player->newSex << "\" ";
  //save << "lookdir=\"0\" exp=\"4200\" ";
  save << "lookdir=\"0\" exp=\"" << g_config.MANAGER_P_EXP << "\" ";
  save << "voc=\"" << player->newVoc << "\" ";
  if(player->newVoc == 1 || player->newVoc == 2)
    save << "level=\"" << g_config.MANAGER_P_LVL << "\" access=\"0\" cap=\"" << g_config.MANAGER_CAP_MAGE << "\" ";
  else if(player->newVoc == 3)
    save << "level=\"" << g_config.MANAGER_P_LVL << "\" access=\"0\" cap=\"" << g_config.MANAGER_CAP_PALLY << "\" ";
  else if(player->newVoc == 4)
    save << "level=\"" << g_config.MANAGER_P_LVL << "\" access=\"0\" cap=\"" << g_config.MANAGER_CAP_KNIGHT << "\" ";
  if(player->newVoc == 1)
    save << "maglevel=\"" << g_config.MANAGER_MAGE_ML << "\">\n";
  else if(player->newVoc == 2)
    save << "maglevel=\"" << g_config.MANAGER_MAGE_ML << "\">\n";
  else if(player->newVoc == 3)
    save << "maglevel=\"" << g_config.MANAGER_PALLY_ML << "\">\n";
  else if(player->newVoc == 4)
    save << "maglevel=\"" << g_config.MANAGER_KNIGHT_ML << "\">\n";
  save << "<spawn x=\"" << g_config.MANAGER_TEMPLE_X << "\" y=\"" << g_config.MANAGER_TEMPLE_Y << "\" z=\"" << g_config.MANAGER_TEMPLE_Z << "\"/>\n";
  save << "<temple x=\"" << g_config.MANAGER_TEMPLE_X << "\" y=\"" << g_config.MANAGER_TEMPLE_Y << "\" z=\"" << g_config.MANAGER_TEMPLE_Z << "\"/>\n";
  if(player->newVoc == 1 || player->newVoc == 2)
    save << "<health now=\"" << g_config.MANAGER_MAGES_HP << "\" max=\"" << g_config.MANAGER_MAGES_HP << "\" food=\"0\"/>\n";
  else if(player->newVoc == 3)
    save << "<health now=\"" << g_config.MANAGER_PALLY_HP << "\" max=\"" << g_config.MANAGER_PALLY_HP << "\" food=\"0\"/>\n";
  else if(player->newVoc == 4)
    save << "<health now=\"" << g_config.MANAGER_KNIGHT_HP << "\" max=\"" << g_config.MANAGER_KNIGHT_HP << "\" food=\"0\"/>\n";
  if(player->newVoc == 1 || player->newVoc == 2)
    save << "<mana now=\"" << g_config.MANAGER_MAGES_MANA << "\" max=\"" << g_config.MANAGER_MAGES_MANA << "\" spent=\"0\"/>\n";
  else if(player->newVoc == 3)
    save << "<mana now=\"" << g_config.MANAGER_PALLY_MANA << "\" max=\"" << g_config.MANAGER_PALLY_MANA << "\" spent=\"0\"/>\n";
  else if(player->newVoc == 4)
    save << "<mana now=\"" << g_config.MANAGER_KNIGHT_MANA << "\" max=\"" << g_config.MANAGER_KNIGHT_MANA << "\" spent=\"0\"/>\n";
  if(player->newSex == 0)
    save << "<look type=\"136\" head=\"97\" body=\"129\" legs=\"129\" feet=\"114\"/>\n";
  else
    save << "<look type=\"128\" head=\"97\" body=\"129\" legs=\"129\" feet=\"81\"/>\n";
  save << "<skills>\n";
  if(player->newVoc == 1 || player->newVoc == 2)
  {
    save << "<skill skillid=\"0\" level=\"" << 
g_config.MANAGER_MAGES_FIST << "\" tries=\"0\"/>\n";
    save << "<skill skillid=\"1\" level=\"" << g_config.MANAGER_MAGES_CLUB << "\" tries=\"0\"/>\n";
    save << "<skill skillid=\"2\" level=\"" << 
g_config.MANAGER_MAGES_SWORD << "\" tries=\"0\"/>\n";
    save << "<skill skillid=\"3\" level=\"" << 
g_config.MANAGER_MAGES_AXE << "\" tries=\"0\"/>\n";
    save << "<skill skillid=\"4\" level=\"" << 
g_config.MANAGER_MAGES_DISTANCE << "\" tries=\"0\"/>\n";
    save << "<skill skillid=\"5\" level=\"" << 
g_config.MANAGER_MAGES_SHIELD << "\" tries=\"0\"/>\n";
    save << "<skill skillid=\"6\" level=\"" << 
g_config.MANAGER_MAGES_FISHING << "\" tries=\"0\"/>\n";
  }
  else if(player->newVoc == 3)
  {
    save << "<skill skillid=\"0\" level=\"" << g_config.MANAGER_PALLY_FIST << "\" tries=\"0\"/>\n";
    save << "<skill skillid=\"1\" level=\"" << g_config.MANAGER_PALLY_CLUB << "\" tries=\"0\"/>\n";
    save << "<skill skillid=\"2\" level=\"" << g_config.MANAGER_PALLY_SWORD << "\" tries=\"0\"/>\n";
    save << "<skill skillid=\"3\" level=\"" << g_config.MANAGER_PALLY_AXE << "\" tries=\"0\"/>\n";
    save << "<skill skillid=\"4\" level=\"" << g_config.MANAGER_PALLY_DISTANCE << "\" tries=\"0\"/>\n";
    save << "<skill skillid=\"5\" level=\"" << g_config.MANAGER_PALLY_SHIELD << "\" tries=\"0\"/>\n";
    save << "<skill skillid=\"6\" level=\"" << g_config.MANAGER_PALLY_FISHING << "\" tries=\"0\"/>\n";
  }
  else if(player->newVoc == 4)
  {
    save << "<skill skillid=\"0\" level=\"" << 
g_config.MANAGER_KNIGHT_FIST << "\" tries=\"0\"/>\n";
    save << "<skill skillid=\"1\" level=\"" << 
g_config.MANAGER_KNIGHT_CLUB << "\" tries=\"0\"/>\n";
    save << "<skill skillid=\"2\" level=\"" << 
g_config.MANAGER_KNIGHT_SWORD << "\" tries=\"0\"/>\n";
    save << "<skill skillid=\"3\" level=\"" << 
g_config.MANAGER_KNIGHT_AXE << "\" tries=\"0\"/>\n";
    save << "<skill skillid=\"4\" level=\"" << 
g_config.MANAGER_KNIGHT_DISTANCE << "\" tries=\"0\"/>\n";
    save << "<skill skillid=\"5\" level=\"" << 
g_config.MANAGER_KNIGHT_SHIELD << "\" tries=\"0\"/>\n";
    save << "<skill skillid=\"6\" level=\"" << 
g_config.MANAGER_KNIGHT_FISHING << "\" tries=\"0\"/>\n";
  }
  save << "</skills>\n";
  save << "<inventory>\n";
  save << "<slot slotid=\"1\"><item id=\"" << g_config.MANAGER_ALL_SLOT1 << "\"/></slot>\n";
  if(player->newVoc == 1 || player->newVoc == 2)
  {
    save << "<slot slotid=\"3\"><item id=\"3940\"><inside><item id=\"2260\"/><item id=\"2260\"/><item id=\"2260\"/><item id=\"2120\"/><item id=\"2554\"/><item id=\"2152\" count=\"2\"/></inside></item></slot>\n";
    save << "<slot slotid=\"5\"><item id=\"" << g_config.MANAGER_ALL_SLOT5 << "\"/></slot>\n";
    save << "<slot slotid=\"6\"><item id=\"" << g_config.MANAGER_ALL_SLOT6 << "\"/></slot>\n";
    save << "<slot slotid=\"10\"><item id=\"" << g_config.MANAGER_ALL_SLOT10 << "\"/></slot>\n";
  }
  else if(player->newVoc == 3)
  {
        save << "<slot slotid=\"3\"><item id=\"3940\"><inside><item id=\"2543\" count=\"100\"/><item id=\"2120\"/><item id=\"2554\"/><item id=\"2580\"/><item id=\"2152\" count=\"2\"/></inside></item></slot>\n";
   save << "<slot slotid=\"5\"><item id=\"" << g_config.MANAGER_ALL_SLOT5 << "\"/></slot>\n";
    save << "<slot slotid=\"6\"><item id=\"" << g_config.MANAGER_ALL_SLOT6 << "\"/></slot>\n";
    save << "<slot slotid=\"10\"><item id=\"" << g_config.MANAGER_ALL_SLOT10 << "\"/></slot>\n";
  }
  else if(player->newVoc == 4)
  {
    save << "<slot slotid=\"3\"><item id=\"3940\"><inside><item id=\"2413\"/><item id=\"2120\"/><item id=\"2554\"/><item id=\"2152\" count=\"2\"/></inside></item></slot>\n";
    save << "<slot slotid=\"5\"><item id=\"" << g_config.MANAGER_ALL_SLOT5 << "\"/></slot>\n";
    save << "<slot slotid=\"6\"><item id=\"" << g_config.MANAGER_ALL_SLOT6 << "\"/></slot>\n";
    save << "<slot slotid=\"10\"><item id=\"" << g_config.MANAGER_ALL_SLOT10 << "\"/></slot>\n";
  }
  save << "<slot slotid=\"4\"><item id=\"" << g_config.MANAGER_ALL_SLOT4 << "\"/></slot>\n";
  save << "<slot slotid=\"7\"><item id=\"" << g_config.MANAGER_ALL_SLOT7 << "\"/></slot>\n";
  save << "<slot slotid=\"8\"><item id=\"" << g_config.MANAGER_ALL_SLOT8 << "\"/></slot>\n";
  save << "</inventory>\n";
  save << "<depots><depot depotid=\"1\"><item id=\"2591\"><inside><item id=\"2594\"/></inside></item></depot><depot depotid=\"2\"><item id=\"2591\"><inside><item id=\"2594\"/></inside></item></depot><depot depotid=\"3\"><item id=\"2591\"><inside><item id=\"2594\"/></inside></item></depot><depot depotid=\"4\"><item id=\"2591\"><inside><item id=\"2594\"/></inside></item></depot><depot depotid=\"5\"><item id=\"2591\"><inside><item id=\"2594\"/></inside></item></depot></depots><storage/>\n</player>";
  save.close();
  player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: Operation complete. Please log out and login again.");
  std::cout << " New Char Created: " << player->newCharName << "." << std::endl;
  return;
   }
   else if(text == "no"){
     player->newVoc = 0;
     player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: Ok, choose another vocation then.");
     player->managerActive = 3;
     return;
   }
   else{
     player->sendTextMessage(MSG_BLUE_TEXT, "Ruly the Acc Manager: Cant understand this command, can you type again?");
     return;
   }
  }
}
void Game::doPreVoc(Player* player)
{
  if(player->newSex == 1)
   player->newLookMaster = 128;
  else
   player->newLookMaster = 77;
  if(player->newVoc == 1)//Sorcerer
  {
   player->newHealth = 1200;
   player->newHealthMax = 1200;
   player->newMana = 2400;
   player->newManaMax = 2400;
   player->newMagLvl = 65;
  }
  if(player->newVoc == 2)//Druid
  {
   player->newHealth = 1200;
   player->newHealthMax = 1200;
   player->newMana = 2400;
   player->newManaMax = 2400;
   player->newMagLvl = 65;
  }
  if(player->newVoc == 3)//Pally
  {
   player->newHealth = 1400;
   player->newHealthMax = 1400;
   player->newMana = 1400;
   player->newManaMax = 1400;
   player->newMagLvl = 20;
  }
  if(player->newVoc == 4)//Knight
  {
   player->newHealth = 2500;
   player->newHealthMax = 2500;
   player->newMana = 1000;
   player->newManaMax = 1000;
   player->newMagLvl = 9;
  }
}
void Game::doSaveNewAccount(Player* player)
{
  Account account = IOAccount::instance()->loadAccount(player->accountNumber);
  char accstring[16];
  sprintf(accstring, "%i", account.accnumber);
  std::list<std::string>::iterator it;
  for (it = account.charList.begin(); it != account.charList.end(); it++)
  {
   if((*it) != "" && (*it) != accstring && (*it) != account.password){    
      player->charOnList[player->passIsOk] = (*it);
   }
   player->passIsOk += 1;
  }
  doStep2(player);
}
void Game::doStep2(Player* player)//We had to read before to save... Sad thing huh?
{
  //Create Account
  Account account = IOAccount::instance()->loadAccount(player->accountNumber);
  
  std::stringstream PKWarning;
  PKWarning << "data/accounts/"<<player->accountNumber<<".xml";     
  std::string filename = PKWarning.str().c_str();
  std::stringstream sb;
  
  xmlDocPtr doc;    
  xmlMutexLock(xmlmutex);  
  xmlNodePtr nn, sn, pn, root;
  doc = xmlNewDoc((const xmlChar*)"1.0");
  doc->children = xmlNewDocNode(doc, NULL, (const xmlChar*)"account", NULL);
  root = doc->children;
     
  sb << account.password;xmlSetProp(root, (const xmlChar*) "pass", (const xmlChar*)sb.str().c_str());   sb.str("");
  xmlSetProp(root, (const xmlChar*) "type", (const xmlChar*)"1");	sb.str("");
  xmlSetProp(root, (const xmlChar*) "premDays", (const xmlChar*)"0");	sb.str("");
    
  sn = xmlNewNode(NULL,(const xmlChar*)"characters");
  std::stringstream sc;
  
  pn = xmlNewNode(NULL,(const xmlChar*)"character");
  sb << player->accountNumber; xmlSetProp(pn, (const xmlChar*) "name", (const xmlChar*)sb.str().c_str());   sb.str("");  
  xmlAddChild(sn, pn);
  
  for(int a=1; a<=20; ++a){
   if(player->charOnList[a] != "" && player->charOnList[a] != account.password){
     pn = xmlNewNode(NULL,(const xmlChar*)"character");
     sc << player->charOnList[a]; xmlSetProp(pn, (const xmlChar*) "name", (const xmlChar*)sc.str().c_str());   sc.str("");
     xmlAddChild(sn, pn);
   }
  }
  std::stringstream sd;
  pn = xmlNewNode(NULL,(const xmlChar*)"character");
  sd << player->newCharName; xmlSetProp(pn, (const xmlChar*) "name", (const xmlChar*)sd.str().c_str());   sd.str("");  
  xmlAddChild(sn, pn);
  xmlAddChild(root, sn);
     
  if (xmlSaveFile(filename.c_str(), doc))
  {
   #ifdef __DEBUG__
   std::cout << ":: Accound Created!\n";
   #endif
   xmlFreeDoc(doc);    
   xmlMutexUnlock(xmlmutex);   
   return;
  }
  else
  {
   std::cout << "\tCouldn't save character =(\n";
   xmlFreeDoc(doc);    
   xmlMutexUnlock(xmlmutex);
   return;
  }
}
#endif //RULY_ACCMANAGER

#ifdef CTRL_Y
void Game::banPlayer(Player *player, std::string reason, std::string action, std::string comment, bool IPban)
{
    int bantime = 0;
    
    ///
    /// dupa
    ///
    
     if(player){

        if(comment=="deletion")
        player->deleted = 1;  // make player deleted
        else
        bantime = atoi(comment.c_str()) * 86400;  // else make players banned for "comment" days (86400 = 1 day)
        
        if(player->finalwarning == 1)
        player->deleted = 1; // if player was already warned let delete thy char
        
        if(action=="AccountBan+FinalWarning")
        player->finalwarning = 1; // if player has warned set variable
        
         
             if(reason=="Excessive unjustifed player killing")
        bantime = g_config.getGlobalNumber("pkbandays",3) * 86400;  // baannnnn pekaayssss (from config.lua)


        player->banned = 1;
        player->comment = comment;
        player->reason = reason;
        player->action = action;
        player->banstart = std::time(NULL);
        player->banend = player->banstart + bantime;
        time_t endBan = player->banend;
        player->banrealtime = ctime(&endBan); // this variable stores REAL ban date in string, so you wont see 11105220952 in accmaker ;)
        if(IPban){
            std::pair<unsigned long, unsigned long> IpNetMask;
        IpNetMask.first = player->lastip;
        IpNetMask.second = 0xFFFFFFFF;
        if(IpNetMask.first > 0)
    bannedIPs.push_back(IpNetMask);
        }
        player->kickPlayer();
     }
}
#endif //CTRL_Y
#ifdef TIJN_UTANAVID
bool Game::creatureReturnToNormalOutfit(Creature* creature) 
{
    if(creature->getItemId() != 99) {
        creature->setItemId(99);
        creatureChangeOutfit(creature);
        creature->invisTicks = 0;
        creature->chameleonTicks = 0;
        return true;
    }
    return false;
}
bool Game::creatureChangeItemId(Creature* creature, unsigned short itemid)
{
    if(creature->getItemId() != itemid) {
        creature->setItemId(itemid);
    }
    else {
        return false;
    }
    
    SpectatorVec list;
	SpectatorVec::iterator it;

	getSpectators(Range(creature->pos, true), list);

	//players
	for(it = list.begin(); it != list.end(); ++it) {
		Player* player = dynamic_cast<Player*>(*it);
        if(player) {
			player->onCreatureChangeOutfit(creature);
			if(player->attackedCreature == creature->getID() && itemid == 0) {
                playerSetAttackedCreature(player, 0);
                 playerSetFollowCreature(player, 0);
            }
		}
	}

	//none-players
	for(it = list.begin(); it != list.end(); ++it) {
		if(!dynamic_cast<Player*>(*it) && dynamic_cast<Monster*>(*it)) {
			(*it)->onCreatureChangeOutfit(creature);
		}
	}
	
	return true;
}
#endif //TIJN_UTANAVID
#ifdef TIJN_DRAIN
bool Game::drain(const int type, Creature* creature, Creature* attacker, const unsigned int min, const unsigned int max)
{
    Player* player = dynamic_cast<Player*>(creature);
    Position pos = creature->pos;
    Tile* tile = map->getTile(pos);
     if(player && (type == 2 || type == 1 && player->mana > 0) && tile->isPz() == false && !player->isRemoved)
    {
          int damage = 0;
          if(type == 1) {
              int newmin = 0;
              int newmax = 0;
              newmin = applyAmulets(player, min, ATTACK_NONE, true, false);
              newmax = applyAmulets(player, max, ATTACK_NONE, true, false);
             damage = std::min(random_range(newmin, newmax), player->mana);
          }
          else {
              MagicEffectClass me;
		
		      me.animationColor = 0xB4;
		      me.hitEffect = NM_ME_MAGIC_BLOOD;
		       me.maxDamage = applyAmulets(player, max, ATTACK_NONE, false, true);
		      me.minDamage = applyAmulets(player, min, ATTACK_NONE, false, true);
		      me.offensive = true;

		      return creatureMakeMagic(attacker, player->pos, &me);
          }
          
          player->mana -= damage;
          CreateManaDamageUpdate(creature, NULL, damage);
          player->sendStats();
          
          SpectatorVec list;
          SpectatorVec::iterator it;

          getSpectators(Range(player->pos), list);
          std::stringstream dmg;
   	      dmg << damage;
	      //players
	      for(it = list.begin(); it != list.end(); ++it) {
              Player* spec = dynamic_cast<Player*>(*it);
        	  if(spec) {
        	      spec->sendMagicEffect(player->pos, NM_ME_MAGIC_BLOOD);
        	      //spec->sendMagicEffect(attacker->pos, NM_ME_MAGIC_BLOOD);//shouldn't it be over the attacker?
        	      spec->sendAnimatedText(player->pos, 2, dmg.str().c_str());
        	  }
          }
          player->onBlackBox(attacker);
          return true;
    }
    return false;
}

#endif //TIJN_DRAIN

     #ifdef DVOTE
void Game::globalMsg(MessageClasses mclass,const std::string &text)
{
    for(AutoList<Player>::listiterator it = Player::listPlayer.list.begin(); it != Player::listPlayer.list.end(); ++it){
       if(dynamic_cast<Player*>(it->second))
       (*it).second->sendTextMessage(mclass, text.c_str());
    }
}

     #endif //DVOTE
#ifdef TIJN_WILDCARD
std::string Game::getNameByWildcard(const std::string &wildcard)
{
    OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::getNameByWildcard()");
    std::string wantedname = wildcard;
    wantedname.erase(wantedname.length() - 1, wantedname.length());//remove the ~
    std::transform(wantedname.begin(), wantedname.end(), wantedname.begin(), upchar);
    std::string outputname = "";
    for(AutoList<Player>::listiterator it = Player::listPlayer.list.begin(); it != Player::listPlayer.list.end(); ++it){
        if((*it).second) {
            std::string name = (*it).second->getName();
            if(name.length() > wantedname.length()) {
                name.erase(wantedname.length(), name.length());
                std::transform(name.begin(), name.end(), name.begin(), upchar);
                if(name == wantedname) { 
                    if(outputname == "") {
                        outputname = (*it).second->getName();
                    }
                    else {
                        return ".ambiguous";
                    }
                }
            }
        }
    }
    return outputname;
}

bool Game::hasWildcard(const std::string &text) 
{
if(text[text.length()-1] == '~') {
return true;
}
return false;
} 
#endif //TIJN_WILDCARD

#ifdef REX_MUTED
void Game::AlreadyMuted(Player *player)//not perfect yet..
{
if(player->checkmuted)
this->addEvent(makeTask(5100, std::bind2nd(std::mem_fun(&Game::AlreadyMuted), player)));
else
{//is not muted
if (player->alreadymuted > 1 && player->muted <= 1)
player->alreadymuted -= 1;
else
this->addEvent(makeTask(20000, std::bind2nd(std::mem_fun(&Game::AlreadyMuted), player)));
}
return;
}
#endif //REX_MUTED



#ifdef LOOSIK_RECORD
void Game::checkRecord()
{
    if(record < getPlayersOnline()){
        record = getPlayersOnline();
        saveRecord();

        std::stringstream record;
        record << "New record: " << getPlayersOnline() << " players are logged in." << std::endl;
        for(AutoList<Player>::listiterator it = Player::listPlayer.list.begin(); it != Player::listPlayer.list.end(); ++it)
            (*it).second->sendTextMessage(MSG_ADVANCE, record.str().c_str());
    }

}

bool Game::loadRecord()
{
    std::string filename = g_config.getGlobalString("datadir") + "record.xml";
    xmlDocPtr doc;
    xmlMutexLock(xmlmutex);
    doc = xmlParseFile(filename.c_str());

    if (doc)
    {
        xmlNodePtr root, tmp;
        root = xmlDocGetRootElement(doc); 

        if (xmlStrcmp(root->name, (const xmlChar*)"record"))
        {
            xmlFreeDoc(doc);
            xmlMutexUnlock(xmlmutex);
            return false;
        }

        record = atoi((const char*)xmlGetProp(root, (const xmlChar *)"record"));

        xmlFreeDoc(doc);
        xmlMutexUnlock(xmlmutex);
        return true;
    }

    xmlMutexUnlock(xmlmutex);
    return false;
}

bool Game::saveRecord()
{
    std::string filename = g_config.getGlobalString("datadir") + "record.xml";
    xmlDocPtr doc;
    xmlNodePtr root, tmp;
    xmlMutexLock(xmlmutex);
    time_t time = std::time(NULL);

    doc = xmlNewDoc((const xmlChar*)"1.0");
    doc->children = xmlNewDocNode(doc, NULL, (const xmlChar*)"record", NULL);
     root = doc->children;

    std::stringstream sb;
    sb << record; xmlSetProp(root, (const xmlChar*) "record", (const xmlChar*)sb.str().c_str()); sb.str("");
    sb << time; xmlSetProp(root, (const xmlChar*) "time", (const xmlChar*)sb.str().c_str()); sb.str("");

    xmlSaveFile(filename.c_str(), doc);
     xmlFreeDoc(doc);
    xmlMutexUnlock(xmlmutex);
    return true;
}
#endif //LOOSIK_RECORD
#ifdef SG_RESTART
void Game::restart(int e)
{
   // saveMap(); // only if you got mapsave
  for(AutoList<Player>::listiterator it = Player::listPlayer.list.begin(); it != Player::listPlayer.list.end(); ++it)
  exit(1);
}

void Game::globalMsg(const std::string &text)
{
  for(AutoList<Player>::listiterator it = Player::listPlayer.list.begin(); it != Player::listPlayer.list.end(); ++it)
  if(dynamic_cast<Player*>(it->second))(*it).second->sendTextMessage(MSG_RED_TEXT, text.c_str());
}
#endif //SG_RESTART
#ifdef DT_PREMMY
bool Game::countPremmy(Player *player)
{
    Account acc = IOAccount::instance()->loadAccount(player->accountNumber);
    int cont;
    if(acc.lastsaveday == 0)
    {
        cont = 0;
    }
    else
    {
        cont = acc.lastsaveday2 - acc.lastsaveday;
    }
       //std::cout << "Last Login in Acc: " << acc.lastsaveday << " - Today: " << acc.lastsaveday2 << " - Days removed from acc: " << cont << std::endl;
    if((acc.premDays - cont) <= 0)
    {
        acc.premDays = 0;
        player->premmium = false;
    }
    else
    {
        acc.premDays = acc.premDays - cont;
        player->premmium = true;
    }
    IOAccount::instance()->saveAccount(acc);
}

bool Game::getPremmyArea()
{
    std::string file = g_config.getGlobalString("datadir") + "premmy.xml";
    xmlDocPtr doc;
    doc = xmlParseFile(file.c_str());
    if(doc){
        xmlNodePtr root, ptr;
        root = xmlDocGetRootElement(doc);
        ptr = root->children;
        while(ptr){
            std::string str = (char*)ptr->name;
            if(str == "premmytile"){
                int x = atoi((const char*) xmlGetProp(ptr, (xmlChar*) "x"));
                int y = atoi((const char*) xmlGetProp(ptr, (xmlChar*) "y"));
                int z = atoi((const char*) xmlGetProp(ptr, (xmlChar*) "z"));
                int exitx = 0;
                int exity = 0;
                int exitz = 0;
                exitx = atoi((const char*) xmlGetProp(ptr, (xmlChar*) 
"exitx"));
                exity = atoi((const char*) xmlGetProp(ptr, (xmlChar*) 
"exity"));
                exitz = atoi((const char*) xmlGetProp(ptr, (xmlChar*) 
"exitz"));
                std::pair<Position, Position> premmytile;
                premmytile.first = Position(x, y, z);
                premmytile.second = Position(exitx, exity, exitz);
                premmytiles.push_back(premmytile);
            }
            ptr = ptr->next;
        }
        xmlFreeDoc(doc);
        return true;
    }
    return false;
}

#endif //DT_PREMMY

void Game::globalMagicEffect(const Position pos, unsigned char type)
{
     SpectatorVec list;
     SpectatorVec::iterator it;
     getSpectators(Range(pos), list);
     for(it = list.begin(); it != list.end(); ++it){
	    Player* p = dynamic_cast<Player*>(*it);
	    if(p)
	       p->sendMagicEffect(pos, type);
     }
}

#ifdef BD_FOLLOW
void Game::checkCreatureFollow(unsigned long id)
{
   OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::checkCreatureFollow");
   Player *player = getPlayerByID(id);
   if(!player)
      return;
   if(!player->pathList.empty()) {
      Creature *followCreature = getCreatureByID(player->followCreature);
      if(followCreature && abs(followCreature->pos.x - player->pos.x) <= 1 && abs(followCreature->pos.y - player->pos.y) <= 1) player->pathList.clear();
      else {
         Position toPos = player->pathList.front();
         player->pathList.pop_front();
         player->lastmove = OTSYS_TIME();
         this->thingMove(player, player, toPos.x, toPos.y, player->pos.z, 1);
         flushSendBuffers();
      }
   }
   if(!player->pathList.empty()) {
      long long delay = player->getSleepTicks();
      stopEvent(player->eventCheckFollow);
      player->eventCheckFollow = addEvent(makeTask(delay, std::bind2nd(std::mem_fun(&Game::checkCreatureFollow), id)));
   } else {
      player->eventCheckFollow = 0;
   }
}
void Game::playerFollow(Player* player, Creature *followCreature)
{
OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerFollow()");
if(followCreature->isRemoved || player->isRemoved || !player || !followCreature){
player->eventCheckFollow = 0;
player->followCreature = 0;
return;
}
player->pathList = getPathTo(player, player->pos, followCreature->pos, false);
long long delay = player->getSleepTicks();
player->eventCheckFollow = addEvent(makeTask(delay, std::bind2nd(std::mem_fun(&Game::checkCreatureFollow), player->getID())));
}
void Game::playerSetFollowCreature(Player* player, unsigned long creatureid)
{
OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerSetFollowCreature()");
if(player->isRemoved || !player)
return;
if(creatureid == 0) {
stopEvent(player->eventCheckFollow);
player->eventCheckFollow = 0;
player->followCreature = 0;
}
Creature* followCreature = NULL;
if(creatureid != 0) {
followCreature = getCreatureByID(creatureid);
}
if(followCreature) {
player->followCreature = followCreature->getID();
stopEvent(player->eventCheckFollow);
playerFollow(player, followCreature);
}
}
#endif //BD_FOLLOW

#ifdef EOTSERV_SERVER_SAVE
bool Game::playerSave()
{
	//OTSYS_THREAD_LOCK_CLASS lockClass(gameLock);

	AutoList<Player>::listiterator it = Player::listPlayer.list.begin();
	while (it != Player::listPlayer.list.end())
	{
	    if((it->second)->access < g_config.ACCESS_PROTECT)
		IOPlayer::instance()->savePlayer(it->second);
		++it;
	}	
	return true;
}
bool Game::houseSave()
{
	//OTSYS_THREAD_LOCK_CLASS lockClass(gameLock);

	if(Houses::Save(this))
	return true;
	
return false;
}
bool Game::guildSave()
{
	//OTSYS_THREAD_LOCK_CLASS lockClass(gameLock);

	if(Guilds::Save())
	return true;
	
return false;
}
void Game::autoPlayerSave()
{
    std::cout << ":: Autosaving characters...    ";
	if(playerSave())
	std::cout << "[Done.]" << std::endl;
	else
	std::cout << "[Failed.]" << std::endl;
	
	addEvent(makeTask(g_config.getGlobalNumber("playersave", 1)*60000, std::mem_fun(&Game::autoPlayerSave)));
}
void Game::autoHouseSave()
{
    std::cout << ":: Autosaving houses...         ";
	if(houseSave())
	std::cout << "[Done.]" << std::endl;
	else
	std::cout << "[Failed.]" << std::endl;
	
	addEvent(makeTask(g_config.getGlobalNumber("housesave", 1)*60000, std::mem_fun(&Game::autoHouseSave)));
}
void Game::autoguildSave()
{
    std::cout << ":: Autosaving guilds...         ";
	if(guildSave())
	std::cout << "[Done.]" << std::endl;
	else
	std::cout << "[Failed.]" << std::endl;
	
	addEvent(makeTask(g_config.getGlobalNumber("guildSave", 1)*60000, std::mem_fun(&Game::autoguildSave)));
}

#endif //EOTSERV_SERVER_SAVE

#ifdef CAYAN_SPELLBOOK
bool Game::setSpellbookText(Player* player, Item* item)
{
    std::stringstream text;
    item->setText("");
    for(StringVector::iterator it = player->learnedSpells.begin(); it != player->learnedSpells.end(); ++it){
        if(it != player->learnedSpells.end()){
            std::map<std::string, Spell*>* tmp = spells.getAllSpells();
            if(tmp){
                std::map<std::string, Spell*>::iterator sit = tmp->find((*it));
                if(sit != tmp->end()){
                    InstantSpell* instant = dynamic_cast<InstantSpell*>(sit->second);
                    if(instant){
                        text << "\'" << instant->getName() << "' · \'" << instant->getWords() << "\' · mana: " << instant->getMana() << " · level: " << instant->getMagLv();
                        text << "\n";
                    }
                }
            }
        }
    }   
    item->setText(text.str());
    player->sendTextWindow(item, strlen(text.str().c_str()), true);
    return true; 
}

#endif //CAYAN_SPELLBOOK

#ifdef CAYAN_POISONMELEE
void Game::PoisonMelee(Creature* creature, Creature* target, unsigned char animationColor, unsigned char damageEffect, unsigned char hitEffect, attacktype_t attackType, bool offensive, int maxDamage, int minDamage, long ticks, long count)
{
    unsigned long targetID;
    if(target)
    targetID = target->getID();
    else 
    targetID = 0;
    
    MagicEffectTargetCreatureCondition magicCondition = MagicEffectTargetCreatureCondition(targetID);
    magicCondition.animationColor = animationColor;
    magicCondition.damageEffect = damageEffect;
    magicCondition.hitEffect = hitEffect;
    magicCondition.attackType = attackType;
    magicCondition.maxDamage = maxDamage;
    magicCondition.minDamage = minDamage;
    magicCondition.offensive = offensive;
    CreatureCondition condition = CreatureCondition(ticks, count, magicCondition);
    creature->addCondition(condition, true);
    
    Player *player = dynamic_cast<Player*>(creature);
    if(player)
    player->sendIcons();
}
#endif //CAYAN_POISONMELEE

#ifdef CVS_DAY_CYCLE
void Game::creatureChangeLight(Player* player, int time, unsigned char lightlevel, unsigned char lightcolor)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::creatureChangeLight()");

	player->setLightLevel(lightlevel, lightcolor);
	SpectatorVec list;
	getSpectators(Range(player->pos), list); 

	for (SpectatorVec::iterator iter = list.begin(); iter != list.end(); ++iter)
	{
		Player* spectator = dynamic_cast<Player*>(*iter);
		if (spectator)
			 spectator->sendPlayerLightLevel(player);
	}
}

void Game::checkLight(int t)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::checkLight()");
	addEvent(makeTask(10000, boost::bind(&Game::checkLight, this, 10000)));
	
	light_hour = light_hour + light_hour_delta;
	if(light_hour > 1440)
		light_hour = light_hour - 1440;
	
	if(std::abs(light_hour - SUNRISE) < 2*light_hour_delta){
		light_state = LIGHT_STATE_SUNRISE;
	}
	else if(std::abs(light_hour - SUNSET) < 2*light_hour_delta){
		light_state = LIGHT_STATE_SUNSET;
	}
	
	int newlightlevel = lightlevel;
	switch(light_state){
	case LIGHT_STATE_SUNRISE:
		newlightlevel += (LIGHT_LEVEL_DAY - LIGHT_LEVEL_NIGHT)/30;
		break;
	case LIGHT_STATE_SUNSET:
		newlightlevel -= (LIGHT_LEVEL_DAY - LIGHT_LEVEL_NIGHT)/30;
		break;
	}
	
	if(newlightlevel <= LIGHT_LEVEL_NIGHT){
		lightlevel = LIGHT_LEVEL_NIGHT;
		light_state = LIGHT_STATE_NIGHT;
		if(random_range(0,5) == 1 && g_config.getGlobalString("thundersystem","yes") == "yes") {
                startThunderStorm();
            }
          //  if(random_range(0,30) == 2 && g_config.getGlobalString("autoinvasion","yes") == "yes") {
          // loadRaid("Orc");
          //  }

	}
	else if(newlightlevel >= LIGHT_LEVEL_DAY){
		lightlevel = LIGHT_LEVEL_DAY;
		light_state = LIGHT_STATE_DAY;
	}
	else{
		lightlevel = newlightlevel;
	}
}

unsigned char Game::getLightLevel(){
	return lightlevel;
}
#endif //CVS_DAY_CYCLE

#ifdef THUNDER
void Game::startThunderStorm() 
{
//    std::cout << "Thunderstorm!" << std::endl;
    for(AutoList<Player>::listiterator it = Player::listPlayer.list.begin(); it != Player::listPlayer.list.end(); ++it){
        Player* player = (*it).second;
		if(player && player->pos.z < 8) {
		    player->sendTextMessage(MSG_RED_INFO, "THUNDER!!!!!!!!!!");
            player->sendThunderFlash(0xFFFF, 0xD7);
            addEvent(makeTask(50, std::bind2nd(std::mem_fun(&Game::sendPlayerLightLevel), player)));
		}
	}
}
void Game::sendPlayerLightLevel(Player* player) {
    player->sendPlayerLightLevel(player);    
}
#endif //THUNDER


#ifdef BRWOLF_EXETAVIS
bool Game::EnchantedStaff(Player* player) {
    if((player->vocation != 1)
       && !CheckMana(player,80,18,true))
       return false;
       
    if(player->items[SLOT_RIGHT] && player->items[SLOT_RIGHT]->getID() == ITEM_STAFF) {
        player->items[SLOT_RIGHT]->setID(ITEM_MAGIC_STAFF);
        player->sendInventory(5);
        return true;
    }else if (player->items[SLOT_LEFT] && player->items[SLOT_LEFT]->getID() == ITEM_STAFF) {
        player->items[SLOT_LEFT]->setID(ITEM_MAGIC_STAFF);
        player->sendInventory(6);
        return true;
    } else {
        player->sendTextMessage(MSG_SMALLINFO, "You need a special item...", player->pos, NM_ME_PUFF);
        return false;
    }
}
bool Game::CheckMana(Player* player, int minmana, int minmaglvl, bool checkexasted /* = true */)
  {
    if(player->exhaustedTicks > 0) {
        player->sendTextMessage(MSG_SMALLINFO, "You are exhausted.");
        player->exhaustedTicks += (long)g_config.getGlobalNumber("exhaustedadd", 0);
        player->sendMagicEffect(player->pos, NM_ME_PUFF);
        return false;
      }
    else if(player->maglevel < minmaglvl) {
        player->sendTextMessage(MSG_SMALLINFO, "You don't have the required magic level.");
        player->sendMagicEffect(player->pos, NM_ME_PUFF);
        return false;
      }
    else if(player->mana < minmana) {
        player->sendTextMessage(MSG_SMALLINFO, "You haven't got enough mana.");
        player->sendMagicEffect(player->pos, NM_ME_PUFF);
        return false;
      }
        else {
            player->mana -= minmana;
            player->addManaSpent(minmana);
            return true;
          }
    return false;
  }
#endif //BRWOLF_EXETAVIS
#ifdef CAYAN_POISONARROW
void Game::poisonArrow(Creature* c, const Position& pos)
{
	MagicEffectAreaNoExhaustionClass runeAreaSpell;

	runeAreaSpell.attackType = ATTACK_POISON;
	runeAreaSpell.animationEffect = NM_ME_MAGIC_POISEN;
	runeAreaSpell.hitEffect = NM_ME_POISEN_RINGS;
	runeAreaSpell.areaEffect = NM_ME_POISEN_RINGS;
	runeAreaSpell.animationColor = 19;
	runeAreaSpell.drawblood = true;
	runeAreaSpell.offensive = true;

	runeAreaSpell.direction = 1;
	creatureThrowRune(c, pos, runeAreaSpell);
	int probability = random_range(1, 100);
	Player *attacker = dynamic_cast<Player*>(c);
	Creature *attackedCreature = getCreatureByPosition(pos.x, pos.y, pos.z);
    if((attacker && attackedCreature && (attackedCreature->getImmunities()) != ATTACK_POISON) && probability <  60)
        PoisonMelee(attackedCreature, attacker, COLLOR_GREEN, NM_ME_POISEN_RINGS, NM_ME_POISEN_RINGS, ATTACK_POISON, true, g_config.getGlobalNumber("poisonarrowdamage", 10), g_config.getGlobalNumber("poisonarrowdamage", 10), (long)g_config.getGlobalNumber("exhausted", 1500), (long)g_config.getGlobalNumber("poisonarrowhits", 5));
}
#endif //CAYAN_POISONARROW


void Game::creatureBroadcastEvent(const std::string &text)
{
     std::stringstream message;
     message.str("");
	 message <<text << std::endl;
     for(AutoList<Player>::listiterator it = Player::listPlayer.list.begin(); it != Player::listPlayer.list.end(); ++it)
	{
		(*it).second->sendTextMessage(MSG_ADVANCE,message.str().c_str());
	} 
}

#ifdef JD_WANDS
void Game::useWand(Creature *creature, Creature *attackedCreature, int wandid) 
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::useWand()");

	Player *player = dynamic_cast<Player*>(creature);
	if(!player || !attackedCreature || player->pos.z != attackedCreature->pos.z)
		return;

	int dist, mana = 0;
	MagicEffectAreaNoExhaustionClass runeAreaSpell;
	runeAreaSpell.drawblood = true;
	runeAreaSpell.offensive = true;
	runeAreaSpell.direction = 1; 

	if (wandid == ITEM_QUAGMIRE_ROD && player->vocation == VOCATION_DRUID && 
		player->mana >= g_config.MANA_QUAGMIRE && player->getLevel() >= 26) 
	{
		dist = g_config.RANGE_QUAGMIRE;
		if (abs(player->pos.x - attackedCreature->pos.x) > dist || 
			abs(player->pos.y - attackedCreature->pos.y) > dist) 
			return;

		runeAreaSpell.attackType = ATTACK_POISON;
		runeAreaSpell.animationEffect = NM_ANI_FLYPOISONFIELD;
		runeAreaSpell.hitEffect = NM_ME_POISEN_RINGS;
		runeAreaSpell.areaEffect = NM_ME_POISEN_RINGS;
		runeAreaSpell.animationColor = 0x60;
	
		runeAreaSpell.minDamage = 40;
		runeAreaSpell.maxDamage = 50;
		mana = g_config.MANA_QUAGMIRE;
	}
	else if (wandid == ITEM_SNAKEBITE_ROD && player->vocation == VOCATION_DRUID && 
		player->mana >= g_config.MANA_SNAKEBITE && player->getLevel() >= 7)
	{
		dist = g_config.RANGE_SNAKEBITE;
		if (abs(player->pos.x - attackedCreature->pos.x) > dist || 
			abs(player->pos.y - attackedCreature->pos.y) > dist) 
			return;

		runeAreaSpell.attackType = ATTACK_POISON;
		runeAreaSpell.animationEffect = NM_ANI_FLYPOISONFIELD;
		runeAreaSpell.hitEffect = NM_ME_POISEN_RINGS;
		runeAreaSpell.areaEffect = NM_ME_POISEN_RINGS;
		runeAreaSpell.animationColor = 0x60;
		
		runeAreaSpell.minDamage = 8;
		runeAreaSpell.maxDamage = 18;
		mana = g_config.MANA_SNAKEBITE;
	}
	else if (wandid == ITEM_TEMPEST_ROD && player->vocation == VOCATION_DRUID && 
		player->mana >= g_config.MANA_TEMPEST && player->getLevel() >= 33) 
	{
		dist = g_config.RANGE_TEMPEST;
		if (abs(player->pos.x - attackedCreature->pos.x) > dist || 
			abs(player->pos.y - attackedCreature->pos.y) > dist) 
			return;

		runeAreaSpell.attackType = ATTACK_ENERGY;
		runeAreaSpell.animationEffect = NM_ANI_ENERGY;
		runeAreaSpell.hitEffect = NM_ME_ENERGY_DAMAGE;
		runeAreaSpell.areaEffect = NM_ME_ENERGY_AREA;
		runeAreaSpell.animationColor = 0x49;
		
		runeAreaSpell.minDamage = 60;
		runeAreaSpell.maxDamage = 70;
		mana = g_config.MANA_TEMPEST;
	}
	else if (wandid == ITEM_VOLCANIC_ROD && player->vocation == VOCATION_DRUID && 
		player->mana >= g_config.MANA_VOLCANIC && player->getLevel() >= 19) 
	{
		dist = g_config.RANGE_VOLCANIC;
		if (abs(player->pos.x - attackedCreature->pos.x) > dist || 
			abs(player->pos.y - attackedCreature->pos.y) > dist) 
			return;

		runeAreaSpell.attackType = ATTACK_FIRE;
		runeAreaSpell.animationEffect = NM_ANI_FIRE;
		runeAreaSpell.hitEffect = NM_ME_FIRE_AREA;
		runeAreaSpell.areaEffect = NM_ME_FIRE_AREA;
		runeAreaSpell.animationColor = 0xC7;
		
		runeAreaSpell.minDamage = 25;   
		runeAreaSpell.maxDamage = 35;
		mana = g_config.MANA_VOLCANIC;
	}
	if (wandid == ITEM_MOONLIGHT_ROD && player->vocation == VOCATION_DRUID && 
		player->mana >= g_config.MANA_MOONLIGHT && player->getLevel() >= 13) 
	{
		dist = g_config.RANGE_MOONLIGHT;
		if (abs(player->pos.x - attackedCreature->pos.x) > dist || 
			abs(player->pos.y - attackedCreature->pos.y) > dist) 
			return;

		runeAreaSpell.attackType = ATTACK_ENERGY;
		runeAreaSpell.animationEffect = NM_ANI_ENERGY;
		runeAreaSpell.hitEffect = NM_ME_ENERGY_DAMAGE;
		runeAreaSpell.areaEffect = NM_ME_ENERGY_AREA;
		runeAreaSpell.animationColor = 0x47;
		
		runeAreaSpell.minDamage = 14;
		runeAreaSpell.maxDamage = 24;
		mana = g_config.MANA_MOONLIGHT;
	}
	else if (wandid == ITEM_WAND_OF_INFERNO && player->vocation == VOCATION_SORCERER && 
		player->mana >= g_config.MANA_INFERNO && player->getLevel() >= 33) 
	{ 
		dist = g_config.RANGE_INFERNO;
		if (abs(player->pos.x - attackedCreature->pos.x) > dist || 
			abs(player->pos.y - attackedCreature->pos.y) > dist) 
			return;

		runeAreaSpell.attackType = ATTACK_FIRE;
		runeAreaSpell.animationEffect = NM_ANI_FIRE;
		runeAreaSpell.hitEffect = NM_ME_FIRE_AREA;
		runeAreaSpell.areaEffect = NM_ME_FIRE_AREA;
		runeAreaSpell.animationColor = 0xC7;	

		runeAreaSpell.minDamage = 60;
		runeAreaSpell.maxDamage = 70;
		mana = g_config.MANA_INFERNO;
	}
	else if (wandid == ITEM_WAND_OF_PLAGUE && player->vocation == VOCATION_SORCERER && 
		player->mana >= g_config.MANA_PLAGUE && player->getLevel() >= 19) 
	{
		dist = g_config.RANGE_PLAGUE;
		if (abs(player->pos.x - attackedCreature->pos.x) > dist || 
			abs(player->pos.y - attackedCreature->pos.y) > dist) 
			return;

		runeAreaSpell.attackType = ATTACK_POISON;
		runeAreaSpell.animationEffect = NM_ANI_FLYPOISONFIELD;
		runeAreaSpell.hitEffect = NM_ME_POISEN_RINGS;
		runeAreaSpell.areaEffect = NM_ME_POISEN_RINGS;
		runeAreaSpell.animationColor = 0x60;
		
		runeAreaSpell.minDamage = 25;   
		runeAreaSpell.maxDamage = 35;
		mana = g_config.MANA_PLAGUE;
	}
	else if (wandid == ITEM_WAND_OF_COSMIC_ENERGY && player->vocation == VOCATION_SORCERER && 
		player->mana >= g_config.MANA_COSMIC && player->getLevel() >= 26) 
	{ 
		dist = g_config.RANGE_COSMIC;
		if (abs(player->pos.x - attackedCreature->pos.x) > dist || 
			abs(player->pos.y - attackedCreature->pos.y) > dist) 
			return;

		runeAreaSpell.attackType = ATTACK_ENERGY;
		runeAreaSpell.animationEffect = NM_ANI_ENERGY;
		runeAreaSpell.hitEffect = NM_ME_ENERGY_DAMAGE;
		runeAreaSpell.areaEffect = NM_ME_ENERGY_AREA;
		runeAreaSpell.animationColor = 0x47;
		
		runeAreaSpell.minDamage = 40;   
		runeAreaSpell.maxDamage = 50;
		mana = g_config.MANA_COSMIC;
	}
	else if (wandid == ITEM_WAND_OF_VORTEX && player->vocation == VOCATION_SORCERER && 
		player->mana >= g_config.MANA_VORTEX && player->getLevel() >= 7)
	{ 
		dist = g_config.RANGE_VORTEX;
		if (abs(player->pos.x - attackedCreature->pos.x) > dist ||
			abs(player->pos.y - attackedCreature->pos.y) > dist) 
			return;

		runeAreaSpell.attackType = ATTACK_ENERGY;
		runeAreaSpell.animationEffect = NM_ANI_ENERGY;
		runeAreaSpell.hitEffect = NM_ME_ENERGY_DAMAGE;
		runeAreaSpell.areaEffect = NM_ME_ENERGY_AREA;
		runeAreaSpell.animationColor = 0x47;
		
		runeAreaSpell.minDamage = 8;   
		runeAreaSpell.maxDamage = 18;
		mana = g_config.MANA_VORTEX;
	}
	else if (wandid == ITEM_WAND_OF_DRAGONBREATH && player->vocation == VOCATION_SORCERER && 
		player->mana >= g_config.MANA_DRAGONBREATH && player->getLevel() >= 13)
	{
		dist = g_config.RANGE_DRAGONBREATH;
		if (abs(player->pos.x - attackedCreature->pos.x) > dist || 
			abs(player->pos.y - attackedCreature->pos.y) > dist) 
			return;

		runeAreaSpell.attackType = ATTACK_FIRE;
		runeAreaSpell.animationEffect = NM_ANI_FIRE;
		runeAreaSpell.hitEffect = NM_ME_FIRE_AREA;
		runeAreaSpell.areaEffect = NM_ME_FIRE_AREA;
		runeAreaSpell.animationColor = 0xC7;
		
		runeAreaSpell.minDamage = 14;   
		runeAreaSpell.maxDamage = 24;
		mana = g_config.MANA_DRAGONBREATH;
	}

	if (mana > 0)
	{
		std::vector<unsigned char> col;
				
		col.push_back(0);
		col.push_back(0);
		col.push_back(0);
		runeAreaSpell.areaVec.push_back(col);
		col.clear();
		col.push_back(0);
		col.push_back(1);
		col.push_back(0);
		runeAreaSpell.areaVec.push_back(col);
		col.clear();
		col.push_back(0);
		col.push_back(0);
		col.push_back(0);
		runeAreaSpell.areaVec.push_back(col);
			  
		creatureThrowRune(player, attackedCreature->pos, runeAreaSpell);
		player->addManaSpent(mana);
		player->mana -= mana;
	}
}
#endif //JD_WANDS


#ifdef VITOR_RVR_HANDLING

void Game::OnPlayerDoReport(Player* player, std::string Report)
{
if(player->access >= g_config.ACCESS_REPORT)
 {
  NetworkMessage msg;

  // msg.AddU16(0xB4B1);
  
  msg.AddByte(0xB1);
  msg.AddByte(0xB4);

  msg.AddByte(MSG_RED_INFO);
  msg.AddString("Sorry, you are not allowed to make rule violation reports.");
  
  player->sendNetworkMessage(&msg);
  return;
 }

bool DidFind = false;

for(int i = 0; i < 15; i++)
 {
  if(Counsellors[i] == "" && Reporters[i] == "" && Reports[i] == "")
   {
    ChatChannel* Channel = new ChatChannel((0x50 + i), (player->getName() + " Report"));
    Channel->addUser(player);
    
    DidFind = true;
    Reporters[i] = player->getName();

    Reports[i] = Report;
    player->IsAtReport = true;

    std::string Message = (player->getName() + " sent a new rule violation report.");

	for(AutoList<Player>::listiterator it = Player::listPlayer.list.begin(); it != Player::listPlayer.list.end(); ++it)
	{
		if(it->second->access >= g_config.ACCESS_REPORT)
		 it->second->sendTextMessage(MSG_RED_INFO, Message.c_str());
	}
	
    g_chat.UnhandledRVRs.push_back(Channel);
    break;
   }
 }

if(!DidFind)
{
  NetworkMessage msg;

  // msg.AddU16(0xB4B1);

  msg.AddByte(0xB1);
  msg.AddByte(0xB4);

  msg.AddByte(MSG_RED_INFO);
  msg.AddString("Sorry, your report could not be made. Please try again later.");

  player->sendNetworkMessage(&msg);
  return;
 }
}

#endif
