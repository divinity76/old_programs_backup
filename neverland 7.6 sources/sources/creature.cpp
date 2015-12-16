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


#include "definitions.h"

#include <string>
#include <sstream>
#include <algorithm>

#include "game.h"
#include "creature.h"
#include "tile.h"
#include "nvsystem.h"
#include "luascript.h"

extern LuaScript g_config;

using namespace std;

OTSYS_THREAD_LOCKVAR AutoID::autoIDLock;
unsigned long AutoID::count = 1000;
AutoID::list_type AutoID::list;

extern Game g_game;

Creature::Creature(const std::string& name): access(0)
{
	direction  = NORTH;
	master = NULL;
	
	this->name = name;
	    
	lookhead   = 0;
	lookbody   = 0;
	looklegs   = 0;
	lookfeet   = 0;
	lookmaster = 0;
	looktype   = PLAYER_MALE_1;
	pzLocked = false;
invisibleTicks = 0;

	//lookcorpse = 3128;
	lookcorpse = ITEM_HUMAN_CORPSE;
	chameleonTicks = 0;
	
	health     = 1000;//150;
	healthmax  = 1000;//150;
	experience = 100000;
	lastmove=0;

	inFightTicks = 0;
	inFightTicks = 0;
	manaShieldTicks = 0;
	stealthTicks = 0;
	hasteTicks = 0;	
	paralyzeTicks = 0;
	exhaustedTicks  = 0;
	pzLocked = false;
	immunities = 0;
	eventCheck = 0;
	eventCheckAttacking = 0;
    attackedCreature = 0;
	speed = 220;
	#ifdef RUL_SOUL
		soul = 0;
#endif //RUL_SOUL
	#ifdef RUL_DRUNK
	dwarvenTicks = 0;
	drunkTicks = 0;
#endif //RUL_DRUNK
#ifdef BD_FOLLOW
    eventCheckFollow = 0;
    followCreature = 0;
#endif //BD_FOLLOW	
#ifdef BD_PARALYZE
    hasteSpeed = 0;
#endif //BD_PARALYZE
#ifdef YUR_BOH
	boh = false;
#endif //YUR_BOH
#ifdef YUR_RINGS_AMULETS
	timeRing = false;
#endif //YUR_RINGS_AMULETS
#ifdef TLM_SKULLS_PARTY
	skullType = SKULL_NONE;
#endif //TLM_SKULLS_PARTY
#ifdef EOTSERV_DAMAGESYSTEM
	canattack= false;
	attackTime = 0;
	numberofattacks =  0;
	defenseTime= 0;	
#endif //EOTSERV_DAMAGESYSTEM
#ifdef TJ_MONSTER_BLOOD
	bloodcolor = COLOR_RED; //the damage string
    bloodeffect = EFFECT_RED; //the hiteffect
    bloodsplash = SPLASH_RED; //splash on ground
#endif //TJ_MONSTER_BLOOD
    chameleonTime = 0;
    #ifdef TIJN_UTANAVID
    invisTicks = 0;
	itemID = 0;
	itemid = 99;
    #endif //TIJN_UTANAVID
}

Creature::~Creature()
{
	std::list<Creature*>::iterator cit;
	for(cit = summons.begin(); cit != summons.end(); ++cit) {
		(*cit)->setAttackedCreature(NULL);
		(*cit)->setMaster(NULL);
		(*cit)->releaseThing();
	}
	
	summons.clear();
}

#ifdef YUR_PVP_ARENA
void Creature::drainHealth(int damage, CreatureVector* arenaLosers)
{
	if (arenaLosers && damage >= health)
	{
		health = healthmax;
		arenaLosers->push_back(this);
	}
	else
		health -= min(health, damage);
}
#else
void Creature::drainHealth(int damage)
{
	health -= min(health, damage);
}
#endif //YUR_PVP_ARENA

