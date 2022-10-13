#pragma once

#include <stdint.h>
#include <string>
#include "gl/common.h"

#include "camera.h"
#include "graphic/display.h"

namespace yc {

class Application {

public:
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

    bool stopped;
    uint32_t width;
    uint32_t height;
    std::string title;

    Camera camera;
    yc::graphic::Display display;
};

}