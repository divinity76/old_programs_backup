//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
// Implementation of tibia v8.0 protocol
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
#include "otpch.h"

#include "protocol80.h"
#include "definitions.h"
#include "networkmessage.h"
#include "items.h"
#include "tile.h"
#include "creature.h"
#include "player.h"
#include "chat.h"
#include "configmanager.h"
#include "otsystem.h"
#include "actions.h"
#include "game.h"
#include "ioplayer.h"
#include "house.h"
#include "waitlist.h"
#include "ban.h"
#include "ioaccount.h"
#include "connection.h"

#include <string>
#include <iostream>
#include <sstream>
#include <time.h>
#include <list>

#include <boost/function.hpp>

extern Game g_game;
extern ConfigManager g_config;
extern Actions actions;
extern RSA* g_otservRSA;
extern Ban g_bans;
Chat g_chat;


#ifdef __SERVER_PROTECTION__
#define ADD_TASK_INTERVAL 40
#define CHECK_TASK_INTERVAL 5000
#else
#define ADD_TASK_INTERVAL -1
#endif

// Helping templates to add dispatcher tasks
template<class T1, class f1, class r>
void Protocol80::addGameTask(r (Game::*f)(f1), T1 p1)
{
	if(m_now > m_nextTask || m_messageCount < 5){
		Dispatcher::getDispatcher().addTask(
			createTask(boost::bind(f, &g_game, p1)));

		m_nextTask = m_now + ADD_TASK_INTERVAL;
	}
	else{
		m_rejectCount++;
		//std::cout << "reject task" << std::endl;
	}
}

template<class T1, class T2, class f1, class f2, class r>
void Protocol80::addGameTask(r (Game::*f)(f1, f2), T1 p1, T2 p2)
{
	if(m_now > m_nextTask || m_messageCount < 5){
		Dispatcher::getDispatcher().addTask(
			createTask(boost::bind(f, &g_game, p1, p2)));

		m_nextTask = m_now + ADD_TASK_INTERVAL;
	}
	else{
		m_rejectCount++;
		//std::cout << "reject task" << std::endl;
	}
}

template<class T1, class T2, class T3,
class f1, class f2, class f3,
class r>
void Protocol80::addGameTask(r (Game::*f)(f1, f2, f3), T1 p1, T2 p2, T3 p3)
{
	if(m_now > m_nextTask || m_messageCount < 5){
		Dispatcher::getDispatcher().addTask(
			createTask(boost::bind(f, &g_game, p1, p2, p3)));

		m_nextTask = m_now + ADD_TASK_INTERVAL;
	}
	else{
		m_rejectCount++;
		//std::cout << "reject task" << std::endl;
	}
}

template<class T1, class T2, class T3, class T4,
class f1, class f2, class f3, class f4,
class r>
void Protocol80::addGameTask(r (Game::*f)(f1, f2, f3, f4), T1 p1, T2 p2, T3 p3, T4 p4)
{
	if(m_now > m_nextTask || m_messageCount < 5){
		Dispatcher::getDispatcher().addTask(
			createTask(boost::bind(f, &g_game, p1, p2, p3, p4)));

		m_nextTask = m_now + ADD_TASK_INTERVAL;
	}
	else{
		m_rejectCount++;
		//std::cout << "reject task" << std::endl;
	}
}

