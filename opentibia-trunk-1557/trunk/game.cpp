//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
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
#include <map>

#ifdef __DEBUG_CRITICALSECTION__
#include <iostream>
#include <fstream>
#endif

#include <boost/config.hpp>
#include <boost/bind.hpp>

#include "otsystem.h"
#include "tasks.h"
#include "items.h"
#include "commands.h"
#include "creature.h"
#include "player.h"
#include "monster.h"
#include "npc.h"
#include "game.h"
#include "tile.h"
#include "house.h"

#include "spells.h"
#include "actions.h"
#include "ioplayer.h"
#include "chat.h"
#include "luascript.h"

#if defined __EXCEPTION_TRACER__
#include "exception.h"
extern OTSYS_THREAD_LOCKVAR maploadlock;
#endif

extern LuaScript g_config;
extern Spells spells;
extern Actions actions;
extern Commands commands;
extern Chat g_chat;

GameState::GameState(Game *game, const Range &range)
{
	this->game = game;
	game->getSpectators(range, spectatorlist);
}

GameState::~GameState()
{
	//
}

void GameState::onAttack(Creature* attacker, const Position& pos, const MagicEffectClass* me)
{
	Tile* tile = game->map->getTile(pos);

	if(!tile)
		return;

	CreatureVector::iterator cit;
	Player* attackPlayer = dynamic_cast<Player*>(attacker);
	Creature *targetCreature = NULL;
	Player *targetPlayer = NULL;
	for(cit = tile->creatures.begin(); cit != tile->creatures.end(); ++cit) {
		targetCreature = (*cit);
		targetPlayer = dynamic_cast<Player*>(targetCreature);

		int damage = me->getDamage(targetCreature, attacker);
		int manaDamage = 0;
		
		if (damage > 0) {
			if(attackPlayer && attackPlayer->access == 0) {
				if(targetPlayer && targetPlayer != attackPlayer && game->getWorldType() != WORLD_TYPE_NO_PVP){
					attackPlayer->pzLocked = true;
					#ifdef __SKULLSYSTEM__
					if(!targetPlayer->hasAttacked(attackPlayer)){
						if(targetPlayer->getSkull() == SKULL_NONE && attackPlayer->getSkull() == SKULL_NONE){
							//add a white skull
							game->changeSkull(attackPlayer, SKULL_WHITE);
						}
						bool sendYellowSkull = false;
						if(!attackPlayer->hasAttacked(targetPlayer) && attackPlayer->getSkull() == SKULL_NONE){
							//show yellow skull
							sendYellowSkull = true;
						}
						attackPlayer->addAttacked(targetPlayer);
						if(sendYellowSkull){
							targetPlayer->sendCreatureSkull(attackPlayer);
						}
					}
					#endif
				}
			}

			if(targetCreature->access == 0 && targetPlayer && game->getWorldType() != WORLD_TYPE_NO_PVP) {
				targetPlayer->inFightTicks = (long)g_config.getGlobalNumber("pzlocked", 0);
				targetPlayer->sendIcons();
			}

			if(game->getWorldType() == WORLD_TYPE_NO_PVP && attackPlayer && targetPlayer && attackPlayer->access == 0){
				damage = 0;
			}
		}
		
		if(damage != 0) {
			game->creatureApplyDamage(targetCreature, damage, damage, manaDamage);
		}

		addCreatureState(tile, targetCreature, damage, manaDamage, me->drawblood);
	}

	//Solid ground items/Magic items (fire/poison/energy)
	MagicEffectItem* newmagicItem = me->getMagicItem(attacker, tile->isPz(),
		tile->hasProperty(BLOCKSOLID) || (tile->ground == NULL) || tile->getTeleportItem() || tile->floorChange());

	if(newmagicItem){
		//game->internalAddItem(tile, newmagicItem);
		//game->startDecay(newmagicItem);

		///*
		MagicEffectItem* magicItem = tile->getFieldItem();
		if(magicItem) {
			//Replace existing magic field
			magicItem->transform(newmagicItem);
			
			tile->__removeThing(magicItem, 0);
			magicItem->setParent(tile);
			tile->__addThing(magicItem);
		}
		else {
			magicItem = new MagicEffectItem(*newmagicItem);
			magicItem->useThing2();

			tile->__addThing(magicItem);
			game->startDecay(magicItem);
		}
	}

	//Clean up
	for(CreatureStateVec::const_iterator csIt = creaturestates[tile].begin(); csIt != creaturestates[tile].end(); ++csIt) {
		onAttackedCreature(tile, attacker, csIt->first, csIt->second.damage, csIt->second.drawBlood);
	}

	if(attackPlayer && attackPlayer->access == 0) {
		//Add exhaustion
		if(me->causeExhaustion(true))
			attackPlayer->exhaustedTicks = (long)g_config.getGlobalNumber("exhausted", 0);
		
		//Fight symbol
		if(me->offensive){
			attackPlayer->inFightTicks = (long)g_config.getGlobalNumber("pzlocked", 0);
			attackPlayer->sendIcons();
		}
	}
}

void GameState::onAttack(Creature* attacker, const Position& pos, Creature* attackedCreature)
{
	//TODO: Decent formulas and such...
	int damage = attacker->getWeaponDamage();
	int armor = attackedCreature->getArmor();
	int defense = attackedCreature->getDefense();
	
	Player* attackPlayer = dynamic_cast<Player*>(attacker);
	Player* attackedPlayer = dynamic_cast<Player*>(attackedCreature);

	if(attackedPlayer)
		attackedPlayer->addSkillShieldTry(1);
		
	int probability = rand() % 10000;
	
	if(probability * damage < defense * 10000)
		damage = 0;
	else
	{
		damage -= (armor * (10000 + rand() % 10000)) / 10000;
	}
	
	int manaDamage = 0;

	if(attackPlayer && attackedPlayer){
		damage -= (int) damage / 2;
	}

	if (attacker->access != 0)
		damage += 1337;

	if(damage < 0 || attackedCreature->access != 0)
		damage = 0;
		
	Tile* tile = game->map->getTile(pos);
	bool blood;
	if(damage != 0){
		game->creatureApplyDamage(attackedCreature, damage, damage, manaDamage);
		blood = true;
	}
	else{//no draw blood
		blood = false;
	}

	addCreatureState(tile, attackedCreature, damage, manaDamage, blood);
	onAttackedCreature(tile, attacker, attackedCreature, damage,  true);		
}

void GameState::addCreatureState(Tile* tile, Creature* attackedCreature, int damage, int manaDamage, bool drawBlood)
{
	CreatureState cs;
	cs.pos = attackedCreature->getPosition();
	cs.damage = damage;
	cs.manaDamage = manaDamage;
	cs.drawBlood = drawBlood;

	creaturestates[tile].push_back(std::make_pair(attackedCreature, cs) );
}

void GameState::onAttackedCreature(Tile* tile, Creature *attacker, Creature* attackedCreature, int damage, bool drawBlood)
{
	Player* attackedPlayer = dynamic_cast<Player*>(attackedCreature);
	
	attackedCreature->addInflictedDamage(attacker, damage);

	if(attackedPlayer){
		attackedPlayer->sendStats();
	}

	if(!attackedCreature->isRemoved()){
		Tile* attackTile = attackedCreature->getTile();

		//Remove player?
		if(attackedCreature->health <= 0){
			//Add blood?
			if(drawBlood || attackedPlayer){
				Item* splash = Item::CreateItem(ITEM_FULLSPLASH, FLUID_BLOOD);
				game->internalAddItem(attackTile, splash);
				game->startDecay(splash);
			}

			//Prepare body
			Item* corpseItem = Item::CreateItem(attackedCreature->getLookCorpse());
			if(corpseItem){
				tile->__addThing(corpseItem);
				game->startDecay(corpseItem);
				
				//Add eventual loot
				if(Container* corpseContainer = corpseItem->getContainer()){
					attackedCreature->dropLoot(corpseContainer);
				}

				if(attackedPlayer){
					std::stringstream ss;
					ss << "You recognize " << attackedPlayer->getName() << ".";
					if(attacker){
						ss << " " << (attackedPlayer->getSex() == PLAYERSEX_FEMALE ? "She" : "He") << " was killed by ";

						Player* attackerPlayer = dynamic_cast<Player*>(attacker);
						if(attackerPlayer) {
							ss << attackerPlayer->getName() << ".";
						}
						else {
							std::string creaturename = attacker->getName();
							std::transform(creaturename.begin(), creaturename.end(), creaturename.begin(), (int(*)(int))tolower);
							ss << "a " << creaturename << ".";
						}
					}

					//set body special description
					corpseItem->setSpecialDescription(ss.str());
				}
			}

			if(attackedPlayer){
				attackedPlayer->die(); //handles exp/skills/maglevel loss/reset spawn position
				
				#ifdef __SKULLSYSTEM__
				Player* attackerPlayer = dynamic_cast<Player*>(attacker);
				if(attackerPlayer && !attackedPlayer->hasAttacked(attackerPlayer) && attackedPlayer->getSkull() == SKULL_NONE){
					attackerPlayer->addUnjustifiedDead(attackedPlayer);
				}
				#endif
			}
			
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
					if(gainExpCreature /*&& !gainExpCreature->isRemoved()*/) {
						int gainedExperience = attackedCreature->getGainedExperience(gainExpCreature);
						if(gainedExperience <= 0)
							continue;

						Player *gainExpPlayer = dynamic_cast<Player*>(gainExpCreature);

						if(gainExpPlayer) {
							gainExpPlayer->addExperience(gainedExperience);
						}

						//Need to add this creature and all that can see it to spectators, unless they already added
						SpectatorVec creaturelist;
						game->getSpectators(Range(gainExpCreature->getPosition(), true), creaturelist);

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

			//remove creature
			game->removeCreature(attackedCreature, false);

			if(attackedCreature && attackedCreature->getMaster() != NULL) {
				attackedCreature->getMaster()->removeSummon(attackedCreature);
			}
		}
		//Add blood?
		else if(drawBlood && damage > 0){
			Item* splash = Item::CreateItem(ITEM_SMALLSPLASH, FLUID_BLOOD);
			game->internalAddItem(attackTile, splash);
			game->startDecay(splash);
		}
	}
}

Game::Game()
{
	eventIdCount = 1000;
	isExecutingEvents = false;
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
	
	int daycycle = 3600;
	//(1440 minutes/day)/(3600 seconds/day)*10 seconds event interval
	light_hour_delta = 1440*10/daycycle;
	/*light_hour = 0;
	lightlevel = LIGHT_LEVEL_NIGHT;
	light_state = LIGHT_STATE_NIGHT;*/
	light_hour = SUNRISE + (SUNSET - SUNRISE)/2;
	lightlevel = LIGHT_LEVEL_DAY;
	light_state = LIGHT_STATE_DAY;
	addEvent(makeTask(10000, boost::bind(&Game::checkLight, this, 10000)));
}


Game::~Game()
{
	if(map){
		delete map;
	}
}

void Game::setWorldType(world_type_t type)
{
	worldType = type;
}

game_state_t Game::getGameState()
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::getGameState()");
	return game_state;
}

