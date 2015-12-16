//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
// Represents an item
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

#include "definitions.h"
#include "item.h"
#include "container.h"
#include "depot.h"
#include "teleport.h"
#include "trashholder.h"
#include "mailbox.h"
#include "house.h"
#include "game.h"
#include "luascript.h"

#include "actions.h"
#include "combat.h"
#include "weapons.h"

#include <iostream>
#include <sstream>
#include <iomanip>

extern Game g_game;
extern Weapons* g_weapons;

Items Item::items;

Item* Item::CreateItem(const unsigned short _type, unsigned short _count /*= 1*/)
{
	Item* newItem = NULL;

	const ItemType& it = Item::items[_type];

	if(it.id != 0){
		if(it.isDepot()){
			newItem = new Depot(_type);
		}
		else if(it.isContainer()){
			newItem = new Container(_type);
		}
		else if(it.isTeleport()){
			newItem = new Teleport(_type);
		}
		else if(it.isMagicField()){
			newItem = new MagicField(_type);
		}
		else if(it.isDoor()){
			newItem = new Door(_type);
		}
		else if(it.isTrashHolder()){
			newItem = new TrashHolder(_type, it.magicEffect);
		}
		else if(it.isMailbox()){
			newItem = new Mailbox(_type);
		}
		else{
			newItem = new Item(_type, _count);
		}

		newItem->useThing2();
	}
	return newItem;
}

Item* Item::CreateItem(PropStream& propStream)
{
	uint16_t _id;
	if(!propStream.GET_USHORT(_id)){
		return NULL;
	}

	const ItemType& iType = Item::items[_id];
	unsigned char _count = 0;

	if(iType.stackable || iType.isSplash() || iType.isFluidContainer()){
		if(!propStream.GET_UCHAR(_count)){
			return NULL;
		}
	}

	return Item::CreateItem(_id, _count);
}

Item::Item(const unsigned short _type, unsigned short _count /*= 0*/) :
	ItemAttributes()
{
	id = _type;

	const ItemType& it = items[id];

	count = 1;
	charges = it.charges;
	fluid = 0;

	if(it.isFluidContainer() || it.isSplash()){
		fluid = _count;
	}
	else if(it.stackable && _count != 0){
		count = _count;
	}
	else if(it.charges != 0 && _count != 0){
		charges = _count;
	}

	setDefaultDuration();
}

Item::Item(const Item &i) :
	Thing(), ItemAttributes()
{
	//std::cout << "Item copy constructor " << this << std::endl;
	id = i.id;
	count = i.count;
	charges = i.charges;
	fluid = i.fluid;

	uint16_t _actionId;
	if((_actionId = i.getActionId())){
		setActionId(_actionId);
	}

	uint16_t _uniqueId;
	if((_uniqueId = i.getUniqueId())){
		setUniqueId(_uniqueId);
	}

	if(i.getSpecialDescription() != ""){
		setSpecialDescription(i.getSpecialDescription());
	}

	if(i.getText() != ""){
		setText(i.getText());
	}

	uint32_t _writtenDate;
	if((_writtenDate = i.getWrittenDate())){
		setWrittenDate(_writtenDate);
	}

	if(i.getWriter() != ""){
		setWriter(i.getWriter());
	}

	uint32_t _owner;
	if((_owner = i.getOwner())){
		setOwner(_owner);
	}

	uint32_t _duration;
	if((_duration = i.getDuration())){
		setDuration(_duration);
	}

	uint32_t _decayState;
	if((_decayState = i.getDecaying())){
		setDecaying((ItemDecayState_t)_decayState);
	}
}

Item::~Item()
{
	//std::cout << "Item destructor " << this << std::endl;
	//
}

void Item::setID(uint16_t newid)
{
	const ItemType& prevIt = Item::items[id];
	id = newid;

	const ItemType& it = Item::items[newid];
	uint32_t newDuration = it.decayTime * 1000;

	if(newDuration == 0 && !it.stopTime && it.decayTo == -1){
		removeAttribute(ATTR_ITEM_DECAYING);
		removeAttribute(ATTR_ITEM_DURATION);
	}

	if(newDuration > 0 && (!prevIt.stopTime || !hasAttribute(ATTR_ITEM_DURATION)) ){
		setDecaying(DECAYING_FALSE);
		setDuration(newDuration);
	}
}

