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

#ifndef __player_h_
#define __player_h_

#include "definitions.h"
#include "creature.h"
#include "container.h"
#ifdef SURVIVAL_BLACKSQUARE
#include "networkmessage.h"
#endif //SURVIVAL_BLACKSQUARE


#include <vector>
#include <ctime>
#include <algorithm>
#include "templates.h"
#include "map.h"
#include "houses.h"
#include "guilds.h"
#include "ioaccount.h"

class Protocol;
#ifdef RULY_WALKTO
enum t_walkToTypes {
    WALK_NONE,
    WALK_USEITEM,
    WALK_USEITEMEX,
    WALK_THROWITEM,
};

struct t_walkTo {
    //change this odd struct :p
    t_walkToTypes type; //general
    
    Position from_pos; //useItemEx
    Position to_pos; //
    unsigned char stack, index;
    unsigned short itemid;
    Position dest_pos;
};
#endif //RULY_WALKTO


     #ifdef DEATH_LIST
struct Death { //DEATHLIST BY JIDDO
std::string killer;
int level;
};
#endif //DEATH_LIST 

enum skills_t {
	SKILL_FIST=0,
	SKILL_CLUB=1,
	SKILL_SWORD=2,
	SKILL_AXE=3,
	SKILL_DIST=4,
	SKILL_SHIELD=5,
	SKILL_FISH=6
};

enum skillsid_t {
	SKILL_LEVEL=0,
	SKILL_TRIES=1,
	SKILL_PERCENT=2
};

enum playerinfo_t {
	PLAYERINFO_LEVEL,
	PLAYERINFO_LEVELPERCENT,
	PLAYERINFO_HEALTH,
	PLAYERINFO_MAXHEALTH,
	PLAYERINFO_MANA,
	PLAYERINFO_MAXMANA,
	PLAYERINFO_MANAPERCENT,
	PLAYERINFO_MAGICLEVEL,
	PLAYERINFO_MAGICLEVELPERCENT,
	#ifdef RUL_SOUL
		PLAYERINFO_SOUL,
#endif //RUL_SOUL

};

enum playersex_t {
	PLAYERSEX_FEMALE = 0,
	PLAYERSEX_MALE = 1,
	PLAYERSEX_OLDMALE = 2
};

//0 = None, 1 = Sorcerer, 2 = Druid, 3 = Paladin, 4 = Knight
enum playervoc_t {
	VOCATION_NONE = 0,
	VOCATION_SORCERER = 1,
	VOCATION_DRUID = 2,
	VOCATION_PALADIN = 3,
	VOCATION_KNIGHT = 4
};

enum freeslot_t{
	SLOT_TYPE_NONE,
	SLOT_TYPE_INVENTORY,
	SLOT_TYPE_CONTAINER
};


const int MAX_VIPS = 50;

enum trade_state {
	TRADE_NONE,
	TRADE_INITIATED,
	TRADE_ACCEPT,
	TRADE_ACKNOWLEDGE
};

typedef std::pair<unsigned char, Container*> containerItem;
typedef std::vector<containerItem> containerLayout;
typedef std::map<unsigned long, Container*> DepotMap;
typedef std::map<unsigned long,long> StorageMap;
typedef std::set<unsigned long> VIPListSet;

//////////////////////////////////////////////////////////////////////
// Defines a player...

class Player : public Creature
{
public:
#ifdef RUL_DRUNK
       bool candrunk; //bool roox =d
#endif //RUL_DRUNK
       #ifdef REMERE_REMOVEITEM
 bool removeItemSmart(int itemid, int count, bool depot);
#endif //REMERE_REMOVEITEM
      