template<class T1, class T2, class T3, class T4, class T5,
class f1, class f2, class f3, class f4, class f5,
class r>
void Protocol80::addGameTask(r (Game::*f)(f1, f2, f3, f4, f5), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
{
	if(m_now > m_nextTask || m_messageCount < 5){
		Dispatcher::getDispatcher().addTask(
			createTask(boost::bind(f, &g_game, p1, p2, p3, p4, p5)));

		m_nextTask = m_now + ADD_TASK_INTERVAL;
	}
	else{
		m_rejectCount++;
		//std::cout << "reject task" << std::endl;
	}
}

template<class T1, class T2, class T3, class T4, class T5, class T6,
class f1, class f2, class f3, class f4, class f5, class f6,
class r>
void Protocol80::addGameTask(r (Game::*f)(f1, f2, f3, f4, f5, f6), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
{
	if(m_now > m_nextTask || m_messageCount < 5){
		Dispatcher::getDispatcher().addTask(
			createTask(boost::bind(f, &g_game, p1, p2, p3, p4, p5, p6)));

		m_nextTask = m_now + ADD_TASK_INTERVAL;
	}
	else{
		m_rejectCount++;
		//std::cout << "reject task" << std::endl;
	}
}

template<class T1, class T2, class T3, class T4, class T5, class T6, class T7,
class f1, class f2, class f3, class f4, class f5, class f6, class f7,
class r>
void Protocol80::addGameTask(r (Game::*f)(f1, f2, f3, f4, f5, f6, f7), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
{
	if(m_now > m_nextTask || m_messageCount < 5){
		Dispatcher::getDispatcher().addTask(
			createTask(boost::bind(f, &g_game, p1, p2, p3, p4, p5, p6, p7)));

		m_nextTask = m_now + ADD_TASK_INTERVAL;
	}
	else{
		m_rejectCount++;
		//std::cout << "reject task" << std::endl;
	}
}

template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8,
class f1, class f2, class f3, class f4, class f5, class f6, class f7, class f8,
class r>
void Protocol80::addGameTask(r (Game::*f)(f1, f2, f3, f4, f5, f6, f7, f8), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
{
	if(m_now > m_nextTask || m_messageCount < 5){
		Dispatcher::getDispatcher().addTask(
			createTask(boost::bind(f, &g_game, p1, p2, p3, p4, p5, p6, p7, p8)));

		m_nextTask = m_now + ADD_TASK_INTERVAL;
	}
	else{
		m_rejectCount++;
		//std::cout << "reject task" << std::endl;
	}
}

Protocol80::Protocol80(Connection* connection) :
	Protocol(connection)
{
	player = NULL;
	m_nextTask = 0;
	m_nextPing = 0;
	m_lastTaskCheck = 0;
	m_messageCount = 0;
	m_rejectCount = 0;
	m_debugAssertSent = false;
}

Protocol80::~Protocol80()
{
	player = NULL;
}

void Protocol80::setPlayer(Player* p)
{
	player = p;
}

void Protocol80::deleteProtocolTask()
{
	//dispatcher thread
	if(player){
		#ifdef __DEBUG_NET_DETAIL__
		std::cout << "Deleting Protocol80 - Protocol:" << this << ", Player: " << player << std::endl;
		#endif

		player->client = NULL;

		g_game.FreeThing(player);
		player = NULL;
	}

	Protocol::deleteProtocolTask();
}

bool Protocol80::login(const std::string& name)
{
	//dispatcher thread
	Player* _player = g_game.getPlayerByName(name);
	if(!_player || g_config.getNumber(ConfigManager::ALLOW_CLONES)){
		player = new Player(name, this);
		player->useThing2();
		player->setID();

		if(!IOPlayer::instance()->loadPlayer(player, name)){
#ifdef __DEBUG__
			std::cout << "Protocol80::login - loadPlayer failed - " << name << std::endl;
#endif
			disconnectClient(0x14, "Your character could not be loaded.");
			return false;
		}

		if(g_bans.isPlayerBanished(name) && !player->hasFlag(PlayerFlag_CannotBeBanned)){
			disconnectClient(0x14, "Your character is banished!");
			return false;
		}

		if(g_bans.isAccountBanished(player->getAccount()) && !player->hasFlag(PlayerFlag_CannotBeBanned)){
			disconnectClient(0x14, "Your account is banished!");
			return false;
		}

		if(g_game.getGameState() == GAME_STATE_CLOSED && !player->hasFlag(PlayerFlag_CanAlwaysLogin)){
			disconnectClient(0x14, "Server temporarly closed.");
			return false;
		}

		if(g_config.getNumber(ConfigManager::CHECK_ACCOUNTS) && !player->hasFlag(PlayerFlag_CanAlwaysLogin)
			&& g_game.getPlayerByAccount(player->getAccount())){
			disconnectClient(0x14, "You may only login with one character per account.");
			return false;
		}

		if(!player->isPremium() && !player->hasFlag(PlayerFlag_CanAlwaysLogin) && !Waitlist::instance()->clientLogin(player)){
			int32_t currentSlot = Waitlist::instance()->getClientSlot(player);
			int32_t retryTime = Waitlist::instance()->getTime(currentSlot);
			std::stringstream ss;

			ss << "Too many players online.\n" << "You are at place "
				<< currentSlot << " on the waiting list.";

			OutputMessage* output = OutputMessagePool::getInstance()->getOutputMessage(this, false);
			output->AddByte(0x16);
			output->AddString(ss.str());
			output->AddByte(retryTime);
			OutputMessagePool::getInstance()->send(output);
			getConnection()->closeConnection();
			return false;
		}

		if(!g_game.placePlayer(player, player->getLoginPosition())){
			if(!g_game.placePlayer(player, player->getTemplePosition(), true)){
				disconnectClient(0x14, "Temple position is wrong. Contact the administrator.");
				return false;
			}
		}

		player->lastip = player->getIP();
		player->lastLoginSaved = std::max(time(NULL), player->lastLoginSaved + 1);

		return true;
	}
	else{
		if(_player->isOnline()){
			disconnectClient(0x14, "You are already logged in.");
			return false;
		}

		if(!_player->isRemoved()){
			player = _player;
			player->useThing2();
			player->client = this;
			player->client->sendAddCreature(player, false);
			player->sendIcons();
			player->lastip = player->getIP();
			player->lastLoginSaved = std::max(time(NULL), player->lastLoginSaved + 1);

			return true;
		}
	}

	return false;
}

bool Protocol80::logout(bool forced)
{
	//dispatcher thread
	if(!player)
		return false;

	if(forced || !player->hasCondition(CONDITION_INFIGHT)){
		bool result = g_game.removeCreature(player);
		if(Connection* connection = getConnection()){
			connection->closeConnection();
		}
		return result;
	}
	else{
		player->sendCancelMessage(RET_YOUMAYNOTLOGOUTDURINGAFIGHT);
		return false;
	}
}

bool Protocol80::parseFirstPacket(NetworkMessage& msg)
{
	if(g_game.getGameState() == GAME_STATE_SHUTDOWN){
		getConnection()->closeConnection();
		return false;
	}

	/*uint16_t clientos =*/ msg.GetU16();
	uint16_t version  = msg.GetU16();

	if(!RSA_decrypt(g_otservRSA, msg)){
		getConnection()->closeConnection();
		return false;
	}

	uint32_t key[4];
	key[0] = msg.GetU32();
	key[1] = msg.GetU32();
	key[2] = msg.GetU32();
	key[3] = msg.GetU32();
	enableXTEAEncryption();
	setXTEAKey(key);

	/*uint8_t isSetGM =*/ msg.GetByte();
	uint32_t accnumber = msg.GetU32();
	const std::string name = msg.GetString();
	const std::string password = msg.GetString();

	if(version < CLIENT_VERSION_MIN || version > CLIENT_VERSION_MAX){
		disconnectClient(0x0A, STRING_CLIENT_VERSION);
		return false;
	}

	if(g_game.getGameState() == GAME_STATE_STARTUP){
		disconnectClient(0x14, "Gameworld is starting up. Please wait.");
		return false;
	}

	if(g_bans.isIpDisabled(getIP())){
		disconnectClient(0x14, "Too many connections attempts from this IP. Try again later.");
		return false;
	}

	if(g_bans.isIpBanished(getIP())){
		disconnectClient(0x14, "Your IP is banished!");
		return false;
	}

	std::string acc_pass;
	if(!(IOAccount::instance()->getPassword(accnumber, name, acc_pass) && passwordTest(password,acc_pass))){
		g_bans.addLoginAttempt(getIP(), false);
		getConnection()->closeConnection();
		return false;
	}

	g_bans.addLoginAttempt(getIP(), true);
	Dispatcher::getDispatcher().addTask(
		createTask(boost::bind(&Protocol80::login, this, name)));

	return true;
}

void Protocol80::onRecvFirstMessage(NetworkMessage& msg)
{
	parseFirstPacket(msg);
}

void Protocol80::disconnectClient(uint8_t error, const char* message)
{
	OutputMessage* output = OutputMessagePool::getInstance()->getOutputMessage(this, false);
	output->AddByte(error);
	output->AddString(message);
	OutputMessagePool::getInstance()->send(output);
	getConnection()->closeConnection();
}

void Protocol80::parsePacket(NetworkMessage &msg)
{
	if(msg.getMessageLength() <= 0 || !player)
		return;

	m_now = OTSYS_TIME();

	#ifdef __SERVER_PROTECTION__
	int64_t interval = m_now - m_lastTaskCheck;
	if(interval > CHECK_TASK_INTERVAL){
		interval = 0;
		m_lastTaskCheck = m_now;
		m_messageCount = 1;
		m_rejectCount = 0;
	}
	else{
		m_messageCount++;
		//std::cout << interval/m_messageCount << " " << m_rejectCount << "/" << m_messageCount << std::endl;
		if(/*m_rejectCount > m_messageCount/2 ||*/ (interval > 800 && interval/m_messageCount < 25)){
			getConnection()->closeConnection();
		}
	}
	#endif

	uint8_t recvbyte = msg.GetByte();
	//a dead player can not performs actions
	if((player->isRemoved() || player->getHealth() <= 0) && recvbyte != 0x14){
		return;
	}

	switch(recvbyte){
	case 0x14: // logout
		parseLogout(msg);
		break;

	case 0x1E: // keep alive / ping response
		parseRecievePing(msg);
		break;

	case 0x64: // move with steps
		parseAutoWalk(msg);
		break;

	case 0x65: // move north
		parseMove(msg, NORTH);
		break;

	case 0x66: // move east
		parseMove(msg, EAST);
		break;

	case 0x67: // move south
		parseMove(msg, SOUTH);
		break;

	case 0x68: // move west
		parseMove(msg, WEST);
		break;

	case 0x69: // stop-autowalk
		parseStopAutoWalk(msg);
		break;

	case 0x6A:
		parseMove(msg, NORTHEAST);
		break;

	case 0x6B:
		parseMove(msg, SOUTHEAST);
		break;

	case 0x6C:
		parseMove(msg, SOUTHWEST);
		break;

	case 0x6D:
		parseMove(msg, NORTHWEST);
		break;

	case 0x6F: // turn north
		parseTurn(msg, NORTH);
		break;

	case 0x70: // turn east
		parseTurn(msg, EAST);
		break;

	case 0x71: // turn south
		parseTurn(msg, SOUTH);
		break;

	case 0x72: // turn west
		parseTurn(msg, WEST);
		break;

	case 0x78: // throw item
		parseThrow(msg);
		break;

	case 0x7D: // Request trade
		parseRequestTrade(msg);
		break;

	case 0x7E: // Look at an item in trade
		parseLookInTrade(msg);
		break;

	case 0x7F: // Accept trade
		parseAcceptTrade(msg);
		break;

	case 0x80: // Close/cancel trade
		parseCloseTrade();
		break;

	case 0x82: // use item
		parseUseItem(msg);
		break;

	case 0x83: // use item
		parseUseItemEx(msg);
		break;

	case 0x84: // battle window
		parseBattleWindow(msg);
		break;

	case 0x85:	//rotate item
		parseRotateItem(msg);
		break;

	case 0x87: // close container
		parseCloseContainer(msg);
		break;

	case 0x88: //"up-arrow" - container
		parseUpArrowContainer(msg);
		break;

	case 0x89:
		parseTextWindow(msg);
		break;

	case 0x8A:
		parseHouseWindow(msg);
		break;

	case 0x8C: // throw item
		parseLookAt(msg);
		break;

	case 0x96:  // say something
		parseSay(msg);
		break;

	case 0x97: // request Channels
		parseGetChannels(msg);
		break;

	case 0x98: // open Channel
		parseOpenChannel(msg);
		break;

	case 0x99: // close Channel
		parseCloseChannel(msg);
		break;

	case 0x9A: // open priv
		parseOpenPriv(msg);
		break;

	case 0xA0: // set attack and follow mode
		parseFightModes(msg);
		break;

	case 0xA1: // attack
		parseAttack(msg);
		break;

	case 0xA2: //follow
		parseFollow(msg);
		break;

	case 0xA3:
		parseInviteToParty(msg);
		break;

	case 0xA4:
		parseJoinParty(msg);
		break;

	case 0xA5:
		parseRevokePartyInvitation(msg);
		break;

	case 0xA6:
		parsePassPartyLeadership(msg);
		break;

	case 0xA7:
		parseLeaveParty(msg);
		break;

	case 0xAA:
		parseCreatePrivateChannel(msg);
		break;

	case 0xAB:
		parseChannelInvite(msg);
		break;

	case 0xAC:
		parseChannelExclude(msg);
		break;

	case 0xBE: // cancel move
		parseCancelMove(msg);
		break;

	case 0xC9: //client request to resend the tile
		parseUpdateTile(msg);
		break;

	case 0xCA: //client request to resend the container (happens when you store more than container maxsize)
		parseUpdateContainer(msg);
		break;

	case 0xD2: // request outfit
		parseRequestOutfit(msg);
		break;

	case 0xD3: // set outfit
		parseSetOutfit(msg);
		break;

	case 0xDC:
		parseAddVip(msg);
		break;

	case 0xDD:
		parseRemoveVip(msg);
		break;

	case 0xE8:
		parseDebugAssert(msg);
		break;

	default:
#ifdef __DEBUG__
		printf("unknown packet header: %x \n", recvbyte);
		parseDebug(msg);
#endif
		break;
	}
}

void Protocol80::GetTileDescription(const Tile* tile, NetworkMessage* msg)
{
	if(tile){
		int count = 0;
		if(tile->ground){
			msg->AddItem(tile->ground);
			count++;
		}

		ItemVector::const_iterator it;
		for(it = tile->topItems.begin(); ((it != tile->topItems.end()) && (count < 10)); ++it){
			msg->AddItem(*it);
			count++;
		}

		CreatureVector::const_iterator itc;
		for(itc = tile->creatures.begin(); ((itc != tile->creatures.end()) && (count < 10)); ++itc){
			bool known;
			uint32_t removedKnown;
			checkCreatureAsKnown((*itc)->getID(), known, removedKnown);
			AddCreature(msg, *itc, known, removedKnown);
			count++;
		}

		for(it = tile->downItems.begin(); ((it != tile->downItems.end()) && (count < 10)); ++it){
			msg->AddItem(*it);
			count++;
		}
	}
}

void Protocol80::GetMapDescription(uint16_t x, uint16_t y, unsigned char z,
	uint16_t width, uint16_t height, NetworkMessage* msg)
{
	int skip = -1;
	int startz, endz, zstep = 0;

	if (z > 7) {
		startz = z - 2;
		endz = std::min(MAP_MAX_LAYERS - 1, z + 2);
		zstep = 1;
	}
	else {
		startz = 7;
		endz = 0;

		zstep = -1;
	}

	for(int nz = startz; nz != endz + zstep; nz += zstep){
		GetFloorDescription(msg, x, y, nz, width, height, z - nz, skip);
	}

	if(skip >= 0){
		msg->AddByte(skip);
		msg->AddByte(0xFF);
		//cc += skip;
	}

#ifdef __DEBUG__
	//printf("tiles in total: %d \n", cc);
#endif
}

void Protocol80::GetFloorDescription(NetworkMessage* msg, int x, int y, int z,
	int width, int height, int offset, int& skip)
{
	Tile* tile;

	for(int nx = 0; nx < width; nx++){
		for(int ny = 0; ny < height; ny++){
			tile = g_game.getTile(x + nx + offset, y + ny + offset, z);
			if(tile){
				if(skip >= 0){
					msg->AddByte(skip);
					msg->AddByte(0xFF);
				}
				skip = 0;

				GetTileDescription(tile, msg);
			}
			else {
				skip++;
				if(skip == 0xFF){
					msg->AddByte(0xFF);
					msg->AddByte(0xFF);
					skip = -1;
				}
			}
		}
	}
}

void Protocol80::checkCreatureAsKnown(uint32_t id, bool &known, uint32_t &removedKnown)
{
	// loop through the known player and check if the given player is in
	std::list<uint32_t>::iterator i;
	for(i = knownPlayers.begin(); i != knownPlayers.end(); ++i)
	{
		if((*i) == id)
		{
			// know... make the creature even more known...
			knownPlayers.erase(i);
			knownPlayers.push_back(id);

			known = true;
			return;
		}
	}

	// ok, he is unknown...
	known = false;

	// ... but not in future
	knownPlayers.push_back(id);

	// to many known players?
	if(knownPlayers.size() > 150) //150 for 7.8x
	{
		// lets try to remove one from the end of the list
		for (int n = 0; n < 150; n++)
		{
			removedKnown = knownPlayers.front();

			Creature *c = g_game.getCreatureByID(removedKnown);
			if ((!c) || (!canSee(c)))
				break;

			// this creature we can't remove, still in sight, so back to the end
			knownPlayers.pop_front();
			knownPlayers.push_back(removedKnown);
		}

		// hopefully we found someone to remove :S, we got only 150 tries
		// if not... lets kick some players with debug errors :)
		knownPlayers.pop_front();
	}
	else
	{
		// we can cache without problems :)
		removedKnown = 0;
	}
}

bool Protocol80::canSee(const Creature* c) const
{
	if(c->isRemoved())
		return false;

	return canSee(c->getPosition());
}

bool Protocol80::canSee(const Position& pos) const
{
	return canSee(pos.x, pos.y, pos.z);
}

bool Protocol80::canSee(int x, int y, int z) const
{
#ifdef __DEBUG__
	if(z < 0 || z >= MAP_MAX_LAYERS) {
		std::cout << "WARNING! Protocol80::canSee() Z-value is out of range!" << std::endl;
	}
#endif

	const Position& myPos = player->getPosition();

	if(myPos.z <= 7){
		//we are on ground level or above (7 -> 0)
		//view is from 7 -> 0
		if(z > 7){
			return false;
		}
	}
	else if(myPos.z >= 8){
		//we are underground (8 -> 15)
		//view is +/- 2 from the floor we stand on
		if(std::abs(myPos.z - z) > 2){
			return false;
		}
	}

	//negative offset means that the action taken place is on a lower floor than ourself
	int offsetz = myPos.z - z;

	if ((x >= myPos.x - 8 + offsetz) && (x <= myPos.x + 9 + offsetz) &&
		(y >= myPos.y - 6 + offsetz) && (y <= myPos.y + 7 + offsetz))
		return true;

	return false;
}

//********************** Parse methods *******************************
void Protocol80::parseLogout(NetworkMessage& msg)
{
	Dispatcher::getDispatcher().addTask(
		createTask(boost::bind(&Protocol80::logout, this, false)));
}

void Protocol80::parseCreatePrivateChannel(NetworkMessage& msg)
{
	addGameTask(&Game::playerCreatePrivateChannel, player->getID());
}

void Protocol80::parseChannelInvite(NetworkMessage& msg)
{
	const std::string name = msg.GetString();

	addGameTask(&Game::playerChannelInvite, player->getID(), name);
}

void Protocol80::parseChannelExclude(NetworkMessage& msg)
{
	const std::string name = msg.GetString();

	addGameTask(&Game::playerChannelExclude, player->getID(), name);
}

void Protocol80::parseGetChannels(NetworkMessage& msg)
{
	addGameTask(&Game::playerRequestChannels, player->getID());
}

void Protocol80::parseOpenChannel(NetworkMessage& msg)
{
	uint16_t channelId = msg.GetU16();

	addGameTask(&Game::playerOpenChannel, player->getID(), channelId);
}

void Protocol80::parseCloseChannel(NetworkMessage &msg)
{
	uint16_t channelId = msg.GetU16();

	addGameTask(&Game::playerCloseChannel, player->getID(), channelId);
}

void Protocol80::parseOpenPriv(NetworkMessage& msg)
{
	const std::string receiver = msg.GetString();

	addGameTask(&Game::playerOpenPrivateChannel, player->getID(), receiver);
}

void Protocol80::parseCancelMove(NetworkMessage& msg)
{
	addGameTask(&Game::playerCancelAttackAndFollow, player->getID());
}

void Protocol80::parseDebug(NetworkMessage& msg)
{
	int dataLength = msg.getMessageLength() - 3;
	if(dataLength != 0){
		printf("data: ");
		int data = msg.GetByte();
		while(dataLength > 0){
			printf("%d ", data);
			if(--dataLength > 0)
				data = msg.GetByte();
		}
		printf("\n");
	}
}

void Protocol80::parseRecievePing(NetworkMessage& msg)
{
	if(m_now > m_nextPing){
		Dispatcher::getDispatcher().addTask(
			createTask(boost::bind(&Game::playerReceivePing, &g_game, player->getID())));

		m_nextPing = m_now + 2000;
	}
}

void Protocol80::parseAutoWalk(NetworkMessage& msg)
{
	// first we get all directions...
	std::list<Direction> path;
	size_t numdirs = msg.GetByte();
	for (size_t i = 0; i < numdirs; ++i) {
		uint8_t rawdir = msg.GetByte();
		Direction dir = SOUTH;

		switch(rawdir) {
		case 1: dir = EAST; break;
		case 2: dir = NORTHEAST; break;
		case 3: dir = NORTH; break;
		case 4: dir = NORTHWEST; break;
		case 5: dir = WEST; break;
		case 6: dir = SOUTHWEST; break;
		case 7: dir = SOUTH; break;
		case 8: dir = SOUTHEAST; break;

		default:
			continue;
		};

		/*
		#ifdef __DEBUG__
		std::cout << "Walk by mouse: Direction: " << dir << std::endl;
		#endif
		*/

		path.push_back(dir);
	}

	addGameTask(&Game::playerAutoWalk, player->getID(), path);
}

void Protocol80::parseStopAutoWalk(NetworkMessage& msg)
{
	addGameTask(&Game::playerStopAutoWalk, player->getID());
}

void Protocol80::parseMove(NetworkMessage& msg, Direction dir)
{
	addGameTask(&Game::playerMove, player->getID(), dir);
}

void Protocol80::parseTurn(NetworkMessage& msg, Direction dir)
{
	addGameTask(&Game::playerTurn, player->getID(), dir);
}

void Protocol80::parseRequestOutfit(NetworkMessage& msg)
{
	addGameTask(&Game::playerRequestOutfit, player->getID());
}

void Protocol80::parseSetOutfit(NetworkMessage& msg)
{
	int looktype = msg.GetU16();
	int lookhead = msg.GetByte();
	int lookbody = msg.GetByte();
	int looklegs = msg.GetByte();
	int lookfeet = msg.GetByte();
	int lookaddons = msg.GetByte();

	Outfit_t newOutfit;
	newOutfit.lookType = looktype;
	newOutfit.lookHead = lookhead;
	newOutfit.lookBody = lookbody;
	newOutfit.lookLegs = looklegs;
	newOutfit.lookFeet = lookfeet;
	newOutfit.lookAddons = lookaddons;

	addGameTask(&Game::playerChangeOutfit, player->getID(), newOutfit);
}

void Protocol80::parseUseItem(NetworkMessage& msg)
{
	Position pos = msg.GetPosition();
	uint16_t spriteId = msg.GetSpriteId();
	uint8_t stackpos = msg.GetByte();
	uint8_t index = msg.GetByte();
	bool isHotkey = (pos.x == 0xFFFF && pos.y == 0 && pos.z == 0);

/*
#ifdef __DEBUG__
	std::cout << "parseUseItem: " << "x: " << pos.x << ", y: " << (int)pos.y <<  ", z: " << (int)pos.z << ", item: " << (int)itemId << ", stack: " << (int)stackpos << ", index: " << (int)index << std::endl;
#endif
*/

	addGameTask(&Game::playerUseItem, player->getID(), pos, stackpos, index, spriteId, isHotkey);
}

void Protocol80::parseUseItemEx(NetworkMessage& msg)
{
	Position fromPos = msg.GetPosition();
	uint16_t fromSpriteId = msg.GetSpriteId();
	uint8_t fromStackPos = msg.GetByte();
	Position toPos = msg.GetPosition();
	uint16_t toSpriteId = msg.GetU16();
	uint8_t toStackPos = msg.GetByte();
	bool isHotkey = (fromPos.x == 0xFFFF && fromPos.y == 0 && fromPos.z == 0);

	addGameTask(&Game::playerUseItemEx, player->getID(), fromPos, fromStackPos, fromSpriteId, toPos, toStackPos, toSpriteId, isHotkey);
}

void Protocol80::parseBattleWindow(NetworkMessage &msg)
{
	Position fromPos = msg.GetPosition();
	uint16_t spriteId = msg.GetSpriteId();
	uint8_t fromStackPos = msg.GetByte();
	uint32_t creatureId = msg.GetU32();
	bool isHotkey = (fromPos.x == 0xFFFF && fromPos.y == 0 && fromPos.z == 0);

	addGameTask(&Game::playerUseBattleWindow, player->getID(), fromPos, fromStackPos, creatureId, spriteId, isHotkey);
}

void Protocol80::parseCloseContainer(NetworkMessage& msg)
{
	uint8_t cid = msg.GetByte();

	addGameTask(&Game::playerCloseContainer, player->getID(), cid);
}

void Protocol80::parseUpArrowContainer(NetworkMessage& msg)
{
	uint8_t cid = msg.GetByte();

	addGameTask(&Game::playerMoveUpContainer, player->getID(), cid);
}

void Protocol80::parseUpdateTile(NetworkMessage& msg)
{
	Position pos = msg.GetPosition();

	//addGameTask(&Game::playerUpdateTile, player->getID(), pos);
}

void Protocol80::parseUpdateContainer(NetworkMessage& msg)
{
	uint8_t cid = msg.GetByte();

	addGameTask(&Game::playerUpdateContainer, player->getID(), cid);
}

void Protocol80::parseThrow(NetworkMessage& msg)
{
	Position fromPos = msg.GetPosition();
	uint16_t spriteId = msg.GetSpriteId();
	uint8_t fromStackpos = msg.GetByte();
	Position toPos = msg.GetPosition();
	uint8_t count = msg.GetByte();

	/*
	std::cout << "parseThrow: " << "from_x: " << (int)fromPos.x << ", from_y: " << (int)fromPos.y
	<<  ", from_z: " << (int)fromPos.z << ", item: " << (int)itemId << ", fromStackpos: "
	<< (int)fromStackpos << " to_x: " << (int)toPos.x << ", to_y: " << (int)toPos.y
	<<  ", to_z: " << (int)toPos.z
	<< ", count: " << (int)count << std::endl;
	*/

	if(toPos != fromPos){
		addGameTask(&Game::playerMoveThing, player->getID(), fromPos, spriteId,
			fromStackpos, toPos, count);
	}
}

void Protocol80::parseLookAt(NetworkMessage& msg)
{
	Position pos = msg.GetPosition();
	uint16_t spriteId = msg.GetSpriteId();
	uint8_t stackpos = msg.GetByte();

/*
#ifdef __DEBUG__
	ss << "You look at x: " << x <<", y: " << y << ", z: " << z << " and see Item # " << itemId << ".";
#endif
*/

	addGameTask(&Game::playerLookAt, player->getID(), pos, spriteId, stackpos);
}

void Protocol80::parseSay(NetworkMessage& msg)
{
	SpeakClasses type = (SpeakClasses)msg.GetByte();

	std::string receiver;
	uint16_t channelId = 0;
	switch(type){
	case SPEAK_PRIVATE:
	case SPEAK_PRIVATE_RED:
		receiver = msg.GetString();
		break;
	case SPEAK_CHANNEL_Y:
	case SPEAK_CHANNEL_R1:
	case SPEAK_CHANNEL_R2:
		channelId = msg.GetU16();
		break;
	default:
		break;
	}

	const std::string text = msg.GetString();

	addGameTask(&Game::playerSay, player->getID(), channelId, type, receiver, text);
}

void Protocol80::parseFightModes(NetworkMessage& msg)
{
	uint8_t rawFightMode = msg.GetByte(); //1 - offensive, 2 - balanced, 3 - defensive
	uint8_t rawChaseMode = msg.GetByte(); // 0 - stand while fightning, 1 - chase opponent
	uint8_t rawSafeMode = msg.GetByte();

	bool safeMode = (rawSafeMode == 1);
	chaseMode_t chaseMode = CHASEMODE_STANDSTILL;

	if(rawChaseMode == 0){
		chaseMode = CHASEMODE_STANDSTILL;
	}
	else if(rawChaseMode == 1){
		chaseMode = CHASEMODE_FOLLOW;
	}

	fightMode_t fightMode = FIGHTMODE_ATTACK;

	if(rawFightMode == 1){
		fightMode = FIGHTMODE_ATTACK;
	}
	else if(rawFightMode == 2){
		fightMode = FIGHTMODE_BALANCED;
	}
	else if(rawFightMode == 3){
		fightMode = FIGHTMODE_DEFENSE;
	}

	addGameTask(&Game::playerSetFightModes, player->getID(), fightMode, chaseMode, safeMode);
}

void Protocol80::parseAttack(NetworkMessage& msg)
{
	uint32_t creatureId = msg.GetU32();

	addGameTask(&Game::playerSetAttackedCreature, player->getID(), creatureId);
}

void Protocol80::parseFollow(NetworkMessage& msg)
{
	uint32_t creatureId = msg.GetU32();

	addGameTask(&Game::playerFollowCreature, player->getID(), creatureId);
}

void Protocol80::parseInviteToParty(NetworkMessage& msg)
{
	uint32_t creatureId = msg.GetU32();

	addGameTask(&Game::playerInviteToParty, player->getID(), creatureId);
}

void Protocol80::parseJoinParty(NetworkMessage& msg)
{
	uint32_t creatureId = msg.GetU32();

	addGameTask(&Game::playerJoinParty, player->getID(), creatureId);
}

void Protocol80::parseRevokePartyInvitation(NetworkMessage& msg)
{
	uint32_t creatureId = msg.GetU32();

	addGameTask(&Game::playerRevokePartyInvitation, player->getID(), creatureId);
}

void Protocol80::parsePassPartyLeadership(NetworkMessage& msg)
{
	uint32_t creatureId = msg.GetU32();

	addGameTask(&Game::playerPassPartyLeadership, player->getID(), creatureId);
}

void Protocol80::parseLeaveParty(NetworkMessage& msg)
{
	addGameTask(&Game::playerLeaveParty, player->getID());
}

void Protocol80::parseTextWindow(NetworkMessage& msg)
{
	uint32_t windowTextId = msg.GetU32();
	const std::string newText = msg.GetString();

	addGameTask(&Game::playerWriteItem, player->getID(), windowTextId, newText);
}

void Protocol80::parseHouseWindow(NetworkMessage &msg)
{
	uint8_t doorId = msg.GetByte();
	uint32_t id = msg.GetU32();
	const std::string text = msg.GetString();

	addGameTask(&Game::playerUpdateHouseWindow, player->getID(), doorId, id, text);
}

void Protocol80::parseRequestTrade(NetworkMessage& msg)
{
	Position pos = msg.GetPosition();
	uint16_t spriteId = msg.GetSpriteId();
	uint8_t stackpos = msg.GetByte();
	uint32_t playerId = msg.GetU32();

	addGameTask(&Game::playerRequestTrade, player->getID(), pos, stackpos, playerId, spriteId);
}

void Protocol80::parseAcceptTrade(NetworkMessage& msg)
{
	addGameTask(&Game::playerAcceptTrade, player->getID());
}

void Protocol80::parseLookInTrade(NetworkMessage& msg)
{
	bool counterOffer = (msg.GetByte() == 0x01);
	int index = msg.GetByte();

	addGameTask(&Game::playerLookInTrade, player->getID(), counterOffer, index);
}

void Protocol80::parseCloseTrade()
{
	addGameTask(&Game::playerCloseTrade, player->getID());
}

void Protocol80::parseAddVip(NetworkMessage& msg)
{
	const std::string name = msg.GetString();
	if(name.size() > 32)
		return;

	addGameTask(&Game::playerRequestAddVip, player->getID(), name);
}

void Protocol80::parseRemoveVip(NetworkMessage& msg)
{
	uint32_t guid = msg.GetU32();

	addGameTask(&Game::playerRequestRemoveVip, player->getID(), guid);
}

void Protocol80::parseRotateItem(NetworkMessage& msg)
{
	Position pos = msg.GetPosition();
	uint16_t spriteId = msg.GetSpriteId();
	uint8_t stackpos = msg.GetByte();

	addGameTask(&Game::playerRotateItem, player->getID(), pos, stackpos, spriteId);
}

void Protocol80::parseDebugAssert(NetworkMessage& msg)
{
	if(g_config.getNumber(ConfigManager::SAVE_CLIENT_DEBUG_ASSERTIONS) == 0){
		return;
	}

	//only accept 1 report each time
	if(m_debugAssertSent){
		return;
	}
	m_debugAssertSent = true;

	std::string assertLine = msg.GetString();
	std::string date = msg.GetString();
	std::string description = msg.GetString();
	std::string comment = msg.GetString();

	//write it in the assertions file
	FILE* f = fopen("client_assertions.txt", "a");
	char bufferDate[32], bufferIp[32];
	time_t tmp = time(NULL);
	formatIP(getIP(), bufferIp);
	formatDate(tmp, bufferDate);
	fprintf(f, "----- %s - %s (%s) -----\n", bufferDate, player->getName().c_str(), bufferIp);
	fprintf(f, "%s\n%s\n%s\n%s\n", assertLine.c_str(), date.c_str(), description.c_str(), comment.c_str());
	fclose(f);
}

//********************** Send methods  *******************************
void Protocol80::sendOpenPrivateChannel(const std::string& receiver)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		msg->AddByte(0xAD);
		msg->AddString(receiver);
	}
}

void Protocol80::sendCreatureOutfit(const Creature* creature, const Outfit_t& outfit)
{
	if(canSee(creature)){
		NetworkMessage* msg = getOutputBuffer();
		if(msg){
			msg->AddByte(0x8E);
			msg->AddU32(creature->getID());
			AddCreatureOutfit(msg, creature, outfit);
		}
	}
}

void Protocol80::sendCreatureInvisible(const Creature* creature)
{
	if(canSee(creature)){
		NetworkMessage* msg = getOutputBuffer();
		if(msg){
			msg->AddByte(0x8E);
			msg->AddU32(creature->getID());
			AddCreatureInvisible(msg, creature);
		}
	}
}

void Protocol80::sendCreatureLight(const Creature* creature)
{
	if(canSee(creature)){
		NetworkMessage* msg = getOutputBuffer();
		if(msg){
			AddCreatureLight(msg, creature);
		}
	}
}

void Protocol80::sendWorldLight(const LightInfo& lightInfo)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		AddWorldLight(msg, lightInfo);
	}
}