int Game::loadMap(std::string filename, std::string filekind)
{
	if(!map){
		map = new Map;
	}

	max_players = atoi(g_config.getGlobalString("maxplayers").c_str());	
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

unsigned long Game::addEvent(SchedulerTask* event)
{
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

	bool isEmpty = eventList.empty();
	eventList.push(event);

	if(isEmpty || *event < *eventList.top())
		do_signal = true;

  OTSYS_THREAD_UNLOCK(eventLock, "addEvent()")

	if (do_signal)
		OTSYS_THREAD_SIGNAL_SEND(eventSignal);

	return event->getEventId();
}

bool Game::stopEvent(unsigned long eventid)
{
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

Cylinder* Game::internalGetCylinder(Player* player, const Position& pos)
{
	if(pos.x != 0xFFFF){
		return getTile(pos.x, pos.y, pos.z);
	}
	else{
		//container
		if(pos.y & 0x40){
			uint8_t from_cid = pos.y & 0x0F;
			return player->getContainer(from_cid);
		}
		//inventory
		else{
			return player;
		}
	}
}

Thing* Game::internalGetThing(Player* player, const Position& pos, int32_t index)
{
	if(pos.x != 0xFFFF){
		Tile* tile = getTile(pos.x, pos.y, pos.z);

		if(tile){
			/*look at*/
			if(index == STACKPOS_LOOK){
				return tile->getTopThing();
			}

			Thing* thing = NULL;

			/*for move operations*/
			if(index == STACKPOS_MOVE){
				//thing = tile->getTopMoveableThing(); //tile->getTopDownItem();

				Item* item = tile->getTopDownItem();
				if(item && !item->isNotMoveable())
					thing = item;
				else
					thing = tile->getTopCreature();
			}
			/*use item*/
			else if(index == STACKPOS_USE){
				//thing = tile->getTopMoveableThing(); //tile->getTopDownItem();
				thing = tile->getTopDownItem();
			}
			else{
				thing = tile->__getThing(index);
			}

			if(player){
				//do extra checks here if the thing is accessable
				if(thing && thing->getItem()){
					if(tile->hasProperty(ISVERTICAL)){
						if(player->getPosition().x + 1 == tile->getPosition().x){
							thing = NULL;
						}
					}
					else if(tile->hasProperty(ISHORIZONTAL)){
						if(player->getPosition().y + 1 == tile->getPosition().y){
							thing = NULL;
						}
					}
				}
			}

			return thing;
		}
	}
	else{
		//container
		if(pos.y & 0x40){
			uint8_t fromCid = pos.y & 0x0F;
			uint8_t slot = pos.z;
			
			Container* parentcontainer = player->getContainer(fromCid);
			if(!parentcontainer)
				return NULL;
			
			return parentcontainer->getItem(slot);
		}
		//inventory
		else{
			slots_t slot = (slots_t)static_cast<unsigned char>(pos.y);
			return player->getInventoryItem(slot);
		}
	}

	return NULL;
}

const Position& Game::internalGetPosition(Player* player, const Position& pos)
{
	static const Position dummyPos(0,0,0);

	if(pos.x == 0xFFFF){
		//container
		if(pos.y & 0x40){
			uint8_t fromCid = pos.y & 0x0F;
			uint8_t slot = pos.z;
			
			Container* container = player->getContainer(fromCid);
			if(!container)
				return dummyPos;
			
			return container->getPosition();
		}
		//inventory
		else{
			return player->getPosition();
		}
	}
	else{
		return pos;
	}
}

Tile* Game::getTile(unsigned short _x, unsigned short _y, unsigned char _z)
{
	return map->getTile(_x, _y, _z);
}

Creature* Game::getCreatureByID(unsigned long id)
{
	if(id == 0)
		return NULL;
	
	AutoList<Creature>::listiterator it = listCreature.list.find(id);
	if(it != listCreature.list.end()){
		if(!(*it).second->isRemoved())
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
		if(!(*it).second->isRemoved())
			return (*it).second;
	}

	return NULL; //just in case the player doesnt exist
}

Creature* Game::getCreatureByName(const std::string& s)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::getCreatureByName()");

	std::string txt1 = s;
	std::transform(txt1.begin(), txt1.end(), txt1.begin(), upchar);
	for(AutoList<Creature>::listiterator it = listCreature.list.begin(); it != listCreature.list.end(); ++it){
		if(!(*it).second->isRemoved()){
			std::string txt2 = (*it).second->getName();
			std::transform(txt2.begin(), txt2.end(), txt2.begin(), upchar);
			if(txt1 == txt2)
				return it->second;
		}
	}

	return NULL; //just in case the creature doesnt exist
}

Player* Game::getPlayerByName(const std::string& s)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::getPlayerByName()");

	std::string txt1 = s;
	std::transform(txt1.begin(), txt1.end(), txt1.begin(), upchar);
	for(AutoList<Player>::listiterator it = Player::listPlayer.list.begin(); it != Player::listPlayer.list.end(); ++it){
		if(!(*it).second->isRemoved()){
			std::string txt2 = (*it).second->getName();
			std::transform(txt2.begin(), txt2.end(), txt2.begin(), upchar);
			if(txt1 == txt2)
				return it->second;
		}
	}

	return NULL; //just in case the player doesnt exist
}

bool Game::placeCreature(const Position &pos, Creature* creature, bool isLogin /*= true*/, bool forceLogin /*= false*/)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::placeCreature()");

	bool isSuccess = false;
	Player* player = creature->getPlayer();

	if(!player || player->access != 0 || getPlayersOnline() < max_players){
		isSuccess = map->placeCreature(pos, creature, forceLogin);		
		if(isSuccess){
			//std::cout << "placeCreature: " << creature << " " << creature->getID() << std::endl;

			creature->useThing2();
			creature->setID();
			listCreature.addList(creature);
			creature->addList();

			SpectatorVec list;
			SpectatorVec::iterator it;

			getSpectators(Range(creature->getPosition(), true), list);

			//send to client
			Player* tmpPlayer = NULL;
			for(it = list.begin(); it != list.end(); ++it) {
				if(tmpPlayer = (*it)->getPlayer()){
					tmpPlayer->sendCreatureAppear(creature, isLogin);
				}
			}
			
			this->isExecutingEvents = true;

			//event method
			for(it = list.begin(); it != list.end(); ++it) {
				(*it)->onCreatureAppear(creature, isLogin);
			}

			creature->getParent()->postAddNotification(creature);

			this->isExecutingEvents = false;

			if(player){
				#ifdef __DEBUG_PLAYERS__
				std::cout << (uint32_t)getPlayersOnline() << " players online." << std::endl;
				#endif

				creature->eventCheck = addEvent(makeTask(1000, std::bind2nd(std::mem_fun(&Game::checkCreature), creature->getID())));
				creature->eventCheckAttacking = addEvent(makeTask(1500, boost::bind(&Game::checkCreatureAttacking, this, creature->getID(), 1500)));
			}
			else{
				creature->eventCheck = addEvent(makeTask(500, std::bind2nd(std::mem_fun(&Game::checkCreature), creature->getID())));
				creature->eventCheckAttacking = addEvent(makeTask(500, boost::bind(&Game::checkCreatureAttacking, this, creature->getID(), 500)));
			}
		}
	}

	return isSuccess;
}

bool Game::removeCreature(Creature* creature, bool isLogout /*= true*/)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::removeCreature()");
	if(creature->isRemoved())
		return false;

#ifdef __DEBUG__
	std::cout << "removing creature "<< std::endl;
#endif

	//std::cout << "remove: " << creature << " " << creature->getID() << std::endl;

	stopEvent(creature->eventCheck);
	stopEvent(creature->eventCheckAttacking);

	SpectatorVec list;
	SpectatorVec::iterator it;

	Cylinder* cylinder = creature->getTile();
	getSpectators(Range(cylinder->getPosition(), true), list);

	uint32_t index = cylinder->__getIndexOfThing(creature);
	cylinder->__removeThing(creature, 0);

	//send to client
	Player* player = NULL;
	for(it = list.begin(); it != list.end(); ++it){
		if(player = (*it)->getPlayer()){
			player->sendCreatureDisappear(creature, index, isLogout);
		}
	}

	this->isExecutingEvents = true;
	
	//event method
	for(it = list.begin(); it != list.end(); ++it){
		(*it)->onCreatureDisappear(creature, index, isLogout);
	}

	creature->getParent()->postRemoveNotification(creature, true);

	listCreature.removeList(creature->getID());
	creature->removeList();
	creature->setRemoved(); //creature->setParent(NULL);
	FreeThing(creature);

	this->isExecutingEvents = false;

	for(std::list<Creature*>::iterator cit = creature->summons.begin(); cit != creature->summons.end(); ++cit){
		removeCreature(*cit);
	}

	return true;
}

void Game::thingMove(Player* player, const Position& fromPos, uint16_t itemId, uint8_t fromStackpos,
	const Position& toPos, uint8_t count)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::thingMove()");
	if(player->isRemoved())
		return;

	Cylinder* fromCylinder = internalGetCylinder(player, fromPos);
	uint8_t fromIndex = 0;
	
	if(fromPos.x == 0xFFFF){
		if(fromPos.y & 0x40){
			fromIndex = static_cast<uint8_t>(fromPos.z);
		}
		else{
			fromIndex = static_cast<uint8_t>(fromPos.y);
		}
	}
	else
		fromIndex = fromStackpos;

	Thing* thing = internalGetThing(player, fromPos, STACKPOS_MOVE /*fromIndex*/);

	Cylinder* toCylinder = internalGetCylinder(player, toPos);
	uint8_t toIndex = 0;

	if(toPos.x == 0xFFFF){
		if(toPos.y & 0x40){
			toIndex = static_cast<uint8_t>(toPos.z);
		}
		else{
			toIndex = static_cast<uint8_t>(toPos.y);
		}
	}

	if(thing){
		if(Creature* movingCreature = thing->getCreature()){
			moveCreature(player, fromCylinder, toCylinder, movingCreature);
		}
		else if(Item* movingItem = thing->getItem()){
			moveItem(player, fromCylinder, toCylinder, toIndex, movingItem, count, itemId);
		}
	}
	else
		player->sendCancelMessage(RET_NOTPOSSIBLE);
}

void Game::moveCreature(Player* player, Cylinder* fromCylinder, Cylinder* toCylinder,
	Creature* moveCreature)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::creatureMove()");
	if(player->isRemoved())
		return;

	ReturnValue ret = RET_NOERROR;

	if(fromCylinder == NULL || toCylinder == NULL || moveCreature == NULL){
		ret = RET_NOTPOSSIBLE;
	}
	else if(toCylinder != toCylinder->getTile()){
		ret = RET_NOTPOSSIBLE;
	}
	else if(!moveCreature->isPushable() && player->access == 0){
		ret = RET_NOTMOVEABLE;
	}
	else if(player->getPosition().z != fromCylinder->getPosition().z){
		ret = RET_NOTPOSSIBLE;
	}
	else if(!Position::areInRange<1,1,0>(moveCreature->getPosition(), player->getPosition())){
		ret = RET_TOOFARAWAY;
	}
	else{
		const Position& fromPos = fromCylinder->getPosition();
		const Position& toPos = toCylinder->getPosition();
		const Position& moveCreaturePos = moveCreature->getPosition();

		//check throw distance
		if( (std::abs(moveCreaturePos.x - toPos.x) > moveCreature->getThrowRange()) ||
				(std::abs(moveCreaturePos.y - toPos.y) > moveCreature->getThrowRange()) ||
				(std::abs(moveCreaturePos.z - toPos.z) * 4 > moveCreature->getThrowRange()) ) {
			ret = RET_DESTINATIONOUTOFREACH;
		}
		else if(player != moveCreature && player->access == 0){
			if(toCylinder->getTile()->hasProperty(BLOCKPATHFIND))
				ret = RET_NOTENOUGHROOM;
			if(fromCylinder->getTile()->hasProperty(PROTECTIONZONE) &&
			!toCylinder->getTile()->hasProperty(PROTECTIONZONE))
				ret = RET_NOTPOSSIBLE;

			/*if(toCylinder->getTile()->getTeleportItem() ||
				toCylinder->getTile()->getFieldItem() ||
				toCylinder->getTile()->floorChange()){
				ret = RET_NOTENOUGHROOM;
			}
			else if(fromCylinder->getTile()->isPz() && !toCylinder->getTile()->isPz())
				ret = RET_NOTPOSSIBLE;
			*/
		}
	}

	if(ret == RET_NOERROR){
		ret = internalMoveCreature(moveCreature, fromCylinder, toCylinder);
	}
	
	if((player == moveCreature || ret == RET_NOTMOVEABLE) && ret != RET_NOERROR){
		player->sendCancelMessage(ret);
		player->sendCancelWalk();
	}
}

ReturnValue Game::internalMoveCreature(Creature* creature, Direction direction)
{
	Cylinder* fromTile = creature->getTile();
	Cylinder* toTile = NULL;

	const Position& currentPos = creature->getPosition();
	Position destPos = currentPos;

	switch(direction){
		case NORTH:
			destPos.y -= 1;
		break;

		case SOUTH:
			destPos.y += 1;
		break;
		
		case WEST:
			destPos.x -= 1;
		break;

		case EAST:
			destPos.x += 1;
		break;

		case SOUTHWEST:
			destPos.x -= 1;
			destPos.y += 1;
		break;

		case NORTHWEST:
			destPos.x -= 1;
			destPos.y -= 1;
		break;

		case NORTHEAST:
			destPos.x += 1;
			destPos.y -= 1;
		break;

		case SOUTHEAST:
			destPos.x += 1;
			destPos.y += 1;
		break;
	}
	
	if(creature->getPlayer()){
		//try go up
		if(currentPos.z != 8 && creature->getTile()->getHeight() >= 3){
			Tile* tmpTile = map->getTile(currentPos.x, currentPos.y, currentPos.z - 1);
			if(tmpTile == NULL || (tmpTile->ground == NULL && !tmpTile->hasProperty(BLOCKSOLID))){
				tmpTile = map->getTile(destPos.x, destPos.y, destPos.z - 1);
				if(tmpTile && tmpTile->ground && !tmpTile->hasProperty(BLOCKSOLID)){
					destPos.z -= 1;
				}
			}
		}
		else{
			//try go down
			Tile* tmpTile = map->getTile(destPos);
			if(creature->getPosition().z != 7 && (tmpTile == NULL || (tmpTile->ground == NULL && !tmpTile->hasProperty(BLOCKSOLID)))){
				tmpTile = map->getTile(destPos.x, destPos.y, destPos.z + 1);

				if(tmpTile && tmpTile->getHeight() >= 3){
					destPos.z += 1;
				}
			}
		}
	}

	toTile = map->getTile(destPos);

	ReturnValue ret = RET_NOTPOSSIBLE;
	if(toTile != NULL){
		ret = internalMoveCreature(creature, fromTile, toTile);
	}

	if(ret != RET_NOERROR){
		if(Player* player = creature->getPlayer()){
			player->sendCancelMessage(ret);
			player->sendCancelWalk();
		}
	}

	return ret;
}

