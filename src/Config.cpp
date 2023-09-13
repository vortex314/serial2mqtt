/*
 * Config.cpp
 *
 *  Created on: Jul 12, 2016
 *      Author: lieven
 */

#include "Config.h"

#include <Sys.h>

#define EEPROM_SIZE 512
#define EEPROM_MAGIC 0xDEADBEEF
#define KEY_SIZE 40
#define VALUE_SIZE 60

Config::Config(uint32_t size)
	: _jsonBuffer(size), _loaded(false) {
	clear();
}

Config::~Config() {
}

void Config::clear() {
	deserializeJson(_jsonBuffer, "{}");
	_root = _jsonBuffer.as<JsonObject>();
	_nameSpace = "unknown";
}

Config& Config::setNameSpace(const char* ns) {
	_nameSpace = ns;
	return *this;
}

const char* Config::getNameSpace() {
	return _nameSpace.c_str();
}
//==================================================
bool Config::hasKey(const char* key) {
	return _root[_nameSpace].containsKey(key);
}

void Config::remove(const char* key) {
	_root[_nameSpace].remove(key);
}

void Config::save(std::string& str) {
	serializeJson(_root, str);
}

void Config::load(const char* str) {
	auto rc = deserializeJson(_jsonBuffer,str);
	if ( rc == DeserializationError::Ok) {
		_root = _jsonBuffer.as<JsonObject>();
		if ( !_root.isNull() ) {
			return;
		} else {
			WARN(" not a JSON object : '%s'",str);
		}
	} else {
		WARN(" no JSON in '%s'",str);
	}
	deserializeJson(_jsonBuffer,"{}");
	_root = _jsonBuffer.as<JsonObject>();
}
void Config::printPretty(std::string& str) {
	serializeJsonPretty(_root, str);
}

#ifdef ESP32_IDF

#include "esp_system.h"
#include "nvs.h"
#include "nvs_flash.h"

void Config::load() {
	if (_loaded) {
		return;
	}

	esp_err_t err = nvs_flash_init();
	if (err == ESP_ERR_NVS_NO_FREE_PAGES) {
		// NVS partition was truncated and needs to be erased
		// Retry nvs_flash_init
		ESP_ERROR_CHECK(nvs_flash_erase());
		err = nvs_flash_init();
	}
	ESP_ERROR_CHECK(err);

	std::string _strBuffer="";
	nvs_handle my_handle;
	err = nvs_open("storage", NVS_READWRITE, &my_handle);
	if (err != ESP_OK) {
		ERROR("Error (%d) opening NVS handle!\n", err);
	} else {
		INFO("NVS storage open \n");

		char _charBuffer[1024];
		uint32_t required_size= sizeof(_charBuffer);

		err = nvs_get_str(my_handle, "config", _charBuffer, &required_size);
		if ( required_size > sizeof(_charBuffer)) WARN(" config too big for buffer. ");

		switch (err) {
			case ESP_OK:
				INFO("load : %s [%d]", _charBuffer, required_size);
				_strBuffer.clear();
				_strBuffer.append((const char*) _charBuffer, required_size);
				break;
			case ESP_ERR_NVS_NOT_FOUND:
				WARN(" no config in NVS ");
				_strBuffer = "{}";
				break;
			default:
				ERROR(" error read NVS : %d ", err);
				_strBuffer = "{}";
		}
		nvs_close(my_handle);
	}
	_loaded = true;
	auto error = deserializeJson(_jsonBuffer, _strBuffer);
	_root = _jsonBuffer.as<JsonObject>();
	if (error) {
		_root = _jsonBuffer.to<JsonObject>();
	}
}

void Config::save() {
	esp_err_t err;
	std::string temp;
	serializeJson(_root, temp);
	nvs_handle my_handle;
	err = nvs_open("storage", NVS_READWRITE, &my_handle);
	if (err != ESP_OK) {
		ERROR("Error (%d) opening NVS handle!\n", err);
		return;
	}
	err = nvs_set_str(my_handle, "config", temp.c_str());
	if (err) {
		ERROR(" nvs_set_str(my_handle,  'config', buffer): %d ", err);
	}

	err = nvs_commit(my_handle);
	if (err) {
		ERROR(" nvs_commit(my_handle) : %d ", err);
		nvs_close(my_handle);
		return;
	}
	INFO("save : %s", temp.c_str());
}
#endif // ESP-IDF