void Protocol80::sendCreatureSkull(const Creature* creature, Skulls_t skull)
{
	if(canSee(creature)){
		NetworkMessage* msg = getOutputBuffer();
		if(msg){
			msg->AddByte(0x90);
			msg->AddU32(creature->getID());
			msg->AddByte(skull);
		}
	}
}

void Protocol80::sendCreatureShield(const Creature* creature, PartyShields_t shield)
{
	if(canSee(creature)){
		NetworkMessage* msg = getOutputBuffer();
		if(msg){
			msg->AddByte(0x91);
			msg->AddU32(creature->getID());
			msg->AddByte(shield);
		}
	}
}

void Protocol80::sendCreatureSquare(const Creature* creature, SquareColor_t color)
{
	if(canSee(creature)){
		NetworkMessage* msg = getOutputBuffer();
		if(msg){
			msg->AddByte(0x86);
			msg->AddU32(creature->getID());
			msg->AddByte((uint8_t)color);
		}
	}
}

void Protocol80::sendStats()
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		AddPlayerStats(msg);
	}
}

void Protocol80::sendTextMessage(MessageClasses mclass, const std::string& message)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		AddTextMessage(msg, mclass, message);
	}
}

void Protocol80::sendClosePrivate(uint16_t channelId)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		msg->AddByte(0xB3);
		msg->AddU16(channelId);
	}
}