ReturnValue Game::internalMoveCreature(Creature* creature, Cylinder* fromCylinder, Cylinder* toCylinder)
{
	//check if we can move the creature to the destination
	uint32_t flags = 0;
	ReturnValue ret = toCylinder->__queryAdd(0, creature, 1, flags);
	if(ret != RET_NOERROR){
		return ret;
	}

	fromCylinder->getTile()->moveCreature(creature, toCylinder);

	int32_t index = 0;
	Item* toItem = NULL;
	Cylinder* subCylinder = NULL;

	uint32_t n = 0;
	while((subCylinder = toCylinder->__queryDestination(index, creature, &toItem, flags)) != toCylinder){
		toCylinder->getTile()->moveCreature(creature, subCylinder);
		toCylinder = subCylinder;
		flags = 0;

		//to prevent infinate loop
		if(++n >= MAP_MAX_LAYERS)
			break;
	}

	creature->lastmove = OTSYS_TIME();
	return RET_NOERROR;
}

void Game::moveItem(Player* player, Cylinder* fromCylinder, Cylinder* toCylinder, int32_t index,
	Item* item, uint32_t count, uint16_t itemId)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::moveItem()");
	if(player->isRemoved())
		return;

	if(fromCylinder == NULL || toCylinder == NULL || item == NULL || item->getID() != itemId){
		player->sendCancelMessage(RET_NOTPOSSIBLE);
		return;
	}

	const Position& fromPos = fromCylinder->getPosition();
	const Position& toPos = toCylinder->getPosition();
	const Position& playerPos = player->getPosition();

	ReturnValue ret = RET_NOERROR;
	if(!item->isPushable() || item->getUniqueId() != 0){
		ret = RET_NOTMOVEABLE;
	}
	else if(playerPos.z > fromPos.z){
		ret = RET_FIRSTGOUPSTAIRS;
	}
	else if(playerPos.z < fromPos.z){
		ret = RET_FIRSTGODOWNSTAIRS;
	}
	else if(!Position::areInRange<1,1,0>(playerPos, fromPos)){
		ret = RET_TOOFARAWAY;
	}
	//check throw distance
	else if((std::abs(playerPos.x - toPos.x) > item->getThrowRange()) ||
			(std::abs(playerPos.y - toPos.y) > item->getThrowRange()) ||
			(std::abs(fromPos.z - toPos.z) * 4 > item->getThrowRange()) ){
		ret = RET_DESTINATIONOUTOFREACH;
	}
	else if(!map->canThrowObjectTo(fromPos, toPos)){
		ret = RET_CANNOTTHROW;
	}

	if(ret == RET_NOERROR){
		ret = internalMoveItem(fromCylinder, toCylinder, index, item, count);
	}

	if(ret != RET_NOERROR){
		player->sendCancelMessage(ret);
	}
}

ReturnValue Game::internalMoveItem(Cylinder* fromCylinder, Cylinder* toCylinder, int32_t index,
	Item* item, uint32_t count, uint32_t flags /*= 0*/)
{
	if(!toCylinder){
		return RET_NOTPOSSIBLE;
	}

	Item* toItem = NULL;
	toCylinder = toCylinder->__queryDestination(index, item, &toItem, flags);

	//destination is the same as the source?
	if(item == toItem){ 
		return RET_NOERROR; //silently ignore move
	}

	//check if we can add this item
	ReturnValue ret = toCylinder->__queryAdd(index, item, count, flags);
	if(ret == RET_NEEDEXCHANGE){
		//check if we can add it to source cylinder
		int32_t fromIndex = fromCylinder->__getIndexOfThing(item);

		ret = fromCylinder->__queryAdd(fromIndex, toItem, toItem->getItemCount(), 0);
		if(ret == RET_NOERROR){
			//check how much we can move
			uint32_t maxExchangeQueryCount = 0;
			ReturnValue retExchangeMaxCount = fromCylinder->__queryMaxCount(-1, toItem, toItem->getItemCount(), maxExchangeQueryCount, 0);

			if(retExchangeMaxCount != RET_NOERROR && maxExchangeQueryCount == 0){
				return retExchangeMaxCount;
			}

			if((toCylinder->__queryRemove(toItem, toItem->getItemCount()) == RET_NOERROR) && ret == RET_NOERROR){

				toCylinder->__removeThing(toItem, toItem->getItemCount());
				fromCylinder->__addThing(toItem);

				toCylinder->postRemoveNotification(toItem, true);
				fromCylinder->postAddNotification(toItem);

				ret = toCylinder->__queryAdd(index, item, count, flags);
				toItem = NULL;
			}
		}
	}
	
	if(ret != RET_NOERROR){
		return ret;
	}

	//check how much we can move
	uint32_t maxQueryCount = 0;
	ReturnValue retMaxCount = toCylinder->__queryMaxCount(index, item, count, maxQueryCount, flags);

	if(retMaxCount != RET_NOERROR && maxQueryCount == 0){
		return retMaxCount;
	}

	uint32_t m = 0;
	uint32_t n = 0;

	if(item->isStackable()){
		m = std::min((uint32_t)count, maxQueryCount);
	}
	else
		m = maxQueryCount;

	Item* moveItem = item;

	//check if we can remove this item
	ret = fromCylinder->__queryRemove(item, m);
	if(ret != RET_NOERROR){
		return ret;
	}

	//remove the item
	fromCylinder->__removeThing(item, m);
	bool isCompleteRemoval = item->isRemoved();

	//update item(s)
	if(item->isStackable()) {
		if(toItem && toItem->getID() == item->getID()){
			n = std::min((uint32_t)100 - toItem->getItemCount(), m);
			toCylinder->__updateThing(toItem, toItem->getItemCount() + n);
		}
		
		if(m - n > 0){
			moveItem = Item::CreateItem(item->getID(), m - n);
		}
		else{
			moveItem = NULL;
		}

		if(item->isRemoved()){
			FreeThing(item);
		}
	}
	
	//add item
	if(moveItem /*m - n > 0*/){
		toCylinder->__addThing(index, moveItem);
	}

	fromCylinder->postRemoveNotification(item, isCompleteRemoval);
	if(moveItem)
		toCylinder->postAddNotification(moveItem);
	else
		toCylinder->postAddNotification(item);

	//we could not move all, inform the player
	if(item->isStackable() && maxQueryCount < count){
		return retMaxCount;
	}

	return ret;
}

ReturnValue Game::internalAddItem(Cylinder* toCylinder, Item* item, int32_t index /*= INDEX_WHEREEVER*/,
	uint32_t flags /*= 0*/, bool test /*= false*/)
{
	if(toCylinder == NULL || item == NULL){
		return RET_NOTPOSSIBLE;
	}

	Item* toItem = NULL;
	toCylinder = toCylinder->__queryDestination(index, item, &toItem, flags);

	//check if we can add this item
	ReturnValue ret = toCylinder->__queryAdd(index, item, item->getItemCount(), flags);
	if(ret != RET_NOERROR){
		return ret;
	}

	//check how much we can move
	uint32_t maxQueryCount = 0;
	ret = toCylinder->__queryMaxCount(index, item, item->getItemCount(), maxQueryCount, flags);

	if(ret != RET_NOERROR){
		return ret;
	}

	uint32_t m = 0;
	uint32_t n = 0;

	if(item->isStackable()){
		m = std::min((uint32_t)item->getItemCount(), maxQueryCount);
	}
	else
		m = maxQueryCount;

	if(!test){
		Item* moveItem = item;

		//update item(s)
		if(item->isStackable()) {
			if(toItem && toItem->getID() == item->getID()){
				n = std::min((uint32_t)100 - toItem->getItemCount(), m);
				toCylinder->__updateThing(toItem, toItem->getItemCount() + n);
			}
			
			if(m - n > 0){
				moveItem = Item::CreateItem(item->getID(), m - n);
			}
			else{
				moveItem = NULL;
				FreeThing(item);
			}
		}

		if(moveItem){
			toCylinder->__addThing(index, moveItem);
			toCylinder->postAddNotification(moveItem);
		}
		else
			toCylinder->postAddNotification(item);
	}

	return RET_NOERROR;
}

ReturnValue Game::internalRemoveItem(Item* item, int32_t count /*= -1*/,  bool test /*= false*/)
{
	Cylinder* cylinder = item->getParent();
	if(cylinder == NULL){
		return RET_NOTPOSSIBLE;
	}

	if(count == -1){
		count = item->getItemCount();
	}

	//check if we can remove this item
	ReturnValue ret = cylinder->__queryRemove(item, count);
	if(ret != RET_NOERROR && ret != RET_NOTMOVEABLE){
		return ret;
	}

	if(!item->canRemove()){
		return RET_NOTPOSSIBLE;
	}

	if(!test){
		//remove the item
		cylinder->__removeThing(item, count);
		bool isCompleteRemoval = false;

		if(item->isRemoved()){
			isCompleteRemoval = true;
			FreeThing(item);
		}

		cylinder->postRemoveNotification(item, isCompleteRemoval);
	}
	
	return RET_NOERROR;
}

bool Game::removeItemOfType(Cylinder* cylinder, uint16_t itemId, uint32_t count)
{
	if(cylinder == NULL || (cylinder->__getItemTypeCount(itemId) < count)){
		return false;
	}
	
	std::list<Container*> listContainer;
	Container* tmpContainer = NULL;
	Thing* thing = NULL;
	Item* item = NULL;
	
	for(int i = cylinder->__getFirstIndex(); i < cylinder->__getLastIndex() && count > 0; ++i){
		if(!(thing = cylinder->__getThing(i)))
			continue;

		if(!(item = thing->getItem()))
			continue;

		if(item->getID() == itemId){
			if(item->isStackable()){
				if(item->getItemCount() > count){
					internalRemoveItem(item, count);
					count = 0;
				}
				else{
					count-= item->getItemCount();
					internalRemoveItem(item);
				}
			}
			else{
				--count;
				internalRemoveItem(item);
			}
		}
		else if(tmpContainer = item->getContainer()){
			listContainer.push_back(tmpContainer);
		}
	}
	
	while(listContainer.size() > 0 && count > 0){
		Container* container = listContainer.front();
		listContainer.pop_front();
		
		for(int i = 0; i < container->size() && count > 0; i++){
			Item* item = container->getItem(i);
			if(item->getID() == itemId){
				if(item->isStackable()){
					if(item->getItemCount() > count){
						internalRemoveItem(item, count);
						count = 0;
					}
					else{
						count-= item->getItemCount();
						internalRemoveItem(item);
					}
				}
				else{
					--count;
					internalRemoveItem(item);
				}
			}
			else if(tmpContainer = item->getContainer()){
				listContainer.push_back(tmpContainer);
			}
		}
	}

	return (count == 0);
}

uint32_t Game::getMoney(Cylinder* cylinder)
{
	if(cylinder == NULL){
		return 0;
	}

	std::list<Container*> listContainer;
	ItemList::const_iterator it;
	Container* tmpContainer = NULL;

	Thing* thing = NULL;
	Item* item = NULL;
	
	uint32_t moneyCount = 0;

	for(int i = cylinder->__getFirstIndex(); i < cylinder->__getLastIndex(); ++i){
		if(!(thing = cylinder->__getThing(i)))
			continue;

		if(!(item = thing->getItem()))
			continue;

		if(tmpContainer = item->getContainer()){
			listContainer.push_back(tmpContainer);
		}
		else{
			if(item->getWorth() != 0){
				moneyCount+= item->getWorth();
			}
		}
	}
	
	while(listContainer.size() > 0){
		Container* container = listContainer.front();
		listContainer.pop_front();

		for(it = container->getItems(); it != container->getEnd(); ++it){
			Item* item = *it;

			if(tmpContainer = item->getContainer()){
				listContainer.push_back(tmpContainer);
			}
			else if(item->getWorth() != 0){
				moneyCount+= item->getWorth();
			}
		}
	}

	return moneyCount;
}