//==========================================================================
//

#ifdef ESP_OPEN_RTOS
extern "C" {
#include "espressif/esp_common.h"
#include <espressif/spi_flash.h>
#include <sysparam.h>
}

void Config::load() {
	if(_loaded) {
		return;
	}
	sysparam_status_t status;
	uint32_t base_addr, num_sectors;
	std::string _strBuffer;

	status = sysparam_get_info(&base_addr, &num_sectors);
	if(status == SYSPARAM_OK) {
		INFO("[current sysparam region is at 0x%08x (%d sectors)]", base_addr, num_sectors);
	} else {
		num_sectors = DEFAULT_SYSPARAM_SECTORS;
		base_addr = sdk_flashchip.chip_size - (5 + num_sectors) * sdk_flashchip.sector_size;
	}
	uint8_t* destPtr;
	size_t actual_length;
	bool is_binary;
	status = sysparam_get_data("config", &destPtr, &actual_length, &is_binary);
	if(status == SYSPARAM_OK) {
		_strBuffer = (const char*)destPtr;
		free(destPtr);
	} else {
		ERROR("sysparam_get_data('config',...) fails : %d ", status);
		sysparam_set_data("config", (uint8_t*)"{}", 3, false);
		return;
	}

	_loaded = true;
	auto error = deserializeJson(_jsonBuffer,_strBuffer);
	if ( error == DeserializationError::Ok) {
		_root = _jsonBuffer.as<JsonObject>();
		_strBuffer.clear();
		serializeJson(_root,_strBuffer);
	} else {
		ERROR(" couldn't parse config '%s' , dropped old config ! ", _strBuffer.c_str());
		_strBuffer= "{}";
		deserializeJson(_jsonBuffer,"{}");
		_root = _jsonBuffer.to<JsonObject>();
	}
	INFO(" config loaded : %s", _strBuffer.c_str());
}

void Config::save() {
	std::string _strBuffer;
	serializeJson(_root,_strBuffer);
	sysparam_status_t status = sysparam_set_string("config", _strBuffer.c_str());
	if(status == SYSPARAM_OK) {
		INFO(" config saved : %s ", _strBuffer.c_str());
	} else {
		ERROR("config save failed : %d ", status);
	}
}
#endif

#if defined( __linux__ ) || defined( __APPLE__)
//TODO save and load to file
#include <string>
#include <fstream>
#include <streambuf>

void Config::load() {
	loadFile("config.json");
}

void Config::loadFile(const char* name) {

	if (_loaded) {
		return;
	}
	std::string str="{}";

	FILE *file = fopen(name,"r");
	if ( file != NULL) {
		str="";
		char buffer[256];
		while(true) {
			int result=fread(buffer,1,256,file);
			if ( result <=0 ) break;
			str.append(buffer,result);
		}
		fclose(file);
	} else {
		WARN(" cannot open %s : %d = %s",name,errno,strerror(errno));
	}
	/*	std::ifstream t(name);
		std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<
		                char>());*/
	_loaded = true;
	auto error = deserializeJson(_jsonBuffer, str.c_str());

	if (!error) {
		_root = _jsonBuffer.as<JsonObject>();
	} else {
		ERROR(" couldn't parse config ! ");
		printf("%s\n",str.c_str());
		_root = _jsonBuffer.to<JsonObject>();
	}
	std::string temp;
	serializeJson(_root, temp);
	INFO(" config loaded : %s", temp.c_str());
}

void Config::save() {
	saveFile("config.json");
}

void Config::saveFile(const char* name) {
	std::ofstream out(name);
	std::string output;
	serializeJsonPretty(_root, output);
	out << output;
	out.close();
}
#endif

Config config;