void Protocol80::sendCreatePrivateChannel(uint16_t channelId, const std::string& channelName)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		msg->AddByte(0xB2);
		msg->AddU16(channelId);
		msg->AddString(channelName);
	}
}

void Protocol80::sendChannelsDialog()
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		ChannelList list;
		list = g_chat.getChannelList(player);

		msg->AddByte(0xAB);
		msg->AddByte(list.size());

		while(list.size()){
			ChatChannel *channel;
			channel = list.front();
			list.pop_front();

			msg->AddU16(channel->getId());
			msg->AddString(channel->getName());
		}
	}
}

void Protocol80::sendChannel(uint16_t channelId, const std::string& channelName)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		msg->AddByte(0xAC);
		msg->AddU16(channelId);
		msg->AddString(channelName);
	}
}

void Protocol80::sendIcons(int icons)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		msg->AddByte(0xA2);
		msg->AddU16(icons);
	}
}

void Protocol80::sendContainer(uint32_t cid, const Container* container, bool hasParent)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		msg->AddByte(0x6E);
		msg->AddByte(cid);
		msg->AddItemId(container);
		msg->AddString(container->getName());
		msg->AddByte(container->capacity());
		msg->AddByte(hasParent ? 0x01 : 0x00);
		if(container->size() > 255){
			msg->AddByte(255);
		}
		else{
			msg->AddByte(container->size());
		}

		ItemList::const_iterator cit;
		uint32_t i = 0;
		for(cit = container->getItems(); cit != container->getEnd() && i < 255; ++cit, ++i){
			msg->AddItem(*cit);
		}
	}
}

