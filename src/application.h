#pragma once

#include <stdint.h>
#include <string>
#include "gl/common.h"

#include "camera.h"
#include "graphic/display.h"
#include "graphic/skybox.h"
#include "world/world.h"

namespace yc {

class Application {

public:
    static float GetDeltaTime();

    Application(uint32_t width, uint32_t height, const std::string& title);
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

    Camera camera;
    yc::graphic::Display display;
    yc::graphic::SkyBox skybox;
    std::shared_ptr<yc::world::World> overworld;
};

}