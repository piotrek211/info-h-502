#pragma once


#include <fstream>
#include <sstream>

#include <iostream>
#include <string>
namespace GEngine {
	class Utils {
		public:
			static std::string open(const char* path) {

				std::ifstream file;
				std::stringstream fileStream;

				file.open(path);
				fileStream << file.rdbuf();
				file.close();

				std::string code = fileStream.str();
				return code;
				}
		};

}