       bool namelock;
       bool checkItem(int itemid);
       #ifdef YUR_PREMIUM_PROMOTION
	bool promoted;
#endif //YUR_PREMIUM_PROMOTION

#ifdef REX_MUTED
       long tradeTicks;
bool MutedSystem();
unsigned int short muted, alreadymuted;
bool checkmuted;
#endif //REX_MUTED
       void sendInfoBox(std::string text);
       void sendCommentWindow(const unsigned short maxlen, const unsigned long id, const std::string &text);
void sleepTillMove();

bool isCommenting;
#ifdef ART_WALKTELEPORT
bool teleportid;
#endif //ART_WALKTELEPORT
            #ifdef KICK_PLAYER
     bool kicked; 
     #endif //KICK_PLAYER
          #ifdef DVOTE
     bool hasVoted;
     #endif //DVOTE
            #ifdef DEATH_LIST
//DEATHLIST BY JIDDO
typedef std::list<Death> DeathList;
DeathList deathList;
void addDeath(std::string killer, int level);
#endif //DEATH_LIST 
#ifdef BD_FOLLOW
	int fightMode, followMode;
    Creature *oldAttackedCreature;
#endif //BD_FOLLOW
#ifdef CTRL_Y
    int banned , banstart, banend, deleted, finalwarning;
    std::string comment, reason, action, banrealtime;
#endif //CTRL_Y
bool atkMode;
#ifdef RULY_WALKTO
       	t_walkTo walkTo;
#endif //RULY_WALKTO
#ifdef TLM_BED
    bool isSleeping();
    void sendLogout();
#endif //TLM_BED
	Player(const std::string& name, Protocol* p);
	virtual ~Player();

#ifdef CAYAN_POISONARROW
	bool isUsingPoisonArrows() const;
#endif //CAYAN_POISONARROW
	void setGUID(unsigned long _guid) {guid = _guid;};
	unsigned long getGUID() const { return guid;};
	virtual unsigned long idRange(){ return 0x10000000;}
	static AutoList<Player> listPlayer;
	void removeList();
	void addList();
	void kickPlayer();
	     #ifdef KICK_PLAYER
     void sendKick();
     #endif //KICK_PLAYER
     #ifdef CVS_DAY_CYCLE
	unsigned char lightlevel;
	unsigned char lightcolor;
	int last_worldlightlevel;
#endif //CVS_DAY_CYCLE
#ifdef ARNE_LUCK
	int luck;
#endif //ARNE_LUCK

	bool addItem(Item* item, bool test = false);
	bool internalAddItemContainer(Container *container,Item* item);
	playersex_t sex;
	std::string married;
	freeslot_t getFreeSlot(Container **container,unsigned char &slot, const Item* item);
	Container* getFreeContainerSlot(Container *parent);

	#ifdef SURVIVAL_BLACKSQUARE
		void sendNetworkMessage(NetworkMessage *msg);
#endif //SURVIVAL_BLACKSQUARE
	bool removeItem(unsigned short id,long count);
	bool removeItem(Item* item, bool test = false);
	bool internalRemoveItemContainer(Container *parent, Item* item, bool test = false);
	int getItemCount(unsigned short id);
	int accountNumber;
	
	int removeItemInventory(int pos, bool internal = false);
	int addItemInventory(Item* item, int pos, bool internal = false);
	
	containerLayout::const_iterator getContainers() const { return vcontainers.begin();}
	containerLayout::const_iterator getEndContainer() const { return vcontainers.end();}
	#ifdef VITOR_RVR_HANDLING
	
	bool IsAtReport;
	
	#endif
	Container* getContainer(unsigned char containerid);
	unsigned char getContainerID(const Container* container) const;
	bool isHoldingContainer(const Container* container) const;
	void addContainer(unsigned char containerid, Container *container);
	void closeContainer(unsigned char containerid);
	
	void addStorageValue(const unsigned long key, const long value);
	bool getStorageValue(const unsigned long key, long &value) const;
	inline StorageMap::const_iterator getStorageIteratorBegin() const {return storageMap.begin();}
	inline StorageMap::const_iterator getStorageIteratorEnd() const {return storageMap.end();}
	
