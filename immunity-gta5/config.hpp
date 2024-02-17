#pragma once
#include "imports.h"
#include "../includes/ini.h"
#include <filesystem>
namespace fs = std::filesystem;

// Config Manager


namespace config {

	mINI::INIFile settingsFile("ragemp.ini");
	mINI::INIStructure ini;

	map<string, string> config_list;

	string config_location = GetFolderLocation(CSIDL_LOCAL_APPDATA) + "/immunity/gta5/";
	string active_config = config_location + "/active.ini";


	void setup_folders() {
		if(fs::exists(config_location)) {
			settingsFile = mINI::INIFile(active_config);
			logs::add("config path " + active_config);
		} else {
			fs::create_directories(config_location);
			settingsFile = mINI::INIFile(active_config);
			logs::add("could not find config path, creating");
		}
	}




	bool exists(string category, string key) {
		bool hasSection = ini.has(category);
		if(hasSection) {
			bool hasKey = ini[category].has(key);
			if(hasKey) {
				return true;
			}
		}
		return false;
	}
	//map<string, string>e;

	map<pair<string, string>, map<string, string>> string_cache;
	map<string, string> get(string category, string key, map<string, string> defaultValue) {
		map<string, string> temp;
		try {
			json temp_map;


			if(string_cache[make_pair(category, key)].size()) {
				return string_cache[make_pair(category, key)];
			}

			if(exists(category, key)) {
				string k = ini.get(category).get(key);
				if(k != "") {
					temp_map = json::parse(k);
				}
			}

			for(auto it = temp_map.begin(); it != temp_map.end(); ++it) {
				string first = it.key();
				string second = it.value();
				temp.insert(make_pair(first, second));
			}


			string_cache[make_pair(category, key)] = temp;

			return temp;
		} catch(const std::exception& e) {
			printf("error %s", e.what());
		}

		return temp;
	}
	bool set(string category, string key, map<string, string> newValue) {
		json temp_data;
		map <string, string>::iterator it = newValue.begin();
		while(it != newValue.end()) {
			string hash = it->first;
			string name = it->second;
			temp_data[hash] = name;
			it++;
		}

		string_cache[make_pair(category, key)] = newValue;

		ini[category][key] = temp_data.dump();
		settingsFile.generate(ini, true);
		return true;
	}

	bool update(map<string, string> newValue, string category, string key, map<string, string> defaultValue) {
		if(newValue != config::get(category, key, defaultValue)) {
			return config::set(category, key, newValue);
		}
		return false;
	}

	map<pair<string, string>, map<DWORD, string>> dword_cache;
	map<DWORD, string> get(string category, string key, map<DWORD, string> defaultValue) {
		map<DWORD, string> temp;
		try {
			json temp_map;


			if(dword_cache[make_pair(category, key)].size()) {
				return dword_cache[make_pair(category, key)];
			}

			if(exists(category, key)) {
				temp_map = json::parse(ini.get(category).get(key));
			}

			for(auto it = temp_map.begin(); it != temp_map.end(); ++it) {
				int64_t hash = stoll(it.key().c_str());
				string name = it.value();
				temp.insert(make_pair(hash, name));
			}

			dword_cache[make_pair(category, key)] = temp;

			return temp;
		} catch(const std::exception& e) {
			printf("error %s", e.what());
		}
		return temp;
	}
	bool set(string category, string key, map<DWORD, string> newValue) {
		json temp_data;
		map <DWORD, string>::iterator it = newValue.begin();
		while(it != newValue.end()) {
			int64_t hash = it->first;
			string name = it->second;
			temp_data[std::to_string(hash)] = name.c_str();
			it++;
		}
		dword_cache[make_pair(category, key)] = newValue;

		ini[category][key] = temp_data.dump();
		settingsFile.generate(ini, true);
		return true;
	}


	bool update(map<DWORD, string> newValue, string category, string key, map<DWORD, string> defaultValue) {
		if(newValue != config::get(category, key, defaultValue)) {
			return config::set(category, key, newValue);
		}
		return false;
	}


