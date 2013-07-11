#ifndef PARSEHOST_H
#define PARSEHOST_H

#include "Poco/Util/Util.h"
#include "Poco/Util/IniFileConfiguration.h"
#include "Poco/AutoPtr.h"
#include "Poco/Exception.h"
#include "Poco/String.h"
#include "Poco/StringTokenizer.h"
#include "Poco/RegularExpression.h"

#include "log.h"

#include <sstream>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>

using namespace std;

using Poco::Util::IniFileConfiguration;
using Poco::AutoPtr;
using Poco::StringTokenizer;
using Poco::NotImplementedException;
using Poco::NotFoundException;
using Poco::RegularExpression;
using Poco::RegularExpressionException;



#include "Poco/BinaryReader.h"

#define HOSTS "/etc/hosts"
#define HOSTDIR "hosts"
#define BAKDIR "hostbak"

using Poco::BinaryReader;
void inicfg();
void map_insert(map<string, string> *_map, string x);
void map_insert(map<string, string> *_map, string index, string x);

void parseLine(string line, map<string, string> *_map);
void read();
map <string, string> getMapIp();
map <string, string> getMapCIp();

#endif // PARSEHOST_H
