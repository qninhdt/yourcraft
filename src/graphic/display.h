#pragma once

#include <glm/glm.hpp>
#include "graphic/crosshair.h"
#include "graphic/skybox.h"
#include "graphic/block_outline.h"
#include "player.h"

namespace yc::graphic {

class Display {

public:
    Display(int32_t width, int32_t height);

    void init();

    void prepareFrame();

    void drawFrame(yc::Player* player);

    void nextFrame();

    int32_t getWidth();

    int32_t getHeight();

private:

    glm::vec3 clearColor = { 0.2f, 0.3f, 0.3f };

    int32_t width;
    int32_t height;

    SkyBox skybox;
    BlockOutline blockOutline;
    CrossHair crosshair;
};

}