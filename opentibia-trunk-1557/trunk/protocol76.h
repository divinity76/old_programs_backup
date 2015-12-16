//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
// The protocoll for the clients version 7.6
// the newer clients still use the same protocoll just with a 
// prequel defined in the 6.5 protocoll
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


#ifndef __OTSERV_PROTOCOL76_H__
#define __OTSERV_PROTOCOL76_H__

#include <string>
#include "protocol.h"

class NetworkMessage;
class Creature;

class Protocol76 : public Protocol
{
public:
	Protocol76(SOCKET s);
	virtual ~Protocol76();
	
	connectResult_t ConnectPlayer();
	void ReceiveLoop();  
	void WriteBuffer(NetworkMessage &add);
	virtual void reinitializeProtocol();
	
private:
	NetworkMessage OutputBuffer;
	std::list<unsigned long> knownPlayers;
	void checkCreatureAsKnown(unsigned long id, bool &known, unsigned long &removedKnown);
	
	virtual bool CanSee(int x, int y, int z) const;
	virtual bool CanSee(const Creature*) const;
	virtual bool CanSee(const Position& pos) const;
	virtual void logout();
	
	void flushOutputBuffer();
	void WriteMsg(NetworkMessage& msg);

	// we have all the parse methods
	void parsePacket(NetworkMessage& msg);
	
	//Parse methods
	void parseLogout(NetworkMessage& msg);	
	void parseCancelMove(NetworkMessage& msg);

	void parseReceivePing(NetworkMessage& msg);
	void parseAutoWalk(NetworkMessage& msg);
	void parseStopAutoWalk(NetworkMessage& msg);	
	void parseMoveNorth(NetworkMessage& msg);
	void parseMoveEast(NetworkMessage& msg);
	void parseMoveSouth(NetworkMessage& msg);
	void parseMoveWest(NetworkMessage& msg);
	void parseMoveNorthEast(NetworkMessage& msg);
	void parseMoveSouthEast(NetworkMessage& msg);
	void parseMoveSouthWest(NetworkMessage& msg);
	void parseMoveNorthWest(NetworkMessage& msg);
	
	void parseTurnNorth(NetworkMessage& msg);
	void parseTurnEast(NetworkMessage& msg);
	void parseTurnSouth(NetworkMessage& msg);
	void parseTurnWest(NetworkMessage& msg);
	
	void parseRequestOutfit(NetworkMessage& msg);
	void parseSetOutfit(NetworkMessage& msg);
	void parseSay(NetworkMessage& msg);
	void parseLookAt(NetworkMessage& msg);
	void parseFightModes(NetworkMessage& msg);
	void parseAttack(NetworkMessage& msg);
	void parseFollow(NetworkMessage& msg);
	
	void parseThrow(NetworkMessage& msg);
	void parseUseItemEx(NetworkMessage& msg);
	void parseBattleWindow(NetworkMessage& msg);
	void parseUseItem(NetworkMessage& msg);
	void parseCloseContainer(NetworkMessage& msg);
	void parseUpArrowContainer(NetworkMessage& msg);
	void parseUpdateContainer(NetworkMessage& msg);
	void parseTextWindow(NetworkMessage& msg);
	void parseHouseWindow(NetworkMessage& msg);
	
	//trade methods
	void parseRequestTrade(NetworkMessage& msg);
	void parseLookInTrade(NetworkMessage& msg);
	void parseAcceptTrade(NetworkMessage& msg);
	void parseCloseTrade();
	
	//VIP methods
	void parseAddVip(NetworkMessage& msg);
	void parseRemVip(NetworkMessage& msg);

	void parseRotateItem(NetworkMessage& msg);
		
	//Channel tabs
	void parseCreatePrivateChannel(NetworkMessage& msg);
	void parseChannelInvite(NetworkMessage& msg);
	void parseChannelExclude(NetworkMessage& msg);
	
	void parseGetChannels(NetworkMessage& msg);
	void parseOpenChannel(NetworkMessage& msg);
	void parseOpenPriv(NetworkMessage& msg);
	void parseCloseChannel(NetworkMessage& msg);

	void parseDebug(NetworkMessage& msg);

	//Send functions
	virtual void sendClosePrivate(unsigned short channelId);
	virtual void sendChannelsDialog();
	virtual void sendChannel(unsigned short channelId, std::string channelName);
	virtual void sendOpenPriv(const std::string& receiver);
	virtual void sendToChannel(const Creature *creature, SpeakClasses type, const std::string &text, unsigned short channelId);
	
	virtual void sendIcons(int icons);

	virtual void sendDistanceShoot(const Position &from, const Position &to, unsigned char type);
	virtual void sendMagicEffect(const Position &pos, unsigned char type);
	virtual void sendAnimatedText(const Position &pos, unsigned char color, std::string text);
	virtual void sendCreatureHealth(const Creature *creature);
	virtual void sendSkills();
	virtual void sendPing();
	virtual void sendCreatureTurn(const Creature *creature, unsigned char stackpos);
	virtual void sendCreatureSay(const Creature *creature, SpeakClasses type, const std::string &text);
	
	virtual void sendCancel(const char *msg);
	virtual void sendCancelWalk();
	virtual void sendChangeSpeed(const Creature* creature);
	virtual void sendCancelTarget();
	virtual void sendSetOutfit(const Creature* creature);
	virtual void sendStats();
	virtual void sendTextMessage(MessageClasses mclass, const char* message);
	virtual void sendTextMessage(MessageClasses mclass, const char* message,const Position &pos, unsigned char type);
	
