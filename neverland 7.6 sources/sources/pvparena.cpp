//////////////////////////////////////////////////////////////////////
// Neverland - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
// PVP arena by Yurez
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
#ifdef YUR_PVP_ARENA
#include "pvparena.h"
#include "game.h"
#include "luascript.h"
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

extern LuaScript g_config;
extern xmlMutexPtr xmlmutex;

Position PvpArena::exit;

bool PvpArena::Load(Game* game)
{
	std::string file = g_config.getGlobalString("datadir") + "pvparena.xml";
	xmlDocPtr doc;
	xmlMutexLock(xmlmutex);

	doc = xmlParseFile(file.c_str());
	if (!doc)
		return false;

	xmlNodePtr root, tileNode;
	root = xmlDocGetRootElement(doc);
	if (xmlStrcmp(root->name, (const xmlChar*)"pvparena")) 
	{
		xmlFreeDoc(doc);
		xmlMutexUnlock(xmlmutex);
		return false;
	}

	exit.x = atoi((const char*)xmlGetProp(root, (const xmlChar *) "exitx"));
	exit.y = atoi((const char*)xmlGetProp(root, (const xmlChar *) "exity"));
	exit.z = atoi((const char*)xmlGetProp(root, (const xmlChar *) "exitz"));

	tileNode = root->children;
	while (tileNode)
	{
		if (strcmp((char*) tileNode->name, "tile") == 0)
		{
			Position pos;
			pos.x = atoi((const char*)xmlGetProp(tileNode, (const xmlChar *) "x"));
			pos.y = atoi((const char*)xmlGetProp(tileNode, (const xmlChar *) "y"));
			pos.z = atoi((const char*)xmlGetProp(tileNode, (const xmlChar *) "z"));

			Tile* tile = game->getTile(pos.x, pos.y, pos.z);
			if (tile)
				tile->setPvpArena(exit);
		}
		tileNode = tileNode->next;
	}

	xmlFreeDoc(doc);
	xmlMutexUnlock(xmlmutex);
	return true;
}

Position PvpArena::GetExit()
{
	return exit;
}
#endif //YUR_PVP_ARENA
