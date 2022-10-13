#pragma once

#include <string>
#include "gl/common.h"

namespace yc::gl {

class Texture {

public:
    Texture();
    void loadFromFile(std::string path);
    int getWidth();
    int getHeight();
    GLuint getId();
    void bind();

private:
    GLuint id;
    int width, height, nrChannels;

};

}