void Protocol80::sendTradeItemRequest(const Player* player, const Item* item, bool ack)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		if(ack){
			msg->AddByte(0x7D);
		}
		else{
			msg->AddByte(0x7E);
		}

		msg->AddString(player->getName());

		if(const Container* tradeContainer = item->getContainer()){
			std::list<const Container*> listContainer;
			ItemList::const_iterator it;
			Container* tmpContainer = NULL;

			listContainer.push_back(tradeContainer);

			std::list<const Item*> listItem;
			listItem.push_back(tradeContainer);

			while(listContainer.size() > 0) {
				const Container* container = listContainer.front();
				listContainer.pop_front();

				for(it = container->getItems(); it != container->getEnd(); ++it){
					if((tmpContainer = (*it)->getContainer())){
						listContainer.push_back(tmpContainer);
					}

					listItem.push_back(*it);
				}
			}

			msg->AddByte(listItem.size());
			while(listItem.size() > 0) {
				const Item* item = listItem.front();
				listItem.pop_front();
				msg->AddItem(item);
			}
		}
		else {
			msg->AddByte(1);
			msg->AddItem(item);
		}
	}
}

void Protocol80::sendCloseTrade()
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		msg->AddByte(0x7F);
	}
}

void Protocol80::sendCloseContainer(uint32_t cid)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		msg->AddByte(0x6F);
		msg->AddByte(cid);
	}
}

void Protocol80::sendCreatureTurn(const Creature* creature, uint8_t stackPos)
{
	if(canSee(creature)){
		NetworkMessage* msg = getOutputBuffer();
		if(msg){
			msg->AddByte(0x6B);
			msg->AddPosition(creature->getPosition());
			msg->AddByte(stackPos);
			msg->AddU16(0x63); /*99*/
			msg->AddU32(creature->getID());
			msg->AddByte(creature->getDirection());
		}
	}
}

void Protocol80::sendCreatureSay(const Creature* creature, SpeakClasses type, const std::string& text)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		AddCreatureSpeak(msg, creature, type, text, 0);
	}
}

void Protocol80::sendToChannel(const Creature * creature, SpeakClasses type, const std::string& text, uint16_t channelId)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		AddCreatureSpeak(msg, creature, type, text, channelId);
	}
}

void Protocol80::sendCancel(const std::string& message)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		AddTextMessage(msg, MSG_STATUS_SMALL, message);
	}
}

void Protocol80::sendCancelTarget()
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		msg->AddByte(0xa3);
	}
}

void Protocol80::sendChangeSpeed(const Creature* creature, uint32_t speed)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		msg->AddByte(0x8F);
		msg->AddU32(creature->getID());
		msg->AddU16(speed);
	}
}

void Protocol80::sendCancelWalk()
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		msg->AddByte(0xB5);
		msg->AddByte(player->getDirection());
	}
}

void Protocol80::sendSkills()
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		AddPlayerSkills(msg);
	}
}

void Protocol80::sendPing()
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		msg->AddByte(0x1E);
	}
}