uint8_t Item::getItemCountOrSubtype() const
{
	const ItemType& it = items[getID()];

	if(it.isFluidContainer() || it.isSplash()){
		return fluid;
	}
	else if(it.charges != 0){
		return charges;
	}
	else{
		return count;
	}
}

void Item::setItemCountOrSubtype(unsigned char n)
{
	const ItemType& it = items[id];

	if(it.isFluidContainer() || it.isSplash()){
		fluid = n;
	}
	else if(it.charges != 0){
		charges = n;
	}
	else{
		count = n;
	}
}

bool Item::hasSubType() const
{
	const ItemType& it = items[id];
	return (it.isFluidContainer() || it.isSplash() || it.stackable || it.charges != 0);
}

uint8_t Item::getSubType() const
{
	const ItemType& it = items[getID()];

	if(it.isFluidContainer() || it.isSplash()){
		return fluid;
	}
	else if(it.charges != 0){
		return charges;
	}

	return 0;
}

bool Item::unserialize(xmlNodePtr nodeItem)
{
	int intValue;
	std::string strValue;

	if(readXMLInteger(nodeItem, "id", intValue)){
		id = intValue;
	}
	else{
		return false;
	}

	if(readXMLInteger(nodeItem, "count", intValue)){
		setItemCountOrSubtype(intValue);
	}

	if(readXMLString(nodeItem, "special_description", strValue)){
		setSpecialDescription(strValue);
	}

	if(readXMLString(nodeItem, "text", strValue)){
		setText(strValue);
	}

	if(readXMLInteger(nodeItem, "written_date", intValue)){
		setWrittenDate(intValue);
	}

	if(readXMLString(nodeItem, "writer", strValue)){
		setWriter(strValue);
	}

	if(readXMLInteger(nodeItem, "actionId", intValue)){
		setActionId(intValue);
	}

	if(readXMLInteger(nodeItem, "uniqueId", intValue)){
		setUniqueId(intValue);
	}

	if(readXMLInteger(nodeItem, "duration", intValue)){
		setDuration(intValue);
	}

	if(readXMLInteger(nodeItem, "decayState", intValue)){
		ItemDecayState_t decayState = (ItemDecayState_t)intValue;
		if(decayState != DECAYING_FALSE){
			setDecaying(DECAYING_PENDING);
		}
	}

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

	if(getSpecialDescription() != ""){
		ss.str("");
		ss << getSpecialDescription();
		xmlSetProp(nodeItem, (const xmlChar*)"special_description", (const xmlChar*)ss.str().c_str());
	}


	if(getText() != ""){
		ss.str("");
		ss << getText();
		xmlSetProp(nodeItem, (const xmlChar*)"text", (const xmlChar*)ss.str().c_str());
	}

	if(getWrittenDate() != 0){
		ss.str("");
		ss << getWrittenDate();
		xmlSetProp(nodeItem, (const xmlChar*)"written_date", (const xmlChar*)ss.str().c_str());
	}

	if(getWriter() != ""){
		ss.str("");
		ss << getWriter();
		xmlSetProp(nodeItem, (const xmlChar*)"writer", (const xmlChar*)ss.str().c_str());
	}

	if(!isNotMoveable() /*moveable*/){
		if(getActionId() != 0){
			ss.str("");
			ss << getActionId();
			xmlSetProp(nodeItem, (const xmlChar*)"actionId", (const xmlChar*)ss.str().c_str());
		}
	}

	if(hasAttribute(ATTR_ITEM_DURATION)){
		uint32_t duration = getDuration();
		ss.str("");
		ss << duration;
		xmlSetProp(nodeItem, (const xmlChar*)"duration", (const xmlChar*)ss.str().c_str());
	}

	uint32_t decayState = getDecaying();
	if(decayState == DECAYING_TRUE || decayState == DECAYING_PENDING){
		ss.str("");
		ss << decayState;
		xmlSetProp(nodeItem, (const xmlChar*)"decayState", (const xmlChar*)ss.str().c_str());
	}

	return nodeItem;
}

