#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "gl/texture.h"

yc::gl::Texture::Texture() {

}

void yc::gl::Texture::loadFromFile(std::string path) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

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

    stbi_image_free(data);
}

void yc::gl::Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, id);
}

void yc::gl::Texture::bind(uint32_t slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    bind();
}

void yc::gl::Texture::allocate(int32_t width, int32_t height) {
    bind();
    glTexImage2D(
        GL_TEXTURE_2D, 0, 
        nrChannels == 4 ? GL_RGBA : GL_RGB, 
        width, height, 0, 
        nrChannels == 4 ? GL_RGBA : GL_RGB, 
        GL_SHORT, nullptr
    );
}

int yc::gl::Texture::getWidth() {
    return width;
}

int yc::gl::Texture::getHeight() {
    return height;
}

GLuint yc::gl::Texture::getId() {
    return id;
}
