#include "parsehost.h"

map <string, string> mapIp;
map <string, string> mapCIp;


void inicfg(){
    static const std::string iniFile =
        "; comment\n"
        "  ; comment  \n"
        "prop1=value1\n"
        "  prop2 = value2  \n"
        "[section1]\n"
        "prop1 = value3\r\n"
        "\tprop2=value4\r\n"
        ";prop3=value7\r\n"
        "\n"
        "  [ section 2 ]\n"
        "prop1 = value 5\n"
        "\t   \n"
        "Prop2 = value6";

    std::istringstream istr(iniFile);
    AutoPtr<IniFileConfiguration> pConf = new IniFileConfiguration(istr);
}

void map_insert(map<string, string> *_map,  string x) {
     stringstream strStream;
     strStream << _map->size();
     string s = strStream.str();

    _map->insert(map<string, string>::value_type(s, x));
}

void map_insert(map<string, string> *_map,string index,  string x) {
    _map->insert(map<string, string>::value_type(index, x));
}

void parseLine(string line, map<string, string> *_map){
    StringTokenizer tokenizer(line, ";, 	");
    string ip = "";
    for (StringTokenizer::Iterator it = tokenizer.begin(); it != tokenizer.end(); ++it) {
        if(ip==""){
            if(RegularExpression::match(*it, "\\d{1,3}.\\d{1,3}.\\d{1,3}.\\d{1,3}")||
               RegularExpression::match(*it, "\\w*::\\d+")    ){
                ip = (*it);
            }
        }
        else if(Poco::trimLeft(*it)!=""){
            map_insert(_map, (*it), ip);
        }
    }
}

void read(){
    mapIp.clear();
    mapCIp.clear();

    ifstream fin(HOSTS);

    if(!fin) {
        cout<<"can not open the input file";
        return ;
    }

    string s;
    while(getline(fin,s)) {
        s = Poco::trimLeft(s);
        if(s != ""){
            if(s[0]=='#'){
                string s_tmp = s.substr(1,s.length());
                    parseLine(s_tmp, &mapCIp);
            }
            else{
                parseLine(s, &mapIp);
            }
        }
    }


}

map <string, string> getMapIp(){
    return mapIp;
}
map <string, string> getMapCIp(){
    return mapCIp;
}
