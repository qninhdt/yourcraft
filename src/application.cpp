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

Application::Application(int32_t width, int32_t height, const std::string& title):
    width(width),
    height(height),
    title(title),
    selectingBlock(false),
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
    this->skybox.init();
    this->blockOutline.init();
    this->crosshair.init(this->width, this->height);

    this->overworld = std::make_shared<yc::world::World>(&persistence);
    this->overworld->init();
}

void Application::process() {

    static float previousTime = glfwGetTime();
    static int frameCount = 0;

    float currentTime = glfwGetTime();
    frameCount++;
    Application::deltaTime = (currentTime-previousTime)/frameCount;

    if (currentTime - previousTime >= 0.05) {
        std::string s = "Yourcraft - FPS: " + std::to_string(static_cast<int>(1/deltaTime));
        glfwSetWindowTitle(this->window, s.c_str());

        frameCount = 0;
        previousTime = currentTime;
    }
    

    float cameraSpeed = 50.0f; // adjust accordingly
    glm::vec3 new_position = this->getCamera()->getPosition();

    if (glfwGetKey(this->window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) cameraSpeed *= 2;

    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        this->stop();
    }

    if (glfwGetKey(this->window, GLFW_KEY_F5) == GLFW_PRESS) {
        overworld->reloadChunks();
    }

    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(this->window, true);

    if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
        new_position += cameraSpeed * deltaTime * this->getCamera()->getFront();
    if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
        new_position -= cameraSpeed * deltaTime * this->getCamera()->getFront();
    if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
        new_position -= this->getCamera()->getRight() * cameraSpeed * deltaTime;
    if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
        new_position += this->getCamera()->getRight() * cameraSpeed * deltaTime;
    if (glfwGetKey(this->window, GLFW_KEY_SPACE) == GLFW_PRESS)
        new_position += yc::VectorUp * cameraSpeed * deltaTime;
    if (glfwGetKey(this->window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        new_position -= yc::VectorUp * cameraSpeed * deltaTime;
    if (glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        if (this->selectingBlock) {
            this->overworld->setBlockDataIfLoadedAt(
                this->selectingBlockCoord + this->selectingFace,
                { world::BlockType::WATER }
            );
        }
    }
    if (glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (this->selectingBlock) {
            this->overworld->destroyBlockIfLoaded(this->selectingBlockCoord);
        }
    }

    this->camera.setPosition(new_position);

    this->display.prepareFrame();
    this->display.drawFrame();

    yc::Resource::GameTexure.bind();
    yc::Resource::ChunkShader.use();
    yc::Resource::ChunkShader.setMat4("projection_view", this->getCamera()->getProjectionViewMatrix());

    overworld->update(*this->getCamera());
    overworld->render();
    this->skybox.render(*this->getCamera());

    this->crosshair.render();

    if (this->selectingBlock) {
        this->blockOutline.render(*this->getCamera(), this->selectingBlockCoord); 
    }

    // begin shit
    auto coord = this->camera.getPosition();
    auto direction = this->camera.getDirection();
    float x = floor(coord.x);
    float y = floor(coord.y);
    float z = floor(coord.z);
    float dx = direction.x;
    float dy = direction.y;
    float dz = direction.z;
    int32_t stepX = util::SignNum(dx);
    int32_t stepY = util::SignNum(dy);
    int32_t stepZ = util::SignNum(dz);

    float tMaxX = util::IntBound(coord.x, dx);
    float tMaxY = util::IntBound(coord.y, dy);
    float tMaxZ = util::IntBound(coord.z, dz);

    float tDeltaX = stepX/dx;
    float tDeltaY = stepY/dy;
    float tDeltaZ = stepZ/dz;

    float radius = 500.0/sqrt(dx*dx+dy*dy+dz*dz);

    while (true) {
        yc::world::BlockData block = this->overworld->getBlockDataIfLoadedAt({ x, y, z });
        yc::world::BlockType blockType = block.getType();

        if (blockType == yc::world::BlockType::NONE) {
            this->selectingBlock = false;
            break;
        } else if (blockType != yc::world::BlockType::AIR) {
            this->selectingBlock = true;
            this->selectingBlockCoord = { x, y, z };
            break;
        }

        if (tMaxX < tMaxY) {
            if (tMaxX < tMaxZ) {
                if (tMaxX > radius) { this->selectingBlock = false; break; }
                // Update which cube we are now in.
                x += stepX;
                // Adjust tMaxX to the next X-oriented boundary crossing.
                tMaxX += tDeltaX;
                // Record the normal vector of the cube face we entered.
                this->selectingFace[0] = -stepX;
                this->selectingFace[1] = 0;
                this->selectingFace[2] = 0;
            } else {
                if (tMaxZ > radius) { this->selectingBlock = false; break; }
                z += stepZ;
                tMaxZ += tDeltaZ;
                this->selectingFace[0] = 0;
                this->selectingFace[1] = 0;
                this->selectingFace[2] = -stepZ;
            }
        } else {
            if (tMaxY < tMaxZ) {
                if (tMaxY > radius) { this->selectingBlock = false; break; }
                y += stepY;
                tMaxY += tDeltaY;
                this->selectingFace[0] = 0;
                this->selectingFace[1] = -stepY;
                this->selectingFace[2] = 0;
            } else {
                // Identical to the second case, repeated for simplicity in
                // the conditionals.
                if (tMaxZ > radius) { this->selectingBlock = false; break; }
                z += stepZ;
                tMaxZ += tDeltaZ;
                this->selectingFace[0] = 0;
                this->selectingFace[1] = 0;
                this->selectingFace[2] = -stepZ;
            }
        }
    }

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

    if (glfwGetKey(app->window, GLFW_KEY_F1) == GLFW_PRESS) {
        app->overworld->saveChunks();
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
}

float Application::deltaTime = 1.0f/60;

float Application::GetDeltaTime() {
    return Application::deltaTime;
}

void Application::terminate() {
    this->overworld->saveChunks();
    glfwTerminate();
}

}