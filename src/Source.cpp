#define _CRT_SECURE_NO_WARNINGS

#include "Source.hpp"
#include "FileApi.h"
#include "Networking.h"
#include <direct.h>
#include <elzip/elzip.hpp>
#include <vector>
#include <map>
#include <cstdlib>
#include <algorithm>

std::map<std::string, std::string> mappings;

int main() {
	std::cout << "Checking for folders... ";

	if (!File::dirExists(_T(MAPPING_STORAGE_FOLDER))) _mkdir(MAPPING_STORAGE_FOLDER);
	if (!File::dirExists(_T(MAPPING_STORAGE_TEMP_FOLDER))) _mkdir(MAPPING_STORAGE_TEMP_FOLDER);
	if (!File::dirExists(_T(IMPORT_FOLDER))) _mkdir(IMPORT_FOLDER);
	if (!File::dirExists(_T(EXPORT_FOLDER))) _mkdir(EXPORT_FOLDER);

	std::cout << "OK!\nWe are downloading the current mappings for the version " << MAPPING_VERSION << "... ";

	std::string _u__fields  = std::string(MAPPING_BASEURL) + std::string(MAPPING_VERSION) + "/fields.csv",
				_u__methods = std::string(MAPPING_BASEURL) + std::string(MAPPING_VERSION) + "/methods.csv";
	if (File::exists(_u__fields)) std::remove(_u__fields.c_str());
	if (File::exists(_u__methods)) std::remove(_u__methods.c_str());
	Internet::downloadFile(_u__fields, MAPPING_FILES_FIELDS);
	Internet::downloadFile(_u__methods, MAPPING_FILES_METHOD);

	std::cout << "OK!\nReading mappings... ";
	
	std::string fields = File::read(MAPPING_FILES_FIELDS),
				methods = File::read(MAPPING_FILES_METHOD);
	if (DEBUG) std::cout << "\n[DEBUG] read ok\n";
	std::vector<std::string> _fields = std::split(fields.c_str(), "\n"),
							 _methods = std::split(methods.c_str(), "\n");
	if (DEBUG) std::cout << "[DEBUG] split ok\n";
	for (int i = 1; i < _fields.size(); i++) {
		std::vector<std::string> _field = std::split(_fields[i].c_str(), ",");
		mappings[_field[0]] = _field[1];
		std::string prnt = _field[0] + "[" + _field[1] + "] -> " + std::to_string(i);
		if (DEBUG) std::cout << prnt << std::endl;
		prnt = "";
		_field.clear();
	}
	for (int i = 1; i < _methods.size(); i++) {
		std::vector<std::string> _method = std::split(_methods[i].c_str(), ",");
		mappings[_method[0]] = _method[1];
		std::string prnt = _method[0] + "[" + _method[1] + "] -> " + std::to_string(i);
		if (DEBUG) std::cout << prnt << std::endl;
		prnt = "";
		_method.clear();
	}
	_fields.clear();
	_methods.clear();
	fields = "";
	methods = "";

	std::cout << "OK!";
	Sleep(1000);
	loop();
	return 1;
}

void loop() {
	system("cls");

	char file[MAX_PATH];
	std::cout << "Input the file you want to modify(inside import folder): ";
	std::cin.getline(file, MAX_PATH);
	
	std::string _i__file = std::string(IMPORT_FOLDER) + std::string(file);
	std::string _o__file = std::string(EXPORT_FOLDER) + std::string(file);
	if (!File::exists(_i__file)) {
		std::cout << "\nThe selected file does not exist! Restarting...";
		Sleep(5000);
		loop();
		return;
	}
	
	std::string _c__file = File::read(_i__file);
	for (auto const& x : mappings)
		_c__file = std::ReplaceAll(_c__file, x.first, x.second);
	File::write(_o__file, _c__file);
	printf("File '%s' wrote! \n\n", _o__file.c_str());
	system("pause");
	loop();
}