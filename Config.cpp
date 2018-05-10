/*
 * Config.cpp
 *
 *  Created on: Jul 12, 2016
 *      Author: lieven
 */

#include "Config.h"

#include <Sys.h>
#include <stdio.h>
#include <malloc.h>

void Config::load()
{
	if (_loaded) {
//        char _buffer[1024];
//       _root->printPretty(_buffer, sizeof(_buffer));
		//    DEBUG(" config object : %s", _buffer);
		return;
	}

	FILE *f = NULL;
	long len = 0;
	char *data = NULL;

	/* open in read binary mode */
	f = fopen(_configFile.c_str(),"rb");
	if ( f!=NULL) {
		/* get the length */
		fseek(f, 0, SEEK_END);
		len = ftell(f);
		fseek(f, 0, SEEK_SET);

		data = (char*)malloc(len + 1);

		fread(data, 1, len, f);
		data[len] = '\0';
		fclose(f);
	} else {
		WARN(" config file %s not found.",_configFile.c_str());
		data = (char*)malloc(10);
		strcpy(data,"{}");
	}
    try {
	_json = json::parse(data);
    } catch(...){
        WARN(" config file cannot be parsed. Invalid JSON.")
    }
	INFO("%s",_json.dump().c_str());
	free(data);
	_loaded=true;
}

void Config::save()
{
	std::string out=_json.dump(4);
	FILE* fd=fopen(_configFile.c_str(),"w");
	if(fd==NULL) {
		ERROR("cannot save config file %s.",_configFile.c_str());
		return;
	}
	if ( fwrite(out.c_str(),1,out.length(),fd)!=out.length()) {
		ERROR("cannot write config file %s.",_configFile.c_str());
		fclose(fd);
		return;
	}

	fclose(fd);
	INFO("saved %s", out.c_str());
	INFO("to : %s",_configFile.c_str());
}

Config::Config() :  _nameSpace(30), _loaded(false),_configFile(256)
{
	_configFile = "config.json";
	_nameSpace="system";
}

Config::~Config() {}

void Config::setFile(const char* s)
{
	_configFile=s;
        _loaded=false;
}


void Config::setNameSpace(const char* ns)
{
	_nameSpace = ns;

}

const char* Config::getNameSpace()
{
	return _nameSpace.c_str();
}
/*
void Config::get(const char* key,std::string& target,const char* defaultValue){
	
	if ( _json[getNameSpace()][key].is_null())
			target = defaultValue;
		else
			target = _json[getNameSpace()][key];
}

*/
//==================================================
bool Config::hasKey(const char* key)
{
	/*   JsonObject& ns = nameSpace();
	   if (ns.containsKey(key)) {
	       return true;
	   }*/
	return false;
}

void Config::remove(const char* key)
{
	load();
	/*    JsonObject& ns = nameSpace();
	    ns.remove(key);
	    INFO(" Config => SAVE  remove %s ", key);*/
}


/*
void Config::get(const char* key, Str& value, const char* defaultValue)
{
	load();
	cJSON* child=cJSON_GetObjectItem(nameSpace(),key);
	if ( cJSON_IsString(child)) {
		value = child->valuestring;
	} else {
		set(key,defaultValue);
		value = defaultValue;
	}
}

void Config::get(const char* key, int32_t& value, int32_t defaultValue)
{
	load();
	cJSON* child=cJSON_GetObjectItem(nameSpace(),key);
	if ( cJSON_IsNumber(child)) {
		value = child->valueint;
	} else {
		set(key,defaultValue);
		value = defaultValue;
	}
}

void Config::get(const char* key, uint32_t& value, uint32_t defaultValue)
{
	load();
	cJSON* child=cJSON_GetObjectItem(nameSpace(),key);
	if ( cJSON_IsNumber(child)) {
		value = child->valueint;
	} else {
		set(key,defaultValue);
		value = defaultValue;
	}
}

void Config::get(const char* key, double& value, double defaultValue)
{
	load();
	cJSON* child=cJSON_GetObjectItem(nameSpace(),key);
	if ( cJSON_IsNumber(child)) {
		value = child->valuedouble;
	} else {
		set(key,defaultValue);
		value = defaultValue;
	}
}

void Config::print(Str& str)
{
	char *out = cJSON_Print(_json);
	str=out;
	free(out);
}

void Config::printPretty(Str& str)
{
	print(str);
}



void Config::clear()
{
	cJSON_Delete(_json);
	_json=cJSON_Parse("{}");
}
*/