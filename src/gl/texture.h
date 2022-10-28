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
    void bind(uint32_t slot);
    void allocate(int32_t width, int32_t height);

private:
    GLuint id;
    int width, height, nrChannels;

};

}