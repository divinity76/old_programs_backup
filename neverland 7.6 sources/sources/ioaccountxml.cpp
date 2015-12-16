//////////////////////////////////////////////////////////////////////
// Neverland - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
// Base class for the Account Loader/Saver
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

#include "ioaccountxml.h"
#include <algorithm>
#include <functional>
#include <sstream>
#include <string.h>
#include "luascript.h"

extern xmlMutexPtr xmlmutex;
extern LuaScript g_config; 

IOAccountXML::IOAccountXML(){
	if(xmlmutex == NULL){
		xmlmutex = xmlNewMutex();
	}
}

Account IOAccountXML::loadAccount(unsigned long accno){
	Account acc;

	std::stringstream accsstr;
	std::string datadir = g_config.getGlobalString("datadir");
	accsstr << datadir + "accounts/" << accno << ".xml";;
	std::string filename = accsstr.str();
	std::transform(filename.begin(), filename.end(), filename.begin(), tolower);
	xmlMutexLock(xmlmutex);
	xmlDocPtr doc = xmlParseFile(filename.c_str());
	if (doc)
	{
		xmlNodePtr root, p, tmp;
		root = xmlDocGetRootElement(doc);

		if (xmlStrcmp(root->name,(const xmlChar*) "account"))
		{
			xmlFreeDoc(doc);			
			xmlMutexUnlock(xmlmutex);
			return acc;
		}

		p = root->children;

		// perhaps verify name
		char* nodeValue = NULL;
		nodeValue = (char*)xmlGetProp(root, (const xmlChar *)"pass");
		acc.password  = nodeValue;
		xmlFreeNVLAND(nodeValue);

		nodeValue = (char*)xmlGetProp(root, (xmlChar*)"type");
		acc.accType  = atoi(nodeValue);
		xmlFreeNVLAND(nodeValue);

		nodeValue = (char*)xmlGetProp(root, (xmlChar*)"premDays");
		acc.premDays  = atoi(nodeValue);
		xmlFreeNVLAND(nodeValue);

#ifdef DT_PREMMY
      nodeValue = (char*)xmlGetProp(root, (xmlChar*)"lastsaveday");
        acc.lastsaveday  = atoi(nodeValue);
        xmlFreeNVLAND(nodeValue);

        struct tm * timeinfo;
        time_t nowTime;
        time(&nowTime);
        timeinfo = localtime(&nowTime);
        mktime(timeinfo);
        acc.lastsaveday2 = timeinfo->tm_yday;

#endif //DT_PREMMY

		// now load in characters.
		while (p)
		{
			const char* str = (char*)p->name;

			if (strcmp(str, "characters") == 0)
			{
				tmp = p->children;
				while(tmp)
				{
					nodeValue = (char*)xmlGetProp(tmp, (xmlChar*)"name");

					if(nodeValue) {
						if(strcmp((const char*)tmp->name, "character") == 0) {
							acc.charList.push_back(std::string(nodeValue));
						}

						xmlFreeNVLAND(nodeValue);
					}

					tmp = tmp->next;
				}
			}
			p = p->next;
		}
		
		xmlFreeDoc(doc);

		// Organize the char list.
		acc.charList.sort();
		acc.accnumber = accno;
	}	
	xmlMutexUnlock(xmlmutex);
	return acc;
}


bool IOAccountXML::getPassword(unsigned long accno, const std::string &name, std::string &password)
{
	std::string acc_password;
	
	std::stringstream accsstr;
	std::string datadir = g_config.getGlobalString("datadir");
	accsstr << datadir + "accounts/" << accno << ".xml";;
	std::string filename = accsstr.str();
	std::transform(filename.begin(), filename.end(), filename.begin(), tolower);
	
	xmlMutexLock(xmlmutex);
	xmlDocPtr doc = xmlParseFile(filename.c_str());
	if (doc)
	{
		xmlNodePtr root, p, tmp;
		root = xmlDocGetRootElement(doc);

		if (xmlStrcmp(root->name,(const xmlChar*) "account"))
		{
			xmlFreeDoc(doc);			
			xmlMutexUnlock(xmlmutex);
			return false;
		}

		p = root->children;

		char* nodeValue = NULL;
		nodeValue = (char*)xmlGetProp(root, (const xmlChar *)"pass");
		acc_password  = nodeValue;
		xmlFreeNVLAND(nodeValue);

		// now load in characters.
		while (p)
		{
			const char* str = (char*)p->name;

			if (strcmp(str, "characters") == 0)
			{
				tmp = p->children;
				while(tmp)
				{
					nodeValue = (char*)xmlGetProp(tmp, (xmlChar*)"name");

					if(nodeValue) {
						if(strcmp((const char*)tmp->name, "character") == 0) {
							if(nodeValue == name){
								password = acc_password;
								xmlFreeNVLAND(nodeValue);
								xmlFreeDoc(doc);
								xmlMutexUnlock(xmlmutex);
								return true;
							}
						}
						xmlFreeNVLAND(nodeValue);
					}

					tmp = tmp->next;
				}
			}
			p = p->next;
		}
		
		xmlFreeDoc(doc);
	}	
	xmlMutexUnlock(xmlmutex);
	return false;
}

#ifdef DT_PREMMY
bool IOAccountXML::saveAccount(Account acc)
{
    std::stringstream accsstr;
    std::string datadir = g_config.getGlobalString("datadir");
    accsstr << datadir + "accounts/" << acc.accnumber << ".xml";;
    std::string filename = accsstr.str();
    std::transform(filename.begin(), filename.end(), filename.begin(), tolower);
    std::stringstream sb;

    xmlDocPtr doc;
    xmlMutexLock(xmlmutex);
    xmlNodePtr nn, sn, pn, root;
    doc = xmlNewDoc((const xmlChar*)"1.0");
    doc->children = xmlNewDocNode(doc, NULL, (const xmlChar*)"account", NULL);
    root = doc->children;

    sb << acc.password;       xmlSetProp(root, (const xmlChar*) "pass",        (const xmlChar*)sb.str().c_str());     sb.str("");
    sb << acc.accType;        xmlSetProp(root, (const xmlChar*) "type",        (const xmlChar*)sb.str().c_str());     sb.str("");
    sb << acc.premDays;       xmlSetProp(root, (const xmlChar*) "premDays",    (const xmlChar*)sb.str().c_str());     sb.str("");
    sb << acc.lastsaveday2;    xmlSetProp(root, (const xmlChar*) "lastsaveday", (const xmlChar*)sb.str().c_str());     sb.str("");

    sn = xmlNewNode(NULL,(const xmlChar*)"characters");
    std::list<std::string>::iterator it;
    for (it = acc.charList.begin(); it != acc.charList.end(); it++)
    {
        pn = xmlNewNode(NULL,(const xmlChar*)"character");
        sb << (*it);       xmlSetProp(pn, (const xmlChar*) "name",     (const xmlChar*)sb.str().c_str());     sb.str("");
        xmlAddChild(sn, pn);
    }
    xmlAddChild(root, sn);

    if (xmlSaveFile(filename.c_str(), doc))
    {
        xmlFreeDoc(doc);
        xmlMutexUnlock(xmlmutex);
    }
    else
    {
        //std::cout << "\tCouldn't save account =(\n";
        xmlFreeDoc(doc);
        xmlMutexUnlock(xmlmutex);
        return false;
    }
}
#endif //DT_PREMMY