bool Game::removeMoney(Cylinder* cylinder, uint32_t money, uint32_t flags /*= 0*/)
{
	if(cylinder == NULL){
		return false;
	}

	std::list<Container*> listContainer;
	Container* tmpContainer = NULL;

	typedef std::multimap<int, Item*, std::less<int> > MoneyMap;
	typedef MoneyMap::value_type moneymap_pair;
	MoneyMap moneyMap;
	Thing* thing = NULL;
	Item* item = NULL;
	
	uint32_t moneyCount = 0;

	for(int i = cylinder->__getFirstIndex(); i < cylinder->__getLastIndex() && money > 0; ++i){
		if(!(thing = cylinder->__getThing(i)))
			continue;

		if(!(item = thing->getItem()))
			continue;

		if(tmpContainer = item->getContainer()){
			listContainer.push_back(tmpContainer);
		}
		else{
			if(item->getWorth() != 0){
				moneyCount+= item->getWorth();
				moneyMap.insert(moneymap_pair(item->getWorth(), item));
			}
		}
	}
	
	while(listContainer.size() > 0 && money > 0){
		Container* container = listContainer.front();
		listContainer.pop_front();

		for(int i = 0; i < container->size() && money > 0; i++){
			Item* item = container->getItem(i);

			if(tmpContainer = item->getContainer()){
				listContainer.push_back(tmpContainer);
			}
			else if(item->getWorth() != 0){
				moneyCount+= item->getWorth();
				moneyMap.insert(moneymap_pair(item->getWorth(), item));
			}
		}
	}

	if(moneyCount < money){
		/*not enough money*/
		return false;
	}

	MoneyMap::iterator it2;
	for(it2 = moneyMap.begin(); it2 != moneyMap.end() && money > 0; it2++){
		Item* item = it2->second;
		internalRemoveItem(item);

		if(it2->first <= money){
			money = money - it2->first;
		}
		else{
		  /* Remove a monetary value from an item*/
			int remaind = item->getWorth() - money;
			int crys = remaind / 10000;
			remaind = remaind - crys * 10000;
			int plat = remaind / 100;
			remaind = remaind - plat * 100;
			int gold = remaind;

			if(crys != 0){
				Item* remaindItem = Item::CreateItem(ITEM_COINS_CRYSTAL, crys);

				ReturnValue ret = internalAddItem(cylinder, remaindItem, INDEX_WHEREEVER, flags);
				if(ret != RET_NOERROR){
					internalAddItem(cylinder->getTile(), remaindItem, INDEX_WHEREEVER, FLAG_NOLIMIT);
				}
			}
			
			if(plat != 0){
				Item* remaindItem = Item::CreateItem(ITEM_COINS_PLATINUM, plat);

				ReturnValue ret = internalAddItem(cylinder, remaindItem, INDEX_WHEREEVER, flags);
				if(ret != RET_NOERROR){
					internalAddItem(cylinder->getTile(), remaindItem, INDEX_WHEREEVER, FLAG_NOLIMIT);
				}
			}
			
			if(gold != 0){
				Item* remaindItem = Item::CreateItem(ITEM_COINS_GOLD, gold);

				ReturnValue ret = internalAddItem(cylinder, remaindItem, INDEX_WHEREEVER, flags);
				if(ret != RET_NOERROR){
					internalAddItem(cylinder->getTile(), remaindItem, INDEX_WHEREEVER, FLAG_NOLIMIT);
				}
			}
			
			money = 0;
		}

		it2->second = NULL;
	}
	
	moneyMap.clear();
	
	return (money == 0);
}

Item* Game::transformItem(Item* item, uint16_t newtype, int32_t count /*= -1*/)
{
	if(item->getID() == newtype && count == -1)
		return item;

	Cylinder* cylinder = item->getParent();
	if(cylinder == NULL){
		return NULL;
	}

	if(Container* container = item->getContainer()){
		//container to container
		if(Item::items[newtype].isContainer()){
			cylinder->postRemoveNotification(item, true);
			item->setID(newtype);
			cylinder->__updateThing(item, item->getItemCount());
			cylinder->postAddNotification(item);
			return item;
		}
		//container to none-container
		else{
			uint32_t index = cylinder->__getIndexOfThing(item);
			if(index == -1){
#ifdef __DEBUG__
				std::cout << "Error: transformItem, index == -1" << std::endl;
#endif
				return item;
			}

			Item* newItem = Item::CreateItem(newtype, (count == -1 ? 1 : count));
			cylinder->__replaceThing(index, newItem);

			cylinder->postAddNotification(newItem);

			item->setParent(NULL);
			cylinder->postRemoveNotification(item, true);
			FreeThing(item);

			return newItem;
		}
	}
	else{
		//none-container to container
		if(Item::items[newtype].isContainer()){
			uint32_t index = cylinder->__getIndexOfThing(item);
			if(index == -1){
#ifdef __DEBUG__
				std::cout << "Error: transformItem, index == -1" << std::endl;
#endif
				return item;
			}

			Item* newItem = Item::CreateItem(newtype);
			cylinder->__replaceThing(index, newItem);

			cylinder->postAddNotification(newItem);

			item->setParent(NULL);
			cylinder->postRemoveNotification(item, true);
			FreeThing(item);

			return newItem;
		}
		else{
			//same type, update count variable only
			if(item->getID() == newtype){
				if(item->isStackable()){
					if(count <= 0){
						internalRemoveItem(item);
					}
					else{
						internalRemoveItem(item, item->getItemCount() - count);
						return item;
					}
				}
				else if(item->isRune()){
					if(count <= 0){
						internalRemoveItem(item);
					}
					else{
						cylinder->__updateThing(item, (count == -1 ? item->getItemCharge() : count));
						return item;
					}
				}
				else{
					cylinder->__updateThing(item, count);
					return item;
				}
			}
			else{
				cylinder->postRemoveNotification(item, true);
				item->setID(newtype);

				if(item->hasSubType()){
					if(count != -1)
						item->setItemCountOrSubtype(count);
				}
				else
					item->setItemCount(1);

				cylinder->__updateThing(item, item->getItemCountOrSubtype());
				cylinder->postAddNotification(item);
				return item;
			}
		}
	}

	return NULL;
}

ReturnValue Game::internalTeleport(Thing* thing, const Position& newPos)
{
	if(newPos == thing->getPosition())
		return RET_NOERROR;
	else if(thing->isRemoved())
		return RET_NOTPOSSIBLE;

	Tile* toTile = getTile(newPos.x, newPos.y, newPos.z);
	if(toTile){
		if(Creature* creature = thing->getCreature()){
			creature->getTile()->moveCreature(creature, toTile, true);
			return RET_NOERROR;
		}
		else if(Item* item = thing->getItem()){
			return internalMoveItem(item->getParent(), toTile, INDEX_WHEREEVER, item, item->getItemCount());
		}
	}

	return RET_NOTPOSSIBLE;
}

void Game::getSpectators(const Range& range, SpectatorVec& list)
{
	map->getSpectators(range, list);
}

//battle system
bool Game::internalCreatureSaySpell(Creature *creature, const std::string &text)
{
	Player* player = dynamic_cast<Player*>(creature);
	std::string temp, var;
	std::string::size_type loc = (uint32_t)text.find("\"", 0);
	if(loc != std::string::npos && loc >= 0){
		temp = std::string(text, 0, loc-1);
		var = std::string(text, (loc+1), text.size()-loc-1);
	}
	else {
		temp = text;
		var = std::string(""); 
	}

	toLowerCaseString(temp);
	
	//temporary place of house spells
	if(player){
		HouseTile* houseTile = dynamic_cast<HouseTile*>(player->getTile());
		if(houseTile){
			House* house = houseTile->getHouse();
			if(house){
				if(temp == "aleta sio"){ //edit guest list
					if(house->canEditAccessList(GUEST_LIST, player)){
						player->sendHouseWindow(house, GUEST_LIST);
						return true;
					}
					else{
						player->sendCancelMessage(RET_NOTPOSSIBLE);
						player->sendMagicEffect(player->getPosition(), NM_ME_PUFF);
					}
				}
				else if(temp == "aleta som"){ //edit subowner list
					if(house->canEditAccessList(SUBOWNER_LIST, player)){
						player->sendHouseWindow(house, SUBOWNER_LIST);
						return true;
					}
					else{
						player->sendCancelMessage(RET_NOTPOSSIBLE);
						player->sendMagicEffect(player->getPosition(), NM_ME_PUFF);
					}
				}
				else if(temp == "aleta grav"){ //edit door list
					Position pos = player->getPosition();

					switch(player->getDirection()){
						case NORTH:
							pos.y -= 1;
							break;
						case SOUTH:
							pos.y += 1;
							break;
						case WEST:
							pos.x -= 1;
							break;
						case EAST:
							pos.x += 1;
							break;
					}

					Door* door = house->getDoorByPosition(pos);
					if(door && house->canEditAccessList(door->getDoorId(), player)){
						player->sendHouseWindow(house, door->getDoorId());
						return true;
					}
					else{
						player->sendCancelMessage(RET_NOTPOSSIBLE);
						player->sendMagicEffect(player->getPosition(), NM_ME_PUFF);
					}
				}
				else if(temp == "alana sio"){ //kick player
					house->kickPlayer(player, var);
					return true;
				}
			}
		}
	}
	// end of house spells
	
	if(creature->access != 0 || !player){
		std::map<std::string, Spell*>::iterator sit = spells.getAllSpells()->find(temp);
		if( sit != spells.getAllSpells()->end() ) {
			sit->second->getSpellScript()->castSpell(creature, creature->getPosition(), var);
			return true;
		}
	}
	else if(player){
		std::map<std::string, Spell*>* tmp = spells.getVocSpells(player->vocation);
		if(tmp){
			std::map<std::string, Spell*>::iterator sit = tmp->find(temp);
			if( sit != tmp->end() ) {
				if(player->maglevel >= sit->second->getMagLv()){
					sit->second->getSpellScript()->castSpell(creature, creature->getPosition(), var);
					return true;
				}
			}
		}
	}

	return false;
}

bool Game::creatureMakeMagic(Creature *creature, const Position& centerpos, const MagicEffectClass* me)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::creatureMakeMagic()");

	/*
	if(this->isExecutingEvents){
		this->isExecutingEvents = true;
		return false;
	}
	*/

	//#ifdef __DEBUG__