void Protocol80::sendDistanceShoot(const Position& from, const Position& to, uint8_t type)
{
	if(canSee(from) || canSee(to)){
		NetworkMessage* msg = getOutputBuffer();
		if(msg){
			AddDistanceShoot(msg, from, to, type);
		}
	}
}

void Protocol80::sendMagicEffect(const Position& pos, uint8_t type)
{
	if(canSee(pos)){
		NetworkMessage* msg = getOutputBuffer();
		if(msg){
			AddMagicEffect(msg, pos, type);
		}
	}
}

void Protocol80::sendAnimatedText(const Position& pos, uint8_t color, std::string text)
{
	if(canSee(pos)){
		NetworkMessage* msg = getOutputBuffer();
		if(msg){
			AddAnimatedText(msg, pos, color, text);
		}
	}
}

void Protocol80::sendCreatureHealth(const Creature* creature)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		AddCreatureHealth(msg, creature);
	}
}

//tile
void Protocol80::sendAddTileItem(const Position& pos, const Item* item)
{
	if(canSee(pos)){
		NetworkMessage* msg = getOutputBuffer();
		if(msg){
			AddTileItem(msg, pos, item);
		}
	}
}

void Protocol80::sendUpdateTileItem(const Position& pos, uint32_t stackpos, const Item* item)
{
	if(canSee(pos)){
		NetworkMessage* msg = getOutputBuffer();
		if(msg){
			UpdateTileItem(msg, pos, stackpos, item);
		}
	}
}

void Protocol80::sendRemoveTileItem(const Position& pos, uint32_t stackpos)
{
	if(canSee(pos)){
		NetworkMessage* msg = getOutputBuffer();
		if(msg){
			RemoveTileItem(msg, pos, stackpos);
		}
	}
}

void Protocol80::sendUpdateTile(const Position& pos)
{
	if(canSee(pos)){
		NetworkMessage* msg = getOutputBuffer();
		if(msg){
			msg->AddByte(0x69);
			msg->AddPosition(pos);

			Tile* tile = g_game.getTile(pos.x, pos.y, pos.z);
			if(tile){
				GetTileDescription(tile, msg);
				msg->AddByte(0);
				msg->AddByte(0xFF);
			}
			else{
				msg->AddByte(0x01);
				msg->AddByte(0xFF);
			}
		}
	}
}

void Protocol80::sendAddCreature(const Creature* creature, bool isLogin)
{
	if(canSee(creature->getPosition())){
		NetworkMessage* msg = getOutputBuffer();
		if(msg){
			if(creature == player){
				msg->AddByte(0x0A);
				msg->AddU32(player->getID());
				msg->AddByte(0x32);
				msg->AddByte(0x00);
				msg->AddByte(0x00); //can report bugs 0,1

				//msg->AddByte(0x0B);//TODO?. GM actions
				//msg->AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);
				//msg->AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);
				//msg->AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);
				//msg->AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);
				//msg->AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);
				//msg->AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);
				//msg->AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);
				//msg->AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);msg.AddByte(0xFF);

				AddMapDescription(msg, player->getPosition());

				if(isLogin){
					AddMagicEffect(msg, player->getPosition(), NM_ME_ENERGY_AREA);
				}

				AddInventoryItem(msg, SLOT_HEAD, player->getInventoryItem(SLOT_HEAD));
				AddInventoryItem(msg, SLOT_NECKLACE, player->getInventoryItem(SLOT_NECKLACE));
				AddInventoryItem(msg, SLOT_BACKPACK, player->getInventoryItem(SLOT_BACKPACK));
				AddInventoryItem(msg, SLOT_ARMOR, player->getInventoryItem(SLOT_ARMOR));
				AddInventoryItem(msg, SLOT_RIGHT, player->getInventoryItem(SLOT_RIGHT));
				AddInventoryItem(msg, SLOT_LEFT, player->getInventoryItem(SLOT_LEFT));
				AddInventoryItem(msg, SLOT_LEGS, player->getInventoryItem(SLOT_LEGS));
				AddInventoryItem(msg, SLOT_FEET, player->getInventoryItem(SLOT_FEET));
				AddInventoryItem(msg, SLOT_RING, player->getInventoryItem(SLOT_RING));
				AddInventoryItem(msg, SLOT_AMMO, player->getInventoryItem(SLOT_AMMO));

				AddPlayerStats(msg);
				AddPlayerSkills(msg);

				//gameworld light-settings
				LightInfo lightInfo;
				g_game.getWorldLightInfo(lightInfo);
				AddWorldLight(msg, lightInfo);

				//player light level
				AddCreatureLight(msg, creature);

				if(isLogin){
					std::string tempstring = g_config.getString(ConfigManager::LOGIN_MSG);
					if(tempstring.size() > 0){
						AddTextMessage(msg, MSG_STATUS_DEFAULT, tempstring.c_str());
					}

					if(player->getLastLoginSaved() != 0){
						tempstring = "Your last visit was on ";
						time_t lastLogin = player->getLastLoginSaved();
						tempstring += ctime(&lastLogin);
						tempstring.erase(tempstring.length() -1);
						tempstring += ".";
						AddTextMessage(msg, MSG_STATUS_DEFAULT, tempstring.c_str());
					}
					else{
						tempstring = "Welcome to ";
						tempstring += g_config.getString(ConfigManager::SERVER_NAME);
						tempstring += ". Please choose an outfit.";
						sendOutfitWindow();
					}
				}

				for(VIPListSet::iterator it = player->VIPList.begin(); it != player->VIPList.end(); it++){
					bool online;
					std::string vip_name;
					if(IOPlayer::instance()->getNameByGuid((*it), vip_name)){
						online = (g_game.getPlayerByName(vip_name) != NULL);
						sendVIP((*it), vip_name, online);
					}
				}
			}
			else{
				AddTileCreature(msg, creature->getPosition(), creature);

				if(isLogin){
					AddMagicEffect(msg, creature->getPosition(), NM_ME_ENERGY_AREA);
				}
			}
		}
	}
}

void Protocol80::sendRemoveCreature(const Creature* creature, const Position& pos, uint32_t stackpos, bool isLogout)
{
	if(canSee(pos)){
		NetworkMessage* msg = getOutputBuffer();
		if(msg){
			RemoveTileItem(msg, pos, stackpos);

			if(isLogout){
				AddMagicEffect(msg, pos, NM_ME_PUFF);
			}
		}
	}
}

void Protocol80::sendMoveCreature(const Creature* creature, const Position& newPos, const Position& oldPos,
	uint32_t oldStackPos, bool teleport)
{
	if(creature == player){
		NetworkMessage* msg = getOutputBuffer();
		if(msg){
			if(teleport || oldStackPos >= 10){
				RemoveTileItem(msg, oldPos, oldStackPos);
				AddMapDescription(msg, player->getPosition());
			}
			else{
				if(oldPos.z == 7 && newPos.z >= 8){
					RemoveTileItem(msg, oldPos, oldStackPos);
				}
				else{
						msg->AddByte(0x6D);
						msg->AddPosition(oldPos);
						msg->AddByte(oldStackPos);
						msg->AddPosition(creature->getPosition());
					}

				//floor change down
				if(newPos.z > oldPos.z){
					MoveDownCreature(msg, creature, newPos, oldPos, oldStackPos);
				}
				//floor change up
				else if(newPos.z < oldPos.z){
					MoveUpCreature(msg, creature, newPos, oldPos, oldStackPos);
				}

				if(oldPos.y > newPos.y){ // north, for old x
					msg->AddByte(0x65);
					GetMapDescription(oldPos.x - 8, newPos.y - 6, newPos.z, 18, 1, msg);
				}
				else if(oldPos.y < newPos.y){ // south, for old x
					msg->AddByte(0x67);
					GetMapDescription(oldPos.x - 8, newPos.y + 7, newPos.z, 18, 1, msg);
				}

				if(oldPos.x < newPos.x){ // east, [with new y]
					msg->AddByte(0x66);
					GetMapDescription(newPos.x + 9, newPos.y - 6, newPos.z, 1, 14, msg);
				}
				else if(oldPos.x > newPos.x){ // west, [with new y]
					msg->AddByte(0x68);
					GetMapDescription(newPos.x - 8, newPos.y - 6, newPos.z, 1, 14, msg);
				}
			}
		}
	}
	else if(canSee(oldPos) && canSee(creature->getPosition())){
		if(teleport || (oldPos.z == 7 && newPos.z >= 8) || oldStackPos >= 10){
			sendRemoveCreature(creature, oldPos, oldStackPos, false);
			sendAddCreature(creature, false);
		}
		else{
				NetworkMessage* msg = getOutputBuffer();
				if(msg){
					msg->AddByte(0x6D);
					msg->AddPosition(oldPos);
					msg->AddByte(oldStackPos);
					msg->AddPosition(creature->getPosition());
				}
			}
		}
	else if(canSee(oldPos)){
		sendRemoveCreature(creature, oldPos, oldStackPos, false);
	}
	else if(canSee(creature->getPosition())){
		sendAddCreature(creature, false);
	}
}

//inventory
void Protocol80::sendAddInventoryItem(slots_t slot, const Item* item)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		AddInventoryItem(msg, slot, item);
	}
}

