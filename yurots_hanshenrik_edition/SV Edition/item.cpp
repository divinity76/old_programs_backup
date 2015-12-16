//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
// Item represents an existing item.
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
#include "item.h"
#include "container.h"
#include "depot.h"
#include "teleport.h"
#include "trashholder.h"
#include "mailbox.h"
#include "house.h"

#include "actions.h"
#include "magic.h"

#include <iostream>
#include <sstream>
#include <iomanip>

Item* Item::CreateItem(const unsigned short _type, unsigned short _count /*= 1*/)
{
	Item* newItem = NULL;

	if(_type == ITEM_LOCKER1 || _type == ITEM_LOCKER2 || _type == ITEM_LOCKER3 || _type == ITEM_LOCKER4){
		newItem = new Depot(_type);
	}
	else if(items[_type].isContainer()){
		newItem = new Container(_type);
	}
	else if(items[_type].isTeleport()){
		newItem = new Teleport(_type);
	}
	else if(items[_type].isMagicField()){
		newItem = new Item(_type, _count);
	}
	else if(items[_type].isDoor()){
		newItem = new Door(_type);
	}
	else if(_type == ITEM_DUSTBIN){
		newItem = new TrashHolder(_type /*, NM_ME_PUFF*/);
	}
	else if(_type == ITEM_MAILBOX1 || _type == ITEM_MAILBOX2 || _type == ITEM_MAILBOX3){
		newItem = new Mailbox(_type);
	}
	else{
		newItem = new Item(_type, _count);
	}

	newItem->useThing2();
	return newItem;
}

Item* Item::CreateItem(PropStream& propStream)
{
	unsigned short _id;
	if(!propStream.GET_USHORT(_id)){
		return NULL;
	}

	ItemType iType = Item::items[_id];
	unsigned char _count = 1;

	if(iType.stackable || iType.isSplash() || iType.isFluidContainer()){
		if(!propStream.GET_UCHAR(_count)){
			return NULL;
		}
	}

	return Item::CreateItem(_id, _count);
}

Item::Item(const unsigned short _type, unsigned short _count)
{
	//std::cout << "Item constructor2 " << this << std::endl;
	id = _type;
	count = 0;
	chargecount = 0;
	fluid = 0;
	actionId = 0;
	uniqueId = 0;
	isDecaying = false;
	specialDescription = NULL;
	text = NULL;
	setItemCountOrSubtype(_count);

	if(count == 0)
		count = 1;
#ifdef YUR_RINGS_AMULETS
	const ItemType& it = items[id];
	time = it.newTime;
	charges = it.newCharges;
#endif //YUR_RINGS_AMULETS

}

Item::Item()
{
	//std::cout << "Item constructor3 " << this << std::endl;
	id = 0;
	count = 1;
	chargecount = 0;
	isDecaying  = false;
	actionId = 0;
	uniqueId = 0;
	specialDescription = NULL;
	text = NULL;
#ifdef YUR_RINGS_AMULETS
	time = 0;
	charges = 0;
#endif //YUR_RINGS_AMULETS

}

Item::Item(const unsigned short _type)
{
	//std::cout << "Item constructor1 " << this << std::endl;
	id = _type;
	count = 1;
	chargecount = 0;
	fluid = 0;
	actionId = 0;
	uniqueId = 0;
	isDecaying = false;
	specialDescription = NULL;
	text = NULL;
#ifdef YUR_RINGS_AMULETS
	const ItemType& it = items[id];
	time = it.newTime;
	charges = it.newCharges;
#endif //YUR_RINGS_AMULETS

}

Item::Item(const Item &i)
{
	//std::cout << "Item copy constructor " << this << std::endl;
	id = i.id;
	count = i.count;
	chargecount = i.chargecount;
	isDecaying  = false;
	actionId = i.actionId;
	uniqueId = i.uniqueId;
	if(i.specialDescription != NULL){
		specialDescription = new std::string(*(i.specialDescription));
	}
	else{
		specialDescription = NULL;
	}
	if(i.text != NULL){
		text = new std::string(*(i.text));
	}
	else{
		text = NULL;
	}
	#ifdef YUR_RINGS_AMULETS
	time = i.time;
	charges = i.charges;
    #endif //YUR_RINGS_AMULETS
}

