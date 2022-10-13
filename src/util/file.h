#pragma once

#include <string>
#include <fstream>
#include <sstream>

namespace yc::util {

    std::string readTextFromFile(std::string path) {
        std::ifstream file(path.c_str());
        std::ostringstream buffer;
        buffer << file.rdbuf();

        return buffer.str();
    }
}