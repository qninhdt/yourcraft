#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "gl/texture.h"

yc::gl::Texture::Texture() {

}

void yc::gl::Texture::loadFromFile(std::string path) {
    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_2D, this->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    uint8_t* data = stbi_load(path.c_str(), &this->width, &this->height, &this->nrChannels, STBI_default);
    if (!data) {
        std::cout << "Failed to load texure " << path << '\n';
        return;
    }
    
    GLuint format = this->nrChannels == 4 ? GL_RGBA : GL_RGB;

    // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, format, this->width, this->height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    std::cout << "Loaded texture " << path << " (" << this->width << "x" << this->height << ")\n";

    // unsigned bytePerPixel = nrChannels;
    // unsigned char* pixelOffset = data + (0+ width * 255) * bytePerPixel;
    // unsigned char r = pixelOffset[0];
    // unsigned char g = pixelOffset[1];
    // unsigned char b = pixelOffset[2];
    // unsigned char a = nrChannels >= 4 ? pixelOffset[3] : 0xff;

    // std::cout << ((int)r) << ' ' << ((int)g) << ' ' << ((int)b) << '\n';

    stbi_image_free(data);
}

void yc::gl::Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, this->id);
}

int yc::gl::Texture::getWidth() {
    return this->width;
}

int yc::gl::Texture::getHeight() {
    return this->height;
}

GLuint yc::gl::Texture::getId() {
    return this->id;
}
