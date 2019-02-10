/*
 * Config.h
 *
 *  Created on: Jul 12, 2016
 *      Author: lieven
 */

#ifndef ACTOR_CONFIG_H_
#define ACTOR_CONFIG_H_
#define ARDUINOJSON_USE_LONG_LONG 1
//#define ARDUINOJSON_USE_INT64 1

#include <Log.h>
#include <Str.h>
//#include <cJSON.h>
#include <nlohmann/json.hpp>
using json=nlohmann::json;


class Config
{

    Str _nameSpace;
    bool _loaded;
//	cJSON* _json;
    Str _configFile;
    json* nameSpace();
    json _json;

public:
    Config();
    ~Config();

    void clear();
    void setFile(const char* s);
    void load();
    void save();
    void print(Str& str);
    void printPretty(Str& str);

    bool hasKey(const char* key);
    void setNameSpace(const char* ns);
    const char* getNameSpace();

//	void get(const char*, std::string&, const char* defaultValue);

    void remove(const char* key);
    template <typename T> void set(const char* key,T value)
    {
        _json[getNameSpace()][key]=value;
    };
    template <typename T,typename T1> void get(const char* key,T& target,T1 defaultValue)
    {
        if ( _json[getNameSpace()][key].is_null()) {
            target = defaultValue;
            _json[getNameSpace()][key]=defaultValue;
        } else
            target = _json[getNameSpace()][key];
    }
    json getJson(const char* key,json defaultValue)
    {
        json target;
        if ( _json[getNameSpace()][key].is_null()) {
            target = defaultValue;
            _json[getNameSpace()][key]=defaultValue;
        } else
            target = _json[getNameSpace()][key];
        return target;
    }

};

extern Config config;


#endif /* ACTOR_CONFIG_H_ */