	virtual void sendTradeItemRequest(const Player* player, const Item* item, bool ack);
	virtual void sendCloseTrade();
	
	virtual void sendTextWindow(Item* item,const unsigned short maxlen, const bool canWrite);
	virtual void sendHouseWindow(House* house, unsigned long listid, const std::string& text);
	
	virtual void sendVIPLogIn(unsigned long guid);
	virtual void sendVIPLogOut(unsigned long guid);
	virtual void sendVIP(unsigned long guid, const std::string &name, bool isOnline);
	
	virtual void sendCreatureLight(const Creature* creature);
	virtual void sendWorldLight(const LightInfo& lightInfo);
	
	virtual void sendCreatureSkull(const Creature* creature);
	virtual void sendCreatureShield(const Creature* creature);
	virtual void sendCreatureSquare(const Creature* creature, SquareColor color);
	
	//tiles
	virtual void sendAddTileItem(const Position& pos, const Item* item);
	virtual void sendUpdateTileItem(const Position& pos, uint32_t stackpos, const Item* item);
	virtual void sendRemoveTileItem(const Position& pos, uint32_t stackpos);
	virtual void UpdateTile(const Position& pos);

	virtual void sendAddCreature(const Creature* creature, bool isLogin);
	virtual void sendRemoveCreature(const Creature* creature, const Position& pos, uint32_t stackpos, bool isLogout);
	virtual void sendMoveCreature(const Creature* creature, const Position& oldPos, uint32_t oldStackPos, bool teleport);
	//virtual void sendTeleportCreature(const Creature* creature, const Position& oldPos, uint32_t oldStackPos);

	//containers
	void sendAddContainerItem(uint8_t cid, const Item* item);
	void sendUpdateContainerItem(uint8_t cid, uint8_t slot, const Item* item);
	void sendRemoveContainerItem(uint8_t cid, uint8_t slot);

	virtual void sendContainer(uint32_t cid, const Container* container, bool hasParent);
	virtual void sendCloseContainer(uint32_t cid);

	//inventory
	virtual void sendAddInventoryItem(slots_t slot, const Item* item);
	virtual void sendUpdateInventoryItem(slots_t slot, const Item* item);
	virtual void sendRemoveInventoryItem(slots_t slot);

	//Help functions

	// translate a tile to clientreadable format
	void GetTileDescription(const Tile* tile, NetworkMessage &msg);
	
	// translate a floor to clientreadable format
	void GetFloorDescription(NetworkMessage& msg, int x, int y, int z, int width, int height, int offset, int& skip);

	// translate a map area to clientreadable format
	void GetMapDescription(unsigned short x, unsigned short y, unsigned char z,
		unsigned short width, unsigned short height,
		NetworkMessage &msg);

	void AddMapDescription(NetworkMessage& msg, const Position& pos);
	void AddTextMessage(NetworkMessage &msg,MessageClasses mclass, const char* message);
	void AddAnimatedText(NetworkMessage &msg,const Position &pos, unsigned char color, std::string text);
	void AddMagicEffect(NetworkMessage &msg,const Position &pos, unsigned char type);
	void AddDistanceShoot(NetworkMessage &msg,const Position &from, const Position &to, unsigned char type);
	void AddCreature(NetworkMessage &msg,const Creature *creature, bool known, unsigned int remove);
	void AddPlayerStats(NetworkMessage &msg);
	void AddCreatureSpeak(NetworkMessage &msg,const Creature *creature, SpeakClasses type, std::string text, unsigned short channelId);
	void AddCreatureHealth(NetworkMessage &msg,const Creature *creature);
	void AddPlayerSkills(NetworkMessage &msg);
	void AddWorldLight(NetworkMessage &msg, const LightInfo& lightInfo);
	void AddCreatureLight(NetworkMessage &msg, const Creature* creature);

	//tiles
	void AddTileItem(NetworkMessage& msg, const Position& pos, const Item* item);
	void AddTileCreature(NetworkMessage& msg, const Position& pos, const Creature* creature);
	void UpdateTileItem(NetworkMessage& msg, const Position& pos, uint32_t stackpos, const Item* item);
	void RemoveTileItem(NetworkMessage& msg, const Position& pos, uint32_t stackpos);
	void UpdateTile(NetworkMessage& msg, const Position& pos);

	void MoveUpCreature(NetworkMessage& msg, const Creature* creature,
		const Position& newPos, const Position& oldPos, uint32_t oldStackPos);
	void MoveDownCreature(NetworkMessage& msg, const Creature* creature,
		const Position& newPos, const Position& oldPos, uint32_t oldStackPos);

	//container
	void AddContainerItem(NetworkMessage& msg, uint8_t cid, const Item *item);
	void UpdateContainerItem(NetworkMessage& msg, uint8_t cid, uint8_t slot, const Item* item);
	void RemoveContainerItem(NetworkMessage& msg, uint8_t cid, uint8_t slot);
	
	//inventory
	void AddInventoryItem(NetworkMessage& msg, slots_t slot, const Item* item);
	void UpdateInventoryItem(NetworkMessage& msg, slots_t slot, const Item* item);
	void RemoveInventoryItem(NetworkMessage& msg, slots_t slot);

	OTSYS_THREAD_LOCKVAR bufferLock;
	unsigned long windowTextID;
	Item* readItem;
	int maxTextLenght;
	
	House* house;
	unsigned long listId;
	
	friend OTSYS_THREAD_RETURN ConnectionHandler(void *dat);
};

#endif
