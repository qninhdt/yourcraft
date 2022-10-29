#include <iostream>
#include <stb_image.h>

#include "application.h"
#include "resource.h"
#include "world/chunk.h"
#include "world/block.h"
#include "util/math.h"

namespace yc {

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void sizeCallback(GLFWwindow* window, int width, int height);
void mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

Application::Application(int32_t width, int32_t height, const std::string& title):
    paused(false) {

    Application::Width = width;
    Application::Height = height;

    // init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    // init opengl
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwSetWindowUserPointer(window, this);

    // callback
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallBack);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetFramebufferSizeCallback(window, sizeCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // disable cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

    Resource::Load();

    persistence = new Persistence();

    gui.init(window);
    
    world = new yc::world::World(persistence);
    world->init();

    player = new Player(50.0f, world);
    player->init();

    display.init();

    // set icon
    int iconWidth, iconHeight, iconChannel;

    uint8_t* iconData = stbi_load("./resources/yourcraft.png", &iconWidth, &iconHeight, &iconChannel, STBI_default);

    GLFWimage icons;
    icons.width = iconWidth;
    icons.height = iconHeight;
    icons.pixels = iconData;

    glfwSetWindowIcon(window, 1, &icons);
}

void Application::process() {

    static float previousTime = glfwGetTime();
    static int frameCount = 0;

    float currentTime = glfwGetTime();
    frameCount++;
    Application::deltaTime = (currentTime-previousTime)/frameCount;
    
    if (currentTime - previousTime >= 0.05) {
        frameCount = 0;
        previousTime = currentTime;
    }

    if (!paused) {
        if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS) {
            world->reloadChunks();
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            player->moveFront();
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            player->moveBack();
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            player->moveLeft();
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            player->moveRight();
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            player->moveUp();
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            player->moveDown();
        player->update();
        world->update(player->getCamera());
    }

    gui.update(this, world, player);

    display.drawFrame(player, world);

    gui.render();

    glfwSwapBuffers(window);
    glfwPollEvents();
}

yc::graphic::Display* Application::getDisplay() {
    return &display;
}

bool Application::isStopped() {
    return glfwWindowShouldClose(window);
}

Player* Application::getPlayer() {
    return player;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Application* app = (Application*) glfwGetWindowUserPointer(window);
    
    if (!app->paused) {
        if (glfwGetKey(app->window, GLFW_KEY_F3) == GLFW_PRESS) {
            app->display.toggleLineMode();
        }

        if (glfwGetKey(app->window, GLFW_KEY_F1) == GLFW_PRESS) {
            app->world->saveChunks();
        }
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        if (app->paused) {
            app->resumeGame();
        } else {
            app->pauseGame();
        }
    }
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Application* app = (Application*) glfwGetWindowUserPointer(window);

    static float lastX = Application::Width / 2;
    static float lastY = Application::Height / 2;
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    if (!app->paused) {
        const float sensitivity = 0.2f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        Camera* camera = app->player->getCamera();
        camera->setOrientation(
            camera->getPitch() + yoffset,
            camera->getYaw() + xoffset
        );
    }
}

void mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods) {
    Application* app = (Application*) glfwGetWindowUserPointer(window);

    if (!app->paused) {
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
            if (app->player->isSelectingBlock()) {
                app->world->setBlockDataIfLoadedAt(
                    app->player->getSelectingBlock() + app->player->getSelectingFace(),
                    { app->player->getCurrentBlockType() }
                );
            }
        }

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            if (app->player->isSelectingBlock()) {
                app->world->destroyBlockIfLoaded(app->player->getSelectingBlock());
            }
        }
    }
}

void sizeCallback(GLFWwindow* window, int width, int height) {
    Application* app = (Application*) glfwGetWindowUserPointer(window);
    glViewport(0, 0, width, height);
    Application::Width = width;
    Application::Height = height;
    app->player->getCamera()->update();
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Application* app = (Application*) glfwGetWindowUserPointer(window);
    
    if (!app->paused) {
        if (yoffset < 0) {
            app->player->nextSlot();
        } else {
            app->player->prevSlot();
        }
    }
}

void Application::stop() {
    glfwSetWindowShouldClose(window, true);
}

float Application::deltaTime = 1.0f/60;

float Application::GetDeltaTime() {
    return Application::deltaTime;
}

void Application::pauseGame() {
    paused = true;
    gui.pause(this);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
}

void Application::resumeGame() {
    paused = false;
    gui.resume();
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
}

void Application::terminate() {
    world->saveChunks();
    glfwTerminate();
}

Application::~Application() {

}

int32_t Application::Width;
int32_t Application::Height;

}