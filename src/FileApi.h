#pragma once
#include <Tlhelp32.h>
#include <CommCtrl.h>
#include <Wininet.h>
#include <Windows.h>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <fstream>
#include <vector>
#include <experimental/filesystem>
#include <iostream>

namespace File {
    BOOL dirExists(LPCTSTR szPath)
    {
        DWORD dwAttrib = GetFileAttributes(szPath);

        return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
            (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
    }
    inline bool exists(const std::string& name) {
        struct stat buffer;
        return (stat(name.c_str(), &buffer) == 0);
    }
	std::string read(const std::string& name) {
		std::ifstream ifs(name);
		std::string content((std::istreambuf_iterator<char>(ifs)),
			(std::istreambuf_iterator<char>()));

		return content;
	}
    int write(const std::string& name, const std::string& content)
    {
        std::ofstream file;
        file.open(name, std::ofstream::out | std::ofstream::trunc);
        file << content.c_str();
        file.close();
        return 0;
    }
    std::vector<std::string> getFilenames(std::experimental::filesystem::path path) {
        std::vector<std::string> filenames;
        
        namespace stdfs = std::experimental::filesystem;
        const stdfs::directory_iterator end{};
        for (stdfs::directory_iterator iter{ path }; iter != end; ++iter) {
            if (stdfs::is_regular_file(*iter))
                filenames.push_back(iter->path().string());
            else if (stdfs::is_directory(*iter)) {
                std::vector<std::string> vector = getFilenames(*iter);
                for (int i = 0; i < vector.size(); i++)
                    filenames.push_back(vector[i]);
            }
        }


        return filenames;
    }
}