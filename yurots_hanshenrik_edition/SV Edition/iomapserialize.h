//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
// Base class for the map serialization
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


#ifndef __OTSERV_IOMAPSERIALIZE_H__
#define __OTSERV_IOMAPSERIALIZE_H__

#include <string>

enum MapError_t{
	LOADMAPERROR_NONE,
	LOADMAPERROR_CANNOTOPENFILE,
	LOADMAPERROR_GETPROPFAILED,
	LOADMAPERROR_OUTDATEDHEADER,
	LOADMAPERROR_GETROOTHEADERFAILED,
	LOADMAPERROR_FAILEDTOCREATEITEM,
	LOADMAPERROR_FAILEDUNSERIALIZEITEM,
	LOADMAPERROR_FAILEDTOREADCHILD,
	LOADMAPERROR_UNKNOWNNODETYPE
};

class Map;

class IOMapSerialize{
public:
	static IOMapSerialize* getInstance();

	/** Load the map from a file/database
	  * \param map pointer to the Map class
	  * \param identifier is the mapfile/database to open
	  * \returns Returns true if the map was loaded successfully
	*/
	virtual bool loadMap(Map* map, const std::string& identifier) = 0;

	/** Save the map to a file/database
	  * \param map pointer to the Map class
	  * \param identifier is the mapfile/database to open
	  * \returns Returns true if the map was saved successfully
	*/
	virtual bool saveMap(Map* map, const std::string& identifier) = 0;

	/** Load the house access list to a file/database
	  * \param map pointer to the Map class
	  * \param identifier is the house access file/database to open
	  * \returns Returns true if the house access list was opened successfully
	*/
	virtual bool loadHouseInfo(Map* map, const std::string& identifier) = 0;

	/** Save the house access list to a file/database
	  * \param map pointer to the Map class
	  * \param identifier is the house access file/database to open
	  * \returns Returns true if the house access list was saved successfully
	*/
	virtual bool saveHouseInfo(Map* map, const std::string& identifier) = 0;

protected:
	static IOMapSerialize* _instance;

	IOMapSerialize(){};
	virtual ~IOMapSerialize(){};
};

#endif
