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


#include "definitions.h"

#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

#include <stdlib.h>

#include "protocol.h"
#include "player.h"
#include "ioplayer.h"
#include "luascript.h"
#include "chat.h"
#include "house.h"

extern LuaScript g_config;
extern Game g_game;
extern Chat g_chat;

AutoList<Player> Player::listPlayer;

//for old mana/health regeneration
//const int Player::gainManaVector[5][2] = {{1,5},{1,5},{1,5},{1,5},{1,5}};
//const int Player::gainHealthVector[5][2] = {{1,5},{1,5},{1,5},{1,5},{1,5}};
const int Player::gainManaVector[5][2] = {{6,1},{3,1},{3,1},{4,1},{6,1}};
const int Player::gainHealthVector[5][2] = {{6,1},{6,1},{6,1},{4,1},{3,1}};
const int Player::CapGain[5] = {10, 10, 10, 20, 25};
const int Player::ManaGain[5] = {5, 30, 30, 15, 5};
const int Player::HPGain[5] = {5, 5, 5, 10, 15};

Player::Player(const std::string& _name, Protocol *p) :
Creature()
{	
	client = p;

	if(client){
		client->setPlayer(this);
	}

	name       = _name;
	looktype   = PLAYER_MALE_1;
	vocation   = VOCATION_NONE;
	capacity   = 300.00;
	mana       = 0;
	manamax    = 0;
	manaspent  = 0;
	food       = 0;
	guildId    = 0;

	eventAutoWalk = 0;
	followCreature = NULL;

	level      = 1;
	experience = 180;
	maglevel   = 20;
	access     = 0;
	lastlogin  = 0;
	lastLoginSaved = 0;
	SendBuffer = false;
	npings = 0;
	internal_ping = 0;

	chaseMode = CHASEMODE_STANDSTILL;
	//fightMode = FIGHTMODE_NONE;

	tradePartner = NULL;
	tradeState = TRADE_NONE;
	tradeItem = NULL;

	for(int i = 0; i < 7; i++){
		skills[i][SKILL_LEVEL] = 10;
		skills[i][SKILL_TRIES] = 0;
		skills[i][SKILL_PERCENT] = 0;
	
		for(int j = 0; j < 2; j++){
			SkillAdvanceCache[i][j].level = 10;
			SkillAdvanceCache[i][j].vocation = VOCATION_NONE;
			SkillAdvanceCache[i][j].tries = 0;
		}
	}

	lastSentStats.health = 0;
	lastSentStats.healthmax = 0;
	lastSentStats.freeCapacity = 0;
	lastSentStats.experience = 0;
	lastSentStats.level = 0;
	lastSentStats.mana = 0;
	lastSentStats.manamax = 0;
	lastSentStats.manaspent = 0;
	lastSentStats.maglevel = 0;
	level_percent = 0;
	maglevel_percent = 0;

	for(int i = 0; i < 11; i++){
		items[i] = NULL;
	}

	for(int i = SKILL_FIRST; i < SKILL_LAST; ++i){
		skills[i][SKILL_LEVEL]= 10;
		skills[i][SKILL_TRIES]= 0;
		skills[i][SKILL_PERCENT] = 0;
	}

	/*
 	CapGain[0]  = 10;     //for level advances
 	CapGain[1]  = 10;     //e.g. Sorcerers will get 10 Cap with each level up
 	CapGain[2]  = 10;     
 	CapGain[3]  = 20;
	CapGain[4]  = 25;
  
 	ManaGain[0] = 5;      //for level advances
 	ManaGain[1] = 30;
 	ManaGain[2] = 30;
 	ManaGain[3] = 15;
 	ManaGain[4] = 5;
  
	HPGain[0]   = 5;      //for level advances
 	HPGain[1]   = 5;
 	HPGain[2]   = 5;
 	HPGain[3]   = 10;
 	HPGain[4]   = 15;  
 	*/

	maxDepotLimit = 1000;

 	manaTick = 0;
 	healthTick = 0;

#ifdef __SKULLSYSTEM__
	redSkullTicks = 0;
	skull = SKULL_NONE;
#endif
} 

Player::~Player()
{
	for(int i = 0; i < 11; i++){
		if(items[i]){
			items[i]->setParent(NULL);
			items[i]->releaseThing2();
			items[i] = NULL;
		}
	}

	DepotMap::iterator it;
	for(it = depots.begin();it != depots.end(); it++){
		it->second->releaseThing2();
	}

	//std::cout << "Player destructor " << this->getID() << std::endl;
	if(client){
		delete client;
	}
}

bool Player::isPushable() const
{
	return ((getSleepTicks() <= 0) && access == 0);
}

std::string Player::getDescription(int32_t lookDistance) const
{
	std::stringstream s;
	std::string str;
	
	if(lookDistance == -1){
		s << "yourself.";

		if(vocation != VOCATION_NONE)
			s << " You are " << g_config.getGlobalStringField("vocations", (int)vocation) << ".";
		else
			s << " You have no vocation.";
	}
	else {	
		s << name << " (Level " << level <<").";
	
		if(sex == PLAYERSEX_FEMALE)
			s << " She";
		else
			s << " He";	
			
		if(vocation != VOCATION_NONE)
			s << " is "<< g_config.getGlobalStringField("vocations", (int)vocation) << ".";
		else
			s << " has no vocation.";
	}
	
	if(guildId)
	{
		if(lookDistance == -1)
			s << " You are ";
		else
		{
			if(sex == PLAYERSEX_FEMALE)
				s << " She is ";
			else
				s << " He is ";
		}
		
		if(guildRank.length())
			s << guildRank;
		else
			s << "a member";
		
		s << " of the " << guildName;
		
		if(guildNick.length())
			s << " (" << guildNick << ")";
		
		s << ".";
	}
	
	str = s.str();
	return str;
}

Item* Player::getInventoryItem(slots_t slot) const
{
	if(slot > 0 && slot < 11)
		return items[slot];

	return NULL;
}

int Player::getWeaponDamage() const
{
	double damagemax = 0;
	//TODO:what happens with 2 weapons?
  for (int slot = SLOT_RIGHT; slot <= SLOT_LEFT; slot++)
	  if (items[slot]){
			if ((items[slot]->isWeapon())){
				// check which kind of skill we use...
				// and calculate the damage dealt
				Item* distitem = NULL;
				switch (items[slot]->getWeaponType()){
					case SWORD:
						//damagemax = 3*skills[SKILL_SWORD][SKILL_LEVEL] + 2*Item::items[items[slot]->getID()].attack;
						damagemax = skills[SKILL_SWORD][SKILL_LEVEL]*Item::items[items[slot]->getID()].attack/20 + Item::items[items[slot]->getID()].attack;
						break;
					case CLUB:
						//damagemax = 3*skills[SKILL_CLUB][SKILL_LEVEL] + 2*Item::items[items[slot]->getID()].attack;
						damagemax = skills[SKILL_CLUB][SKILL_LEVEL]*Item::items[items[slot]->getID()].attack/20 + Item::items[items[slot]->getID()].attack;
						break;
					case AXE:
						//damagemax = 3*skills[SKILL_AXE][SKILL_LEVEL] + 2*Item::items[items[slot]->getID()].attack;
						damagemax = skills[SKILL_AXE][SKILL_LEVEL]*Item::items[items[slot]->getID()].attack/20 + Item::items[items[slot]->getID()].attack; 
						break;
					case DIST:
						distitem = GetDistWeapon();
						if(distitem){
							//damagemax = 3*skills[SKILL_DIST][SKILL_LEVEL]+ 2*Item::items[distitem->getID()].attack;
							damagemax = skills[SKILL_DIST][SKILL_LEVEL]*Item::items[distitem->getID()].attack/20 + Item::items[distitem->getID()].attack; 
							//hit or miss
							int hitchance;
							if(distitem->getWeaponType() == AMO){//projectile
								hitchance = 90;
							}
							else{//thrown weapons
								hitchance = 50;
							}
							if(rand()%100 < hitchance){ //hit
								return 1+(int)(damagemax*rand()/(RAND_MAX+1.0));
							}
							else{	//miss
								return 0;
							}
						}
						break;
					case MAGIC:
						damagemax = (level*2+maglevel*3) * 1.25;
						break;
					case AMO:
					case NONE:
					case SHIELD:
						// nothing to do
						break;
			  }
		  }
		  if(damagemax != 0)
		  	break;
    }

	// no weapon found -> fist fighting
	if (damagemax == 0)
		damagemax = 2*skills[SKILL_FIST][SKILL_LEVEL] + 5;

	// return it
	return 1+(int)(damagemax*rand()/(RAND_MAX+1.0));
}

int Player::getArmor() const
{
	int armor=0;
	
	if(items[SLOT_HEAD])
		armor += items[SLOT_HEAD]->getArmor();
	if(items[SLOT_NECKLACE])
		armor += items[SLOT_NECKLACE]->getArmor();
	if(items[SLOT_ARMOR])
		armor += items[SLOT_ARMOR]->getArmor();
	if(items[SLOT_LEGS])
		armor += items[SLOT_LEGS]->getArmor();
	if(items[SLOT_FEET])
		armor += items[SLOT_FEET]->getArmor();
	if(items[SLOT_RING])
		armor += items[SLOT_RING]->getArmor();
	
	return armor;
}

