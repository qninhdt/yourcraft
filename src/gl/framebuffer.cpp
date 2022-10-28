#include "gl/framebuffer.h"
#include <iostream>

namespace yc::gl {

const float FrameBuffer::Vertices[] = {
    -1.0f, -1.0f,
    +1.0f, -1.0f, 
    +1.0f, +1.0f,
    -1.0f, +1.0f, 
};

const float FrameBuffer::TexCoords[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
};

const uint32_t FrameBuffer::Indices[] = {
    0, 1, 2,
    0, 2, 3
};

FrameBuffer::FrameBuffer(int32_t width, int32_t height):
    width(width),
    height(height) {}
    
void FrameBuffer::init() {

    mesh.init();
    mesh.bind();

    mesh.addStaticBuffer(2, &Vertices[0], sizeof(Vertices)/sizeof(float));
    mesh.addStaticBuffer(2, &TexCoords[0], sizeof(TexCoords)/sizeof(float));
    mesh.addIndices(&Indices[0], sizeof(Indices)/sizeof(uint32_t));

    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);  

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);  

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}

FrameBuffer::~FrameBuffer() {
    
}

void FrameBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void FrameBuffer::bindTexture() {
    glBindTexture(GL_TEXTURE_2D, texture);
}

void FrameBuffer::render() {
    bindTexture();
    mesh.bind();
    mesh.draw();
}

}