	namespace fast {
		double pow10(int n) {
			double ret = 1.0;
			double r = 10.0;
			if(n < 0) {
				n = -n;
				r = 0.1;
			}

			while(n) {
				if(n & 1) {
					ret *= r;
				}
				r *= r;
				n >>= 1;
			}
			return ret;
		}

		int atoi(const char* str) {
			int val = 0;
			while(*str) {
				val = val * 10 + (*str++ - '0');
			}
			return val;
		}

		double atof(const char* num) {
			if(!num || !*num) {
				return 0;
			}

			int sign = 1;
			double integerPart = 0.0;
			double fractionPart = 0.0;
			bool hasFraction = false;
			bool hasExpo = false;

			// Take care of +/- sign
			if(*num == '-') {
				++num;
				sign = -1;
			} else if(*num == '+') {
				++num;
			}

			while(*num != '\0') {
				if(*num >= '0' && *num <= '9') {
					integerPart = integerPart * 10 + (*num - '0');
				} else if(*num == '.') {
					hasFraction = true;
					++num;
					break;
				} else if(*num == 'e') {
					hasExpo = true;
					++num;
					break;
				} else {
					return sign * integerPart;
				}
				++num;
			}

			if(hasFraction) {
				double fractionExpo = 0.1;

				while(*num != '\0') {
					if(*num >= '0' && *num <= '9') {
						fractionPart += fractionExpo * (*num - '0');
						fractionExpo *= 0.1;
					} else if(*num == 'e') {
						hasExpo = true;
						++num;
						break;
					} else {
						return sign * (integerPart + fractionPart);
					}
					++num;
				}
			}

			// parsing exponet part
			double expPart = 1.0;
			if(*num != '\0' && hasExpo) {
				int expSign = 1;
				if(*num == '-') {
					expSign = -1;
					++num;
				} else if(*num == '+') {
					++num;
				}

				int e = 0;
				while(*num != '\0' && *num >= '0' && *num <= '9') {
					e = e * 10 + *num - '0';
					++num;
				}

				expPart = pow10(expSign * e);
			}

			return sign * (integerPart + fractionPart) * expPart;
		}
	}


	//Vector3
	map<pair<string, string>, map<string, Vector3>> vector3_cache;


	map<string, Vector3> get(string category, string key, map<string, Vector3> defaultValue) {
		map<string, Vector3> temp;
		try {
			json temp_map;
			if(vector3_cache[make_pair(category, key)].size()) {
				return vector3_cache[make_pair(category, key)];
			}
			if(exists(category, key)) {
				temp_map = json::parse(ini.get(category).get(key));
			}
			for(auto it = temp_map.begin(); it != temp_map.end(); ++it) {
				string name = it.key().c_str();


				string position_data = it.value();


				float x = fast::atof(position_data.substr(0, position_data.find("|")).c_str()); // token is "scott"
				position_data.erase(0, position_data.find("|") + string("|").length());
				float y = fast::atof(position_data.substr(0, position_data.find("|")).c_str());
				position_data.erase(0, position_data.find("|") + string("|").length());
				float z = fast::atof(position_data.substr(0, position_data.find("|")).c_str());

				auto v = Vector3(x, y, z);
				temp.insert(make_pair(name, v));
			}

			vector3_cache[make_pair(category, key)] = temp;

			return temp;
		} catch(const std::exception& e) {
			printf("error %s", e.what());
		}
		return temp;
	}

	bool set(string category, string key, map<string, Vector3> newValue) {
		json temp_data;
		map <string, Vector3>::iterator it = newValue.begin();
		while(it != newValue.end()) {
			string name = it->first;
			Vector3 vec = it->second;

			temp_data[name] = to_string(vec.x) + "|" + to_string(vec.y) + "|" + to_string(vec.z);
			it++;
		}
		vector3_cache[make_pair(category, key)] = newValue;

		ini[category][key] = temp_data.dump();
		settingsFile.generate(ini, true);
		return true;
	}