void Creature::drainMana(int damage)
{
	mana -= min(mana, damage);
}

//void Creature::setAttackedCreature(unsigned long id)
void Creature::setAttackedCreature(const Creature* creature)
{
	std::list<Creature*>::iterator cit;
	for(cit = summons.begin(); cit != summons.end(); ++cit) {
		(*cit)->setAttackedCreature(creature);
	}
	
	if(creature) {
		attackedCreature = creature->getID();
	}
	else
		attackedCreature = 0;
}

void Creature::setMaster(Creature* creature)
{
	//std::cout << "setMaster: " << this << " master=" << creature << std::endl;
	master = creature;
}

void Creature::addSummon(Creature *creature)
{
	//std::cout << "addSummon: " << this << " summon=" << creature << std::endl;
	creature->setMaster(this);
	creature->useThing();
	summons.push_back(creature);
	
}

void Creature::removeSummon(Creature *creature)
{
	//std::cout << "removeSummon: " << this << " summon=" << creature << std::endl;
	std::list<Creature*>::iterator cit = std::find(summons.begin(), summons.end(), creature);
	if(cit != summons.end()) {
		(*cit)->setMaster(NULL);
		(*cit)->releaseThing();
		summons.erase(cit);
	}
}

void Creature::addCondition(const CreatureCondition& condition, bool refresh)
{
	if(condition.getCondition()->attackType == ATTACK_NONE)
		return;
	
	ConditionVec &condVec = conditions[condition.getCondition()->attackType];
	
	if(refresh) {
		condVec.clear();
	}
	
	condVec.push_back(condition);
}

void Creature::addInflictedDamage(Creature* attacker, int damage)
{
	if(damage <= 0)
		return;
	
	unsigned long id = 0;
	if(attacker) {
		id = attacker->getID();
	}
	
	totaldamagelist[id].push_back(make_pair(OTSYS_TIME(), damage));
}
#ifdef RUL_BLESS
int Creature::getLostExperience() {
    Player* player = dynamic_cast<Player*>(this);
if(bless == 1 && player->promoted == 0) {
return (int)std::floor(((float)experience * g_config.DIE_BLESS_1/100.0));
}
else if(bless == 2 && player->promoted == 0) {
return (int)std::floor(((float)experience * g_config.DIE_BLESS_2/100.0));
}
else if(bless == 3 && player->promoted == 0) {
return (int)std::floor(((float)experience * g_config.DIE_BLESS_3/100.0));
}
else if(bless == 4 && player->promoted == 0) {
return (int)std::floor(((float)experience * g_config.DIE_BLESS_4/100.0));
}
else if(bless == 5 && player->promoted == 0) {
return (int)std::floor(((float)experience * g_config.DIE_BLESS_5/100.0));
}
else if(bless == 0 && player->promoted == 0) {
return (int)std::floor(((float)experience * g_config.DIE_BLESS_0/100.0));
} 
else if(bless == 1 && player->promoted == 1) {
return (int)std::floor(((float)experience * g_config.DIE_BLESS_PROMO_1/100.0));
}
else if(bless == 2 && player->promoted == 1) {
return (int)std::floor(((float)experience * g_config.DIE_BLESS__PROMO_2/100.0));
}
else if(bless == 3 && player->promoted == 1) {
return (int)std::floor(((float)experience * g_config.DIE_BLESS__PROMO__3/100.0));
}
else if(bless == 4 && player->promoted == 1) {
return (int)std::floor(((float)experience * g_config.DIE_BLESS__PROMO__4/100.0));
}
else if(bless == 5 && player->promoted == 1) {
return (int)std::floor(((float)experience * g_config.DIE_BLESS__PROMO__5/100.0));
}
else if(bless == 0 && player->promoted == 1) {
return (int)std::floor(((float)experience * g_config.DIE_BLESS__PROMO__0/100.0));
} 
else {
return (int)std::floor(((float)experience * g_config.DIE_BLESS_0/100.0));
} 
bless = 0;
blessa = 0;
blessb = 0;
blessc = 0;
blessd = 0;
blesse = 0;
}
#endif //RUL_BLESS