Item::~Item()
{
	//std::cout << "Item destructor " << this << std::endl;
	if(specialDescription)
		delete specialDescription;
	if(text)
		delete text;
}

unsigned short Item::getID() const
{
	return id;
}

void Item::setID(unsigned short newid)
{
	id = newid;
}

unsigned short Item::getItemCount() const
{
	return count;
}

void Item::setItemCount(uint8_t n)
{
	count = n;
}

unsigned char Item::getItemCountOrSubtype() const
{
	if(isFluidContainer() || isSplash())
		return fluid;
	else if(items[id].runeMagLevel != -1)
		return chargecount;
	else
		return count;
}

void Item::setItemCountOrSubtype(unsigned char n)
{
	if(isFluidContainer() || isSplash())
		fluid = n;
	else if(items[id].runeMagLevel != -1)
		chargecount = n;
	else{
		count = n;
	}
}

bool Item::hasSubType() const
{
	const ItemType& it = items[id];
	return (it.isFluidContainer() || it.isSplash() || it.stackable || it.runeMagLevel != -1);
}

void Item::setActionId(unsigned short n)
{
	if(n < 100)
		n = 100;
	actionId = n;
}

unsigned short Item::getActionId() const
{
	return actionId;
}

void Item::setUniqueId(unsigned short n)
{
	//uniqueId only can be set 1 time
	if(uniqueId != 0)
		return;
	 if(n < 1000)
	 	n = 1000;
	uniqueId = n;
	ActionScript::AddThingToMapUnique(this);
}

unsigned short Item::getUniqueId() const
{
	return uniqueId;
}

long Item::getDecayTime()
{
	return items[id].decayTime * 1000;
}

bool Item::unserialize(xmlNodePtr nodeItem)
{
	char* nodeValue;
	nodeValue = (char*)xmlGetProp(nodeItem, (const xmlChar *) "id");
	if(nodeValue){
		id = atoi(nodeValue);
		xmlFreeOTSERV(nodeValue);
	}
	else
		return false;

	nodeValue = (char*)xmlGetProp(nodeItem, (const xmlChar *) "count");
	if(nodeValue){
		setItemCountOrSubtype(atoi(nodeValue));
		xmlFreeOTSERV(nodeValue);
	}

	nodeValue = (char*)xmlGetProp(nodeItem, (const xmlChar *) "special_description");
	if(nodeValue){
		specialDescription = new std::string(nodeValue);
		xmlFreeOTSERV(nodeValue);
	}

	nodeValue = (char*)xmlGetProp(nodeItem, (const xmlChar *) "text");
	if(nodeValue){
		text = new std::string(nodeValue);
		xmlFreeOTSERV(nodeValue);
	}

	nodeValue = (char*)xmlGetProp(nodeItem, (const xmlChar *) "actionId");
	if(nodeValue){
		setActionId(atoi(nodeValue));
		xmlFreeOTSERV(nodeValue);
	}

	nodeValue = (char*)xmlGetProp(nodeItem, (const xmlChar *) "uniqueId");
	if(nodeValue){
		setUniqueId(atoi(nodeValue));
		xmlFreeOTSERV(nodeValue);
	}
#ifdef YUR_RINGS_AMULETS
   	nodeValue = (char*)xmlGetProp(nodeItem, (const xmlChar *) "charges");
	if(nodeValue){
		charges = atoi(nodeValue);
		xmlFreeOTSERV(nodeValue);
	}
   	nodeValue = (char*)xmlGetProp(nodeItem, (const xmlChar *) "time");
	if(nodeValue){
		time = atoi(nodeValue);
		xmlFreeOTSERV(nodeValue);
	}
#endif //YUR_RINGS_AMULETS

	return true;
}

