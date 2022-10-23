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

    yc::graphic::Display* getDisplay();

    Player* getPlayer();

    void terminate();

private:

    friend void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    friend void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    friend void sizeCallback(GLFWwindow* window, int width, int height);
    friend void mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods);

    static float deltaTime;

    bool stopped;

    graphic::Display display;

    GLFWwindow* window;
    yc::world::World* world;
    Player* player;
    Persistence* persistence;
};

}