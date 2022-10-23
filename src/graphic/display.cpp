#include <glad/glad.h>
#include "graphic/display.h"

namespace yc::graphic {

Display::Display(int32_t width, int32_t height):
    width(width),
    height(height)
{}

void Display::init() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    crosshair.init(width, height);
    skybox.init();
    blockOutline.init();
}

void Display::prepareFrame() {
    glClearDepth(1.0f);
    glClearColor(this->clearColor.x, this->clearColor.y, this->clearColor.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::drawFrame(yc::Player* player) {
    crosshair.render();
    skybox.render(player->getCamera());

    if (player->isSelectingBlock()) {
        blockOutline.render(player->getCamera(), player->getSelectingBlock());
    }
}

void Display::nextFrame() {
    
}

int32_t Display::getHeight() {
    return height;
}

int32_t Display::getWidth() {
    return width;
}

}