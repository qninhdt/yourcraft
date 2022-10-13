#include <glad/glad.h>
#include "graphic/display.h"

namespace yc::graphic {

Display::Display() {}

void Display::init() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}

void Display::prepareFrame() {
    glClearDepth(1.0f);
    glClearColor(this->clearColor.x, this->clearColor.y, this->clearColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::drawFrame() {

}

void Display::nextFrame() {
    
}

}