	bool update(map<string, Vector3> newValue, string category, string key, map<string, Vector3> defaultValue) {
		if(newValue != config::get(category, key, defaultValue)) {
			return config::set(category, key, newValue);
		}
		return false;
	}





	map<pair<string, string>, string> normal_cache;

	bool set(string category, string name, string setting) {
		ini[category][name] = setting;





		normal_cache[make_pair(category, name)] = setting;

		settingsFile.generate(ini, true);
		return true;
	}


	float get(string category, string key, float defaultVal) {

		if(normal_cache[make_pair(category, key)].length()) {
			return fast::atof((normal_cache[make_pair(category, key)]).c_str());
		}

		if(exists(category, key)) {

			normal_cache[make_pair(category, key)] = ini.get(category).get(key).c_str();


			return fast::atof((normal_cache[make_pair(category, key)]).c_str());
		}

		return defaultVal;
	}
	int get(string category, string key, int defaultVal) {
		if(normal_cache[make_pair(category, key)].length()) {
			return fast::atoi((normal_cache[make_pair(category, key)]).c_str());
		}
		if(exists(category, key)) {
			normal_cache[make_pair(category, key)] = ini.get(category).get(key).c_str();


			return fast::atoi((normal_cache[make_pair(category, key)]).c_str());
		}
		return defaultVal;
	}
	string get(string category, string key, string defaultVal) {
		if(normal_cache[make_pair(category, key)].length()) {
			return (normal_cache[make_pair(category, key)]);
		}
		if(exists(category, key)) {
			normal_cache[make_pair(category, key)] = ini.get(category).get(key).c_str();

			return normal_cache[make_pair(category, key)];
		}
		return defaultVal;
	}


	bool update(float newValue, string category, string key, float defaultVal) {
		if(newValue != config::get(category, key, defaultVal)) {
			return config::set(category, key, to_string(newValue));
		}
		return false;
	}
	bool update(int newValue, string category, string key, int defaultVal) {
		if(newValue != config::get(category, key, defaultVal)) {
			return config::set(category, key, to_string(newValue));
		}
		return false;
	}
	bool update(string newValue, string category, string key, string defaultVal) {
		if(newValue != config::get(category, key, defaultVal)) {
			return config::set(category, key, newValue);
		}
		return false;
	}

	void list_configs() {
		config_list.clear();


		for(const auto& entry : fs::directory_iterator(config_location)) {
			config_list.insert(make_pair(entry.path().filename().string(), entry.path().string()));
		}
	}
	void refresh() {
		normal_cache.clear();
		dword_cache.clear();
		string_cache.clear();
	}
	void load_config(string path) {

		settingsFile = mINI::INIFile(path);
		settingsFile.read(ini);
		active_config = path;
		logs::add("loaded config" + active_config);

		refresh();
	}

	void new_config(string config_name) {

		settingsFile = mINI::INIFile(config_location + "/" + config_name + ".ini");
		settingsFile.read(ini);
		active_config = config_location + "/" + config_name + ".ini";
		logs::add("created new config " + active_config);

		refresh();
		list_configs();
	}
	void rename_config(string new_config_name, string old_config_name_path) {

		string new_config_name_path = config_location + "/" + new_config_name + ".ini";

		int result = std::rename(old_config_name_path.c_str(), new_config_name_path.c_str());
		if(result == 0) {

			settingsFile = mINI::INIFile(new_config_name_path);
			settingsFile.read(ini);
			active_config = new_config_name_path;
			logs::add("renamed config" + old_config_name_path + " -> " + new_config_name_path);
		}

		refresh();
		list_configs();
	}

	bool load() {
		setup_folders();
		Sleep(10);
		list_configs();
		logs::add("loaded config" + active_config);
		settingsFile.read(ini);







		refresh();
		return true;
	}



}