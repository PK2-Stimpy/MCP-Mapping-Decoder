#pragma once
#include <iostream>
#include <Windows.h>
#include <string>

#pragma comment(lib, "urlmon.lib")


namespace Internet {
	void downloadFile(const std::string& url, const std::string& file) {
		URLDownloadToFileA(NULL, url.c_str(), file.c_str(), 0, NULL);
	}
}