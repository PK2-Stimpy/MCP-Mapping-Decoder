#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include "Source.hpp"
#include "FileApi.h"
#include "Networking.h"
#include <direct.h>
#include <vector>
#include <map>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <fstream>

std::map<std::string, std::string> mappings;
std::vector<std::string> mapNames;

int main() {
	system("title MCPFunctionDecoder by PK2_Stimpy#0001");

	std::string host;

	std::cout << "Checking for folders... ";

	if (!File::dirExists(_T(MAPPING_STORAGE_FOLDER))) _mkdir(MAPPING_STORAGE_FOLDER);
	if (!File::dirExists(_T(MAPPING_STORAGE_TEMP_FOLDER))) _mkdir(MAPPING_STORAGE_TEMP_FOLDER);
	if (!File::dirExists(_T(IMPORT_FOLDER))) _mkdir(IMPORT_FOLDER);
	if (!File::dirExists(_T(EXPORT_FOLDER))) _mkdir(EXPORT_FOLDER);
	if (!File::dirExists(_T(JAR_FOLDER))) _mkdir(JAR_FOLDER);
	if (!File::dirExists(_T(LIBS_FOLDER))) _mkdir(LIBS_FOLDER);
	std::cout << "OK!" << std::endl;

	std::cout << "Checking for files... ";
	if (File::exists(MAPPING_FILES_FIELDS)) remove(MAPPING_FILES_FIELDS);
	if (File::exists(MAPPING_FILES_MAPS)) remove(MAPPING_FILES_MAPS);
	if (File::exists(MAPPING_FILES_METHOD)) remove(MAPPING_FILES_METHOD);
	if (File::exists(MAPPING_FILES_PARAMS)) remove(MAPPING_FILES_PARAMS);
	if (File::exists(DECOMPILER_FILE)) remove(DECOMPILER_FILE);
	Internet::downloadFile("https://www.benf.org/other/cfr/cfr-0.151.jar", DECOMPILER_FILE);
	std::cout << "OK!" << std::endl;

	std::cout << "Host server: ";
	std::getline(std::cin, host);
	std::cout << std::endl;

	std::string url = host + "mcpmappings/";
	std::string url_info = url + "mappings.info";

	Internet::downloadFile(url_info.c_str(), MAPPING_FILES_MAPS);
	std::ifstream file(MAPPING_FILES_MAPS);
	
	std::cout << "List loaded from server!" << std::endl;
	int l = 0;
	std::string line = "";
	while (std::getline(file, line)) {
		std::string gwer = "   " + std::to_string(l) + ") " + line;
		std::cout << "  " << l << ") " << line.c_str() << std::endl;
		mapNames.push_back(url+line);
		l++;
	}
	std::cout << "\nSelect version: ";
	std::cin >> l;
	std::cout << std::endl;

	if (l < 0 || l >= mapNames.size()) {
		std::cout << "Invalid mapping selected!";
		std::cin.get();
		return 0;
	}
	url = mapNames[l];

	std::cout << "Currently downloading mappings... ";

	std::string _u__fields  = url + "/fields.csv",
				_u__methods = url + "/methods.csv";
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
	char clearCache[MAX_PATH];
	std::cin.getline(clearCache, MAX_PATH);
	menu();
	return 1;
}

bool dirExists(const std::string& dirName_in) {
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;
	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;
	return false;
}

void loop() {
	system("cls");
	char clearCache[MAX_PATH];
	std::cin.getline(clearCache, MAX_PATH);

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
	menu();
}

void menu() {
	system("cls");

	int option = 0;
	std::cout << "Please select a mode from this list: " << std::endl;
	std::cout << "   0) File mode" << std::endl;
	std::cout << "   1) Folder mode" << std::endl;
	std::cout << "   2) Jar mode" << std::endl;
	std::cout << "Select: ";
	std::cin >> option;

	if (option > 2 || option < 0) {
		menu();
		return;
	}
	switch (option)
	{
		case 0: {
			loop();
			break;
		}
		case 1: {
			folderMode();
			break;
		}
		case 2: {
			jarMode();
			break;
		}
		default:
			break;
	}
}

void folderMode() {
	system("cls");
	char clearCache[MAX_PATH];
	std::cin.getline(clearCache, MAX_PATH);

	int sel = 0;
	std::cout << "This method will override all the contents on the folder. Continue? (yes=1, no=0)  ";
	std::cin >> sel;

	if (sel != 1) {
		menu();
		return;
	}
	std::cin.getline(clearCache, MAX_PATH);
	char folder[MAX_PATH];
	std::cout << "Select folder inside import folder: ";
	std::cin.getline(folder, MAX_PATH);
	std::cout << std::endl;
	std::cout << "Writing all files... ";

	std::string folderPath = std::string(IMPORT_FOLDER) + std::string(folder);
	for (const auto& name : File::getFilenames(folderPath.c_str())) {
		std::string contents = File::read(name);
		for (const auto& x : mappings)
			contents = std::ReplaceAll(contents, x.first, x.second);
		File::write(name, contents);
	}
	std::cout << "OK!" << std::endl;
	std::cout << "\nDone writing everything!" << std::endl;
	system("pause");
	menu();
}

void jarMode() {
	system("cls");
	char clearCache[MAX_PATH];
	std::cin.getline(clearCache, MAX_PATH);
	
	char name[MAX_PATH];
	std::cout << "Select jar inside jars folder(without .jar): ";
	std::cin.getline(name, MAX_PATH);
	std::string pathNoJar = std::string(JAR_FOLDER) + std::string(name);
	std::string path = pathNoJar + ".jar";
	std::string command = "java -jar " + std::string(DECOMPILER_FILE) + " " + path + " --outputdir " + std::string(JAR_FOLDER) + std::string(name);

	std::cout << "Decompiling... " << std::endl;
	system(command.c_str());

	std::cout << "Done decompiling! " << std::endl;

	std::cout << "Decoding all files... ";
	for (const auto& f : File::getFilenames(pathNoJar.c_str())) {
		std::string contents = File::read(f);
		for (const auto& x : mappings)
			contents = std::ReplaceAll(contents, x.first, x.second);
		File::write(f, contents);
	}
	std::cout << "OK!\n\nAll done! Output path is: '" << pathNoJar.c_str() << "'" << std::endl;

	system("pause");
	menu();
}