int Player::getDefense() const
{
	int defense = 0;
	
	if(items[SLOT_LEFT]){		
		if(items[SLOT_LEFT]->getWeaponType() == SHIELD)
			defense += skills[SKILL_SHIELD][SKILL_LEVEL] + items[SLOT_LEFT]->getDefense();
		else
			defense += items[SLOT_LEFT]->getDefense();
	}
	if(items[SLOT_RIGHT]){
		if(items[SLOT_RIGHT]->getWeaponType() == SHIELD)
			defense += skills[SKILL_SHIELD][SKILL_LEVEL] + items[SLOT_RIGHT]->getDefense();
		else
			defense += items[SLOT_RIGHT]->getDefense();		
	}
	//////////
	if(defense == 0) 
		defense = (int)random_range(0,(int)skills[SKILL_SHIELD][SKILL_LEVEL]);
	else 
		defense += (int)random_range(0,(int)skills[SKILL_SHIELD][SKILL_LEVEL]);

  	return random_range(int(defense*0.25), int(1+(int)(defense*rand())/(RAND_MAX+1.0)));
  	///////////
	//return defense;
}

void Player::sendIcons()
{
	int icons = 0;
	if(inFightTicks >= 6000 || inFightTicks ==4000 || inFightTicks == 2000){
		icons |= ICON_SWORDS;
	}
	if(manaShieldTicks >= 1000){
		icons |= ICON_MANASHIELD;
	}
	if(speed > getNormalSpeed()){
		icons |= ICON_HASTE;
	}
	if(conditions.hasCondition(ATTACK_FIRE) /*burningTicks >= 1000*/){
		icons |= ICON_BURN | ICON_SWORDS;
	}
	if(conditions.hasCondition(ATTACK_ENERGY) /*energizedTicks >= 1000*/){
		icons |= ICON_ENERGY | ICON_SWORDS;
	}
	if(conditions.hasCondition(ATTACK_POISON)/*poisonedTicks >= 1000*/){
		icons |= ICON_POISON | ICON_SWORDS;
	}
	if(conditions.hasCondition(ATTACK_PARALYZE) /*speed < getNormalSpeed()*/ /*paralyzeTicks >= 1000*/) {
		icons |= ICON_PARALYZE | ICON_SWORDS;
	}

	client->sendIcons(icons);             
}

void Player::updateInventoryWeigth()
{
	inventoryWeight = 0.00;
	
	if(access == 0){
		for(int i = SLOT_FIRST; i < SLOT_LAST; ++i){
			Item* item = getInventoryItem((slots_t)i);
			if(item){
				inventoryWeight += item->getWeight();
			}
		}
	}
}

unsigned int Player::getReqSkillTries(int skill, int level, playervoc_t voc)
{
	//first find on cache
	for(int i=0;i<2;i++){
		if(SkillAdvanceCache[skill][i].level == level && SkillAdvanceCache[skill][i].vocation == voc){
#ifdef __DEBUG__
	std::cout << "Skill cache hit: " << this->name << " " << skill << " " << level << " " << voc <<std::endl;
#endif
			return SkillAdvanceCache[skill][i].tries;
		}
	}
	// follows the order of enum skills_t  
	unsigned short int SkillBases[7] = { 50, 50, 50, 50, 30, 100, 20 };
	float SkillMultipliers[7][5] = {
                                   {1.5f, 1.5f, 1.5f, 1.2f, 1.1f},     // Fist
                                   {2.0f, 2.0f, 1.8f, 1.2f, 1.1f},     // Club
                                   {2.0f, 2.0f, 1.8f, 1.2f, 1.1f},     // Sword
                                   {2.0f, 2.0f, 1.8f, 1.2f, 1.1f},     // Axe
                                   {2.0f, 2.0f, 1.8f, 1.1f, 1.4f},     // Distance
                                   {1.5f, 1.5f, 1.5f, 1.1f, 1.1f},     // Shielding
                                   {1.1f, 1.1f, 1.1f, 1.1f, 1.1f}      // Fishing
	                           };
#ifdef __DEBUG__
	std::cout << "Skill cache miss: " << this->name << " "<< skill << " " << level << " " << voc <<std::endl;
#endif
	//update cache
	//remove minor level
	int j;
	if(SkillAdvanceCache[skill][0].level > SkillAdvanceCache[skill][1].level){
		j = 1;
	}
	else{
		j = 0;
	}	
	SkillAdvanceCache[skill][j].level = level;
	SkillAdvanceCache[skill][j].vocation = voc;
	SkillAdvanceCache[skill][j].tries = (unsigned int) ( SkillBases[skill] * pow((float) SkillMultipliers[skill][voc], (float) ( level - 11) ) );	
    return SkillAdvanceCache[skill][j].tries;
}

void Player::addSkillTry(int skilltry)
{
	int skill;
	bool foundSkill;
	foundSkill = false;
	std::string skillname;
	
	for(int slot = SLOT_RIGHT; slot <= SLOT_LEFT; slot++){
		if(items[slot]){
			if(items[slot]->isWeapon()) {
				
				switch (items[slot]->getWeaponType()) {
					case SWORD: skill = 2; break;
					case CLUB: skill = 1; break;
					case AXE: skill = 3; break;
					case DIST: 
						if(GetDistWeapon())
							skill = 4;
						else
							skill = 0;
						break;
					case SHIELD: continue; break;
					case MAGIC: return; break;//TODO: should add skill try?
					default: skill = 0; break;
			 	}

			 	addSkillTryInternal(skilltry,skill);
			 	foundSkill = true;
			 	break;
			}			
		}
	}
	
	if(foundSkill == false)
		addSkillTryInternal(skilltry,0); //add fist try
}

void Player::addSkillShieldTry(int skilltry)
{
	//look for a shield
	
	for (int slot = SLOT_RIGHT; slot <= SLOT_LEFT; slot++) {
		if (items[slot]) {
			if (items[slot]->isWeapon()) {
				if (items[slot]->getWeaponType() == SHIELD) {
					addSkillTryInternal(skilltry,5);
					break;
			 	}			 	
			}			
		}
	}	
}

int Player::getPlayerInfo(playerinfo_t playerinfo) const
{
	switch(playerinfo) {
		case PLAYERINFO_LEVEL: return level; break;
		case PLAYERINFO_LEVELPERCENT: return level_percent; break;
		case PLAYERINFO_MAGICLEVEL: return maglevel; break;
		case PLAYERINFO_MAGICLEVELPERCENT: return maglevel_percent; break;
		case PLAYERINFO_HEALTH: return health; break;
		case PLAYERINFO_MAXHEALTH: return healthmax; break;
		case PLAYERINFO_MANA: return mana; break;
		case PLAYERINFO_MAXMANA: return manamax; break;
		case PLAYERINFO_MANAPERCENT: return maglevel_percent; break;
		case PLAYERINFO_SOUL: return 100; break;
		default:
			return 0; break;
	}

	return 0;
}

int Player::getSkill(skills_t skilltype, skillsid_t skillinfo) const
{
	return skills[skilltype][skillinfo];
}

std::string Player::getSkillName(int skillid)
{
	std::string skillname;
	switch(skillid){
	case 0:
		skillname = "fist fighting"; 
		break;
	case 1:
		skillname = "club fighting";
		break;
	case 2:
		skillname = "sword fighting";
		break;
	case 3:
		skillname = "axe fighting";
		break;
	case 4:
		skillname = "distance fighting";
		break;
	case 5:
		skillname = "shielding";
		break;
	case 6:
		skillname = "fishing";
		break;
	default:
		skillname = "unknown";
		break;
	}
	return skillname;
}

void Player::addSkillTryInternal(int skilltry,int skill)
{
	skills[skill][SKILL_TRIES] += skilltry;
	//for skill level advances
	//int reqTries = (int) ( SkillBases[skill] * pow((float) VocMultipliers[skill][voc], (float) ( skills[skill][SKILL_LEVEL] - 10) ) );			 
#if __DEBUG__
	//for debug
	cout << getName() << ", has the vocation: " << (int)vocation << " and is training his " << getSkillName(skill) << "(" << skill << "). Tries: " << skills[skill][SKILL_TRIES] << "(" << getReqSkillTries(skill, (skills[skill][SKILL_LEVEL] + 1), vocation) << ")" << std::endl;
	cout << "Current skill: " << skills[skill][SKILL_LEVEL] << std::endl;
#endif			 
	//Need skill up?
	if (skills[skill][SKILL_TRIES] >= getReqSkillTries(skill, (skills[skill][SKILL_LEVEL] + 1), vocation)) {
	 	skills[skill][SKILL_LEVEL]++;
	 	skills[skill][SKILL_TRIES] = 0;
		skills[skill][SKILL_PERCENT] = 0;				 
		std::stringstream advMsg;
		advMsg << "You advanced in " << getSkillName(skill) << ".";
		client->sendTextMessage(MSG_EVENT_ADVANCE, advMsg.str().c_str());
		client->sendSkills();
	}
	else{
	 //update percent
	 int new_percent = (unsigned int)(100*(skills[skill][SKILL_TRIES])/(1.*getReqSkillTries(skill, (skills[skill][SKILL_LEVEL]+1), vocation)));
				 
	 	if(skills[skill][SKILL_PERCENT] != new_percent){
			skills[skill][SKILL_PERCENT] = new_percent;
			client->sendSkills();
	 	}
	}
}


