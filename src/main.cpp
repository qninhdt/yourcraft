#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "gl/shader.h"
#include "gl/texture.h"
#include "application.h"
#include <stb_image.h>
#include "world/chunk.h"
#include <vector>
#include "graphic/block_vertex.h"
#include <iostream>
#include "resource.h"
#include "world/world.h"
#include "world/world_generator.h"
#include "util/math.h"

int main()
{
    yc::Application app(1600, 800, "Yourcraft");
    auto overworld = std::make_shared<yc::world::World>();
    bool lineMode = false;

    float previousTime = glfwGetTime();
    int frameCount = 0;
    float deltaTime = 1.0f/60;
    app.getCamera()->setPosition({ 0, 52, 0 });
    app.getCamera()->setOrientation(-89, 0);

    while (!app.isStopped()) {
        float currentTime = glfwGetTime();
        frameCount++;
        deltaTime = (currentTime-previousTime)/frameCount;

        if (currentTime - previousTime >= 0.01) {
            std::string s = "Yourcraft - FPS: " + std::to_string(static_cast<int>(1/deltaTime));
            glfwSetWindowTitle(app.window, s.c_str());

            frameCount = 0;
            previousTime = currentTime;
        }
        

        float cameraSpeed = 50.0f; // adjust accordingly
        glm::vec3 new_position = app.getCamera()->getPosition();

        if (glfwGetKey(app.window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) cameraSpeed *= 2;

        if (glfwGetKey(app.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            app.stop();
        }

        if (glfwGetKey(app.window, GLFW_KEY_F5) == GLFW_PRESS) {
            overworld->reloadChunks();
        }

        if (glfwGetKey(app.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(app.window, true);

        if (glfwGetKey(app.window, GLFW_KEY_W) == GLFW_PRESS)
            new_position += cameraSpeed * deltaTime * app.getCamera()->getFront();
        if (glfwGetKey(app.window, GLFW_KEY_S) == GLFW_PRESS)
            new_position -= cameraSpeed * deltaTime * app.getCamera()->getFront();
        if (glfwGetKey(app.window, GLFW_KEY_A) == GLFW_PRESS)
            new_position -= app.getCamera()->getRight() * cameraSpeed * deltaTime;
        if (glfwGetKey(app.window, GLFW_KEY_D) == GLFW_PRESS)
            new_position += app.getCamera()->getRight() * cameraSpeed * deltaTime;
        if (glfwGetKey(app.window, GLFW_KEY_SPACE) == GLFW_PRESS)
            new_position += yc::VectorUp * cameraSpeed * deltaTime;
        if (glfwGetKey(app.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            new_position -= yc::VectorUp * cameraSpeed * deltaTime;

        app.getCamera()->setPosition(new_position);

        // app.process();
        app.getDisplay()->prepareFrame();
        app.getDisplay()->drawFrame();

        yc::Resource::GameTexure.bind();
        yc::Resource::ChunkShader.use();
        yc::Resource::ChunkShader.setMat4("projection_view", app.getCamera()->getProjectionViewMatrix());

        overworld->update(*app.getCamera());
        overworld->render();

        glfwSwapBuffers(app.window);
        glfwPollEvents();    
    }
    return 0;
}