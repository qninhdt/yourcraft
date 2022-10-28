#include <string>
#include <iostream>
#include <stb_image.h>
#include "resource.h"
#include "graphic/skybox.h"

namespace yc::graphic {

const float SkyBox::Vertices[] = {
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

SkyBox::SkyBox() {}

void SkyBox::init() {
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(SkyBox::Vertices), &SkyBox::Vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(0);

    // glGenTextures(1, &this->texture);
    // glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture);

    // int32_t width, height, nrChannels;
    // const std::string faces[] = {
    //     "./resources/skybox/right.jpg",
    //     "./resources/skybox/left.jpg",
    //     "./resources/skybox/top.jpg",
    //     "./resources/skybox/bottom.jpg",
    //     "./resources/skybox/front.jpg",
    //     "./resources/skybox/back.jpg"
    // };

    // for (size_t i=0; i<6; ++i) {
    //     uint8_t* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
    //     glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
    //         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //     stbi_image_free(data);
    // }

    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void SkyBox::render(yc::Camera* camera) {
    glDepthFunc(GL_LEQUAL);

    yc::Resource::SkyBoxShader.use();
    yc::Resource::SkyBoxShader.setMat4("projection_view",camera->getProjectionMatrix()*glm::mat4(glm::mat3(camera->getViewMatrix())));
    yc::Resource::SkyBoxShader.setInt("skybox", 0);

    glBindVertexArray(this->vao);
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

}