//	cout << "creatureMakeMagic: " << (creature ? creature->getName() : "No name") << ", x: " << centerpos.x << ", y: " << centerpos.y << ", z: " << centerpos.z << std::endl;
//#endif

	Position frompos;

	if(creature){
		frompos = creature->getPosition();

		if(!creatureOnPrepareMagicAttack(creature, centerpos, me)){      
			return false;
		}
	}
	else{
		frompos = centerpos;
	}

	MagicAreaVec tmpMagicAreaVec;
	me->getArea(centerpos, tmpMagicAreaVec);
	
	std::vector<Position> poslist;

	Position topLeft(0xFFFF, 0xFFFF, frompos.z), bottomRight(0, 0, frompos.z);

	//Filter out the tiles we actually can work on
	for(MagicAreaVec::iterator maIt = tmpMagicAreaVec.begin(); maIt != tmpMagicAreaVec.end(); ++maIt) {
		Tile *t = map->getTile(maIt->x, maIt->y, maIt->z);
		if(t && (!creature || (creature->access != 0 || !me->offensive || !t->hasProperty(PROTECTIONZONE)) ) ) {
			if(!t->hasProperty(BLOCKPROJECTILE) && (me->isIndirect() ||
				(map->canThrowObjectTo(centerpos, *maIt) && !t->floorChange()))) {
				
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

//#ifdef __DEBUG__	
//	printf("top left %d %d %d\n", topLeft.x, topLeft.y, topLeft.z);
//	printf("bottom right %d %d %d\n", bottomRight.x, bottomRight.y, bottomRight.z);
//#endif

	//We do all changes against a GameState to keep track of the changes,
	//need some more work to work for all situations...
	GameState gamestate(this, Range(topLeft, bottomRight));

	Tile *targettile = map->getTile(centerpos);
	bool bSuccess = false;
	bool hasTarget = false;
	bool isBlocking = true;
	if(targettile){
		hasTarget = !targettile->creatures.empty();
		isBlocking = targettile->hasProperty(BLOCKSOLID);
		//isBlocking = (targettile->isBlocking(BLOCK_SOLID, true) != RET_NOERROR);
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
		Player* spectator = (*it)->getPlayer();
		
		if(!spectator /*|| spectator->isRemoved()*/)
			continue;

		if(bSuccess){
			//if(!spectator->isRemoved())
				me->getDistanceShoot(spectator, creature, centerpos, hasTarget);

			std::vector<Position>::const_iterator tlIt;
			for(tlIt = poslist.begin(); tlIt != poslist.end(); ++tlIt) {
				Position pos = *tlIt;
				Tile *tile = map->getTile(pos);
				const CreatureStateVec& creatureStateVec = gamestate.getCreatureStateList(tile);
					
				if(creatureStateVec.empty()) { //no targets
					//if(!spectator->isRemoved())
						me->getMagicEffect(spectator, creature, NULL, pos, 0, targettile->isPz(), isBlocking);
				}
				else {
					for(CreatureStateVec::const_iterator csIt = creatureStateVec.begin(); csIt != creatureStateVec.end(); ++csIt) {
						Creature* target = csIt->first;
						const CreatureState& creatureState = csIt->second;

						//if(!spectator->isRemoved())
							me->getMagicEffect(spectator, creature, target, creatureState.pos, creatureState.damage, tile->isPz(), false);

						//could be death due to a magic damage with no owner (fire/poison/energy)
						if(creature && target->isRemoved() /*gamestate.isRemoved(target)*/){

							for(std::vector<Creature*>::const_iterator cit = creatureState.attackerlist.begin(); cit != creatureState.attackerlist.end(); ++cit) {
								Creature* gainExpCreature = *cit;
								if(gainExpCreature->isRemoved())
									continue;

								if(dynamic_cast<Player*>(gainExpCreature))
									dynamic_cast<Player*>(gainExpCreature)->sendStats();
								
								const Position& gainExpCreaturePos = gainExpCreature->getPosition();
								if(spectator->CanSee(gainExpCreaturePos.x, gainExpCreaturePos.y, gainExpCreaturePos.z)){
									std::stringstream exp;
									exp << target->getGainedExperience(gainExpCreature);
									spectator->sendAnimatedText(gainExpCreature->getPosition(), 983, exp.str());
								}
							}

						}

						if(spectator->CanSee(creatureState.pos))
						{
							if(creatureState.damage != 0) {
								std::stringstream dmg;
								dmg << std::abs(creatureState.damage);
								spectator->sendAnimatedText(creatureState.pos, me->animationColor, dmg.str());
							}

							if(creatureState.manaDamage > 0){
								spectator->sendMagicEffect(creatureState.pos, NM_ME_LOOSE_ENERGY);
								std::stringstream manaDmg;
								manaDmg << std::abs(creatureState.manaDamage);
								spectator->sendAnimatedText(creatureState.pos, 2, manaDmg.str());
							}

							if(target->health > 0 && !target->isRemoved() /*!gamestate.isRemoved(target)*/)
								spectator->sendCreatureHealth(target);

							if(spectator == target){
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

void Game::creatureApplyDamage(Creature *creature, int damage, int &outDamage, int &outManaDamage)
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
			creature->drainHealth(outDamage);
		else if(outManaDamage > 0){
			creature->drainHealth(outDamage);
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

bool Game::creatureOnPrepareAttack(Creature *creature, Position pos)
{
  if(creature){
		Player* player = dynamic_cast<Player*>(creature);

		Tile* tile = map->getTile(creature->getPosition());
		Tile* targettile = map->getTile(pos);

		if(creature->access == 0){
			if(tile && tile->isPz()){
				if(player){					
					player->sendTextMessage(MSG_STATUS_SMALL, "You may not attack a person while you are in a protection zone.");	
					playerSetAttackedCreature(player, 0);
				}

				return false;
			}
			else if(targettile && targettile->isPz()){
				if(player){
					player->sendTextMessage(MSG_STATUS_SMALL, "You may not attack a person in a protection zone.");
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

		Player* player = dynamic_cast<Player*>(creature);
		if(player) {
			if(player->access == 0) {
				if(player->exhaustedTicks >= 1000 && me->causeExhaustion(true)) {
					if(me->offensive) {
						player->sendTextMessage(MSG_STATUS_SMALL, "You are exhausted.",player->getPosition(), NM_ME_PUFF);
						player->exhaustedTicks += (long)g_config.getGlobalNumber("exhaustedadd", 0);
					}

					return false;
				}
				else if(player->mana < me->manaCost) {															
					player->sendTextMessage(MSG_STATUS_SMALL, "You do not have enough mana.",player->getPosition(), NM_ME_PUFF);					
					return false;
				}
				else
					player->mana -= me->manaCost;
					//player->manaspent += me->manaCost;
					player->addManaSpent(me->manaCost);
			}
		}

		return true;
	}

	return false;
}

void Game::creatureMakeDamage(Creature *creature, Creature *attackedCreature, fight_t damagetype)
{
	/*
	if(this->isExecutingEvents){
		this->isExecutingEvents = true;
		return;
	}
	*/

	if(!creatureOnPrepareAttack(creature, attackedCreature->getPosition()))
		return;
	
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::creatureMakeDamage()");
	
	Player* player = dynamic_cast<Player*>(creature);
	Player* attackedPlayer = dynamic_cast<Player*>(attackedCreature);

	//Tile* targettile = getTile(attackedCreature->pos.x, attackedCreature->pos.y, attackedCreature->pos.z);
	Tile* targettile = map->getTile(attackedCreature->getPosition());

	//can the attacker reach the attacked?
	bool inReach = false;

	switch(damagetype){
		case FIGHT_MELEE:
			if(Position::areInRange<1,1,0>(creature->getPosition(), attackedCreature->getPosition())){
					inReach = true;
			}
		break;
		case FIGHT_DIST:
			if(Position::areInRange<8,5,0>(creature->getPosition(), attackedCreature->getPosition())){
					if(map->canThrowObjectTo(creature->getPosition(), attackedCreature->getPosition()))
						inReach = true;
				}
		break;
		case FIGHT_MAGICDIST:
			if(Position::areInRange<8,5,0>(creature->getPosition(), attackedCreature->getPosition())){
					if(map->canThrowObjectTo(creature->getPosition(), attackedCreature->getPosition()))
						inReach = true;
				}	
		break;
		
	}	

	if(player && player->access == 0) {
		player->inFightTicks = (long)g_config.getGlobalNumber("pzlocked", 0);
		player->sendIcons();
		
		if(attackedPlayer){
			player->pzLocked = true;
			#ifdef __SKULLSYSTEM__
			if(!attackedPlayer->hasAttacked(player)){
				if(attackedPlayer->getSkull() == SKULL_NONE && player->getSkull() == SKULL_NONE){
					//add a white skull
					changeSkull(player, SKULL_WHITE);
				}
				bool sendYellowSkull = false;
				if(!player->hasAttacked(attackedPlayer) && player->getSkull() == SKULL_NONE){
					//show yellow skull
					sendYellowSkull = true;
				}
				player->addAttacked(attackedPlayer);
				if(sendYellowSkull){
					attackedPlayer->sendCreatureSkull(player);
				}
			}
			#endif
		}
	}

	if(attackedPlayer && attackedPlayer->access ==0){
		attackedPlayer->inFightTicks = (long)g_config.getGlobalNumber("pzlocked", 0);
		attackedPlayer->sendIcons();
	}
	
	if(!inReach){
		return;
	}

	const Position creaturePos = creature->getPosition();
	const Position attackPosition = attackedCreature->getPosition();

	//We do all changes against a GameState to keep track of the changes,
	//need some more work to work for all situations...
	GameState gamestate(this, Range(creature->getPosition(), attackedCreature->getPosition()));
	gamestate.onAttack(creature, attackedCreature->getPosition(), attackedCreature);

	/*bool isTargetRemoved = false;
	if(attackedCreature && attackedCreature->isRemoved()){
		attackedCreature->setParent(getTile(attackPosition.x, attackPosition.y, attackPosition.z));
		isTargetRemoved = true;
	}

	bool isAttackerRemoved = false;
	if(creature && creature->isRemoved()){
		creature->setParent(getTile(creaturePos.x, creaturePos.y, creaturePos.z));
		isAttackerRemoved = true;
	}*/

	const CreatureStateVec& creatureStateVec = gamestate.getCreatureStateList(targettile);
	const CreatureState& creatureState = creatureStateVec[0].second;

	if(player && (creatureState.damage > 0 || creatureState.manaDamage > 0)) {
		player->addSkillTry(1);
	}
	else if(player)
		player->addSkillTry(1);
	
	SpectatorVec spectatorlist = gamestate.getSpectators();
	SpectatorVec::iterator it;

	for(it = spectatorlist.begin(); it != spectatorlist.end(); ++it) {
		Player* spectator = dynamic_cast<Player*>(*it);
		if(!spectator /*|| spectator->isRemoved()*/)
			continue;

		if(damagetype != FIGHT_MELEE){
			spectator->sendDistanceShoot(creature->getPosition(), attackPosition, creature->getSubFightType());
		}
		
		if(attackedCreature->manaShieldTicks < 1000 && (creatureState.damage == 0) &&
			(spectator->CanSee(attackPosition))){
				spectator->sendMagicEffect(attackPosition, NM_ME_PUFF);
		}
		else if(attackedCreature->manaShieldTicks < 1000 && (creatureState.damage < 0) &&
			spectator->CanSee(attackPosition)) {
				spectator->sendMagicEffect(attackPosition, NM_ME_BLOCKHIT);
		}
		else {
			for(std::vector<Creature*>::const_iterator cit = creatureState.attackerlist.begin(); cit != creatureState.attackerlist.end(); ++cit) {
				Creature* gainExpCreature = *cit;
				if(gainExpCreature->isRemoved())
					continue;

				if(dynamic_cast<Player*>(gainExpCreature))
					dynamic_cast<Player*>(gainExpCreature)->sendStats();
				
				if(spectator->CanSee(gainExpCreature->getPosition())) {
					std::stringstream exp;
					exp << attackedCreature->getGainedExperience(gainExpCreature);
					spectator->sendAnimatedText(gainExpCreature->getPosition(), 983, exp.str());
				}
			}

			if(spectator->CanSee(attackPosition)){
				if(creatureState.damage > 0) {
					std::stringstream dmg;
					dmg << std::abs(creatureState.damage);
					spectator->sendAnimatedText(attackPosition, 0xB4, dmg.str());
					spectator->sendMagicEffect(attackPosition, NM_ME_DRAW_BLOOD);
				}

				if(creatureState.manaDamage >0) {
					std::stringstream manaDmg;
					manaDmg << std::abs(creatureState.manaDamage);
					spectator->sendMagicEffect(attackPosition, NM_ME_LOOSE_ENERGY);
					spectator->sendAnimatedText(attackPosition, 2, manaDmg.str());
				}

				if (attackedCreature->health > 0 && !attackedCreature->isRemoved() /*!gamestate.isRemoved(attackedCreature)*/)
					spectator->sendCreatureHealth(attackedCreature);

				if (spectator == attackedCreature) {
					CreateManaDamageUpdate(attackedCreature, creature, creatureState.manaDamage);
					CreateDamageUpdate(attackedCreature, creature, creatureState.damage);
				}
			}
		}
	}

	if(damagetype != FIGHT_MELEE && player) {
		player->removeDistItem();
	}
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

		player->sendTextMessage(MSG_EVENT_DEFAULT, dmgmesg.str().c_str());
	}

	if(player->isRemoved()){
		player->sendTextMessage(MSG_EVENT_ADVANCE, "You are dead.");	
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

		player->sendTextMessage(MSG_EVENT_DEFAULT, dmgmesg.str().c_str());
	}
}

bool Game::creatureThrowRune(Creature *creature, const Position& centerpos, const MagicEffectClass& me)
{	
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::creatureThrowRune()");
	
	Player* player = creature->getPlayer();
	if(creature->getPosition().z != centerpos.z){	
		if(player)
			player->sendCancel("You need to be on the same floor.");
		return false;
	}
	else if(!map->canThrowObjectTo(creature->getPosition(), centerpos)){
		if(player)
			player->sendCancelMessage(RET_CANNOTTHROW);
		return false;
	}
	else
		return creatureMakeMagic(creature, centerpos, &me);
}

void Game::checkCreatureAttacking(unsigned long creatureid, unsigned long time)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::checkCreatureAttacking()");

	Creature* creature = getCreatureByID(creatureid);
	if(creature){
		if(Monster* monster = creature->getMonster()){
			monster->eventCheckAttacking = 0;
			monster->onAttack();

			flushSendBuffers();
		}
		else{
			Creature* attackedCreature = creature->getAttackedCreature();

			if(attackedCreature){
				Tile* tile = creature->getTile();
				if(!attackedCreature->isAttackable() == 0 && tile->isPz() && creature->access == 0){
					if(Player* player = creature->getPlayer()){
						player->sendTextMessage(MSG_STATUS_SMALL, "You may not attack a person in a protection zone.");
						playerSetAttackedCreature(player, 0);
					}
				}
				else{
					creatureMakeDamage(creature, attackedCreature, creature->getFightType());
				}

				flushSendBuffers();
			}
			creature->eventCheckAttacking = addEvent(makeTask(time, boost::bind(&Game::checkCreatureAttacking, this, creature->getID(), time)));
		}
	}
}

//Implementation of player invoked events
bool Game::movePlayer(Player* player, Direction direction)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::movePlayer()");
	if(player->isRemoved())
		return false;

	player->checkStopAutoWalk();
	return (internalMoveCreature(player, direction) == RET_NOERROR);
}

bool Game::playerWhisper(Player* player, const std::string& text)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerWhisper()");
	if(player->isRemoved())
		return false;

	SpectatorVec list;
	SpectatorVec::iterator it;

	getSpectators(Range(player->getPosition()), list);
	
	//event method
	for(it = list.begin(); it != list.end(); ++it) {
		if(!Position::areInRange<1,1,0>(player->getPosition(), (*it)->getPosition())){
			(*it)->onCreatureSay(player, SPEAK_WHISPER, std::string("pspsps"));
		}
		else{
			(*it)->onCreatureSay(player, SPEAK_WHISPER, text);
		}
	}

	return true;
}

bool Game::playerYell(Player* player, std::string &text)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerYell()");
	if(player->isRemoved())
		return false;

	if(player->access == 0 && player->exhaustedTicks >=1000){
		player->exhaustedTicks += (long)g_config.getGlobalNumber("exhaustedadd", 0);		
		player->sendTextMessage(MSG_STATUS_SMALL, "You are exhausted.");
		return false;
	}
	else{
		player->exhaustedTicks = (long)g_config.getGlobalNumber("exhausted", 0);
		std::transform(text.begin(), text.end(), text.begin(), upchar);

		SpectatorVec list;
		SpectatorVec::iterator it;

		getSpectators(Range(player->getPosition(), 18, 18, 14, 14), list);

		for(it = list.begin(); it != list.end(); ++it) {
			(*it)->onCreatureSay(player, SPEAK_YELL, text);
		}
	}

	return true;
}

bool Game::playerSpeakTo(Player* player, SpeakClasses type, const std::string& receiver,
	const std::string& text)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerSpeakTo");
	if(player->isRemoved())
		return false;
	
	Player* toPlayer = getPlayerByName(receiver);
	if(!toPlayer) {
		player->sendTextMessage(MSG_STATUS_SMALL, "A player with this name is not online.");
		return false;
	}

	if(player->access == 0){
		type = SPEAK_PRIVATE;
	}

	toPlayer->onCreatureSay(player, type, text);	

	std::stringstream ss;
	ss << "Message sent to " << toPlayer->getName() << ".";
	player->sendTextMessage(MSG_STATUS_SMALL, ss.str().c_str());
	return true;
}

bool Game::playerTalkToChannel(Player* player, SpeakClasses type, const std::string& text, unsigned short channelId)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerTalkToChannel");
	if(player->isRemoved())
		return false;
	
	if(player->access == 0){
		type = SPEAK_CHANNEL_Y;
	}
	
	g_chat.talkToChannel(player, type, text, channelId);
	return true;
}

bool Game::playerBroadcastMessage(Player* player, const std::string& text)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerBroadcastMessage()");
	if(player->isRemoved() || player->access == 0)
		return false;

	for(AutoList<Player>::listiterator it = Player::listPlayer.list.begin(); it != Player::listPlayer.list.end(); ++it){
		(*it).second->onCreatureSay(player, SPEAK_BROADCAST, text);
	}

	return true;
}

bool Game::playerAutoWalk(Player* player, std::list<Direction>& listDir)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerAutoWalk()");
	if(player->isRemoved())
		return false;

	return player->startAutoWalk(listDir);
}

bool Game::playerStopAutoWalk(Player* player)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerStopAutoWalk()");
	if(player->isRemoved())
		return false;

	return player->stopAutoWalk();
}

bool Game::playerUseItemEx(Player* player, const Position& fromPos, uint8_t fromStackPos, uint16_t fromItemId,
	const Position& toPos, uint8_t toStackPos, uint16_t toItemId)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerUseItemEx()");
	if(player->isRemoved())
		return false;

	Item* item = dynamic_cast<Item*>(internalGetThing(player, fromPos, fromStackPos));

	if(item){
		//Runes
		std::map<unsigned short, Spell*>::iterator sit = spells.getAllRuneSpells()->find(item->getID());
		if(sit != spells.getAllRuneSpells()->end()){
			if(!Position::areInRange<1,1,0>(item->getPosition(), player->getPosition())){
				player->sendCancelMessage(RET_TOOFARAWAY);
				return false;
			}

			std::string var = std::string("");
			if(player->access != 0 || sit->second->getMagLv() <= player->maglevel)
			{
				bool success = sit->second->getSpellScript()->castSpell(player, toPos, var);
				if(success){
					int32_t newCharge = std::max(0, item->getItemCharge() - 1);
					transformItem(item, item->getID(), newCharge);
				}
			}
			else{
				player->sendCancelMessage(RET_NOTREQUIREDLEVELTOUSERUNE);
				return false;
			}
		}
		else{
			actions.UseItemEx(player, fromPos, fromStackPos, toPos, toStackPos, fromItemId);
			return true;
		}
	}
	
	return false;
}

bool Game::playerUseItem(Player* player, const Position& pos, uint8_t stackpos, uint8_t index, uint16_t itemId)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerUseItem()");
	if(player->isRemoved())
		return false;

	actions.UseItem(player, pos, stackpos, itemId, index);
	return true;
}

bool Game::playerUseBattleWindow(Player* player, const Position& fromPos, uint8_t fromStackPos,
	uint32_t creatureId, uint16_t itemId)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerUseBattleWindow");
	if(player->isRemoved())
		return false;

	Creature* creature = getCreatureByID(creatureId);
	if(!creature || creature->getPlayer())
		return false;

	if(!Position::areInRange<7,5,0>(creature->getPosition(), player->getPosition())){
		return false;
	}

	Item* item = dynamic_cast<Item*>(internalGetThing(player, fromPos, STACKPOS_USE /*fromStackPos*/));

	if(item){
		if(!Position::areInRange<1,1,0>(item->getPosition(), player->getPosition())){
			player->sendCancelMessage(RET_TOOFARAWAY);
			return false;
		}

		//Runes
		std::map<unsigned short, Spell*>::iterator sit = spells.getAllRuneSpells()->find(item->getID());
		if(sit != spells.getAllRuneSpells()->end()) {
			std::string var = std::string("");
			if(player->access != 0 || sit->second->getMagLv() <= player->maglevel)
			{
				bool success = sit->second->getSpellScript()->castSpell(player, creature->getPosition(), var);
				if(success){
					int32_t newCharge = std::max(0, item->getItemCharge() - 1);
					transformItem(item, item->getID(), newCharge);
					return true;
				}
				else{
				return false;
				}
			}
			else{
				player->sendCancelMessage(RET_NOTREQUIREDLEVELTOUSERUNE);
				return false;
			}
		}
	}

	player->sendCancelMessage(RET_CANNOTUSETHISOBJECT);
	return false;
}

bool Game::playerRotateItem(Player* player, const Position& pos, uint8_t stackpos, const uint16_t itemId)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerRotateItem()");
	if(player->isRemoved())
		return false;

	Item* item = dynamic_cast<Item*>(internalGetThing(player, pos, stackpos));
	if(item == NULL || itemId != item->getID() || !item->isRoteable()){
		player->sendCancelMessage(RET_NOTPOSSIBLE);
		return false;
	}
	
	if(!Position::areInRange<1,1,0>(item->getPosition(), player->getPosition())){
		player->sendCancelMessage(RET_TOOFARAWAY);
		return false;
	}

	uint16_t newtype = Item::items[item->getID()].rotateTo;
	if(newtype != 0){
		transformItem(item, newtype);
	}

	return true;
}