xmlNodePtr Item::serialize()
{
	xmlNodePtr nodeItem = xmlNewNode(NULL,(const xmlChar*)"item");

	std::stringstream ss;
	ss.str("");
	ss << getID();
	xmlSetProp(nodeItem, (const xmlChar*)"id", (const xmlChar*)ss.str().c_str());

	if(hasSubType()){
		ss.str("");
		ss << (int)getItemCountOrSubtype();
		xmlSetProp(nodeItem, (const xmlChar*)"count", (const xmlChar*)ss.str().c_str());
	}

	if(specialDescription){
		ss.str("");
		ss << getSpecialDescription();
		xmlSetProp(nodeItem, (const xmlChar*)"special_description", (const xmlChar*)ss.str().c_str());
	}

	if(text){
		ss.str("");
		ss << getText();
		xmlSetProp(nodeItem, (const xmlChar*)"text", (const xmlChar*)ss.str().c_str());
	}

	if(!isNotMoveable() /*moveable*/){
		if(actionId != 0){
			ss.str("");
			ss << actionId;
			xmlSetProp(nodeItem, (const xmlChar*)"actionId", (const xmlChar*)ss.str().c_str());
		}
	}

	/*we are not saving unique ids
	if(uniqueId != 0){
		ss.str("");
		ss << uniqueId;
		xmlSetProp(nodeItem, (const xmlChar*)"uniqueId", (const xmlChar*)ss.str().c_str());
	}
	*/
#ifdef YUR_RINGS_AMULETS
	ss.str("");
	if(charges != 0){
		ss << charges;
		xmlSetProp(nodeItem, (const xmlChar*)"charges", (const xmlChar*)ss.str().c_str());
	}
	ss.str("");
	if (time != 0){
		ss << time;
		xmlSetProp(nodeItem, (const xmlChar*)"time", (const xmlChar*)ss.str().c_str());
	}
#endif //YUR_RINGS_AMULETS

	return nodeItem;
}

bool Item::readAttr(AttrTypes_t attr, PropStream& propStream)
{
	switch(attr){
		case ATTR_COUNT:
		{
			unsigned char _count = 0;
			if(!propStream.GET_UCHAR(_count)){
				return false;
			}

			setItemCountOrSubtype(_count);
			break;
		}

		case ATTR_ACTION_ID:
		{
			unsigned short _actionid = 0;
			if(!propStream.GET_USHORT(_actionid)){
				return false;
			}

			setActionId(_actionid);
			break;
		}

		case ATTR_UNIQUE_ID:
		{
			unsigned short _uniqueid;
			if(!propStream.GET_USHORT(_uniqueid)){
				return false;
			}

			setUniqueId(_uniqueid);
			break;
		}

		case ATTR_TEXT:
		{
			std::string _text;
			if(!propStream.GET_STRING(_text)){
				return false;
			}

			setText(_text);
			break;
		}

		case ATTR_DESC:
		{
			std::string _text;
			if(!propStream.GET_STRING(_text)){
				return false;
			}

			setSpecialDescription(_text);
			break;
		}

		case ATTR_RUNE_CHARGES:
		{
			unsigned char _charges = 1;
			if(!propStream.GET_UCHAR(_charges)){
				return false;
			}

			setItemCountOrSubtype(_charges);
			break;
		}

		//these should be handled through derived classes
		//If these are called then something has changed in the items.otb since the map was saved
		//just read the values

		//Depot class
		case ATTR_DEPOT_ID:
		{
			unsigned short _depotId;
			if(!propStream.GET_USHORT(_depotId)){
				return false;
			}

			return true;
		}

		//Door class
		case ATTR_HOUSEDOORID:
		{
			unsigned char _doorId;
			if(!propStream.GET_UCHAR(_doorId)){
				return false;
			}

			return true;
		}

		//Teleport class
		case ATTR_TELE_DEST:
		{
			TeleportDest* tele_dest;
			if(!propStream.GET_STRUCT(tele_dest)){
				return false;
			}

			return true;
		}

		default:
			return false;
		break;
	}

	return true;
}

bool Item::unserializeAttr(PropStream& propStream)
{
	unsigned char attr_type;
	while(propStream.GET_UCHAR(attr_type)){
		if(!readAttr((AttrTypes_t)attr_type, propStream)){
			return false;
			break;
		}
	}

	return true;
}