void Protocol80::sendUpdateInventoryItem(slots_t slot, const Item* item)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		UpdateInventoryItem(msg, slot, item);
	}
}

void Protocol80::sendRemoveInventoryItem(slots_t slot)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		RemoveInventoryItem(msg, slot);
	}
}

//containers
void Protocol80::sendAddContainerItem(uint8_t cid, const Item* item)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		AddContainerItem(msg, cid, item);
	}
}

void Protocol80::sendUpdateContainerItem(uint8_t cid, uint8_t slot, const Item* item)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		UpdateContainerItem(msg, cid, slot, item);
	}
}

void Protocol80::sendRemoveContainerItem(uint8_t cid, uint8_t slot)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		RemoveContainerItem(msg, cid, slot);
	}
}

void Protocol80::sendTextWindow(uint32_t windowTextId, Item* item, uint16_t maxlen, bool canWrite)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		msg->AddByte(0x96);
		msg->AddU32(windowTextId);
		msg->AddItemId(item);
		if(canWrite){
			msg->AddU16(maxlen);
			msg->AddString(item->getText());
		}
		else{
			msg->AddU16(item->getText().size());
			msg->AddString(item->getText());
		}

		const std::string& writer = item->getWriter();
		if(writer.size()){
			msg->AddString(writer);
		}
		else{
			msg->AddString("");
		}

		time_t writtenDate = item->getWrittenDate();
		if(writtenDate > 0){
			char date[16];
			formatDate2(writtenDate, date);
			msg->AddString(date);
		}
		else{
			msg->AddString("");
		}
	}
}

void Protocol80::sendTextWindow(uint32_t windowTextId, uint32_t itemId, const std::string& text)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		msg->AddByte(0x96);
		msg->AddU32(windowTextId);
		msg->AddItemId(itemId);

		msg->AddU16(text.size());
		msg->AddString(text);

		msg->AddString("");
		msg->AddString("");
	}
}

void Protocol80::sendHouseWindow(uint32_t windowTextId, House* _house,
	uint32_t listId, const std::string& text)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		msg->AddByte(0x97);
		msg->AddByte(0);
		msg->AddU32(windowTextId);
		msg->AddString(text);
	}
}

void Protocol80::sendOutfitWindow()
{
	#define MAX_NUMBER_OF_OUTFITS 25
	//client 8.0 outfits limit is 25

	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		msg->AddByte(0xC8);
		AddCreatureOutfit(msg, player, player->getDefaultOutfit());

		const OutfitListType& player_outfits = player->getPlayerOutfits();
		size_t count_outfits = player_outfits.size();

		if(count_outfits > MAX_NUMBER_OF_OUTFITS){
			msg->AddByte(MAX_NUMBER_OF_OUTFITS);
		}
		else if(count_outfits == 0){
			return;
		}
		else{
			msg->AddByte(count_outfits);
		}

		uint32_t counter = 0;
		OutfitListType::const_iterator it;
		for(it = player_outfits.begin(); it != player_outfits.end() && (counter < MAX_NUMBER_OF_OUTFITS); ++it, ++counter){
			msg->AddU16((*it)->looktype);
			msg->AddString(Outfits::getInstance()->getOutfitName((*it)->looktype));
			msg->AddByte((*it)->addons);
		}
	}
}

void Protocol80::sendVIPLogIn(uint32_t guid)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		msg->AddByte(0xD3);
		msg->AddU32(guid);
	}
}

void Protocol80::sendVIPLogOut(uint32_t guid)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		msg->AddByte(0xD4);
		msg->AddU32(guid);
	}
}

void Protocol80::sendVIP(uint32_t guid, const std::string& name, bool isOnline)
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		msg->AddByte(0xD2);
		msg->AddU32(guid);
		msg->AddString(name);
		msg->AddByte(isOnline == true ? 1 : 0);
	}
}

void Protocol80::sendReLoginWindow()
{
	NetworkMessage* msg = getOutputBuffer();
	if(msg){
		msg->AddByte(0x28);
	}
}

////////////// Add common messages
void Protocol80::AddMapDescription(NetworkMessage* msg, const Position& pos)
{
	msg->AddByte(0x64);
	msg->AddPosition(player->getPosition());
	GetMapDescription(pos.x - 8, pos.y - 6, pos.z, 18, 14, msg);
}

void Protocol80::AddTextMessage(NetworkMessage* msg, MessageClasses mclass, const std::string& message)
{
	msg->AddByte(0xB4);
	msg->AddByte(mclass);
	msg->AddString(message);
}

void Protocol80::AddAnimatedText(NetworkMessage* msg, const Position& pos,
	uint8_t color, const std::string& text)
{
	msg->AddByte(0x84);
	msg->AddPosition(pos);
	msg->AddByte(color);
	msg->AddString(text);
}

void Protocol80::AddMagicEffect(NetworkMessage* msg,const Position& pos, uint8_t type)
{
	msg->AddByte(0x83);
	msg->AddPosition(pos);
	msg->AddByte(type + 1);
}

void Protocol80::AddDistanceShoot(NetworkMessage* msg, const Position& from, const Position& to,
	uint8_t type)
{
	msg->AddByte(0x85);
	msg->AddPosition(from);
	msg->AddPosition(to);
	msg->AddByte(type + 1);
}

void Protocol80::AddCreature(NetworkMessage* msg,const Creature* creature, bool known, uint32_t remove)
{
	if(known){
		msg->AddU16(0x62);
		msg->AddU32(creature->getID());
	}
	else{
		msg->AddU16(0x61);
		msg->AddU32(remove);
		msg->AddU32(creature->getID());
		msg->AddString(creature->getName());
	}

	msg->AddByte((int32_t)std::ceil(((float)creature->getHealth()) * 100 / std::max(creature->getMaxHealth(), (int32_t)1)));
	msg->AddByte((uint8_t)creature->getDirection());

	if(!creature->isInvisible()){
		AddCreatureOutfit(msg, creature, creature->getCurrentOutfit());
	}
	else{
		AddCreatureInvisible(msg, creature);
	}

	LightInfo lightInfo;
	creature->getCreatureLight(lightInfo);
	msg->AddByte(lightInfo.level);
	msg->AddByte(lightInfo.color);

	msg->AddU16(creature->getSpeed());
#ifdef __SKULLSYSTEM__
	msg->AddByte(player->getSkullClient(creature->getPlayer()));
#else
	msg->AddByte(SKULL_NONE);
#endif
	msg->AddByte(player->getPartyShield(creature->getPlayer()));
}

inline int32_t checkConstrains(int32_t value, int32_t min, int32_t max)
{
	if(value > max){
		return max;
	}
	else if(value < min){
		return min;
	}
	else{
		return value;
	}
}

void Protocol80::AddPlayerStats(NetworkMessage* msg)
{
	msg->AddByte(0xA0);
	msg->AddU16(player->getHealth());
	msg->AddU16(player->getPlayerInfo(PLAYERINFO_MAXHEALTH));
	msg->AddU16((int32_t)player->getFreeCapacity());
	msg->AddU32(player->getExperience());
	msg->AddU16(player->getPlayerInfo(PLAYERINFO_LEVEL));
	msg->AddByte(checkConstrains(player->getPlayerInfo(PLAYERINFO_LEVELPERCENT), 0, 100));
	msg->AddU16(player->getMana());
	msg->AddU16(player->getPlayerInfo(PLAYERINFO_MAXMANA));
	msg->AddByte(player->getMagicLevel());
	msg->AddByte(checkConstrains(player->getPlayerInfo(PLAYERINFO_MAGICLEVELPERCENT), 0, 100));
	msg->AddByte(player->getPlayerInfo(PLAYERINFO_SOUL));
	msg->AddU16(1440); //stamina(minutes)
}

void Protocol80::AddPlayerSkills(NetworkMessage* msg)
{
	msg->AddByte(0xA1);
	msg->AddByte(player->getSkill(SKILL_FIST,   SKILL_LEVEL));
	msg->AddByte(player->getSkill(SKILL_FIST,   SKILL_PERCENT));
	msg->AddByte(player->getSkill(SKILL_CLUB,   SKILL_LEVEL));
	msg->AddByte(player->getSkill(SKILL_CLUB,   SKILL_PERCENT));
	msg->AddByte(player->getSkill(SKILL_SWORD,  SKILL_LEVEL));
	msg->AddByte(player->getSkill(SKILL_SWORD,  SKILL_PERCENT));
	msg->AddByte(player->getSkill(SKILL_AXE,    SKILL_LEVEL));
	msg->AddByte(player->getSkill(SKILL_AXE,    SKILL_PERCENT));
	msg->AddByte(player->getSkill(SKILL_DIST,   SKILL_LEVEL));
	msg->AddByte(player->getSkill(SKILL_DIST,   SKILL_PERCENT));
	msg->AddByte(player->getSkill(SKILL_SHIELD, SKILL_LEVEL));
	msg->AddByte(player->getSkill(SKILL_SHIELD, SKILL_PERCENT));
	msg->AddByte(player->getSkill(SKILL_FISH,   SKILL_LEVEL));
	msg->AddByte(player->getSkill(SKILL_FISH,   SKILL_PERCENT));
}

