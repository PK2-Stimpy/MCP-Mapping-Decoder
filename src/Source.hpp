#pragma once
#include <Windows.h>
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <string.h>
#include <istream>
#include <stdio.h>
#include "atlstr.h"
#include <regex>

#define MAPPING_VERSION "1.12_snapshot_20180814"
#define MAPPING_BASEURL "https://oratoriofestnovel.000webhostapp.com/mcpmappings/"
#define MAPPING_STORAGE_FOLDER "./mappings/"
#define MAPPING_STORAGE_TEMP_FOLDER "./temp_mappings/"
#define MAPPING_STORAGE_ZIP "./temp_mappings/mappings.zip"
#define MAPPING_FILES_FIELDS "./mappings/fields.csv"
#define MAPPING_FILES_METHOD "./mappings/method.csv"
#define MAPPING_FILES_PARAMS "./mappings/params.csv"
#define EXPORT_FOLDER "./export/"
#define IMPORT_FOLDER "./import/"
#define DEBUG 1

std::thread currentThread;

void loop();
int main();

namespace std {
    std::vector<std::string> split(const std::string& input, const std::string& regex) {
        std::regex re(regex);
        std::sregex_token_iterator
            first{ input.begin(), input.end(), re, -1 },
            last;
        return { first, last };
    }
    std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return str;
    }
}