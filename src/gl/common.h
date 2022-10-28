#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GL_CHECK(stmt) \
    stmt; \
    yc::gl::checkOpenGLError(#stmt, __FILE__, __LINE__); 

namespace yc::gl {

void checkOpenGLError(const char* stmt, const char* fname, int line);

}