void Protocol80::AddCreatureSpeak(NetworkMessage* msg, const Creature* creature,
	SpeakClasses type, std::string text, uint16_t channelId)
{
	msg->AddByte(0xAA);
	msg->AddU32(0);

	//Do not add name for anonymous channel talk
	if(type != SPEAK_CHANNEL_R2){
		msg->AddString(creature->getName());
	}
	else{
		msg->AddString("");
	}

	//Add level only for players
	if(const Player* player = creature->getPlayer()){
		msg->AddU16(player->getPlayerInfo(PLAYERINFO_LEVEL));
	}
	else{
		msg->AddU16(0);
	}

	msg->AddByte(type);
	switch(type){
		case SPEAK_SAY:
		case SPEAK_WHISPER:
		case SPEAK_YELL:
		case SPEAK_MONSTER_SAY:
		case SPEAK_MONSTER_YELL:
			msg->AddPosition(creature->getPosition());
			break;
		case SPEAK_CHANNEL_Y:
		case SPEAK_CHANNEL_R1:
		case SPEAK_CHANNEL_R2:
		case SPEAK_CHANNEL_O:
			msg->AddU16(channelId);
			break;
		default:
			break;
	}

	msg->AddString(text);
}

void Protocol80::AddCreatureHealth(NetworkMessage* msg,const Creature* creature)
{
	msg->AddByte(0x8C);
	msg->AddU32(creature->getID());
	msg->AddByte((int32_t)std::ceil(((float)creature->getHealth()) * 100 / std::max(creature->getMaxHealth(), (int32_t)1)));
}

void Protocol80::AddCreatureInvisible(NetworkMessage* msg, const Creature* creature)
{
	msg->AddU16(0);
	msg->AddU16(0);
}

void Protocol80::AddCreatureOutfit(NetworkMessage* msg, const Creature* creature, const Outfit_t& outfit)
{
	msg->AddU16(outfit.lookType);
	if(outfit.lookType != 0){
		msg->AddByte(outfit.lookHead);
		msg->AddByte(outfit.lookBody);
		msg->AddByte(outfit.lookLegs);
		msg->AddByte(outfit.lookFeet);
		msg->AddByte(outfit.lookAddons);
	}
	else{
		msg->AddItemId(outfit.lookTypeEx);
	}
}

void Protocol80::AddWorldLight(NetworkMessage* msg, const LightInfo& lightInfo)
{
	msg->AddByte(0x82);
	msg->AddByte(lightInfo.level);
	msg->AddByte(lightInfo.color);
}

void Protocol80::AddCreatureLight(NetworkMessage* msg, const Creature* creature)
{
	LightInfo lightInfo;
	creature->getCreatureLight(lightInfo);
	msg->AddByte(0x8D);
	msg->AddU32(creature->getID());
	msg->AddByte(lightInfo.level);
	msg->AddByte(lightInfo.color);
}

//tile
void Protocol80::AddTileItem(NetworkMessage* msg, const Position& pos, const Item* item)
{
	msg->AddByte(0x6A);
	msg->AddPosition(pos);
	msg->AddItem(item);
}

void Protocol80::AddTileCreature(NetworkMessage* msg, const Position& pos, const Creature* creature)
{
	msg->AddByte(0x6A);
	msg->AddPosition(pos);

	bool known;
	uint32_t removedKnown;
	checkCreatureAsKnown(creature->getID(), known, removedKnown);
	AddCreature(msg, creature, known, removedKnown);
}

void Protocol80::UpdateTileItem(NetworkMessage* msg, const Position& pos, uint32_t stackpos, const Item* item)
{
	if(stackpos < 10){
		msg->AddByte(0x6B);
		msg->AddPosition(pos);
		msg->AddByte(stackpos);
		msg->AddItem(item);
	}
}

void Protocol80::RemoveTileItem(NetworkMessage* msg, const Position& pos, uint32_t stackpos)
{
	if(stackpos < 10){
		msg->AddByte(0x6C);
		msg->AddPosition(pos);
		msg->AddByte(stackpos);
	}
}

void Protocol80::MoveUpCreature(NetworkMessage* msg, const Creature* creature,
	const Position& newPos, const Position& oldPos, uint32_t oldStackPos)
{
	if(creature == player){
		//floor change up
		msg->AddByte(0xBE);

		//going to surface
		if(newPos.z == 7){
			int skip = -1;
			GetFloorDescription(msg, oldPos.x - 8, oldPos.y - 6, 5, 18, 14, 3, skip); //(floor 7 and 6 already set)
			GetFloorDescription(msg, oldPos.x - 8, oldPos.y - 6, 4, 18, 14, 4, skip);
			GetFloorDescription(msg, oldPos.x - 8, oldPos.y - 6, 3, 18, 14, 5, skip);
			GetFloorDescription(msg, oldPos.x - 8, oldPos.y - 6, 2, 18, 14, 6, skip);
			GetFloorDescription(msg, oldPos.x - 8, oldPos.y - 6, 1, 18, 14, 7, skip);
			GetFloorDescription(msg, oldPos.x - 8, oldPos.y - 6, 0, 18, 14, 8, skip);

			if(skip >= 0){
				msg->AddByte(skip);
				msg->AddByte(0xFF);
			}
		}
		//underground, going one floor up (still underground)
		else if(newPos.z > 7){
			int skip = -1;
			GetFloorDescription(msg, oldPos.x - 8, oldPos.y - 6, oldPos.z - 3, 18, 14, 3, skip);

			if(skip >= 0){
				msg->AddByte(skip);
				msg->AddByte(0xFF);
			}
		}

		//moving up a floor up makes us out of sync
		//west
		msg->AddByte(0x68);
		GetMapDescription(oldPos.x - 8, oldPos.y + 1 - 6, newPos.z, 1, 14, msg);

		//north
		msg->AddByte(0x65);
		GetMapDescription(oldPos.x - 8, oldPos.y - 6, newPos.z, 18, 1, msg);
	}
}

void Protocol80::MoveDownCreature(NetworkMessage* msg, const Creature* creature,
	const Position& newPos, const Position& oldPos, uint32_t oldStackPos)
{
	if(creature == player){
		//floor change down
		msg->AddByte(0xBF);

		//going from surface to underground
		if(newPos.z == 8){
			int skip = -1;

			GetFloorDescription(msg, oldPos.x - 8, oldPos.y - 6, newPos.z, 18, 14, -1, skip);
			GetFloorDescription(msg, oldPos.x - 8, oldPos.y - 6, newPos.z + 1, 18, 14, -2, skip);
			GetFloorDescription(msg, oldPos.x - 8, oldPos.y - 6, newPos.z + 2, 18, 14, -3, skip);

			if(skip >= 0){
				msg->AddByte(skip);
				msg->AddByte(0xFF);
			}
		}
		//going further down
		else if(newPos.z > oldPos.z && newPos.z > 8 && newPos.z < 14){
			int skip = -1;
			GetFloorDescription(msg, oldPos.x - 8, oldPos.y - 6, newPos.z + 2, 18, 14, -3, skip);

			if(skip >= 0){
				msg->AddByte(skip);
				msg->AddByte(0xFF);
			}
		}

		//moving down a floor makes us out of sync
		//east
		msg->AddByte(0x66);
		GetMapDescription(oldPos.x + 9, oldPos.y - 1 - 6, newPos.z, 1, 14, msg);

		//south
		msg->AddByte(0x67);
		GetMapDescription(oldPos.x - 8, oldPos.y + 7, newPos.z, 18, 1, msg);
	}
}

//inventory
void Protocol80::AddInventoryItem(NetworkMessage* msg, slots_t slot, const Item* item)
{
	if(item == NULL){
		msg->AddByte(0x79);
		msg->AddByte(slot);
	}
	else{
		msg->AddByte(0x78);
		msg->AddByte(slot);
		msg->AddItem(item);
	}
}

void Protocol80::UpdateInventoryItem(NetworkMessage* msg, slots_t slot, const Item* item)
{
	if(item == NULL){
		msg->AddByte(0x79);
		msg->AddByte(slot);
	}
	else{
		msg->AddByte(0x78);
		msg->AddByte(slot);
		msg->AddItem(item);
	}
}

void Protocol80::RemoveInventoryItem(NetworkMessage* msg, slots_t slot)
{
	msg->AddByte(0x79);
	msg->AddByte(slot);
}

//containers
void Protocol80::AddContainerItem(NetworkMessage* msg, uint8_t cid, const Item* item)
{
	msg->AddByte(0x70);
	msg->AddByte(cid);
	msg->AddItem(item);
}

void Protocol80::UpdateContainerItem(NetworkMessage* msg, uint8_t cid, uint8_t slot, const Item* item)
{
	msg->AddByte(0x71);
	msg->AddByte(cid);
	msg->AddByte(slot);
	msg->AddItem(item);
}

void Protocol80::RemoveContainerItem(NetworkMessage* msg, uint8_t cid, uint8_t slot)
{
	msg->AddByte(0x72);
	msg->AddByte(cid);
	msg->AddByte(slot);
}
