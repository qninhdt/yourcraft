#include <iostream>
#include <stb_image.h>

#include "application.h"
#include "resource.h"
#include "world/chunk.h"

namespace yc {

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void sizeCallback(GLFWwindow* window, int width, int height);

Application::Application(uint32_t width, uint32_t height, const std::string& title):
    width(width),
    height(height),
    title(title),
    stopped(false) {

    // init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // init stb_image
    stbi_set_flip_vertically_on_load(true); 

    // create window
    this->window = glfwCreateWindow(this->width, this->height, this->title.c_str(), NULL, NULL);

    if (this->window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(this->window);

    // init opengl
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwSetWindowUserPointer(this->window, this);

    // callback
    glfwSetKeyCallback(this->window, keyCallback);
    glfwSetCursorPosCallback(this->window, mouseCallback);
    glfwSetFramebufferSizeCallback(this->window, sizeCallback);

    // disable cursor
    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

    Resource::Load();
    this->camera.init(this->width, this->height);
    this->display.init();
}

void Application::process() {

    this->display.prepareFrame();
    this->display.drawFrame();

    glfwSwapBuffers(this->window);
    glfwPollEvents();
}

yc::graphic::Display* Application::getDisplay() {
    return &this->display;
}

uint32_t Application::getWidth() {
    return this->width;
}

uint32_t Application::getHeight() {
    return this->height;
}

std::string Application::getTitle() {
    return this->title;
}

bool Application::isStopped() {
    return this->stopped;
}

Camera* Application::getCamera() {
    return &this->camera;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Application* app = (Application*) glfwGetWindowUserPointer(window);
    
    static bool lineMode = false;

    if (glfwGetKey(app->window, GLFW_KEY_F3) == GLFW_PRESS) {
        lineMode = !lineMode;

        if (lineMode) {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        } else {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }
    }
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    Application* app = (Application*) glfwGetWindowUserPointer(window);

    static float lastX = app->getWidth() / 2;
    static float lastY = app->getWidth() / 2;

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.2f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    Camera* camera = app->getCamera();
    camera->setOrientation(
        camera->getPitch() + yoffset,
        camera->getYaw() + xoffset
    );
}

void sizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Application::stop() {
    this->stopped = true;
    glfwSetWindowShouldClose(this->window, true);
}

Application::~Application() {
    glfwTerminate();
}

}