bool Item::readAttr(AttrTypes_t attr, PropStream& propStream)
{
	switch(attr){
		case ATTR_COUNT:
		{
			uint8_t _count = 0;
			if(!propStream.GET_UCHAR(_count)){
				return false;
			}

			setItemCountOrSubtype(_count);
			break;
		}

		case ATTR_ACTION_ID:
		{
			uint16_t _actionid = 0;
			if(!propStream.GET_USHORT(_actionid)){
				return false;
			}

			setActionId(_actionid);
			break;
		}

		case ATTR_UNIQUE_ID:
		{
			uint16_t _uniqueid;
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

		case ATTR_WRITTENDATE:
		{
			uint32_t _writtenDate;
			if(!propStream.GET_ULONG(_writtenDate)){
				return false;
			}

			setWrittenDate(_writtenDate);
			break;
		}

		case ATTR_WRITTENBY:
		{
			std::string _writer;
			if(!propStream.GET_STRING(_writer)){
				return false;
			}

			setWriter(_writer);
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
			uint8_t _charges = 1;
			if(!propStream.GET_UCHAR(_charges)){
				return false;
			}

			setItemCountOrSubtype(_charges);
			break;
		}

		case ATTR_DURATION:
		{
			uint32_t duration = 0;
			if(!propStream.GET_ULONG(duration)){
				return false;
			}
			setDuration(duration);
			break;
		}

		case ATTR_DECAYING_STATE:
		{
			uint8_t state = 0;
			if(!propStream.GET_UCHAR(state)){
				return false;
			}

			if(state != DECAYING_FALSE){
				setDecaying(DECAYING_PENDING);
			}
			break;
		}

		//these should be handled through derived classes
		//If these are called then something has changed in the items.otb since the map was saved
		//just read the values

		//Depot class
		case ATTR_DEPOT_ID:
		{
			uint16_t _depotId;
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
	/*
	if(isStackable() || isSplash() || isFluidContainer()){
		unsigned char _count = getItemCountOrSubtype();
		propWriteStream.ADD_UCHAR(ATTR_COUNT);
		propWriteStream.ADD_UCHAR(_count);
	}
	*/

	if(isRune()){
		unsigned char _count = getItemCharge();
		propWriteStream.ADD_UCHAR(ATTR_RUNE_CHARGES);
		propWriteStream.ADD_UCHAR(_count);
	}

	if(!isNotMoveable() /*moveable*/){
		unsigned short _actionId = getActionId();
		if(_actionId){
			propWriteStream.ADD_UCHAR(ATTR_ACTION_ID);
			propWriteStream.ADD_USHORT(_actionId);
		}
	}

	const std::string& _text = getText();
	if(_text.length() > 0){
		propWriteStream.ADD_UCHAR(ATTR_TEXT);
		propWriteStream.ADD_STRING(_text);
	}

	const time_t _writtenDate = getWrittenDate();
	if(_writtenDate > 0){
		propWriteStream.ADD_UCHAR(ATTR_WRITTENDATE);
		propWriteStream.ADD_ULONG(_writtenDate);
	}

	const std::string& _writer = getWriter();
	if(_writer.length() > 0){
		propWriteStream.ADD_UCHAR(ATTR_WRITTENBY);
		propWriteStream.ADD_STRING(_writer);
	}

	const std::string& _specialDesc = getSpecialDescription();
	if(_specialDesc.length() > 0){
		propWriteStream.ADD_UCHAR(ATTR_DESC);
		propWriteStream.ADD_STRING(_specialDesc);
	}

	if(hasAttribute(ATTR_ITEM_DURATION)){
		uint32_t duration = getDuration();
		propWriteStream.ADD_UCHAR(ATTR_DURATION);
		propWriteStream.ADD_ULONG(duration);
	}

	uint32_t decayState = getDecaying();
	if(decayState == DECAYING_TRUE || decayState == DECAYING_PENDING){
		propWriteStream.ADD_UCHAR(ATTR_DECAYING_STATE);
		propWriteStream.ADD_UCHAR(decayState);
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

		case MOVEABLE:
			if(it.moveable && getUniqueId() == 0)
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

		case BLOCKINGANDNOTMOVEABLE:
			if(it.blockSolid && (!it.moveable || getUniqueId() != 0))
				return true;
			break;
		
		case SUPPORTHANGABLE:
			if(it.isHorizontal || it.isVertical)
				return true;
			break;

		default:
			return false;
	}
	return false;
}

double Item::getWeight() const
{
	if(isStackable()){
		return items[id].weight * std::max(1, (int)count);
	}

	return items[id].weight;
}

std::string Item::getDescription(int32_t lookDistance) const
{
	std::stringstream s;
	const ItemType& it = items[id];

	if(it.name.length()){
		if(isStackable() && getItemCount() > 1){
			if(it.showCount){
				s << (int)getItemCount() << " ";
			}

			s << it.pluralName;
		}
		else{
			if(it.article.length()){
				s << it.article << " ";
			}
			s << it.name;
		}
	}
	else{
		s << "an item of type " << getID();
	}

	if(it.isRune()){
		s << " for magic level " << (int)it.runeMagLevel << "." << std::endl;
		s << "It's an \"" << it.runeSpellName << "\" spell(";
		if(getItemCharge()){
			s << (int)getItemCharge();
		}
		else{
			s << "1";
		}
		s << "x).";
	}
	else if(isWeapon())
	{
		if(getWeaponType() != WEAPON_AMMO){ // Arrows and Bolts doesn't show atk
			if(getAttack()){
				if(getExtraDef()){
					s << " (Atk:" << (int)getAttack() << " Def:" << (int)getDefense() << " " << std::showpos << (int)getExtraDef() << ")" << std::noshowpos;
				}
				else{
					s << " (Atk:" << (int)getAttack() << " Def:" << (int)getDefense() << ")";
				}
			}
			else if(getDefense()){
				s << std::endl << " (Def:" << (int)getDefense() << ")";
			}
		}
		s << ".";

		const Weapon* weapon = g_weapons->getWeapon(this);
		if(weapon && weapon->getWieldInfo()){
			const uint32_t wieldInfo = weapon->getWieldInfo();
			s << std::endl << "It can only be wielded properly by ";
			if(wieldInfo & WIELDINFO_PREMIUM){
				s << "premium ";
			}

			if(wieldInfo & WIELDINFO_VOCREQ){
				s << weapon->getVocationString();
			}
			else{
				s << "players";
			}

			if(wieldInfo & WIELDINFO_LEVEL){
				s << " of level " << (int)weapon->getReqLevel() << " or higher";
			}

			if(wieldInfo & WIELDINFO_MAGLV){
				if(wieldInfo & WIELDINFO_LEVEL){
					s << " and";
				}
				else{
					s << " of";
				}
				s << " magic level " << (int)weapon->getReqMagLv() << " or higher";
			}
			s << ".";
		}
	}
	else if(getArmor()){
		s << " (Arm:" << (int)getArmor() << ").";
	}
	else if(isFluidContainer()){
		if(fluid == 0){
			s << ". It is empty.";
		}
		else{
			s << " of " << items[fluid].name << ".";
		}
	}
	else if(isSplash()){
		s << " of ";
		if(fluid == 0){
			s << items[1].name << ".";
		}
		else{
			s << items[fluid].name << ".";
		}
	}
	else if(it.isKey()){
		s << " (Key:" << (int)getActionId() << ").";
	}
	else if(it.isContainer()){
		s << " (Vol:" << (int)getContainer()->capacity() << ").";
	}
	else if(it.allowDistRead){
		s << "." << std::endl;

		if(getText() != ""){
			if(lookDistance <= 4){
				if(getWriter().length()){
					s << getWriter() << " wrote";

					time_t wDate = getWrittenDate();
					if(wDate > 0){
						char date[16];
						formatDate2(wDate, date);
						s << " on " << date;
					}
					s << ": ";
				}
				else{
					s << "You read: ";
				}
				s << getText();
			}
			else{
				s << "You are too far away to read it.";
			}
		}
		else{
			s << "Nothing is written on it.";
		}
	}
	else if(it.showCharges){
		uint32_t charges = getItemCharge();
		if(charges > 1){
			s << " that has " << (int)charges << " charges left.";
		}
		else{
			s << " that has 1 charge left.";
		}
	}
	else if(it.showDuration){
		if(hasAttribute(ATTR_ITEM_DURATION)){
			uint32_t duration = getDuration() / 1000;
			s << " that has energy for ";

			if(duration >= 120){
				s << duration / 60 << " minutes left.";
			}
			else if(duration > 60){
				s << "1 minute left.";
			}
			else{
				s << " less than a minute left.";
			}
		}
		else{
			s << " that is brand-new.";
		}
	}
	else{
		s << ".";
	}

	if(lookDistance <= 1 ){
		double weight = getWeight();
		if(weight > 0){
			s << std::endl << getWeightDescription(weight);
		}
	}

	if(getSpecialDescription() != ""){
		s << std::endl << getSpecialDescription().c_str();
	}
	else if(it.description.length() && lookDistance <= 1){
		s << std::endl << it.description;
	}

	return s.str();
}

std::string Item::getWeightDescription() const
{
	double weight = getWeight();
	if(weight > 0){
		return getWeightDescription(weight);
	}
	else{
		return "";
	}
}

std::string Item::getWeightDescription(double weight) const
{
	std::stringstream ss;
	if(isStackable() && count > 1){
		ss << "They weigh " << std::fixed << std::setprecision(2) << weight << " oz.";
	}
	else{
		ss << "It weighs " << std::fixed << std::setprecision(2) << weight << " oz.";
	}
	return ss.str();
}

void Item::setUniqueId(unsigned short n)
{
	if(getUniqueId() != 0)
		return;

	ItemAttributes::setUniqueId(n);
	ScriptEnviroment::addUniqueThing(this);
}

bool Item::canDecay()
{
	if(isRemoved()){
		return false;
	}

	if(getUniqueId() != 0 || getActionId() != 0){
		return false;
	}

	return (items[id].decayTo != -1);
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

std::string ItemAttributes::emptyString("");

const std::string& ItemAttributes::getStrAttr(itemAttrTypes type) const
{
	if(!validateStrAttrType(type))
		return emptyString;

	//this will *NOT* create the attribute if it does not exist
	Attribute* attr = getAttrConst(type);
	if(attr){
		return *(std::string*)attr->value;
	}
	else{
		return emptyString;
	}
}

void ItemAttributes::setStrAttr(itemAttrTypes type, const std::string& value)
{
	if(!validateStrAttrType(type))
		return;

	if(value.length() == 0)
		return;

	//this will create the attribute if it does not exist
	Attribute* attr = getAttr(type);
	if(attr){
		//if has a previous value delete it
		if(attr->value){
			delete (std::string*)attr->value;
		}
		//create the new value as string
		attr->value = (void*)new std::string(value);
	}
}

bool ItemAttributes::hasAttribute(itemAttrTypes type) const
{
	if(!validateIntAttrType(type))
		return false;

	Attribute* attr = getAttrConst(type);
	if(attr){
		return true;
	}

	return false;
}

void ItemAttributes::removeAttribute(itemAttrTypes type)
{
	//check if we have it
	if((type & m_attributes) != 0){
		//go trough the linked list until find it
		Attribute* prevAttr = NULL;
		Attribute* curAttr = m_firstAttr;
		while(curAttr != NULL){
			if(curAttr->type == type){
				//found so remove it from the linked list
				if(prevAttr){
					prevAttr->next = curAttr->next;
				}
				else{
					m_firstAttr = curAttr->next;
				}
				//remove it from flags
				m_attributes = m_attributes & ~type;

				//delete string if it is string type
				if(validateStrAttrType(type)){
					delete (std::string*)curAttr->value;
				}
				//finally delete the attribute and return
				delete curAttr;
				return;
			}

			//advance in the linked list
			prevAttr = curAttr;
			curAttr = curAttr->next;
		}
	}
}

uint32_t ItemAttributes::getIntAttr(itemAttrTypes type) const
{
	if(!validateIntAttrType(type))
		return 0;

	Attribute* attr = getAttrConst(type);
	if(attr){
		return (uint32_t)(long)attr->value;
	}
	else{
		return 0;
	}
}

void ItemAttributes::setIntAttr(itemAttrTypes type, int32_t value)
{
	if(!validateIntAttrType(type))
		return;

	Attribute* attr = getAttr(type);
	if(attr){
		attr->value = (void*)value;
	}
}

void ItemAttributes::increaseIntAttr(itemAttrTypes type, int32_t value)
{
	if(!validateIntAttrType(type))
		return;

	Attribute* attr = getAttr(type);
	if(attr){
		attr->value = (void*)((long)attr->value + value);
	}
}

inline bool ItemAttributes::validateIntAttrType(itemAttrTypes type) const
{
	//list of numeric type attributes
	switch(type){
	case ATTR_ITEM_ACTIONID:
	case ATTR_ITEM_UNIQUEID:
	case ATTR_ITEM_OWNER:
	case ATTR_ITEM_DURATION:
	case ATTR_ITEM_DECAYING:
	case ATTR_ITEM_WRITTENDATE:
		return true;
		break;

	default:
		return false;
		break;
	}
	return false;
}

inline bool ItemAttributes::validateStrAttrType(itemAttrTypes type) const
{
	//list of text type attributes
	switch(type){
	case ATTR_ITEM_DESC:
	case ATTR_ITEM_TEXT:
	case ATTR_ITEM_WRITTENBY:
		return true;
		break;
	default:
		return false;
		break;
	}
	return false;
}

void ItemAttributes::addAttr(Attribute* attr)
{
	//add an attribute to the linked list
	if(m_firstAttr){
		//is not the first, so look for the end of the list
		Attribute* curAttr = m_firstAttr;
		while(curAttr->next){
			curAttr = curAttr->next;
		}
		//and add it at the end
		curAttr->next = attr;
	}
	else{
		//is the first
		m_firstAttr = attr;
	}
	//add it to flags
	m_attributes = m_attributes | attr->type;
}

ItemAttributes::Attribute* ItemAttributes::getAttrConst(itemAttrTypes type) const
{
	//check flags
	if((type & m_attributes) == 0){
		return NULL;
	}
	//it is here, so search it in the linked list
	Attribute* curAttr = m_firstAttr;
	while(curAttr){
		if(curAttr->type == type){
			//found
			return curAttr;
		}
		curAttr = curAttr->next;
	}
	//not found?
	std::cout << "Warning: [ItemAttributes::getAttrConst] (type & m_attributes) != 0 but attribute not found" << std::endl;
	return NULL;
}

ItemAttributes::Attribute* ItemAttributes::getAttr(itemAttrTypes type)
{
	Attribute* curAttr;
	if((type & m_attributes) == 0){
		//if that type was not present add it
		curAttr = new Attribute(type);
		addAttr(curAttr);
		return curAttr;
	}
	else{
		//was present, search and return it
		curAttr = m_firstAttr;
		while(curAttr){
			if(curAttr->type == type){
				return curAttr;
			}
			curAttr = curAttr->next;
		}
	}
	std::cout << "Warning: [ItemAttributes::getAttr] (type & m_attributes) != 0 but attribute not found" << std::endl;
	curAttr = new Attribute(type);
	addAttr(curAttr);
	return curAttr;
}

void ItemAttributes::deleteAttrs(Attribute* attr)
{
	//deletes all attributes recursively
	if(attr){
		//if is string type, delete the allocated string
		if(validateStrAttrType(attr->type)){
			delete (std::string*)attr->value;
		}
		Attribute* next_attr = attr->next;
		attr->next = NULL;
		//delete next while it was not NULL
		if(next_attr){
			deleteAttrs(next_attr);
		}
		delete attr;
	}
}

void Item::__startDecaying()
{
	g_game.startDecay(this);
}