bool Game::playerWriteItem(Player* player, Item* item, const std::string& text)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerWriteItem()");	
	if(player->isRemoved())
		return false;

	if(item->isRemoved()){
		player->sendCancelMessage(RET_NOTPOSSIBLE);
		return false;
	}

	Cylinder* topParent = item->getTopParent();

	Player* owner = dynamic_cast<Player*>(topParent);
	if(owner && owner != player){
		player->sendCancelMessage(RET_NOTPOSSIBLE);
		return false;
	}

	if(!Position::areInRange<1,1,0>(item->getPosition(), player->getPosition())){
		player->sendCancelMessage(RET_NOTPOSSIBLE);
		return false;
	}

	item->setText(text);
	
	uint16_t newtype = Item::items[item->getID()].readOnlyId;
	if(newtype != 0){
		transformItem(item, newtype);
	}
	
	//TODO: set last written by
	return true;
}

bool Game::playerRequestTrade(Player* player, const Position& pos, uint8_t stackpos,
	uint32_t playerId, uint16_t itemId)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerRequestTrade()");
	if(player->isRemoved())
		return false;

	Player* tradePartner = getPlayerByID(playerId);
	if(!tradePartner || tradePartner == player) {
		player->sendTextMessage(MSG_INFO_DESCR, "Sorry, not possible.");
		return false;
	}
	
	if(!Position::areInRange<2,2,0>(tradePartner->getPosition(), player->getPosition())){
		std::stringstream ss;
		ss << tradePartner->getName() << " tells you to move closer.";
		player->sendTextMessage(MSG_INFO_DESCR, ss.str().c_str());
		return false;
	}

	Item* tradeItem = dynamic_cast<Item*>(internalGetThing(player, pos, STACKPOS_USE /*stackpos*/));
	if(!tradeItem || tradeItem->getID() != itemId || !tradeItem->isPickupable()) {
		player->sendCancelMessage(RET_NOTPOSSIBLE);
		return false;
	}
	else if(player->getPosition().z > tradeItem->getPosition().z){
		player->sendCancelMessage(RET_FIRSTGOUPSTAIRS);
		return false;
	}
	else if(player->getPosition().z < tradeItem->getPosition().z){
		player->sendCancelMessage(RET_FIRSTGODOWNSTAIRS);
		return false;
	}
	else if(!Position::areInRange<1,1,0>(tradeItem->getPosition(), player->getPosition())){
		player->sendCancelMessage(RET_TOOFARAWAY);
		return false;
	}

	std::map<Item*, unsigned long>::const_iterator it;
	const Container* container = NULL;
	for(it = tradeItems.begin(); it != tradeItems.end(); it++) {
		if(tradeItem == it->first || 
			((container = dynamic_cast<const Container*>(tradeItem)) && container->isHoldingItem(it->first)) ||
			((container = dynamic_cast<const Container*>(it->first)) && container->isHoldingItem(tradeItem)))
		{
			player->sendTextMessage(MSG_INFO_DESCR, "This item is already being traded.");
			return false;
		}
	}

	Container* tradeContainer = tradeItem->getContainer();
	if(tradeContainer && tradeContainer->getItemHoldingCount() + 1 > 100){
		player->sendTextMessage(MSG_INFO_DESCR, "You can not trade more than 100 items.");
		return false;
	}

	return internalStartTrade(player, tradePartner, tradeItem);
}

bool Game::internalStartTrade(Player* player, Player* tradePartner, Item* tradeItem)
{
	if(player->tradeState != TRADE_NONE && !(player->tradeState == TRADE_ACKNOWLEDGE && player->tradePartner == tradePartner)) {
		player->sendCancelMessage(RET_YOUAREALREADYTRADING);
		return false;
	}
	else if(tradePartner->tradeState != TRADE_NONE && tradePartner->tradePartner != player) {
		player->sendCancelMessage(RET_THISPLAYERISALREADYTRADING);
		return false;
	}
	
	player->tradePartner = tradePartner;
	player->tradeItem = tradeItem;
	player->tradeState = TRADE_INITIATED;
	tradeItem->useThing2();
	tradeItems[tradeItem] = player->getID();

	player->sendTradeItemRequest(player, tradeItem, true);

	if(tradePartner->tradeState == TRADE_NONE){
		std::stringstream trademsg;
		trademsg << player->getName() <<" wants to trade with you.";
		tradePartner->sendTextMessage(MSG_INFO_DESCR, trademsg.str().c_str());
		tradePartner->tradeState = TRADE_ACKNOWLEDGE;
		tradePartner->tradePartner = player;
	}
	else{
		Item* counterOfferItem = tradePartner->tradeItem;
		player->sendTradeItemRequest(tradePartner, counterOfferItem, false);
		tradePartner->sendTradeItemRequest(player, tradeItem, false);
	}

	return true;

}

