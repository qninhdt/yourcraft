#pragma once

#include <glm/glm.hpp>

namespace yc::graphic {

class Display {

public:
    Display();

    void init();

    void prepareFrame();

    void drawFrame();

    void nextFrame();

private:

    glm::vec3 clearColor = { 0.2f, 0.3f, 0.3f };

};

}