unsigned int Player::getReqMana(int maglevel, playervoc_t voc)
{
  //ATTENTION: MAKE SURE THAT CHARS HAVE REASONABLE MAGIC LEVELS. ESPECIALY KNIGHTS!!!!!!!!!!!
  float ManaMultiplier[5] = { 1.0f, 1.1f, 1.1f, 1.4f, 3};

	//will calculate required mana for a magic level
  unsigned int reqMana = (unsigned int) ( 400 * pow(ManaMultiplier[(int)voc], maglevel-1) );

	if (reqMana % 20 < 10) //CIP must have been bored when they invented this odd rounding
    reqMana = reqMana - (reqMana % 20);
  else
    reqMana = reqMana - (reqMana % 20) + 20;

  return reqMana;
}

Container* Player::getContainer(uint32_t cid)
{
  for(ContainerVector::iterator it = containerVec.begin(); it != containerVec.end(); ++it){
		if(it->first == cid)
			return it->second;
	}

	return NULL;
}

int32_t Player::getContainerID(const Container* container) const
{
  for(ContainerVector::const_iterator cl = containerVec.begin(); cl != containerVec.end(); ++cl){
	  if(cl->second == container)
			return cl->first;
	}

	return -1;
}

void Player::addContainer(uint32_t cid, Container* container)
{
#ifdef __DEBUG__
	cout << getName() << ", addContainer: " << (int)cid << std::endl;
#endif
	if(cid > 0xF)
		return;

	for(ContainerVector::iterator cl = containerVec.begin(); cl != containerVec.end(); ++cl) {
		if(cl->first == cid) {
			cl->second = container;
			return;
		}
	}
	
	//id doesnt exist, create it
	containervector_pair cv;
	cv.first = cid;
	cv.second = container;

	containerVec.push_back(cv);
}

void Player::closeContainer(uint32_t cid)
{
  for(ContainerVector::iterator cl = containerVec.begin(); cl != containerVec.end(); ++cl){
	  if(cl->first == cid){
		  containerVec.erase(cl);
			break;
		}
	}

#ifdef __DEBUG__
	cout << getName() << ", closeContainer: " << (int)cid << std::endl;
#endif
}

int Player::getLookCorpse()
{
	if(sex != 0)
		return ITEM_MALE_CORPSE;
	else
		return ITEM_FEMALE_CORPSE;
}