bool Item::unserializeItemNode(FileLoader& f, NODE node, PropStream& propStream)
{
	return unserializeAttr(propStream);
}

bool Item::serializeAttr(PropWriteStream& propWriteStream)
{
	if(isStackable() || isSplash() || isFluidContainer()){
		unsigned char _count = getItemCountOrSubtype();
		propWriteStream.ADD_UCHAR(ATTR_COUNT);
		propWriteStream.ADD_UCHAR(_count);
	}

	if(isRune()){
		unsigned char _count = getItemCharge();
		propWriteStream.ADD_UCHAR(ATTR_RUNE_CHARGES);
		propWriteStream.ADD_UCHAR(_count);
	}

	if(!isNotMoveable() /*moveable*/){
		if(actionId){
			unsigned short _actionId = getActionId();
			propWriteStream.ADD_UCHAR(ATTR_ACTION_ID);
			propWriteStream.ADD_USHORT(_actionId);
		}
	}

	/*we are not saving unique ids
	if(uniqueId){
		unsigned short _uniqueId = getUniqueId();
		propWriteStream.ADD_UCHAR(ATTR_UNIQUE_ID);
		propWriteStream.ADD_USHORT(_uniqueId);
	}
	*/

	const std::string& _text = getText();
	if(_text.length() > 0){
		propWriteStream.ADD_UCHAR(ATTR_TEXT);
		propWriteStream.ADD_STRING(_text);
	}

	const std::string& _specialDesc = getSpecialDescription();
	if(_specialDesc.length() > 0){
		propWriteStream.ADD_UCHAR(ATTR_DESC);
		propWriteStream.ADD_STRING(_specialDesc);
	}

	return true;
}

bool Item::hasProperty(enum ITEMPROPERTY prop) const
{
	const ItemType& it = items[id];

	switch(prop){
		case BLOCKSOLID:
			if(it.blockSolid)
				return true;
		break;

		case HASHEIGHT:
			if(it.hasHeight)
				return true;
		break;

		case BLOCKPROJECTILE:
			if(it.blockProjectile)
				return true;
		break;

		case BLOCKPATHFIND:
			if(it.blockPathFind)
				return true;
		break;

		case ISVERTICAL:
			if(it.isVertical)
				return true;
		break;

		case ISHORIZONTAL:
			if(it.isHorizontal)
				return true;
		break;
	}

	return false;
}

bool Item::isBlocking() const{
	const ItemType& it = items[id];
	return it.blockSolid;
}

bool Item::isStackable() const{
	return items[id].stackable;
}

bool Item::isRune() const{
	return (items[id].group == ITEM_GROUP_RUNE);
}

bool Item::isFluidContainer() const{
	return (items[id].isFluidContainer());
}

bool Item::isAlwaysOnTop() const{
	return items[id].alwaysOnTop;
}

bool Item::isNotMoveable() const{
	return !items[id].moveable;
}

bool Item::isGroundTile() const{
	return items[id].isGroundTile();
}

bool Item::isSplash() const{
	return items[id].isSplash();
}

bool Item::isMagicField() const{
	return items[id].isMagicField();
}

bool Item::isPickupable() const{
	return items[id].pickupable;
}

bool Item::isUseable() const{
	return items[id].useable;
}

bool Item::isHangable() const{
	return items[id].isHangable;
}

bool Item::isRoteable() const{
	const ItemType& it = items[id];
	return it.rotable && it.rotateTo;
}

bool Item::isDoor() const
{
	const ItemType& it = items[id];
	return it.isDoor();
}

bool Item::floorChangeDown() const{
	return items[id].floorChangeDown;
}

bool Item::floorChangeNorth() const{
	return items[id].floorChangeNorth;
}
bool Item::floorChangeSouth() const{
	return items[id].floorChangeSouth;
}
bool Item::floorChangeEast() const{
	return items[id].floorChangeEast;
}
bool Item::floorChangeWest() const{
	return items[id].floorChangeWest;
}

bool Item::isWeapon() const{
  //now also returns true on SHIELDS!!! Check back with getWeaponType!
  //old: return (items[id].weaponType != NONE && items[id].weaponType != SHIELD && items[id].weaponType != AMO);
  return (items[id].weaponType != NONE && items[id].weaponType != AMO);
}