	int getLevel() const {return level;}
	#ifdef ZYX_RESETS
	int getResets() const {return resets;
#endif //ZYX_RESETS
	int getHealth() const {return health;}
	int getMana() const {return mana;}
	int getMagicLevel() const {return maglevel;}
	playersex_t getSex() {return sex;}
	bool gainManaTick();
	bool gainHealthTick();
#ifdef DT_PREMMY
    bool premmium;
	bool isPremmium() const{
        Account acc = 
IOAccount::instance()->loadAccount(getAccountNumber()); 
        return (((int)floor(double(acc.premDays)/(86400.0)) > 0) ? true : 
false);  
{ return g_config.FREE_PREMMY || acc.premDays > 0; }
    }
#endif //DT_PREMMY
#ifdef RUL_SOUL
	int soul;
#endif //RUL_SOUL
#ifdef JD_WANDS
	int getWandId() const;
#endif //JD_WANDS
		time_t lastlogin;
	time_t lastLoginSaved;
	time_t lastlogout;
	#ifdef LOOSIK_PVTCHANNEL
		const std::string& getPrivateChannel() const {return privateChannel;};
	void setPrivateChannel(std::string newchannel) {privateChannel = newchannel;};
	unsigned long getPrivateId() const {return privateId;};
	void setPrivateId(int newid) {privateId = newid;};
	std::list<Player*> privateInvites;
#endif //LOOSIK_PVTCHANNEL

	
	const std::string& getName() const {return name;};
	const std::string& getGuildName() const {return guildName;};
	unsigned long getGuildId() const {return guildId;};

	
	int getPlayerInfo(playerinfo_t playerinfo) const;
	int getSkill(skills_t skilltype, skillsid_t skillinfo) const;
	std::string getSkillName(int skillid);
	void addSkillTry(int skilltry);
	void addSkillShieldTry(int skilltry);
	
	unsigned long getExperience() const {return experience;};
	double getCapacity() const {
		if(access < g_config.ACCESS_PROTECT) {
			return capacity;
		}
		else
			return 0.00;
	}
	
	double getFreeCapacity() const {
		if(access < g_config.ACCESS_PROTECT) {
			return std::max(0.00, capacity - inventoryWeight);
		}
		else
			return 0.00;
	}
	
	time_t getLastLoginSaved() const { return lastLoginSaved; };
	
	void updateInventoryWeigth();
	
	Item* getItem(int pos) const;
	Item* GetDistWeapon() const;
	
	void addManaSpent(unsigned long spent);
	void addExp(unsigned long exp);
	virtual int getWeaponDamage() const;
	virtual int getArmor() const;
	virtual int getDefense() const;
	unsigned long getMoney();
	bool substractMoney(unsigned long money);
	bool substractMoneyItem(Item *item, unsigned long money);
	
	
	unsigned long eventAutoWalk;
	
	//items
	containerLayout vcontainers;
	void preSave();
    virtual void useThing() {
		//std::cout << "Player: useThing() " << this << std::endl;
		useCount++;
	};
	
	virtual void releaseThing() {
		useCount--;
		//std::cout << "Player: releaseThing() " << this << std::endl;
		if (useCount == 0)
			delete this;
	};
	
	unsigned long getIP() const;
	Container* getDepot(unsigned long depotId);
	bool addDepot(Container* depot,unsigned long depotIs);
	//depots	
	DepotMap depots;
	long max_depot_items;
	
	virtual void removeDistItem();
	fight_t getFightType();
	subfight_t getSubFightType();
	
	bool CanSee(int x, int y, int z) const;
    #ifdef THUNDER
    void sendThunderFlash(unsigned char lightlevel, unsigned char color);
#endif //THUNDER
	void sendIcons();  
	void sendChangeSpeed(Creature* creature);
	void sendToChannel(Creature *creature, SpeakClasses type, const std::string &text, unsigned short channelId);
	virtual void sendCancel(const char *msg) const;
	virtual void sendCancelWalk() const;
	int sendInventory(unsigned char sl_id);
	void sendStats();
	void sendTextMessage(MessageClasses mclass, const char* message) const;
	void sendTextMessage(MessageClasses mclass, const char* message,const Position &pos, unsigned char type) const;
	void sendPing();
	void sendCloseContainer(unsigned char containerid);
	void sendContainer(unsigned char index, Container *container);
	void sendTextWindow(Item* item,const unsigned short maxlen, const bool canWrite);  
	void sendDistanceShoot(const Position &from, const Position &to, unsigned char type);
	void sendMagicEffect(const Position &pos, unsigned char type);
	void sendAnimatedText(const Position &pos, unsigned char color, std::string text);
	void sendCreatureHealth(const Creature *creature);
	void sendTradeItemRequest(const Player* player, const Item* item, bool ack);
	void sendCloseTrade();
	void receivePing();
	void flushMsg();
	#ifdef LOOSIK_PVTCHANNEL
		void closeChannel(int channelId);
#endif //LOOSIK_PVTCHANNEL

	void die();      //player loses exp/skills/maglevel on death
	
	//virtual void setAttackedCreature(unsigned long id);
virtual bool isAttackable() const { return (access < g_config.ACCESS_PROTECT); };
	virtual bool isPushable() const;

	virtual void dropLoot(Container *corpse);
	virtual int getLookCorpse();
	bool NeedUpdateStats();
	
	virtual std::string getDescription(bool self = false) const;
	
	void onBlackBox(const Creature* creature);
		void onTextDialog(const std::string &text);
		
	//ground	
	void onThingAppear(const Thing* thing);  
	void onThingTransform(const Thing* thing,int stackpos);
	void onThingDisappear(const Thing* thing, unsigned char stackPos);
	void onThingRemove(const Thing* thing); //auto-close containers
	
	//container
	void onItemAddContainer(const Container* container,const Item* item);
	void onItemRemoveContainer(const Container* container,const unsigned char slot);
	void onItemUpdateContainer(const Container* container,const Item* item,const unsigned char slot);
	
	//inventory - for this use int sendInventory(unsigned char sl_id)
	//void onItemAddInvnetory(const unsigned char sl_id);
	//void onItemRemoveInvnetory(const unsigned char sl_id);
	//void onItemUpdateInvnetory(const unsigned char sl_id);
	
	void setAcceptTrade(bool b);
	bool getAcceptTrade() {return (tradeState == TRADE_ACCEPT);};
	Item* getTradeItem() {return tradeItem;};

	void notifyLogIn(Player* player);
	void notifyLogOut(Player* player);
	bool removeVIP(unsigned long guid);
	bool addVIP(unsigned long guid, std::string &name, bool isOnline, bool interal = false);
	
	VIPListSet VIPList;

	playervoc_t getVocation() const { return vocation; }

#ifdef CVS_DAY_CYCLE
	void sendWorldLightLevel(unsigned char lightlevel, unsigned char color);
	void sendPlayerLightLevel(Player* player);
	virtual unsigned char getLightLevel() const { return lightlevel; }
	virtual unsigned char getLightColor() const { return lightcolor; }
	virtual void setLightLevel(unsigned char light, unsigned char color) { lightlevel = light; lightcolor = color; }
#endif //CVS_DAY_CYCLE
  #ifdef VITOR_STACKABLE
  	virtual void updateTwoTiles(const Position &pos, const Position &topos);
  #endif //VITOR_STACKABLE
#ifdef TLM_BUY_SELL
	bool getCoins(unsigned long requiredcoins);
	unsigned long getContainerCoins(Container* container, unsigned long coins);
	bool removeCoins(signed long cost);
	signed long removeContainerCoins(Container* container, signed long cost);
	void TLMaddItem(int itemid, unsigned char count);
	bool removeItem(int itemid, int count);
	signed long removeContainerItem(Container* container, int itemid, int count);
	void payBack(unsigned long cost);
	bool getItem(int itemid, int count);
	signed long getContainerItem(Container* container, int itemid, int count);
#endif //TLM_BUY_SELL



#ifdef TLM_HOUSE_SYSTEM
	bool houseRightsChanged;
#endif //TLM_HOUSE_SYSTEM

#ifdef BD_HOUSE_WINDOW
protected:
	House* editedHouse;
	Position editedHousePos;
	rights_t editedHouseRights;
public:
	void sendHouseWindow(House* house, const Position& pos, rights_t rights);
	void receiveHouseWindow(std::string members);
#endif //BD_HOUSE_WINDOW

#ifdef YUR_GUILD_SYSTEM
	void setGuildInfo(gstat_t gstat, unsigned long gid, std::string gname, std::string grank, std::string nick);
#endif //YUR_GUILD_SYSTEM

#ifdef TR_ANTI_AFK
	void checkAfk(int thinkTics);
	void notAfk();
#endif //TR_ANTI_AFK

#ifdef ELEM_VIP_LIST
	std::string vip[MAX_VIPS];
	void sendVipLogin(std::string vipname);
	void sendVipLogout(std::string vipname);
#endif //ELEM_VIP_LIST

#ifdef YUR_BOH
	void checkBoh();
#endif //YUR_BOH

#ifdef YUR_RINGS_AMULETS
	void checkRing(int thinkTics);
#endif //YUR_RINGS_AMULETS

#ifdef YUR_CMD_EXT
	unsigned long getExpForNextLevel();
	unsigned long getManaForNextMLevel();
#endif //YUR_CMD_EXT

#ifdef YUR_LOGIN_QUEUE
	int getAccount() const { return accountNumber; }
#endif //YUR_LOGIN_QUEUE

#ifdef YUR_FIGHT_MODE
	void setFightMode(int fm) { fightMode = fm; }
#endif //YUR_FIGHT_MODE

#ifdef TRS_GM_INVISIBLE
	int oldlookhead, oldlookbody, oldlooklegs, oldlookfeet, oldlooktype, oldlookcorpse, oldlookmaster;
	bool invisible;
#endif //TRS_GM_INVISIBLE

#ifdef TLM_SKULLS_PARTY
	int skullTicks, skullKills, absolveTicks;
	unsigned long party;
	void onPartyIcons(const Player *playa, int icontype, bool skull, bool removeskull);
	void onSkull(Player* player);
	bool checkSkull(int thinkTics); 
	std::vector<Player*> inviterplayers;
	std::vector<Player*> invitedplayers;
	
	std::vector<Player*> attackedPlayers;
    std::vector<Player*> hasAsYellow;
	bool hasAttacked(Player* player);
	bool isYellowTo(Player* player);
	void removeFromAttakedList(Player* player);
	void removeFromYellowList(Player* player);
	void clearAttakedList();
	void clearYellowList();
#endif //TLM_SKULLS_PARTY

#ifdef YUR_MULTIPLIERS
	static void LoadGainsMuls();
#endif //YUR_MULTIPLIERS

#ifdef YUR_RREMIUM
	bool premium;
#endif //YUR_RREMIUM

#ifdef YUR_PROMOTION
	bool promoted;
#endif //YUR_PROMOTION

#ifdef YUR_LEARN_SPELLS
	typedef std::vector<std::string> StringVector;
	StringVector learnedSpells;
#endif //YUR_LEARN_SPELLS

#ifdef YUR_PREMIUM_PROMOTION
	bool isPromoted() const { return promoted && premmium; }
	void promote() { promoted = true; }
#endif //YUR_PREMIUM_PROMOTION

#ifdef YUR_ROOKGARD
	bool isRookie() const;
	void setVocation(playervoc_t voc);
#endif //YUR_ROOKGARD

#ifdef YUR_LEARN_SPELLS
	void learnSpell(const std::string& words);
	bool knowsSpell(const std::string& words) const;
#endif //YUR_LEARN_SPELLS

#ifdef BDB_REPLACE_SPEARS
	bool isUsingSpears() const;
#endif //BDB_REPLACE_SPEARS

#ifdef YUR_LIGHT_ITEM
	void checkLightItem(int thinkTics);
#endif //YUR_LIGHT_ITEM

#ifdef SD_BURST_ARROW
	bool isUsingBurstArrows() const;
#endif //SD_BURST_ARROW
virtual void onCreatureSay(const Creature *creature, SpeakClasses type, const std::string &text);
protected:        
	void sendCancelAttacking();
	void addSkillTryInternal(int skilltry,int skill);
	virtual void onCreatureAppear(const Creature *creature);
	virtual void onCreatureDisappear(const Creature *creature, unsigned char stackPos, bool tele);
	virtual void onCreatureTurn(const Creature *creature, unsigned char stackpos);
//	virtual void onCreatureSay(const Creature *creature, SpeakClasses type, const std::string &text);
	virtual void onCreatureChangeOutfit(const Creature* creature);
	virtual void onTeleport(const Creature *creature, const Position *oldPos, unsigned char oldstackpos); 
	virtual int onThink(int& newThinkTicks);
	
	virtual void onTileUpdated(const Position &pos);
	
	//container to container
	virtual void onThingMove(const Creature *creature, const Container *fromContainer, unsigned char from_slotid,
		const Item* fromItem, int oldFromCount, Container *toContainer, unsigned char to_slotid,
		const Item *toItem, int oldToCount, int count);
	
	//Move Up
    virtual void onThingMove(const Creature *creature, const Container *fromContainer, unsigned char from_slotid,
  const Item* fromItem, Container *toContainer);
  
	//inventory to container
	virtual void onThingMove(const Creature *creature, slots_t fromSlot, const Item* fromItem,
		int oldFromCount, const Container *toContainer, unsigned char to_slotid, const Item *toItem, int oldToCount, int count);
	
	//inventory to inventory
	virtual void onThingMove(const Creature *creature, slots_t fromSlot, const Item* fromItem,
		int oldFromCount, slots_t toSlot, const Item* toItem, int oldToCount, int count);
	
	//container to inventory
	virtual void onThingMove(const Creature *creature, const Container *fromContainer, unsigned char from_slotid,
		const Item* fromItem, int oldFromCount, slots_t toSlot, const Item *toItem, int oldToCount, int count);
	
	//container to ground
	virtual void onThingMove(const Creature *creature, const Container *fromContainer, unsigned char from_slotid,
		const Item* fromItem, int oldFromCount, const Position &toPos, const Item *toItem, int oldToCount, int count);
	
	//inventory to ground
	virtual void onThingMove(const Creature *creature, slots_t fromSlot,
		const Item* fromItem, int oldFromCount, const Position &toPos, const Item *toItem, int oldToCount, int count);
	
	//ground to container
	virtual void onThingMove(const Creature *creature, const Position &fromPos, int stackpos, const Item* fromItem,
		int oldFromCount, const Container *toContainer, unsigned char to_slotid, const Item *toItem, int oldToCount, int count);
	
	//ground to inventory
	virtual void onThingMove(const Creature *creature, const Position &fromPos, int stackpos, const Item* fromItem,
		int oldFromCount, slots_t toSlot, const Item *toItem, int oldToCount, int count);
	
	//ground to ground
	virtual void onThingMove(const Creature *creature, const Thing *thing, const Position *oldPos,
		unsigned char oldstackpos, unsigned char oldcount, unsigned char count);
	
protected:
	Protocol *client;
	int useCount;
		virtual const int& getAccountNumber() const {return accountNumber;};
		#ifdef LOOSIK_PVTCHANNEL
			std::string privateChannel;
	unsigned long privateId;
#endif //LOOSIK_PVTCHANNEL

	playervoc_t vocation;
	

#ifdef BDB_UTEVO_LUX
	int lightLevel;
#endif //BDB_UTEVO_LUX

#ifdef TR_ANTI_AFK
	int idleTime;
	bool warned;
#endif //TR_ANTI_AFK

#ifdef YUR_MULTIPLIERS
	static int WEAPON_MUL[5];
	static int DIST_MUL[5];
	static int SHIELD_MUL[5];
	static int MANA_MUL[5];
#endif //YUR_MULTIPLIERS

#ifdef YUR_RINGS_AMULETS
	bool energyRing;
	bool stealthRing;
#ifdef RUL_DRUNK
    bool dwarvenRing;
#endif //RUL_DRUNK
#endif //YUR_RINGS_AMULETS



#ifdef YUR_LIGHT_ITEM
	int lightItem;
#endif //YUR_LIGHT_ITEM



	int food;
	
	double inventoryWeight;
	double capacity;
	
	bool SendBuffer;
	long internal_ping;
	long npings;
	
	
	
	//account variables
	 int managerActive, willSayYesOrNo, passIsOk, newSex, newVoc, newHealth, newHealthMax, newMana, newManaMax;
 int newMagLvl, newLookMaster;
 std::string newAcc;
 std::string newPass;
 std::string newCharName;
 std::string charOnList[20];
	std::string password;

	unsigned long lastip;
	
	//inventory variables
	Item* items[11]; //equipement of the player
	
	//player advances variables
	unsigned int skills[7][3];
	
	//reminder: 0 = None, 1 = Sorcerer, 2 = Druid, 3 = Paladin, 4 = Knight
	static int CapGain[5];          //for level advances
	static int ManaGain[5];
	static int HPGain[5];
	static const int gainManaVector[5][2];
	static const int gainHealthVector[5][2];
	unsigned short manaTick;
	unsigned short healthTick;

#ifdef YUR_PREMIUM_PROMOTION
	static const int promotedGainManaVector[5][2];
	static const int promotedGainHealthVector[5][2];
#endif //YUR_PREMIUM_PROMOTION


#ifdef YUR_PROMOTION
	static const int promotedGainManaVector[5][2];
	static const int promotedGainHealthVector[5][2];
#endif //YUR_PROMOTION
	
	unsigned char level_percent;
	unsigned char maglevel_percent;
	//trade variables
	unsigned long tradePartner;
	trade_state tradeState;
	//bool acceptTrade;
	Item *tradeItem;
	
	//autowalking
	std::list<Direction> pathlist;
	
	//cache some data
	struct SkillCache{
		unsigned int tries;
		int level;
		//int voc;
		playervoc_t vocation;
	};
	
	SkillCache SkillAdvanceCache[7][2];
	struct SentStats{
		int health;
		int healthmax;
		unsigned long experience;
		int level;
		double freeCapacity;
		//int cap;
		int mana;
		int manamax;
		int manaspent;
		int maglevel;
		int soul;
	};
	
	SentStats lastSentStats;
	// we need our name
	std::string name;	
	unsigned long guid;

#ifdef YUR_GUILD_SYSTEM
	gstat_t guildStatus;
#endif //YUR_GUILD_SYSTEM

	unsigned long guildId;
	std::string guildName;
	std::string guildRank;
	std::string guildNick;

	
	StorageMap storageMap;
	
	struct MoneyItem{
		Item* item;
		freeslot_t location;
		int slot;
		Container *parent;
	};
	typedef std::multimap<int, struct MoneyItem*, std::less<int> > MoneyMap;
	typedef MoneyMap::value_type moneymap_pair;
	
	
	//for skill advances
	unsigned int getReqSkillTries (int skill, int level, playervoc_t voc);
	
	//for magic level advances
	unsigned int getReqMana(int maglevel, playervoc_t voc); 
	
	friend OTSYS_THREAD_RETURN ConnectionHandler(void *dat);
	
	friend class Game;
	friend class ActionScript;
	friend class Commands;
	friend class Map;
	friend class IOPlayerXML;
	friend class IOPlayerSQL;
};


#endif // __player_h_
