#include <iostream>
#include "gl/common.h"

void yc::gl::checkOpenGLError(const char* stmt, const char* fname, int line) {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "OpenGL error %08x, at %s:%i - for %s\n", err, fname, line, stmt;
    }
}