WeaponType Item::getWeaponType() const {
	  return items[id].weaponType;
}

amu_t Item::getAmuType() const{
	 return items[id].amuType;
}

subfight_t Item::getSubfightType() const {
	return items[id].shootType;
}

int Item::getAttack() const {
	  return items[id].attack;
}

int Item::getArmor() const {
	  return items[id].armor;
}

int Item::getDefense() const {
	  return items[id].defence;
}

int Item::getSlotPosition() const {
	return items[id].slot_position;
}

double Item::getWeight() const {
	if(isStackable()){
		return items[id].weight * std::max(1, (int)count);
	}

	return items[id].weight;
}

std::string Item::getDescription(int32_t lookDistance) const
{
	std::stringstream s;
	const ItemType& it = items[id];

	if (it.name.length()) {
		if(isStackable() && count > 1){
			s << (int)count << " " << it.name << "s.";

			if(lookDistance <= 1) {
				s << std::endl << "They weight " << std::fixed << std::setprecision(2) << ((double) count * it.weight) << " oz.";
			}
		}
		else{
			if(items[id].runeMagLevel != -1)
			{
				s << "a spell rune for level " << it.runeMagLevel << "." << std::endl;

				s << "It's an \"" << it.name << "\" spell (";
				if(getItemCharge())
					s << (int)getItemCharge();
				else
					s << "1";

				s << "x).";
			}
			else if(isWeapon() && (getAttack() || getDefense()))
			{
				if(getAttack()){
					s << "a " << it.name << " (Atk:" << (int)getAttack() << " Def:" << (int)getDefense() << ").";
				}
				else{
					s << "a " << it.name << " (Def:" << (int)getDefense() << ").";
				}
			}
			else if(getArmor()){
				s << "a " << it.name << " (Arm:" << (int)getArmor() << ").";
			}
			else if(isFluidContainer()){
				s << "a " << it.name;
				if(fluid == 0){
					s << ". It is empty.";
				}
				else{
					s << " of " << items[fluid].name << ".";
				}
			}
			else if(isSplash()){
				s << "a " << it.name << " of ";
				if(fluid == 0){
					s << items[1].name << ".";
				}
				else{
					s << items[fluid].name << ".";
				}
			}
			else if(it.isKey()){
				s << "a " << it.name << " (Key:" << actionId << ").";
			}
#ifdef YUR_RINGS_AMULETS
			else if (charges)
			{
				s << it.name << ". ";
				if (charges == 1)
					s << "\nIt has 1 charge left";
				else
					s << "\nIt has " << charges << " charges left";
			}
			else if (time)
			{
				s << it.name << ". ";
				if (time < 60*1000)
					s << "\nIt has less than a minute left";
				else if (time == items[id].newTime)
					s << "\nIt is brand new";
				else
					s << "\nIt has " << (int)ceil(time/(60.0*1000.0)) << " minutes left";
			}
#endif //YUR_RINGS_AMULETS

			else if(it.isGroundTile()){
				s << it.name << ".";
			}
			else if(it.isContainer()){
				s << "a " << it.name << " (Vol:" << getContainer()->capacity() << ").";
			}
			else if(it.allowDistRead){
				s << it.name << "." << std::endl;

				if(lookDistance <= 4){
					if(text && text->length() > 0){
						s << "You read: " << *text;
					}
					else
						s << "Nothing is written on it.";
				}
				else
					s << "You are too far away to read it.";
			}
			else{
				s << "a " << it.name << ".";
			}

			if(lookDistance <= 1){
				double weight = getWeight();
				if(weight > 0)
					s << std::endl << "It weighs " << std::fixed << std::setprecision(2) << weight << " oz.";
			}

			if(specialDescription)
				s << std::endl << specialDescription->c_str();
			else if(lookDistance <= 1 && it.description.length()){
				s << std::endl << it.description;
			}
		}
	}
	else
		s << "an item of type " << id <<".";

	return s.str();
}

