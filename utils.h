#ifndef UTILS
#define UTILS

#include <string>
#include <iostream>
#include <fstream>

inline std::ofstream LOG_FILE("log_file.txt");

inline void exitWithError(const std::string& str) {
    std::cerr << str << std::endl;
    exit(1);
}

inline void log(const std::string& str, std::ofstream& file) {
    if (!file.is_open()) {
        exitWithError("File not open");
    }

    file << str << std::endl;
}



#endif