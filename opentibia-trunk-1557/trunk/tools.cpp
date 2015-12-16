//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
// Various functions.
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

#include "tools.h"

bool fileExists(const char* filename)
{
	FILE* f = fopen(filename, "rb");
	bool exists = (f != NULL);
	if(f != NULL)
		fclose(f);

	return exists;
}

void replaceString(std::string& str, const std::string sought, const std::string replacement)
{
	int pos = 0;
	int start = 0;
	int soughtLen = sought.length();
	int replaceLen = replacement.length();
	while((pos = str.find(sought, start)) != std::string::npos){
		str = str.substr(0, pos) + replacement + str.substr(pos + soughtLen);
		start = pos + replaceLen;
	}
}

void trim_right(std::string &source, const std::string &t)
{
	source.erase(source.find_last_not_of(t)+1);
}

void trim_left(std::string &source, const std::string &t)
{
	source.erase(0, source.find_first_not_of(t));
}

void toLowerCaseString(std::string& source)
{
	std::transform(source.begin(), source.end(), source.begin(), tolower);
}

bool readXMLInteger(xmlNodePtr node, const char* tag, int& value)
{
	char* nodeValue = (char*)xmlGetProp(node, (xmlChar*)tag);
	if(nodeValue){
		value = atoi(nodeValue);
		xmlFreeOTSERV(nodeValue);
		return true;
	}

	return false;
}

bool readXMLString(xmlNodePtr node, const char* tag, std::string& value)
{
	char* nodeValue = (char*)xmlGetProp(node, (xmlChar*)tag);
	if(nodeValue){
		value = nodeValue;
		xmlFreeOTSERV(nodeValue);
		return true;
	}

	return false;
}

int random_range(int lowest_number, int highest_number)
{
	if(lowest_number > highest_number){
		int nTmp = highest_number;
		highest_number = lowest_number;
		lowest_number = nTmp;
	}

	double range = highest_number - lowest_number + 1;
	return lowest_number + int(range * rand()/(RAND_MAX + 1.0));
}

// dump a part of the memory to stderr.
void hexdump(unsigned char *_data, int _len) {
	int i;
	for(; _len > 0; _data += 16, _len -= 16) {
		for (i = 0; i < 16 && i < _len; i++)
			fprintf(stderr, "%02x ", _data[i]);
		for(; i < 16; i++)
			fprintf(stderr, "   ");
		
		fprintf(stderr, " ");
		for(i = 0; i < 16 && i < _len; i++)
			fprintf(stderr, "%c", (_data[i] & 0x70) < 32 ? '�' : _data[i]);
		
		fprintf(stderr, "\n");
	}
}

// Upcase a char.
char upchar(char c) {
  if (c >= 'a' && c <= 'z')
      return c - 'a' + 'A';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else if (c == '�')
      return '�';
  else
      return c;
}