std::string Item::getWeightDescription() const
{
	double weight = getWeight();

	std::stringstream ss;
	if(weight > 0){
		ss << " It weighs " << std::fixed << std::setprecision(2) << weight << " oz.";
	}

	return ss.str();
}

std::string Item::getName() const
{
	return items[id].name;
}



void Item::setSpecialDescription(const std::string& desc){
	if(specialDescription){
		delete specialDescription;
		specialDescription = NULL;
	}
	if(desc.length() > 1)
		specialDescription = new std::string(desc);
}

std::string Item::getSpecialDescription()
{
	if(!specialDescription)
		return std::string("");
	return *specialDescription;
}

void Item::clearSpecialDescription(){
	if(specialDescription)
		delete specialDescription;
	specialDescription = NULL;
}

void Item::setText(const std::string& desc)
{
	if(text){
//        std::cout << "IT HAS TEXT";
		delete text;
		text = NULL;
	}

	if(desc.length() > 1){
		text = new std::string(desc);
	}
}

void Item::clearText()
{
	if(text){
		delete text;
		text = NULL;
	}
}

std::string Item::getText()
{
	if(!text)
		return std::string("");

	return *text;
}

int Item::getRWInfo(int& maxlen) const
{
	const ItemType& it = items[id];
	maxlen = it.maxTextLen;
	return it.RWInfo;
}

bool Item::canDecay()
{
	if(isRemoved())
		return false;

	return items[id].canDecay;
}

int Item::getWorth() const
{
	switch(getID()){
	case ITEM_COINS_GOLD:
		return getItemCount();
	case ITEM_COINS_PLATINUM:
		return getItemCount() * 100;
	case ITEM_COINS_CRYSTAL:
		return getItemCount() * 10000;
	default:
		return 0;
	}
}

void Item::getLight(LightInfo& lightInfo)
{
	const ItemType& it = items[id];
	lightInfo.color = it.lightColor;
	lightInfo.level = it.lightLevel;
}


#ifdef YUR_RINGS_AMULETS
void Item::setGlimmer()
{
	switch (getID())
	{
		case ITEM_TIME_RING: setID(ITEM_TIME_RING_IN_USE); break;
		case ITEM_SWORD_RING: setID(ITEM_SWORD_RING_IN_USE); break;
		case ITEM_AXE_RING: setID(ITEM_AXE_RING_IN_USE); break;
		case ITEM_CLUB_RING: setID(ITEM_CLUB_RING_IN_USE); break;
		case ITEM_POWER_RING: setID(ITEM_POWER_RING_IN_USE); break;
		case ITEM_ENERGY_RING: setID(ITEM_ENERGY_RING_IN_USE); break;
		case ITEM_STEALTH_RING: setID(ITEM_STEALTH_RING_IN_USE); break;
		#ifdef RULY_LIFE_RINGS
        case ITEM_LIFE_RING: setID(ITEM_LIFE_RING_IN_USE); break;
        case ITEM_RING_OF_HEALING: setID(ITEM_RING_OF_HEALING_IN_USE); break;
        #endif
	}
}

void Item::removeGlimmer()
{
	switch (getID())
	{
		case ITEM_TIME_RING_IN_USE: setID(ITEM_TIME_RING); break;
		case ITEM_SWORD_RING_IN_USE: setID(ITEM_SWORD_RING); break;
		case ITEM_AXE_RING_IN_USE: setID(ITEM_AXE_RING); break;
		case ITEM_CLUB_RING_IN_USE: setID(ITEM_CLUB_RING); break;
		case ITEM_POWER_RING_IN_USE: setID(ITEM_POWER_RING); break;
		case ITEM_ENERGY_RING_IN_USE: setID(ITEM_ENERGY_RING); break;
		case ITEM_STEALTH_RING_IN_USE: setID(ITEM_STEALTH_RING); break;
        #ifdef RULY_LIFE_RINGS
        case ITEM_RING_OF_HEALING_IN_USE: setID(ITEM_RING_OF_HEALING); break;
        case ITEM_LIFE_RING_IN_USE: setID(ITEM_LIFE_RING); break;
        #endif
	}
}
#endif //YUR_RINGS_AMULETS