bool Game::playerAcceptTrade(Player* player)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerAcceptTrade()");	
	if(player->isRemoved())
		return false;

	if(!(player->getTradeState() == TRADE_ACKNOWLEDGE || player->getTradeState() == TRADE_INITIATED)){
		return false;
	}

	player->setTradeState(TRADE_ACCEPT);
	Player* tradePartner = player->tradePartner;
	if(tradePartner && tradePartner->getTradeState() == TRADE_ACCEPT){

		Item* tradeItem1 = player->tradeItem;
		Item* tradeItem2 = tradePartner->tradeItem;

		player->setTradeState(TRADE_TRANSFER);
		tradePartner->setTradeState(TRADE_TRANSFER);

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
		
		bool isSuccess = false;

		ReturnValue ret1 = internalAddItem(tradePartner, tradeItem1, INDEX_WHEREEVER, 0, true);
		ReturnValue ret2 = internalAddItem(player, tradeItem2, INDEX_WHEREEVER, 0, true);
		
		if(ret1 == RET_NOERROR && ret2 == RET_NOERROR){
			ret1 = internalRemoveItem(tradeItem1, tradeItem1->getItemCount(), true);
			ret2 = internalRemoveItem(tradeItem2, tradeItem2->getItemCount(), true);
			
			if(ret1 == RET_NOERROR && ret2 == RET_NOERROR){
				Cylinder* cylinder1 = tradeItem1->getParent();
				Cylinder* cylinder2 = tradeItem2->getParent();
				
				internalMoveItem(cylinder1, tradePartner, INDEX_WHEREEVER, tradeItem1, tradeItem1->getItemCount());
				internalMoveItem(cylinder2, player, INDEX_WHEREEVER, tradeItem2, tradeItem2->getItemCount());
				
				tradeItem1->onTradeEvent(ON_TRADE_TRANSFER, tradePartner);
				tradeItem2->onTradeEvent(ON_TRADE_TRANSFER, player);

				isSuccess = true;
			}
		}

		if(!isSuccess){
			std::stringstream ss;
			if(ret1 == RET_NOTENOUGHCAPACITY){
				ss << "You do not have enough capacity to carry this object." << std::endl << tradeItem1->getWeightDescription();
			}
			else if(ret1 == RET_NOTENOUGHROOM || ret2 == RET_CONTAINERNOTENOUGHROOM){
				ss << "You do not have enough room to carry this object.";
			}
			else
				ss << "Trade could not be completed.";

			tradePartner->sendTextMessage(MSG_INFO_DESCR, ss.str().c_str());
			tradePartner->tradeItem->onTradeEvent(ON_TRADE_CANCEL, tradePartner);
			
			ss.str("");
			if(ret2 == RET_NOTENOUGHCAPACITY){
				ss << "You do not have enough capacity to carry this object." << std::endl << tradeItem2->getWeightDescription();
			}
			else if(ret2 == RET_NOTENOUGHROOM || ret2 == RET_CONTAINERNOTENOUGHROOM){
				ss << "You do not have enough room to carry this object.";
			}
			else
				ss << "Trade could not be completed.";

			player->sendTextMessage(MSG_INFO_DESCR, ss.str().c_str());
			player->tradeItem->onTradeEvent(ON_TRADE_CANCEL, player);
		}

		player->setTradeState(TRADE_NONE);
		player->tradeItem = NULL;
		player->tradePartner = NULL;
		player->sendCloseTrade();

		tradePartner->setTradeState(TRADE_NONE);
		tradePartner->tradeItem = NULL;
		tradePartner->tradePartner = NULL;
		tradePartner->sendCloseTrade();

		return isSuccess;
	}

	return false;
}

bool Game::playerLookInTrade(Player* player, bool lookAtCounterOffer, int index)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerLookInTrade()");
	if(player->isRemoved())
		return false;

	Player* tradePartner = player->tradePartner;
	if(!tradePartner)
		return false;

	Item* tradeItem = NULL;

	if(lookAtCounterOffer)
		tradeItem = tradePartner->getTradeItem();
	else
		tradeItem = player->getTradeItem();

	if(!tradeItem)
		return false;

	int32_t lookDistance = std::max(std::abs(player->getPosition().x - tradeItem->getPosition().x),
		std::abs(player->getPosition().y - tradeItem->getPosition().y));

	if(index == 0){
		std::stringstream ss;
		ss << "You see " << tradeItem->getDescription(lookDistance);
		player->sendTextMessage(MSG_INFO_DESCR, ss.str().c_str());
		return false;
	}

	Container* tradeContainer = tradeItem->getContainer();
	if(!tradeContainer || index > tradeContainer->getItemHoldingCount())
		return false;

	bool foundItem = false;
	std::list<const Container*> listContainer;
	ItemList::const_iterator it;
	Container* tmpContainer = NULL;

	listContainer.push_back(tradeContainer);

	while(!foundItem && listContainer.size() > 0){
		const Container* container = listContainer.front();
		listContainer.pop_front();

		for(it = container->getItems(); it != container->getEnd(); ++it){
			if(tmpContainer = (*it)->getContainer()){
				listContainer.push_back(tmpContainer);
			}

			--index;
			if(index == 0){
				tradeItem = *it;
				foundItem = true;
				break;
			}
		}
	}
	
	if(foundItem){
		std::stringstream ss;
		ss << "You see " << tradeItem->getDescription(lookDistance);
		player->sendTextMessage(MSG_INFO_DESCR, ss.str().c_str());
	}

	return foundItem;
}

bool Game::playerCloseTrade(Player* player)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerCloseTrade()");
	if(player->isRemoved())
		return false;

	Player* tradePartner = player->tradePartner;
	if((tradePartner && tradePartner->getTradeState() == TRADE_TRANSFER) || 
		  player->getTradeState() == TRADE_TRANSFER){
  		std::cout << "Warning: [Game::playerCloseTrade] TradeState == TRADE_TRANSFER. " << 
		  	player->getName() << " " << player->getTradeState() << " , " << 
		  	tradePartner->getName() << " " << tradePartner->getTradeState() << std::endl;
		return true;
	}

	std::vector<Item*>::iterator it;
	if(player->getTradeItem()){
		std::map<Item*, unsigned long>::iterator it = tradeItems.find(player->getTradeItem());
		if(it != tradeItems.end()) {
			FreeThing(it->first);
			tradeItems.erase(it);
		}
	
		player->tradeItem->onTradeEvent(ON_TRADE_CANCEL, player);
		player->tradeItem = NULL;
	}
	
	player->setTradeState(TRADE_NONE);
	player->tradePartner = NULL;

	player->sendTextMessage(MSG_STATUS_SMALL, "Trade cancelled.");
	player->sendCloseTrade();

	if(tradePartner){
		if(tradePartner->getTradeItem()){
			std::map<Item*, unsigned long>::iterator it = tradeItems.find(tradePartner->getTradeItem());
			if(it != tradeItems.end()) {
				FreeThing(it->first);
				tradeItems.erase(it);
			}

			tradePartner->tradeItem->onTradeEvent(ON_TRADE_CANCEL, tradePartner);
			tradePartner->tradeItem = NULL;
		}

		tradePartner->setTradeState(TRADE_NONE);		
		tradePartner->tradePartner = NULL;

		tradePartner->sendTextMessage(MSG_STATUS_SMALL, "Trade cancelled.");
		tradePartner->sendCloseTrade();
	}

	return true;
}

bool Game::playerLookAt(Player* player, const Position& pos, uint16_t itemId, uint8_t stackpos)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerLookAt()");
	if(player->isRemoved())
		return false;
	
	Thing* thing = internalGetThing(player, pos, STACKPOS_LOOK /*stackpos*/);
	if(!thing){
		player->sendCancelMessage(RET_NOTPOSSIBLE);
		return false;
	}
	
	Position thingPos = thing->getPosition();
	if(!player->CanSee(thingPos)){
		player->sendCancelMessage(RET_NOTPOSSIBLE);
		return false;
	}
	
	Position playerPos = player->getPosition();
	
	int32_t lookDistance = 0;
	if(thing == player)
		lookDistance = -1;
	else{
		lookDistance = std::max(std::abs(playerPos.x - thingPos.x), std::abs(playerPos.y - thingPos.y));
		if(playerPos.z != thingPos.z)
			lookDistance = lookDistance + 9 + 6;
	}

	std::stringstream ss;
	ss << "You see " << thing->getDescription(lookDistance);
	player->sendTextMessage(MSG_INFO_DESCR, ss.str().c_str());

	return true;
}

bool Game::playerSetAttackedCreature(Player* player, unsigned long creatureid)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerSetAttackedCreature()");
	if(player->isRemoved())
		return false;

	if(player->attackedCreature2 != 0 && creatureid == 0){
		player->setAttackedCreature(NULL);
		player->sendCancelTarget();
	}

	Creature* attackedCreature = getCreatureByID(creatureid);
	if(!attackedCreature){
		return false;
	}

	if(attackedCreature->access != 0 || (getWorldType() == WORLD_TYPE_NO_PVP && player->access == 0 && attackedCreature->getPlayer())) {
		player->setAttackedCreature(NULL);
		player->sendCancelTarget();
		player->sendTextMessage(MSG_STATUS_SMALL, "You may not attack this player.");
	}
	else{
		player->setAttackedCreature(attackedCreature);
	}
	
	return true;
}

bool Game::playerFollowCreature(Player* player, unsigned long creatureId)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerFollowCreature");
	if(player->isRemoved())
		return false;

	player->setAttackedCreature(NULL);
	Creature* followCreature = NULL;
	
	if(creatureId != 0){
		followCreature = getCreatureByID(creatureId);
	}

	return internalFollowCreature(player, followCreature);
}

bool Game::playerSetFightModes(Player* player, uint8_t fightMode, uint8_t chaseMode)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerSetFightModes");
	if(player->isRemoved())
		return false;

	//player->setFightMode(fightMode);
	player->setChaseMode(chaseMode);
	return true;
}

bool Game::playerRequestAddVip(Player* player, const std::string& vip_name)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::requestAddVip");
	if(player->isRemoved())
		return false;

	std::string real_name;
	real_name = vip_name;
	unsigned long guid;
	unsigned long access_lvl;
	
	if(!IOPlayer::instance()->getGuidByNameEx(guid, access_lvl, real_name)){
		player->sendTextMessage(MSG_STATUS_SMALL, "A player with that name does not exist.");
		return false;
	}
	if(access_lvl > player->access){
		player->sendTextMessage(MSG_STATUS_SMALL, "You can not add this player.");
		return false;
	}

	bool online = (getPlayerByName(real_name) != NULL);
	return player->addVIP(guid, real_name, online);
}

bool Game::playerTurn(Player* player, Direction dir)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerTurn()");
	if(player->isRemoved())
		return false;

	return internalCreatureTurn(player, dir);
}

bool Game::playerSay(Player* player, SpeakClasses type, const std::string& text)
{	
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerSay()");
	if(player->isRemoved())
		return false;

	return internalCreatureSay(player, type, text);
}

bool Game::playerChangeOutfit(Player* player)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerChangeOutfit()");
	if(player->isRemoved())
		return false;

	return internalCreatureChangeOutfit(player);
}

bool Game::playerSaySpell(Player* player, const std::string& text)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::playerSaySpell()");
	if(player->isRemoved())
		return false;

	return internalCreatureSaySpell(player, text);
}

//--
bool Game::getPathTo(Creature* creature, Position toPosition, std::list<Direction>& listDir)
{
	return map->getPathTo(creature, toPosition, listDir);
}

bool Game::internalCreatureTurn(Creature* creature, Direction dir)
{
	if(creature->getDirection() != dir){
		creature->setDirection(dir);

		int32_t stackpos = creature->getParent()->__getIndexOfThing(creature);

		SpectatorVec list;
		SpectatorVec::iterator it;
		map->getSpectators(Range(creature->getPosition(), true), list);

		//send to client
		Player* player = NULL;
		for(it = list.begin(); it != list.end(); ++it) {
			if(player = (*it)->getPlayer()){
				player->sendCreatureTurn(creature, stackpos);
			}
		}
		
		//event method
		for(it = list.begin(); it != list.end(); ++it) {
			(*it)->onCreatureTurn(creature, stackpos);
		}

		return true;
	}

	return false;
}

bool Game::internalCreatureSay(Creature* creature, SpeakClasses type, const std::string& text)
{	
	//First, check if this was a command
	for(int i = 0; i < commandTags.size(); i++){
		if(commandTags[i] == text.substr(0,1)){
			return commands.exeCommand(creature, text);
		}
	}

	SpectatorVec list;
	SpectatorVec::iterator it;

	getSpectators(Range(creature->getPosition()), list);

	for(it = list.begin(); it != list.end(); ++it) {
		(*it)->onCreatureSay(creature, type, text);
	}

	return true;
}

