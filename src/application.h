#pragma once

#include <stdint.h>
#include <string>
#include "gl/common.h"

#include "camera.h"
#include "graphic/display.h"
#include "graphic/skybox.h"
#include "graphic/block_outline.h"
#include "world/world.h"
#include "graphic/crosshair.h"

namespace yc {

class Application {

public:
    static float GetDeltaTime();

    Application(int32_t width, int32_t height, const std::string& title);
    ~Application();

    bool isStopped();
    void stop();
    void process();
    uint32_t getWidth();
    uint32_t getHeight();
    std::string getTitle();
    Camera* getCamera();
    yc::graphic::Display* getDisplay();
    GLFWwindow* window;


private:

    static float deltaTime;

    bool stopped;
    uint32_t width;
    uint32_t height;
    std::string title;

    glm::ivec3 selectingBlockCoord;
    glm::ivec3 selectingFace;
    bool selectingBlock;

    Camera camera;
    yc::graphic::Display display;
    yc::graphic::SkyBox skybox;
    yc::graphic::BlockOutline blockOutline;
    yc::graphic::CrossHair crosshair;
    std::shared_ptr<yc::world::World> overworld;
};

}