void Player::dropLoot(Container *corpse)
{
	for(int i = SLOT_FIRST; i < SLOT_LAST; ++i){
		Item* item = items[i];		
		#ifdef __SKULLSYSTEM__
		if(item && ((item->getContainer()) || random_range(1, 100) <= 10 || getSkull() == SKULL_RED)){
		#else
		if(item && ((item->getContainer()) || random_range(1, 100) <= 10)){
		#endif
			corpse->__internalAddThing(item);
			sendRemoveInventoryItem((slots_t)i, item);
			onRemoveInventoryItem((slots_t)i, item);
			items[i] = NULL;
		}
	}
}

fight_t Player::getFightType()
{
  for(int slot = SLOT_RIGHT; slot <= SLOT_LEFT; slot++){
    if(items[slot]){
			if((items[slot]->isWeapon())){
				Item* distItem = NULL;
				switch(items[slot]->getWeaponType()){
					case DIST:
						distItem = GetDistWeapon();
						if(distItem){
							return FIGHT_DIST;
						}
						else{
							return FIGHT_MELEE;
						}
					break;

					case MAGIC:
						return FIGHT_MAGICDIST;
				
					default:
						break;
				}
			}
    }
  }

	return FIGHT_MELEE;
}

void Player::removeDistItem()
{
	Item* distItem = GetDistWeapon();

	if(distItem && distItem->isStackable()){
		g_game.internalRemoveItem(distItem, 1);
	}
}

subfight_t Player::getSubFightType()
{
	fight_t type = getFightType();
	if(type == FIGHT_DIST){
		Item *DistItem = GetDistWeapon();
		if(DistItem){
			return DistItem->getSubfightType();
		}
	}

	if(type == FIGHT_MAGICDIST) {
	Item* distItem = GetDistWeapon();
		if(distItem){
			return distItem->getSubfightType();
		}	
	}

	return DIST_NONE;
}

Item* Player::GetDistWeapon() const
{
	for(int slot = SLOT_RIGHT; slot <= SLOT_LEFT; slot++){
	    if(items[slot]){
				if((items[slot]->isWeapon())) {
					switch (items[slot]->getWeaponType()){
					case DIST:
						//find ammunition
						if(items[slot]->getAmuType() == AMU_NONE){
							return items[slot];
						}

						if(items[SLOT_AMMO]){
							//compare ammo types
							if(items[SLOT_AMMO]->getWeaponType() == AMO && 
								items[slot]->getAmuType() == items[SLOT_AMMO]->getAmuType()){
									return items[SLOT_AMMO];
							}
							else{
								return NULL;
							}
							
						}
						else{
							return NULL;
						}

					break;

					case MAGIC:
						return items[slot];
					default:
						break;
					}
				}
			}
  	}
  	return NULL;
}

void Player::addStorageValue(const unsigned long key, const long value)
{
	storageMap[key] = value;
}

bool Player::getStorageValue(unsigned long key, long &value) const
{
	StorageMap::const_iterator it;
	it = storageMap.find(key);
	if(it != storageMap.end()){
		value = it->second;
		return true;
	}
	else{
		value = 0;
		return false;
	}
}

bool Player::CanSee(const Position& pos) const
{
	return client->CanSee(pos);
}

bool Player::CanSee(int x, int y, int z) const
{
  return client->CanSee(x, y, z);
}

Depot* Player::getDepot(uint32_t depotId, bool autoCreateDepot)
{	
	DepotMap::iterator it = depots.find(depotId);
	if(it != depots.end()){	
		return it->second;
	}

	//depot does not yet exist

	//create a new depot?
	if(autoCreateDepot){
		Depot* depot = NULL;
		Item* tmpDepot = Item::CreateItem(ITEM_LOCKER1);
		if(tmpDepot->getContainer() && (depot = tmpDepot->getContainer()->getDepot())){
			Item* depotChest = Item::CreateItem(ITEM_DEPOT);
			depot->__internalAddThing(depotChest);

			addDepot(depot, depotId);
			return depot;
		}
		else{
			g_game.FreeThing(tmpDepot);
			std::cout << "Failure: Creating a new depot with id: "<< depotId <<
				", for player: " << getName() << std::endl;
		}
	}

	return NULL;
}

bool Player::addDepot(Depot* depot, uint32_t depotId)
{
	Depot* depot2 = getDepot(depotId, false);
	if(depot2){
		return false;
	}
	
	depots[depotId] = depot;
	depot->setMaxDepotLimit(maxDepotLimit);
	return true;
}

void Player::sendCancelMessage(ReturnValue message) const
{
	switch(message){
		case RET_DESTINATIONOUTOFREACH:
			sendCancel("Destination is out of reach.");
		break;

		case RET_NOTMOVEABLE:
			sendCancel("You cannot move this object.");
		break;

		case RET_DROPTWOHANDEDITEM:
			sendCancel("Drop the double-handed object first.");
		break;

		case RET_BOTHHANDSNEEDTOBEFREE:
			sendCancel("Both hands needs to be free.");
		break;

		case RET_CANNOTBEDRESSED:
			sendCancel("You cannot dress this object there.");
		break;

		case RET_PUTTHISOBJECTINYOURHAND:
			sendCancel("Put this object in your hand.");
		break;

		case RET_PUTTHISOBJECTINBOTHHANDS:
			sendCancel("Put this object in both hands.");
		break;

		case RET_CANONLYUSEONEWEAPON:
			sendCancel("You may only use one weapon.");
		break;

		case RET_TOOFARAWAY:
			sendCancel("Too far away.");
		break;

		case RET_FIRSTGODOWNSTAIRS:
			sendCancel("First go downstairs.");
		break;

		case RET_FIRSTGOUPSTAIRS:
			sendCancel("First go upstairs.");
		break;

		case RET_NOTENOUGHCAPACITY:
			sendCancel("This object is to heavy.");
		break;
		
		case RET_CONTAINERNOTENOUGHROOM:
			sendCancel("You cannot put more objects in this container.");
		break;

		case RET_NEEDEXCHANGE:
		case RET_NOTENOUGHROOM:
			sendCancel("There is not enough room.");
		break;

		case RET_CANNOTPICKUP:
			sendCancel("You cannot pickup this object.");
		break;

		case RET_CANNOTTHROW:
			sendCancel("You cannot throw there.");
		break;

		case RET_THEREISNOWAY:
			sendCancel("There is no way.");
		break;
		
		case RET_THISISIMPOSSIBLE:
			sendCancel("This is impossible.");
		break;

		case RET_PLAYERISPZLOCKED:
			sendCancel("You can not enter a protection zone after attacking another player.");
		break;

		case RET_PLAYERISNOTINVITED:
			sendCancel("You are not invited.");
		break;

		case RET_DEPOTISFULL:
			sendCancel("You cannot put more items in this depot.");
		break;

		case RET_CANNOTUSETHISOBJECT:
			sendCancel("You can not use this object.");
		break;

		case RET_NOTREQUIREDLEVELTOUSERUNE:
			sendCancel("You do not have the required magic level to use this rune.");
		break;
		
		case RET_YOUAREALREADYTRADING:
			sendCancel("You are already trading.");
		break;

		case RET_THISPLAYERISALREADYTRADING:
			sendCancel("This player is already trading.");
		break;

		case RET_YOUMAYNOTLOGOUTDURINGAFIGHT:
			sendCancel("You may not logout during or immediately after a fight!");
		break;

		case RET_NOTPOSSIBLE:
		default:
			sendCancel("Sorry, not possible.");
		break;
	}
}

void Player::sendCancel(const char* msg) const
{
  client->sendCancel(msg);
}

void Player::sendChangeSpeed(Creature* creature)
{
	client->sendChangeSpeed(creature);
}

void Player::sendClosePrivate(unsigned short channelId) const
{
	client->sendClosePrivate(channelId);	
}

void Player::sendToChannel(Creature* creature, SpeakClasses type,
	const std::string &text, unsigned short channelId)
{
	client->sendToChannel(creature, type, text, channelId);
}

void Player::sendCancelTarget()
{
	client->sendCancelTarget();
}

void Player::sendCancelWalk() const
{
  client->sendCancelWalk();
}

void Player::sendStats()
{
	//update level and maglevel percents
	if(lastSentStats.experience != this->experience || lastSentStats.level != this->level)
		level_percent  = (unsigned char)(100*(experience-getExpForLv(level))/(1.*getExpForLv(level+1)-getExpForLv(level)));
			
	if(lastSentStats.manaspent != this->manaspent || lastSentStats.maglevel != this->maglevel)
		maglevel_percent  = (unsigned char)(100*(manaspent/(1.*getReqMana(maglevel+1,vocation))));
			
	//save current stats 
	lastSentStats.health = this->health;
	lastSentStats.healthmax = this->healthmax;
	lastSentStats.freeCapacity = this->getFreeCapacity();
	lastSentStats.experience = this->experience;
	lastSentStats.level = this->level;
	lastSentStats.mana = this->mana;
	lastSentStats.manamax = this->manamax;
	lastSentStats.manaspent = this->manaspent;
	lastSentStats.maglevel = this->maglevel;
	
	client->sendStats();
}

void Player::sendTextMessage(MessageClasses mclass, const char* message) const
{
	client->sendTextMessage(mclass,message);
}

void Player::sendCreatureLight(const Creature* creature)
{
	client->sendCreatureLight(creature);
}

void Player::sendWorldLight(LightInfo& lightInfo)
{
	client->sendWorldLight(lightInfo);
}

void Player::flushMsg()
{
	client->flushOutputBuffer();
}

void Player::sendTextMessage(MessageClasses mclass, const char* message,
	const Position &pos, unsigned char type) const
{
	client->sendTextMessage(mclass,message,pos,type);
}

void Player::sendPing()
{
	internal_ping++;
	if(internal_ping >= 5){ //1 ping each 5 seconds
		internal_ping = 0;
		npings++;
		client->sendPing();
	}
	if(npings >= 6){
		//std::cout << "logout" << std::endl;
		if(inFightTicks >=1000 && health >0) {
			//logout?
			//client->logout();
		}
		else{
			//client->logout();			
		}
	}
}

void Player::receivePing()
{
	if(npings > 0)
		npings--;
}

void Player::sendDistanceShoot(const Position &from, const Position &to, unsigned char type)
{
	client->sendDistanceShoot(from, to,type);
}

void Player::sendMagicEffect(const Position &pos, unsigned char type)
{
	client->sendMagicEffect(pos,type);
}

void Player::sendAnimatedText(const Position &pos, unsigned char color, std::string text)
{
	client->sendAnimatedText(pos,color,text);
}

void Player::sendCreatureHealth(const Creature *creature)
{
	client->sendCreatureHealth(creature);
}

void Player::sendTradeItemRequest(const Player* player, const Item* item, bool ack)
{
	client->sendTradeItemRequest(player, item, ack);
}

void Player::sendCloseTrade()
{
	client->sendCloseTrade();
}

void Player::sendTextWindow(Item* item, const unsigned short maxlen, const bool canWrite)
{
	client->sendTextWindow(item,maxlen,canWrite);
}

void Player::sendHouseWindow(House* _house, unsigned long _listid)
{
	std::string text;
	if(_house->getAccessList(_listid, text)){
		client->sendHouseWindow(_house, _listid, text);
	}
}

//tile
//send methods
void Player::sendAddTileItem(const Position& pos, const Item* item)
{
	client->sendAddTileItem(pos, item);
}

void Player::sendUpdateTileItem(const Position& pos, uint32_t stackpos, const Item* olditem, const Item* newitem)
{
	client->sendUpdateTileItem(pos, stackpos, newitem);
}

void Player::sendRemoveTileItem(const Position& pos, uint32_t stackpos, const Item* item)
{
	client->sendRemoveTileItem(pos, stackpos);
}

void Player::sendUpdateTile(const Position& pos)
{
	client->UpdateTile(pos);
}

void Player::sendCreatureAppear(const Creature* creature, bool isLogin)
{
	client->sendAddCreature(creature, isLogin);
}

void Player::sendCreatureDisappear(const Creature* creature, uint32_t stackpos, bool isLogout)
{
	client->sendRemoveCreature(creature, creature->getPosition(), stackpos, isLogout);
}

void Player::sendCreatureMove(const Creature* creature, const Position& oldPos, uint32_t oldStackPos, bool teleport)
{
	client->sendMoveCreature(creature, oldPos, oldStackPos, teleport);
}

void Player::sendCreatureTurn(const Creature* creature, uint32_t stackPos)
{
  client->sendCreatureTurn(creature, stackPos);
}

void Player::sendCreatureChangeOutfit(const Creature* creature)
{
	client->sendSetOutfit(creature);
}

void Player::sendCreatureSay(const Creature* creature, SpeakClasses type, const std::string& text)
{
  client->sendCreatureSay(creature, type, text);
}

void Player::sendCreatureSquare(const Creature* creature, SquareColor color)
{
	client->sendCreatureSquare(creature, color);
}

void Player::sendAddContainerItem(const Container* container, const Item* item)
{
  for(ContainerVector::const_iterator cl = containerVec.begin(); cl != containerVec.end(); ++cl){
		if(cl->second == container){
			client->sendAddContainerItem(cl->first, item);
		}
	}
}

void Player::sendUpdateContainerItem(const Container* container, uint8_t slot, const Item* oldItem, const Item* newItem)
{
  for(ContainerVector::const_iterator cl = containerVec.begin(); cl != containerVec.end(); ++cl){
		if(cl->second == container){
			client->sendUpdateContainerItem(cl->first, slot, newItem);
		}
	}
}

void Player::sendRemoveContainerItem(const Container* container, uint8_t slot, const Item* item)
{
  for(ContainerVector::const_iterator cl = containerVec.begin(); cl != containerVec.end(); ++cl){
		if(cl->second == container){
			client->sendRemoveContainerItem(cl->first, slot);
		}
	}
}

//inventory
void Player::sendAddInventoryItem(slots_t slot, const Item* item)
{
	client->sendAddInventoryItem(slot, item);
}

void Player::sendUpdateInventoryItem(slots_t slot, const Item* oldItem, const Item* newItem)
{
	client->sendUpdateInventoryItem(slot, newItem);
}

void Player::sendRemoveInventoryItem(slots_t slot, const Item* item)
{
	client->sendRemoveInventoryItem(slot);
}

void Player::onAddTileItem(const Position& pos, const Item* item)
{
	//
}

void Player::onUpdateTileItem(const Position& pos, uint32_t stackpos, const Item* oldItem, const Item* newItem)
{
	if(oldItem != newItem){
		onRemoveTileItem(pos, stackpos, oldItem);
	}

	if(tradeState != TRADE_TRANSFER){
		if(tradeItem && oldItem == tradeItem){
			g_game.playerCloseTrade(this);
		}
	}
}

void Player::onRemoveTileItem(const Position& pos, uint32_t stackpos, const Item* item)
{
	if(tradeState != TRADE_TRANSFER){
		checkTradeState(item);

		if(tradeItem){
			const Container* container = item->getContainer();
			if(container && container->isHoldingItem(tradeItem)){
				g_game.playerCloseTrade(this);
			}
		}
	}
}

void Player::onUpdateTile(const Position& pos)
{
	//
}

void Player::onCreatureAppear(const Creature* creature, bool isLogin)
{
	//
}

void Player::onCreatureDisappear(const Creature* creature, uint32_t stackpos, bool isLogout)
{
	if(attackedCreature2 == creature->getID()){
		setAttackedCreature(NULL);
		sendCancelTarget();

		if(isLogout){
			sendTextMessage(MSG_STATUS_SMALL, "Target lost.");
		}
	}

	if(followCreature == creature){
		setFollowCreature(NULL);
		sendCancelTarget();

		if(isLogout){
			sendTextMessage(MSG_STATUS_SMALL, "Target lost.");
		}
	}

	if(creature == this){
		if(isLogout){
			loginPosition = getPosition();
		}

		if(eventAutoWalk != 0){
			setFollowCreature(NULL);
		}

		if(tradePartner){
			g_game.playerCloseTrade(this);
		}

		g_chat.removeUserFromAllChannels(this);
		bool saved = false;
		for(long tries = 0; tries < 3; tries++){
			if(IOPlayer::instance()->savePlayer(this)){
				saved = true;
				break;
			}
		}
		if(!saved){
			std::cout << "Error while saving player: " << getName() << std::endl;
		}

#ifdef __DEBUG_PLAYERS__
		std::cout << (uint32_t)getPlayersOnline() << " players online." << std::endl;
		#endif
	}
}

void Player::onCreatureMove(const Creature* creature, const Position& oldPos, uint32_t oldStackPos, bool teleport)
{
	if(followCreature && (creature == followCreature || creature == this)){
		if(!Position::areInRange<7,5,0>(followCreature->getPosition(), getPosition())){
			setFollowCreature(NULL);
			sendCancelTarget();
			sendTextMessage(MSG_STATUS_SMALL, "Target lost.");
		}
	}

	Creature* targetCreature = getAttackedCreature();
	if((creature == this && targetCreature) || targetCreature == creature){
		if(!Position::areInRange<7,5,0>(targetCreature->getPosition(), getPosition())){
			setAttackedCreature(NULL);
			sendCancelTarget();
			sendTextMessage(MSG_STATUS_SMALL, "Target lost.");
		} 
	}

	if(creature == this){
		if(tradeState != TRADE_TRANSFER){
			//check if we should close trade
			if(tradeItem){
				if(!Position::areInRange<1,1,0>(tradeItem->getPosition(), getPosition())){
					g_game.playerCloseTrade(this);
				}
			}

			if(tradePartner){
				if(!Position::areInRange<2,2,0>(tradePartner->getPosition(), getPosition())){
					g_game.playerCloseTrade(this);
				}
			}
		}
	}
}

void Player::onCreatureTurn(const Creature* creature, uint32_t stackPos)
{
  //
}

void Player::onCreatureChangeOutfit(const Creature* creature)
{
	//
}

void Player::onCreatureSay(const Creature* creature, SpeakClasses type, const std::string& text)
{
  sendCreatureSay(creature, type, text);
}

//container
void Player::onAddContainerItem(const Container* container, const Item* item)
{
	checkTradeState(item);
}

void Player::onUpdateContainerItem(const Container* container, uint8_t slot, const Item* oldItem, const Item* newItem)
{
	if(oldItem != newItem){
		onRemoveContainerItem(container, slot, oldItem);
	}

	if(tradeState != TRADE_TRANSFER){
		checkTradeState(oldItem);
	}
}

void Player::onRemoveContainerItem(const Container* container, uint8_t slot, const Item* item)
{
	if(tradeState != TRADE_TRANSFER){
		checkTradeState(item);
		
		if(tradeItem){
			if(tradeItem->getParent() != container && container->isHoldingItem(tradeItem)){
				g_game.playerCloseTrade(this);
			}
		}
	}
}

void Player::onCloseContainer(const Container* container)
{
  for(ContainerVector::const_iterator cl = containerVec.begin(); cl != containerVec.end(); ++cl){
		if(cl->second == container){
			client->sendCloseContainer(cl->first);
		}
	}
}

void Player::onSendContainer(const Container* container)
{
	bool hasParent = (dynamic_cast<const Container*>(container->getParent()) != NULL);

	for(ContainerVector::const_iterator cl = containerVec.begin(); cl != containerVec.end(); ++cl){
		if(cl->second == container){
			client->sendContainer(cl->first, container, hasParent);
		}
	}
}

//inventory
void Player::onAddInventoryItem(slots_t slot, const Item* item)
{
	//
}

void Player::onUpdateInventoryItem(slots_t slot, const Item* oldItem, const Item* newItem)
{
	if(oldItem != newItem){
		onRemoveInventoryItem(slot, oldItem);
	}

	if(tradeState != TRADE_TRANSFER){
		checkTradeState(oldItem);
	}
}

void Player::onRemoveInventoryItem(slots_t slot, const Item* item)
{
	if(tradeState != TRADE_TRANSFER){
		checkTradeState(item);

		if(tradeItem){
			const Container* container = item->getContainer();
			if(container && container->isHoldingItem(tradeItem)){
				g_game.playerCloseTrade(this);
			}
		}
	}
}

void Player::checkTradeState(const Item* item)
{
	if(tradeItem && tradeState != TRADE_TRANSFER){
		if(tradeItem == item){
			g_game.playerCloseTrade(this);
		}
		else{
			const Container* container = dynamic_cast<const Container*>(item->getParent());

			while(container != NULL){
				if(container == tradeItem){
					g_game.playerCloseTrade(this);
					break;
				}

				container = dynamic_cast<const Container*>(container->getParent());
			}
		}
	}
}

bool Player::NeedUpdateStats()
{
	if(lastSentStats.health != this->health ||
		 lastSentStats.healthmax != this->healthmax ||
		 (int)lastSentStats.freeCapacity != (int)this->getFreeCapacity() ||
		 lastSentStats.experience != this->experience ||
		 lastSentStats.level != this->level ||
		 lastSentStats.mana != this->mana ||
		 lastSentStats.manamax != this->manamax ||
		 lastSentStats.manaspent != this->manaspent ||
		 lastSentStats.maglevel != this->maglevel){
		return true;
	}
	else{
		return false;
	}
}

int Player::onThink(int& newThinkTicks)
{
	newThinkTicks = 1000;
	return 1000;
}

void Player::addManaSpent(unsigned long spent)
{
	if(spent == 0)
		return;

	manaspent += spent;
	int reqMana = getReqMana(maglevel + 1, vocation);
	if(access == 0 && manaspent >= reqMana){
		manaspent -= reqMana;
		maglevel++;
		
		std::stringstream MaglvMsg;
		MaglvMsg << "You advanced to magic level " << maglevel << ".";
		sendTextMessage(MSG_EVENT_ADVANCE, MaglvMsg.str().c_str());
		sendStats();
	}
}

void Player::addExperience(unsigned long exp)
{
	experience += exp;
	int lastLevel = level;

	while(experience >= getExpForLv(level + 1)){
		++level;
		healthmax += HPGain[(int)vocation];
		health += HPGain[(int)vocation];
		manamax += ManaGain[(int)vocation];
		mana += ManaGain[(int)vocation];
		capacity += CapGain[(int)vocation];
	}

	if(lastLevel != level){
		setNormalSpeed();
		g_game.changeSpeed(getID(), getSpeed());

		std::stringstream levelMsg;
		levelMsg << "You advanced from Level " << lastLevel << " to Level " << level << ".";
		sendTextMessage(MSG_EVENT_ADVANCE, levelMsg.str().c_str());
		sendStats();
	}
}

unsigned long Player::getIP() const
{
	return client->getIP();
}

void Player::die()
{
	loginPosition = masterPos;

	//Magic level loss
	unsigned long sumMana = 0;
	long lostMana = 0;
	for (int i = 1; i <= maglevel; i++) {              //sum up all the mana
		sumMana += getReqMana(i, vocation);
	}

	sumMana += manaspent;

	lostMana = (long)(sumMana * 0.1);   //player loses 10% of all spent mana when he dies
    
	while(lostMana > manaspent){
		lostMana -= manaspent;
		manaspent = getReqMana(maglevel, vocation);
		maglevel--;
	}

	manaspent -= lostMana;
	//

	//Skill loss
	long lostSkillTries;
	unsigned long sumSkillTries;
	for(int i = 0; i <= 6; i++){  //for each skill
		lostSkillTries = 0;         //reset to 0
		sumSkillTries = 0;

		for(unsigned c = 11; c <= skills[i][SKILL_LEVEL]; c++) { //sum up all required tries for all skill levels
			sumSkillTries += getReqSkillTries(i, c, vocation);
		}

		sumSkillTries += skills[i][SKILL_TRIES];
		lostSkillTries = (long) (sumSkillTries * 0.1);           //player loses 10% of his skill tries

		while(lostSkillTries > skills[i][SKILL_TRIES]){
			lostSkillTries -= skills[i][SKILL_TRIES];
			skills[i][SKILL_TRIES] = getReqSkillTries(i, skills[i][SKILL_LEVEL], vocation);
			if(skills[i][SKILL_LEVEL] > 10){
				skills[i][SKILL_LEVEL]--;
			}
			else{
				skills[i][SKILL_LEVEL] = 10;
				skills[i][SKILL_TRIES] = 0;
				lostSkillTries = 0;
				break;
			}
		}
		skills[i][SKILL_TRIES] -= lostSkillTries;
	}               
	//

	//Level loss
	long newLevel = level;
	while((unsigned long)(experience - getLostExperience()) < getExpForLv(newLevel)){
		if(newLevel > 1)
			newLevel--;
		else
			break;
	}

	if(newLevel != level){
		std::stringstream lvMsg;
		lvMsg << "You were downgraded from level " << level << " to level " << newLevel << ".";
		client->sendTextMessage(MSG_EVENT_ADVANCE, lvMsg.str().c_str());
	}
}

void Player::preSave()
{
	if(health <= 0){
		health = healthmax;
		experience -= getLostExperience();
				
		while(experience < getExpForLv(level)){
			if(level > 1)                               
				level--;
			else
				break;
			
			// This checks (but not the downgrade sentences) aren't really necesary cause if the
			// player has a "normal" hp,mana,etc when he gets level 1 he will not lose more
			// hp,mana,etc... but here they are :P 
			if((healthmax -= HPGain[(int)vocation]) < 0) //This could be avoided with a proper use of unsigend int
				healthmax = 10;
			
			health = healthmax;
			
			if((manamax -= ManaGain[(int)vocation]) < 0) //This could be avoided with a proper use of unsigend int
				manamax = 0;
			
			mana = manamax;
			
			if((capacity -= CapGain[(int)vocation]) < 0) //This could be avoided with a proper use of unsigend int
				capacity = 0.0;         
		}
	}
}

void Player::kickPlayer()
{
	client->logout();
}

bool Player::gainManaTick()
{
	int add;
	manaTick++;
	if(vocation >= 0 && vocation < 5){
		if(manaTick < gainManaVector[vocation][0])
			return false;
		manaTick = 0;
		add = gainManaVector[vocation][1];
	}
	else{
		add = 5;
	}
	mana += min(add, manamax - mana);
	return true;
}

bool Player::gainHealthTick()
{
	int add;
	healthTick++;
	if(vocation >= 0 && vocation < 5){
		if(healthTick < gainHealthVector[vocation][0])
			return false;
		healthTick = 0;
		add = gainHealthVector[vocation][1];
	}
	else{
		add = 5;
	}
	health += min(add, healthmax - health);
	return true;
}

void Player::removeList()
{
	listPlayer.removeList(getID());
	
	for(AutoList<Player>::listiterator it = Player::listPlayer.list.begin(); it != Player::listPlayer.list.end(); ++it)
	{
		(*it).second->notifyLogOut(this);
	}	
}

void Player::addList()
{
	for(AutoList<Player>::listiterator it = Player::listPlayer.list.begin(); it != Player::listPlayer.list.end(); ++it)
	{
		(*it).second->notifyLogIn(this);
	}	
	
	listPlayer.addList(this);
}

void Player::notifyLogIn(Player* login_player)
{
	VIPListSet::iterator it = VIPList.find(login_player->getGUID());
	if(it != VIPList.end()){
		client->sendVIPLogIn(login_player->getGUID());
		sendTextMessage(MSG_STATUS_SMALL, (login_player->getName() + " has logged in.").c_str());
	}
}

void Player::notifyLogOut(Player* logout_player)
{
	VIPListSet::iterator it = VIPList.find(logout_player->getGUID());
	if(it != VIPList.end()){
		client->sendVIPLogOut(logout_player->getGUID());
		sendTextMessage(MSG_STATUS_SMALL, (logout_player->getName() + " has logged out.").c_str());
	}
}

bool Player::removeVIP(unsigned long _guid)
{
	VIPListSet::iterator it = VIPList.find(_guid);
	if(it != VIPList.end()){
		VIPList.erase(it);
		return true;
	}
	return false;
}

bool Player::addVIP(unsigned long _guid, std::string &name, bool isOnline, bool internal /*=false*/)
{
	if(guid == _guid){
		if(!internal)
			sendTextMessage(MSG_STATUS_SMALL, "You cannot add yourself.");
		return false;
	}
	
	if(VIPList.size() > 50){
		if(!internal)
			sendTextMessage(MSG_STATUS_SMALL, "You cannot add more buddies.");
		return false;
	}
	
	VIPListSet::iterator it = VIPList.find(_guid);
	if(it != VIPList.end()){
		if(!internal)
			sendTextMessage(MSG_STATUS_SMALL, "This player is already in your list.");
		return false;
	}
	
	VIPList.insert(_guid);
	
	if(!internal)
		client->sendVIP(_guid, name, isOnline);
	
	return true;
}

//close container and its child containers
void Player::autoCloseContainers(const Container* container)
{
	typedef std::vector<uint32_t> CloseList;
	CloseList closeList;
	
	for(ContainerVector::iterator it = containerVec.begin(); it != containerVec.end(); ++it){
		Container* tmpcontainer = it->second;
		while(tmpcontainer != NULL){
			if(tmpcontainer->isRemoved() || tmpcontainer == container){
				closeList.push_back(it->first);
				break;
			}
			
			tmpcontainer = dynamic_cast<Container*>(tmpcontainer->getParent());
		}
	}
	
	for(CloseList::iterator it = closeList.begin(); it != closeList.end(); ++it){
		closeContainer(*it);
		client->sendCloseContainer(*it);
	}						
}

bool Player::hasCapacity(const Item* item, uint32_t count) const
{
	if(access == 0 && item->getTopParent() != this){
		double itemWeight = 0;

		if(item->isStackable()){
			itemWeight = Item::items[item->getID()].weight * count;
		}
		else
			itemWeight = item->getWeight();

		return (itemWeight < getFreeCapacity());
	}

	return true;
}

ReturnValue Player::__queryAdd(int32_t index, const Thing* thing, uint32_t count,
	uint32_t flags) const
{
	const Item* item = thing->getItem();
	if(item == NULL){
		return RET_NOTPOSSIBLE;
	}

	bool childIsOwner = ((flags & FLAG_CHILDISOWNER) == FLAG_CHILDISOWNER);
	bool skipLimit = ((flags & FLAG_NOLIMIT) == FLAG_NOLIMIT);

	if(childIsOwner){
		//a child container is querying the player, just check if enough capacity
		if(skipLimit || hasCapacity(item, count))
			return RET_NOERROR;
		else
			return RET_NOTENOUGHCAPACITY;
	}

	if(!item->isPickupable()){
		return RET_CANNOTPICKUP;
	}
	
	ReturnValue ret = RET_NOERROR;
	
	if((item->getSlotPosition() & SLOTP_HEAD) ||
		(item->getSlotPosition() & SLOTP_NECKLACE) ||
		(item->getSlotPosition() & SLOTP_BACKPACK) ||
		(item->getSlotPosition() & SLOTP_ARMOR) ||
		(item->getSlotPosition() & SLOTP_LEGS) ||
		(item->getSlotPosition() & SLOTP_FEET) ||
		(item->getSlotPosition() & SLOTP_RING)){
		ret = RET_CANNOTBEDRESSED;
	}
	else if(item->getSlotPosition() & SLOTP_TWO_HAND){
		ret = RET_PUTTHISOBJECTINBOTHHANDS;
	}
	else if((item->getSlotPosition() & SLOTP_RIGHT) || (item->getSlotPosition() & SLOTP_LEFT)){
		ret = RET_PUTTHISOBJECTINYOURHAND;
	}

	//check if we can dress this object
	switch(index){
		case SLOT_HEAD:
			if(item->getSlotPosition() & SLOTP_HEAD)
				ret = RET_NOERROR;
			break;
		case SLOT_NECKLACE:
			if(item->getSlotPosition() & SLOTP_NECKLACE)
				ret = RET_NOERROR;
			break;
		case SLOT_BACKPACK:
			if(item->getSlotPosition() & SLOTP_BACKPACK)
				ret = RET_NOERROR;
			break;
		case SLOT_ARMOR:
			if(item->getSlotPosition() & SLOTP_ARMOR)
				ret = RET_NOERROR;
			break;
		case SLOT_RIGHT:
			if(item->getSlotPosition() & SLOTP_RIGHT){
				//check if we already carry an item in the other hand
				if(item->getSlotPosition() & SLOTP_TWO_HAND){
					if(items[SLOT_LEFT] && items[SLOT_LEFT] != item){
						ret = RET_BOTHHANDSNEEDTOBEFREE;
					}
					else
					ret = RET_NOERROR;
				}
				else{
					//check if we already carry a double-handed item
					if(items[SLOT_LEFT]){
						if(items[SLOT_LEFT]->getSlotPosition() & SLOTP_TWO_HAND){
							ret = RET_DROPTWOHANDEDITEM;
						}
						//check if weapon, can only carry one weapon
						else if(item != items[SLOT_LEFT] && items[SLOT_LEFT]->isWeapon() &&
							(items[SLOT_LEFT]->getWeaponType() != SHIELD) &&
							(items[SLOT_LEFT]->getWeaponType() != AMO) &&
							item->isWeapon() && (item->getWeaponType() != SHIELD) && (item->getWeaponType() != AMO)){
								ret = RET_CANONLYUSEONEWEAPON;
						}
						else
							ret = RET_NOERROR;
					}
					else
						ret = RET_NOERROR;
				}
			}
			break;
		case SLOT_LEFT:
			if(item->getSlotPosition() & SLOTP_LEFT){
				//check if we already carry an item in the other hand
				if(item->getSlotPosition() & SLOTP_TWO_HAND){
					if(items[SLOT_RIGHT] && items[SLOT_RIGHT] != item){
						ret = RET_BOTHHANDSNEEDTOBEFREE;
					}
					else
						ret = RET_NOERROR;
				}
				else{
					//check if we already carry a double-handed item
					if(items[SLOT_RIGHT]){
						if(items[SLOT_RIGHT]->getSlotPosition() & SLOTP_TWO_HAND){
							ret = RET_DROPTWOHANDEDITEM;
						}
						//check if weapon, can only carry one weapon
						else if(item != items[SLOT_RIGHT] && items[SLOT_RIGHT]->isWeapon() &&
							(items[SLOT_RIGHT]->getWeaponType() != SHIELD) &&
							(items[SLOT_RIGHT]->getWeaponType() != AMO) &&
							item->isWeapon() && (item->getWeaponType() != SHIELD) && (item->getWeaponType() != AMO)){
								ret = RET_CANONLYUSEONEWEAPON;
						}
						else
							ret = RET_NOERROR;
					}
					else
						ret = RET_NOERROR;
				}
			}
			break;
		case SLOT_LEGS:
			if(item->getSlotPosition() & SLOTP_LEGS)
				ret = RET_NOERROR;
			break;
		case SLOT_FEET:
			if(item->getSlotPosition() & SLOTP_FEET)
				ret = RET_NOERROR;
			break;
		case SLOT_RING:
			if(item->getSlotPosition() & SLOTP_RING)
				ret = RET_NOERROR;
			break;
		case SLOT_AMMO:
			if(item->getSlotPosition() & SLOTP_AMMO)
				ret = RET_NOERROR;
			break;
		case SLOT_WHEREEVER:
			ret = RET_NOTENOUGHROOM;
			break;
		case -1:
			ret = RET_NOTENOUGHROOM;
			break;

		default:
			ret = RET_NOTPOSSIBLE;
			break;
	}

	if(ret == RET_NOERROR || ret == RET_NOTENOUGHROOM){
		//need an exchange with source?
		if(getInventoryItem((slots_t)index) != NULL){
			if(!getInventoryItem((slots_t)index)->isStackable() || getInventoryItem((slots_t)index)->getID() != item->getID()){
				return RET_NEEDEXCHANGE;
			}
		}

		//check if enough capacity
		if(hasCapacity(item, count))
			return ret;
		else
			return RET_NOTENOUGHCAPACITY;
	}

	return ret;
}

ReturnValue Player::__queryMaxCount(int32_t index, const Thing* thing, uint32_t count, uint32_t& maxQueryCount,
	uint32_t flags) const
{
	const Item* item = thing->getItem();
	if(item == NULL){
		maxQueryCount = 0;
		return RET_NOTPOSSIBLE;
	}

	const Thing* destThing = __getThing(index);
	const Item* destItem = NULL;
	if(destThing)
		destItem = destThing->getItem();

	if(destItem){
		if(destItem->isStackable() && item->getID() == destItem->getID()){
			maxQueryCount = 100 - destItem->getItemCount();
		}
		else
			maxQueryCount = 0;
	}
	else{
		if(item->isStackable())
			maxQueryCount = 100;
		else
			maxQueryCount = 1;

		return RET_NOERROR;
	}

	if(maxQueryCount < count)
		return RET_NOTENOUGHROOM;
	else
		return RET_NOERROR;
}

ReturnValue Player::__queryRemove(const Thing* thing, uint32_t count) const
{
	uint32_t index = __getIndexOfThing(thing);

	if(index == -1){
		return RET_NOTPOSSIBLE;
	}
	
	const Item* item = thing->getItem();
	if(item == NULL){
		return RET_NOTPOSSIBLE;
	}

	if(count == 0 || (item->isStackable() && count > item->getItemCount())){
		return RET_NOTPOSSIBLE;
	}

	if(item->isNotMoveable()){
		return RET_NOTMOVEABLE;
	}

	return RET_NOERROR;
}

Cylinder* Player::__queryDestination(int32_t& index, const Thing* thing, Item** destItem,
	uint32_t& flags)
{
	if(index == 0 /*drop to capacity window*/ || index == INDEX_WHEREEVER){
		*destItem = NULL;

		const Item* item = thing->getItem();
		if(item == NULL){
			return this;
		}

		//find a appropiate slot
		for(int i = SLOT_FIRST; i < SLOT_LAST; ++i){
			if(items[i] == NULL){
				if(__queryAdd(i, item, item->getItemCount(), 0) == RET_NOERROR){
					index = i;
					return this;
				}
			}
		}

		//try containers
		for(int i = SLOT_FIRST; i < SLOT_LAST; ++i){
			if(Container* subContainer = dynamic_cast<Container*>(items[i])){
				if(subContainer != tradeItem && subContainer->__queryAdd(-1, item, item->getItemCount(), 0) == RET_NOERROR){
					index = INDEX_WHEREEVER;
					*destItem = NULL;
					return subContainer;
				}
			}
		}

		return this;
	}

	Thing* destThing = __getThing(index);

	if(destThing)
		*destItem = destThing->getItem();

	Cylinder* subCylinder = dynamic_cast<Cylinder*>(destThing);

	if(subCylinder){
		index = INDEX_WHEREEVER;
		*destItem = NULL;
		return subCylinder;
	}
	else
		return this;
}

void Player::__addThing(Thing* thing)
{
	__addThing(0, thing);
}

void Player::__addThing(int32_t index, Thing* thing)
{
	if(index < 0 || index > 11){
#ifdef __DEBUG__MOVESYS__
		std::cout << "Failure: [Player::__addThing], " << "player: " << getName() << ", index: " << index << ", index < 0 || index > 11" << std::endl;
		DEBUG_REPORT
#endif
		return /*RET_NOTPOSSIBLE*/;
	}

	if(index == 0){
#ifdef __DEBUG__MOVESYS__
		std::cout << "Failure: [Player::__addThing], " << "player: " << getName() << ", index == 0" << std::endl;
		DEBUG_REPORT
#endif
		return /*RET_NOTENOUGHROOM*/;
	}

	Item* item = thing->getItem();
	if(item == NULL){
#ifdef __DEBUG__MOVESYS__
		std::cout << "Failure: [Player::__addThing], " << "player: " << getName() << ", item == NULL" << std::endl;
		DEBUG_REPORT
#endif
		return /*RET_NOTPOSSIBLE*/;
	}

	item->setParent(this);
	items[index] = item;

	//send to client
	sendAddInventoryItem((slots_t)index, item);

	//event methods
	onAddInventoryItem((slots_t)index, item);
}

void Player::__updateThing(Thing* thing, uint32_t count)
{
	int32_t index = __getIndexOfThing(thing);
	if(index == -1){
#ifdef __DEBUG__MOVESYS__
		std::cout << "Failure: [Player::__updateThing], " << "player: " << getName() << ", index == -1" << std::endl;
		DEBUG_REPORT
#endif
		return /*RET_NOTPOSSIBLE*/;
	}

	Item* item = thing->getItem();
	if(item == NULL){
#ifdef __DEBUG__MOVESYS__
		std::cout << "Failure: [Player::__updateThing], " << "player: " << getName() << ", item == NULL" << std::endl;
		DEBUG_REPORT
#endif
		return /*RET_NOTPOSSIBLE*/;
	}

	item->setItemCountOrSubtype(count);

	//send to client
	sendUpdateInventoryItem((slots_t)index, item, item);

	//event methods
	onUpdateInventoryItem((slots_t)index, item, item);
}

void Player::__replaceThing(uint32_t index, Thing* thing)
{
	if(index < 0 || index > 11){
#ifdef __DEBUG__MOVESYS__
		std::cout << "Failure: [Player::__replaceThing], " << "player: " << getName() << ", index: " << index << ",  index < 0 || index > 11" << std::endl;
		DEBUG_REPORT
#endif
		return /*RET_NOTPOSSIBLE*/;
	}
	
	Item* oldItem = getInventoryItem((slots_t)index);
	if(!oldItem){
#ifdef __DEBUG__MOVESYS__
		std::cout << "Failure: [Player::__updateThing], " << "player: " << getName() << ", oldItem == NULL" << std::endl;
		DEBUG_REPORT
#endif
		return /*RET_NOTPOSSIBLE*/;
	}

	Item* item = thing->getItem();
	if(item == NULL){
#ifdef __DEBUG__MOVESYS__
		std::cout << "Failure: [Player::__updateThing], " << "player: " << getName() << ", item == NULL" << std::endl;
		DEBUG_REPORT
#endif
		return /*RET_NOTPOSSIBLE*/;
	}

	//send to client
	sendUpdateInventoryItem((slots_t)index, oldItem, item);
	
	//event methods
	onUpdateInventoryItem((slots_t)index, oldItem, item);

	item->setParent(this);
	items[index] = item;
}

void Player::__removeThing(Thing* thing, uint32_t count)
{
	Item* item = thing->getItem();
	if(item == NULL){
#ifdef __DEBUG__MOVESYS__
		std::cout << "Failure: [Player::__removeThing], " << "player: " << getName() << ", item == NULL" << std::endl;
		DEBUG_REPORT
#endif
		return /*RET_NOTPOSSIBLE*/;
	}

	int32_t index = __getIndexOfThing(thing);
	if(index == -1){
#ifdef __DEBUG__MOVESYS__
		std::cout << "Failure: [Player::__removeThing], " << "player: " << getName() << ", index == -1" << std::endl;
		DEBUG_REPORT
#endif
		return /*RET_NOTPOSSIBLE*/;
	}

	if(item->isStackable()){
		if(count == item->getItemCount()){
			//send change to client
			sendRemoveInventoryItem((slots_t)index, item);

			//event methods
			onRemoveInventoryItem((slots_t)index, item);

			item->setParent(NULL);
			items[index] = NULL;
		}
		else{
			int newCount = std::max(0, (int)(item->getItemCount() - count));
			item->setItemCount(newCount);

			//send change to client
			sendUpdateInventoryItem((slots_t)index, item, item);

			//event methods
			onUpdateInventoryItem((slots_t)index, item, item);
		}
	}
	else{
		//send change to client
		sendRemoveInventoryItem((slots_t)index, item);
	
		//event methods
		onRemoveInventoryItem((slots_t)index, item);

		item->setParent(NULL);
		items[index] = NULL;
	}
}

int32_t Player::__getIndexOfThing(const Thing* thing) const
{
	for(int i = SLOT_FIRST; i < SLOT_LAST; ++i){
		if(items[i] == thing)
			return i;
	}

	return -1;
}

int32_t Player::__getFirstIndex() const
{
	return SLOT_FIRST;
}

int32_t Player::__getLastIndex() const
{
	return SLOT_LAST;
}

uint32_t Player::__getItemTypeCount(uint16_t itemId) const
{
	uint32_t count = 0;

	std::list<const Container*> listContainer;
	ItemList::const_iterator cit;
	Container* tmpContainer = NULL;
	Item* item = NULL;

	for(int i = SLOT_FIRST; i < SLOT_LAST; i++){
		if(item = items[i]){
			if(item->getID() == itemId){
				if(item->isStackable()){
					count+= item->getItemCount();
				}
				else{
					++count;
				}
			}

			if(tmpContainer = item->getContainer()){
				listContainer.push_back(tmpContainer);
			}
		}
	}
	
	while(listContainer.size() > 0){
		const Container* container = listContainer.front();
		listContainer.pop_front();
		
		count+= container->__getItemTypeCount(itemId);

		for(cit = container->getItems(); cit != container->getEnd(); ++cit){
			if(tmpContainer = (*cit)->getContainer()){
				listContainer.push_back(tmpContainer);
			}
		}
	}

	return count;
}

Thing* Player::__getThing(uint32_t index) const
{
	if(index >= SLOT_FIRST && index < SLOT_LAST)
		return items[index];

	return NULL;
}

void Player::postAddNotification(Thing* thing, bool hasOwnership /*= true*/)
{
	if(hasOwnership){
		updateItemsLight();
		updateInventoryWeigth();
		client->sendStats();
	}

	if(const Item* item = thing->getItem()){
		if(const Container* container = item->getContainer()){
			onSendContainer(container);
		}
	}
	else if(const Creature* creature = thing->getCreature()){
		if(creature == this){
			//check containers
			std::vector<Container*> containers;
			for(ContainerVector::iterator it = containerVec.begin(); it != containerVec.end(); ++it){
				if(!Position::areInRange<1,1,0>(it->second->getPosition(), getPosition())){
					containers.push_back(it->second);
				}
			}

			for(std::vector<Container*>::const_iterator it = containers.begin(); it != containers.end(); ++it){
				autoCloseContainers(*it);
			}
		}
	}
}

void Player::postRemoveNotification(Thing* thing, bool isCompleteRemoval, bool hadOwnership /*= true*/)
{
	if(hadOwnership){
		updateItemsLight();
		updateInventoryWeigth();
		client->sendStats();
	}

	if(const Item* item = thing->getItem()){
		if(const Container* container = item->getContainer()){
			if(!container->isRemoved() &&
					(container->getTopParent() == this || (dynamic_cast<const Container*>(container->getTopParent()))) &&
					Position::areInRange<1,1,0>(getPosition(), container->getPosition())){
				onSendContainer(container);
			}
			else{
				autoCloseContainers(container);
			}
		}
	}
}

void Player::__internalAddThing(Thing* thing)
{
	__internalAddThing(0, thing);
}

void Player::__internalAddThing(uint32_t index, Thing* thing)
{
#ifdef __DEBUG__MOVESYS__NOTICE
	std::cout << "[Player::__internalAddThing] index: " << index << std::endl;
#endif

	Item* item = thing->getItem();
	if(item == NULL){
#ifdef __DEBUG__MOVESYS__
		std::cout << "Failure: [Player::__internalAddThing] item == NULL" << std::endl;
#endif
		return;
	}
		
	//index == 0 means we should equip this item at the most appropiate slot
	if(index == 0){
#ifdef __DEBUG__MOVESYS__
		std::cout << "Failure: [Player::__internalAddThing] index == 0" << std::endl;
		DEBUG_REPORT
#endif
		return;
	}

	if(index > 0 && index < 11){
		if(items[index]){
#ifdef __DEBUG__MOVESYS__
			std::cout << "Warning: [Player::__internalAddThing], player: " << getName() << ", items[index] is not empty." << std::endl;
			//DEBUG_REPORT
#endif
			return;
		}

		items[index] = item;
		item->setParent(this);
  }
}

void Player::setAttackedCreature(const Creature* creature)
{
	Creature::setAttackedCreature(creature);

	if(chaseMode == CHASEMODE_FOLLOW && creature){
		if(followCreature != creature){
			//chase opponent
			g_game.internalFollowCreature(this, creature);
		}
	}
	else{
		setFollowCreature(NULL);
	}
}

void Player::setFollowCreature(const Creature* creature)
{
	if(followCreature != creature){
		followCreature = creature;

		if(!followCreature){
			stopAutoWalk();
		}
	}
}

void Player::setChaseMode(uint8_t mode)
{
	chaseMode_t prevChaseMode = chaseMode;

	if(mode == 1){
		chaseMode = CHASEMODE_FOLLOW;
	}
	else{
		chaseMode = CHASEMODE_STANDSTILL;
	}
	
	if(prevChaseMode != chaseMode){
		if(chaseMode == CHASEMODE_FOLLOW){
			if(!followCreature && getAttackedCreature()){
				//chase opponent
				g_game.internalFollowCreature(this, getAttackedCreature());
			}
		}
		else if(getAttackedCreature()){
			setFollowCreature(NULL);
			stopAutoWalk();
		}
	}
}

bool Player::startAutoWalk(std::list<Direction>& listDir)
{
	if(eventAutoWalk == 0){
		/*start a new event*/
		listWalkDir = listDir;
		return addEventAutoWalk();
	}
	else{
		/*event already running*/
		listWalkDir = listDir;
	}

	return true;
}

bool Player::addEventAutoWalk()
{
	if(isRemoved()){
		eventAutoWalk = 0;
		return false;
	}

	int ticks = getEventStepTicks();
	eventAutoWalk = g_game.addEvent(makeTask(ticks, std::bind2nd(std::mem_fun(&Game::checkAutoWalkPlayer), getID())));
	return true;
}

bool Player::stopAutoWalk()
{
	if(eventAutoWalk != 0){
		g_game.stopEvent(eventAutoWalk);
		eventAutoWalk = 0;

		if(!listWalkDir.empty()){
			listWalkDir.clear();
			sendCancelWalk();
		}
	}

	return true;
}

bool Player::checkStopAutoWalk(bool pathInvalid /*= false*/)
{
	if(followCreature){
		if(pathInvalid || chaseMode == CHASEMODE_FOLLOW){
			if(g_game.internalFollowCreature(this, followCreature)){
				return false;
			}
		}
	}

	setFollowCreature(NULL);
	stopAutoWalk();
	return true;
}

void Player::getCreatureLight(LightInfo& light) const
{
	if(internalLight.level > itemsLight.level){
		light = internalLight;
	}
	else{
		light = itemsLight;
	}
}

void Player::updateItemsLight(bool internal /*=false*/)
{
	LightInfo maxLight;
	LightInfo curLight;
	for(int i = SLOT_FIRST; i < SLOT_LAST; ++i){
		Item* item = getInventoryItem((slots_t)i);
		if(item){
			item->getLight(curLight);
			if(curLight.level > maxLight.level){
				maxLight = curLight;
			}
		}
	}
	if(itemsLight.level != maxLight.level || itemsLight.color != maxLight.color){
		itemsLight = maxLight;
		if(!internal){
			g_game.changeLight(this);
		}
	}
}

#ifdef __SKULLSYSTEM__
skulls_t Player::getSkull() const
{
	if(access != 0)
		return SKULL_NONE;
		
	return skull;
}

skulls_t Player::getSkullClient(const Player* player) const
{
	skulls_t skull;
	skull = player->getSkull();
	if(skull == SKULL_NONE){
		if(player->hasAttacked(this)){
			skull = SKULL_YELLOW;
		}
	}
	return skull;
}

bool Player::hasAttacked(const Player* attacked) const
{
	if(access != 0)
		return false;
	if(!attacked)
		return false;
	
	AttackedSet::const_iterator it;
	unsigned long attacked_id = attacked->getID();
	it = attackedSet.find(attacked_id);
	if(it != attackedSet.end()){
		return true;
	}
	else{
		return false;
	}
}

void Player::addAttacked(const Player* attacked)
{
	if(access != 0)
		return;
	
	if(!attacked || attacked == this)
		return;
	AttackedSet::iterator it;
	unsigned long attacked_id = attacked->getID();
	it = attackedSet.find(attacked_id);
	if(it == attackedSet.end()){
		attackedSet.insert(attacked_id);
	}
}

void Player::clearAttacked()
{
	attackedSet.clear();
}

void Player::addUnjustifiedDead(const Player* attacked)
{
	if(access != 0 || attacked == this)
		return;
		
	std::stringstream Msg;
	Msg << "Warning! The murder of " << attacked->getName() << " was not justified.";
	client->sendTextMessage(MSG_STATUS_WARNING, Msg.str().c_str());
	redSkullTicks = redSkullTicks + 12 * 3600 * 1000;
	if(redSkullTicks >= 3*24*3600*1000){
		g_game.changeSkull(this, SKULL_RED);
	}
}

void Player::setSkull(skulls_t new_skull)
{
	skull = new_skull;
}

void Player::sendCreatureSkull(const Creature* creature) const
{
	client->sendCreatureSkull(creature);
}

void Player::checkRedSkullTicks(long ticks)
{
	if(redSkullTicks - ticks > 0)
		redSkullTicks = redSkullTicks - ticks;
	
	if(redSkullTicks < 1000 && inFightTicks < 1000 && skull != SKULL_NONE){
		g_game.changeSkull(this, SKULL_NONE);
	}
}
#endif