bool Game::internalCreatureChangeOutfit(Creature* creature)
{
	SpectatorVec list;
	SpectatorVec::iterator it;

	getSpectators(Range(creature->getPosition(), true), list);

	//send to client
	Player* player = NULL;
	for(it = list.begin(); it != list.end(); ++it) {
		if(player = (*it)->getPlayer()){
			player->sendCreatureChangeOutfit(creature);
		}
	}

	//event method
	for(it = list.begin(); it != list.end(); ++it) {
		(*it)->onCreatureChangeOutfit(creature);
	}

	return true;
}

bool Game::internalMonsterYell(Monster* monster, const std::string& text) 
{
	SpectatorVec list;
	SpectatorVec::iterator it;

	map->getSpectators(Range(monster->getPosition(), 18, 18, 14, 14), list);

	//players
	for(it = list.begin(); it != list.end(); ++it) {
		if((*it)->getPlayer()){
			(*it)->onCreatureSay(monster, SPEAK_MONSTER1, text);
		}
	}

	return true;
}

bool Game::internalFollowCreature(Player* player, const Creature* followCreature)
{
	if(!followCreature || !Position::areInRange<7,5,0>(player->getPosition(), followCreature->getPosition())){
		player->setFollowCreature(NULL);
		player->setAttackedCreature(NULL);

		player->sendCancelTarget();
		if(followCreature){
			player->sendCancelMessage(RET_NOTPOSSIBLE);
		}

		player->stopAutoWalk();
		return false;
	}

	std::list<Direction> listDir;
	if(!Position::areInRange<1,1,0>(player->getPosition(), followCreature->getPosition())){
		if(!map->getPathTo(player, followCreature->getPosition(), listDir)){
			player->setFollowCreature(NULL);
			player->setAttackedCreature(NULL);

			player->sendCancelTarget();
			player->sendCancelMessage(RET_THEREISNOWAY);
			return false;
		}

		listDir.pop_back(); //remove the followCreature position
	}

	player->setFollowCreature(followCreature);
	return playerAutoWalk(player, listDir);
}

void Game::checkAutoWalkPlayer(unsigned long id)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::checkAutoWalkPlayer");

	Player* player = getPlayerByID(id);
	if(player){
		bool continueWalk = true;

		if(player->listWalkDir.empty()){
			if(player->checkStopAutoWalk(true)){
				continueWalk = false;
			}
		}
		
		if(continueWalk){
			if(!player->listWalkDir.empty()){
				Position pos = player->getPosition();
				Direction dir = player->listWalkDir.front();
				player->listWalkDir.pop_front();

				if(internalMoveCreature(player, dir) == RET_NOERROR || !player->checkStopAutoWalk(true)){
					player->addEventAutoWalk();
				}

				flushSendBuffers();
			}
			else
				player->addEventAutoWalk();
		}
	}
}

void Game::checkCreature(unsigned long creatureid)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::checkCreature()");

	Creature* creature = getCreatureByID(creatureid);

	if(creature){
		int thinkTicks = 0;
		int oldThinkTicks = creature->onThink(thinkTicks);

		if(!creature->isRemoved()){

			Conditions& conditions = creature->getConditions();
			for(Conditions::iterator condIt = conditions.begin(); condIt != conditions.end(); ++condIt) {
				if(creature->isRemoved())
					break;

				if(condIt->first == ATTACK_FIRE || condIt->first == ATTACK_ENERGY || condIt->first == ATTACK_POISON) {
					ConditionVec &condVec = condIt->second;

					if(condVec.empty())
						continue;

					CreatureCondition& condition = condVec[0];

					if(condition.onTick(oldThinkTicks)){
						const MagicEffectTargetCreatureCondition* magicTargetCondition = condition.getCondition();
						Creature* c = getCreatureByID(magicTargetCondition->getOwnerID());
						creatureMakeMagic(c, creature->getPosition(), magicTargetCondition);

						if(condition.getCount() <= 0){
							condVec.erase(condVec.begin());
						}
					}
				}
			}

			if(!creature->isRemoved()){
				if(thinkTicks > 0) {
					creature->eventCheck = addEvent(makeTask(thinkTicks, std::bind2nd(std::mem_fun(&Game::checkCreature), creatureid)));
				}
				else
					creature->eventCheck = 0;
					//creature->eventCheck = addEvent(makeTask(oldThinkTicks, std::bind2nd(std::mem_fun(&Game::checkCreature), creatureid)));

				if(Player* player = creature->getPlayer()){
					Tile* tile = player->getTile();
					if(!tile->isPz()){
						if(player->food > 1000){
							player->gainManaTick();
							player->food -= thinkTicks;
							if(player->healthmax - player->health > 0){
								if(player->gainHealthTick()){
									SpectatorVec list;
									SpectatorVec::iterator it;
									getSpectators(Range(creature->getPosition()), list);
									for(it = list.begin(); it != list.end(); ++it) {
										Player* p = dynamic_cast<Player*>(*it);
										if(p)
											p->sendCreatureHealth(player);
									}
								}
							}
						}				
					}

					//send stats only if have changed
					if(player->NeedUpdateStats()){
						player->sendStats();
					}
					
					player->sendPing();
					#ifdef __SKULLSYSTEM__
					player->checkRedSkullTicks(thinkTicks);
					#endif
					
					if(player->inFightTicks >= 1000) {
						player->inFightTicks -= thinkTicks;
						
						if(player->inFightTicks < 1000){
							player->pzLocked = false;
							player->sendIcons();
							#ifdef __SKULLSYSTEM__
							if(player->getSkull() != SKULL_RED){
								player->clearAttacked();
								changeSkull(player, SKULL_NONE);
							}
							#endif
						}
					}
					
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
					
				}
				else{
					if(creature->manaShieldTicks >=1000){
						creature->manaShieldTicks -= thinkTicks;
					}
						
					if(creature->hasteTicks >=1000){
						creature->hasteTicks -= thinkTicks;
					}
				}
			}

		}

		flushSendBuffers();
	}
}

void Game::changeOutfit(unsigned long id, int looktype)
{     
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::changeOutfit()");

	Creature* creature = getCreatureByID(id);
	if(creature){
		creature->looktype = looktype;
		internalCreatureChangeOutfit(creature);
	}
}

void Game::changeOutfitAfter(unsigned long id, int looktype, long time)
{
	addEvent(makeTask(time, boost::bind(&Game::changeOutfit, this,id, looktype)));
}

void Game::changeSpeed(unsigned long id, unsigned short speed)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::changeSpeed()");

	Creature *creature = getCreatureByID(id);
	if(creature && creature->hasteTicks < 1000 && creature->speed != speed)
	{
		creature->speed = speed;
		Player* player = dynamic_cast<Player*>(creature);
		if(player){
			player->sendChangeSpeed(creature);
			player->sendIcons();
		}

		SpectatorVec list;
		SpectatorVec::iterator it;

		getSpectators(Range(creature->getPosition()), list);

		for(it = list.begin(); it != list.end(); ++it) {
			Player* p = dynamic_cast<Player*>(*it);
			if(p)
				p->sendChangeSpeed(creature);
		}
	}	
}

void Game::changeLight(const Creature* creature)
{
	SpectatorVec list;
	SpectatorVec::iterator it;

	getSpectators(Range(creature->getPosition(), true), list);

	Player* player;
	for(it = list.begin(); it != list.end(); ++it){
		if(player = (*it)->getPlayer()){
			player->sendCreatureLight(creature);
		}
	}
}

#ifdef __SKULLSYSTEM__
void Game::changeSkull(Player* player, skulls_t new_skull)
{
	SpectatorVec list;
	SpectatorVec::iterator it;

	player->setSkull(new_skull);
	getSpectators(Range(player->getPosition(), true), list);

	Player* spectator;
	for(it = list.begin(); it != list.end(); ++it){
		if(spectator = (*it)->getPlayer()){
			spectator->sendCreatureSkull(player);
		}
	}
}
#endif

void Game::AddMagicEffectAt(const Position& pos, uint8_t type)
{
	SpectatorVec list;
	SpectatorVec::iterator it;

	getSpectators(Range(pos), list);

	Player* player;
	for(it = list.begin(); it != list.end(); ++it){
		if(player = (*it)->getPlayer()){
			player->sendMagicEffect(pos, type);
		}
	}
}


void Game::checkDecay(int t)
{
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::checkDecay()");
	addEvent(makeTask(DECAY_INTERVAL, boost::bind(&Game::checkDecay, this, DECAY_INTERVAL)));

	std::list<decayBlock*>::iterator it;
	for(it = decayVector.begin(); it != decayVector.end();){
		(*it)->decayTime -= t;
		if((*it)->decayTime <= 0){
			std::list<Item*>::iterator it2;
			for(it2 = (*it)->decayItems.begin(); it2 != (*it)->decayItems.end(); it2++){
				Item* item = *it2;
				item->isDecaying = false;
				if(item->canDecay()){
					uint32_t decayTo = Item::items[item->getID()].decayTo;

					if(decayTo != 0){
						Item* newItem = transformItem(item, decayTo);
						startDecay(newItem);
					}
					else{
						ReturnValue ret = internalRemoveItem(item);

						if(ret != RET_NOERROR){
							std::cout << "DEBUG, checkDecay failed, error code: " << (int) ret << "item id: " << item->getID() << std::endl;
						}
					}
				}

				FreeThing(item);
			}

			delete *it;
			it = decayVector.erase(it);
		}
		else{
			it++;
		}
	}
		
	flushSendBuffers();
}

void Game::startDecay(Item* item)
{
	if(item->isDecaying)
		return; //dont add 2 times the same item

	//get decay time
	unsigned long dtime = item->getDecayTime();
	if(dtime == 0)
		return;

	item->isDecaying = true;

	//round time
	if(dtime < DECAY_INTERVAL)
		dtime = DECAY_INTERVAL;
	dtime = (dtime/DECAY_INTERVAL)*DECAY_INTERVAL;
	item->useThing2();

	//search if there are any block with this time
	std::list<decayBlock*>::iterator it;
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
	
	SpawnManager::getInstance().checkSpawns(t);
	this->addEvent(makeTask(t, std::bind2nd(std::mem_fun(&Game::checkSpawns), t)));
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
	bool lightChange = false;
	switch(light_state){
	case LIGHT_STATE_SUNRISE:
		newlightlevel += (LIGHT_LEVEL_DAY - LIGHT_LEVEL_NIGHT)/30;
		lightChange = true;
		break;
	case LIGHT_STATE_SUNSET:
		newlightlevel -= (LIGHT_LEVEL_DAY - LIGHT_LEVEL_NIGHT)/30;
		lightChange = true;
		break;
	}
	
	if(newlightlevel <= LIGHT_LEVEL_NIGHT){
		lightlevel = LIGHT_LEVEL_NIGHT;
		light_state = LIGHT_STATE_NIGHT;
	}
	else if(newlightlevel >= LIGHT_LEVEL_DAY){
		lightlevel = LIGHT_LEVEL_DAY;
		light_state = LIGHT_STATE_DAY;
	}
	else{
		lightlevel = newlightlevel;
	}
	
	if(lightChange){
		LightInfo lightInfo;
		lightInfo.level = lightlevel;
		lightInfo.color = 0xD7;
		for(AutoList<Player>::listiterator it = Player::listPlayer.list.begin(); it != Player::listPlayer.list.end(); ++it){
			(*it).second->sendWorldLight(lightInfo);
		}
	}
}

void Game::getWorldLightInfo(LightInfo& lightInfo)
{
	lightInfo.level = lightlevel;
	lightInfo.color = 0xD7;
}

void Game::addCommandTag(std::string tag)
{
	bool found = false;
	for(int i=0;i< commandTags.size() ;i++){
		if(commandTags[i] == tag){
			found = true;
			break;
		}
	}

	if(!found){
		commandTags.push_back(tag);
	}
}

void Game::resetCommandTag()
{
	commandTags.clear();
}

void Game::flushSendBuffers()
{	
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::flushSendBuffers()");

	for(std::vector<Player*>::iterator it = BufferedPlayers.begin(); it != BufferedPlayers.end(); ++it) {
		(*it)->flushMsg();
		(*it)->SendBuffer = false;
		(*it)->releaseThing2();

		/*
		#ifdef __DEBUG__
				std::cout << "flushSendBuffers() - releaseThing()" << std::endl;
		#endif
		*/
	}

	BufferedPlayers.clear();
	
	//free memory
	for(std::vector<Thing*>::iterator it = ToReleaseThings.begin(); it != ToReleaseThings.end(); ++it){
		(*it)->releaseThing2();
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
		p->useThing2();
		BufferedPlayers.push_back(p);
		p->SendBuffer = true;
	}
	
	return;
}

void Game::FreeThing(Thing* thing)
{	
	OTSYS_THREAD_LOCK_CLASS lockClass(gameLock, "Game::FreeThing()");
	//std::cout << "freeThing() " << thing <<std::endl;
	ToReleaseThings.push_back(thing);
	
	return;
}
