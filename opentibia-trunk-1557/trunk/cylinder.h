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

#ifndef __OTSERV_CYLINDER_H__
#define __OTSERV_CYLINDER_H__

#include "definitions.h"
#include "thing.h"

class Item;
class Creature;

#define INDEX_WHEREEVER -1

enum cylinderflags_t {
	FLAG_NOLIMIT = 1,
	FLAG_CHILDISOWNER = 2
};

class Cylinder : virtual public Thing{
public:	
	/**
	  * Query if the cylinder can add an object
	  * \param index points to the destination index (inventory slot/container position)
		* -1 is a internal value and means add to a empty position, with no destItem
	  * \param thing the object to move/add
	  * \param count is the amount that we want to move/add
	  * \param flags if FLAG_CHILDISOWNER if set the query is from a child-cylinder (check cap etc.)
		* if FLAG_NOLIMIT is set blocking items/container limits is ignored
	  * \returns ReturnValue holds the return value
	  */
	virtual ReturnValue __queryAdd(int32_t index, const Thing* Item, uint32_t count,
		uint32_t flags) const = 0;

	/**
	  * Query the cylinder how much it can accept
	  * \param index points to the destination index (inventory slot/container position)
		* -1 is a internal value and means add to a empty position, with no destItem
	  * \param item the object to move/add
	  * \param count is the amount that we want to move/add
	  * \param maxQueryCount is the max amount that the cylinder can accept
	  * \param flags optional flags to modifiy the default behaviour
	  * \returns ReturnValue holds the return value
	  */
	virtual ReturnValue __queryMaxCount(int32_t index, const Thing* thing, uint32_t count, uint32_t& maxQueryCount,
		uint32_t flags) const = 0;

	/**
	  * Query if the cylinder can remove an object
	  * \param item the object to move/remove
	  * \param count is the amount that we want to remove
	  * \returns ReturnValue holds the return value
	  */
	virtual ReturnValue __queryRemove(const Thing* thing, uint32_t count) const = 0;

	/**
	  * Query the destination cylinder
	  * \param index points to the destination index (inventory slot/container position),
		* -1 is a internal value and means add to a empty position, with no destItem
		* this method can change the index to point to the new cylinder index
	  * \param destItem is the destination object
	  * \param flags optional flags to modifiy the default behaviour
		* this method can modifiy the flags
	  * \returns Cylinder returns the destination cylinder
	  */
	virtual Cylinder* __queryDestination(int32_t& index, const Thing* thing, Item** destItem,
		uint32_t& flags) = 0;

	/**
	  * Add the object to the cylinder
	  * \param item is the object to add
	  */
	virtual void __addThing(Thing* thing) = 0;

	/**
	  * Add the object to the cylinder
	  * \param index points to the destination index (inventory slot/container position)
	  * \param item is the object to add
	  */
	virtual void __addThing(int32_t index, Thing* thing) = 0;

	/**
	  * Update the item count or type for an object
	  * \param thing is the object to update
	  * \param count is the new count value
	  */
	virtual void __updateThing(Thing* thing, uint32_t count) = 0;

	/**
	  * Replace an object with a new
	  * \param index is the position to change (inventory slot/container position)
	  * \param thing is the object to update
	  */
	virtual void __replaceThing(uint32_t index, Thing* thing) = 0;

	/**
	  * Remove an object
	  * \param thing is the object to delete
	  * \param count is the new count value
	  */
	virtual void __removeThing(Thing* thing, uint32_t count) = 0;

	/**
	  * Is sent after an operation (move/add) to update internal values
	  * \param thing is the object that has been added
	  * \param hasOwnership if this value is true the cylinder (or its children) has added the object to itself
		* otherwise another cylinder (like Tile class and wish to inform this change) has sent the message.
	  */
	virtual void postAddNotification(Thing* thing, bool hasOwnership = true) = 0;

	/**
	  * Is sent after an operation (move/remove) to update internal values
	  * \param thing is the object that has been removed
	  * \param isCompleteRemoval indicates if the item was completely removed or just partially (stackables)
	  * \param hadOwnership if this value is true the cylinder (or its children) has removed the object from itself
		* otherwise another cylinder (like Tile class and wish to inform this change) has sent the message.
	  */
	virtual void postRemoveNotification(Thing* thing, bool isCompleteRemoval, bool hadOwnership = true) = 0;

	/**
	  * Gets the index of an object
	  * \param thing the object to get the index value from
	  * \returns the index of the object, returns -1 if not found
	  */
	virtual int32_t __getIndexOfThing(const Thing* thing) const;

	/**
	  * Returns the first index
	  * \returns the first index, if not implemented -1 is returned
	  */
	virtual int32_t __getFirstIndex() const;

	/**
	  * Returns the last index
	  * \returns the last index, if not implemented -1 is returned
	  */
	virtual int32_t __getLastIndex() const;

	/**
	  * Gets the object based on index
	  * \returns the object, returns NULL if not found
	  */
	virtual Thing* __getThing(uint32_t index) const;
	
	/**
	  * Get the amount of items of a certain type
	  * \param itemId is the item type to the get the count of
	  * \param returns the amount of items of the asked item type
	  */
	virtual uint32_t __getItemTypeCount(uint16_t itemId) const;

	/**
	  * Adds an object to the cylinder without sending to the client(s)
	  * \param thing is the object to add
	  */
	virtual void __internalAddThing(Thing* thing);

	/**
	  * Adds an object to the cylinder without sending to the client(s)
	  * \param thing is the object to add
	  * \param index points to the destination index (inventory slot/container position)
	  */
	virtual void __internalAddThing(uint32_t index, Thing* thing);
};

#endif