int Creature::getInflicatedDamage(unsigned long id)
{
	int ret = 0;
	std::map<long, DamageList >::const_iterator tdIt = totaldamagelist.find(id);
	if(tdIt != totaldamagelist.end()) {
		for(DamageList::const_iterator dlIt = tdIt->second.begin(); dlIt != tdIt->second.end(); ++dlIt) {
			ret += dlIt->second;
		}
	}
	
	return ret;
}

int Creature::getInflicatedDamage(Creature* attacker)
{
	unsigned long id = 0;
	if(attacker) {
		id = attacker->getID();
	}
	
	return getInflicatedDamage(id);
}

int Creature::getTotalInflictedDamage()
{
	int ret = 0;
	std::map<long, DamageList >::const_iterator tdIt;
	for(tdIt = totaldamagelist.begin(); tdIt != totaldamagelist.end(); ++tdIt) {
		ret += getInflicatedDamage(tdIt->first);
	}
	
	return ret;
}

int Creature::getGainedExperience(Creature* attacker)
{
	int totaldamage = getTotalInflictedDamage();
	int attackerdamage = getInflicatedDamage(attacker);
	int lostexperience = getLostExperience();
	int gainexperience = 0;
	
	if(attackerdamage > 0 && totaldamage > 0) {
		gainexperience = (int)std::floor(((double)attackerdamage / totaldamage) * lostexperience);
	}

#ifdef YUR_MULTIPLIERS
	static const int EXP_MUL = g_config.getGlobalNumber("expmul",1),
		EXP_MUL_PVP = g_config.getGlobalNumber("expmulpvp",1);

	if (dynamic_cast<Player*>(this))
		return gainexperience * EXP_MUL_PVP;
	else
		return gainexperience * EXP_MUL;

#else
	return gainexperience;

#endif //YUR_MULTIPLIERS
}

std::vector<long> Creature::getInflicatedDamageCreatureList()
{
	std::vector<long> damagelist;	
	std::map<long, DamageList >::const_iterator tdIt;
	for(tdIt = totaldamagelist.begin(); tdIt != totaldamagelist.end(); ++tdIt) {
		damagelist.push_back(tdIt->first);
	}
	
	return damagelist;
}

bool Creature::canMovedTo(const Tile *tile) const
{
	if(tile) {
		return (tile->isBlocking(BLOCK_SOLID) == RET_NOERROR);
	}

	return false;
}

std::string Creature::getDescription(bool self) const
{
	std::stringstream s;
	std::string str;	
	s << "a " << name << "."; 
	str = s.str();
	return str;
}

int Creature::getStepDuration() const
{
	int duration = 500;
	Tile *tile = g_game.getTile(pos.x, pos.y, pos.z);
	if(tile && tile->ground){
		int groundid = tile->ground->getID();
		uint16_t stepspeed = Item::items[groundid].speed;
		if(stepspeed != 0) {
			duration =  (1000 * stepspeed) / (getSpeed() != 0 ? getSpeed() : 220);
		}
	}
	return duration;
};

long long Creature::getSleepTicks() const
{
	long long delay = 0;
	int stepDuration = getStepDuration();
	
	if(lastmove != 0) {
		delay = (((long long)(lastmove)) + ((long long)(stepDuration))) - ((long long)(OTSYS_TIME()));
	}
	
	return delay;
}


#ifdef TR_SUMMONS
bool Creature::isPlayersSummon() const
{
	return master && dynamic_cast<Player*>(master);
}
#endif //TR_SUMMONS

void Creature::removeCondition(attacktype_t attackType)
{
if(attackType == ATTACK_NONE)
return;
ConditionVec &condVec = conditions[attackType];
condVec.clear();
}

