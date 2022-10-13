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

int main()
{
    yc::Application app(1600, 800, "Yourcraft");
    auto overworld = std::make_shared<yc::world::World>();
    bool lineMode = false;

    overworld->loadChunks();
    double previousTime = glfwGetTime();
    int frameCount = 0;

    app.getCamera()->setPosition({ 0, 52, 0 });
    app.getCamera()->setOrientation(-89, 0);

    while (!app.isStopped()) {
        double currentTime = glfwGetTime();
        frameCount++;
        // If a second has passed.
        if ( currentTime - previousTime >= 1.0 ) {
            std::cout << "FPS: " << frameCount << '\n';
            frameCount = 0;
            previousTime = currentTime;
        }

        const float cameraSpeed = 0.1f; // adjust accordingly
        glm::vec3 new_position = app.getCamera()->getPosition();

        if (glfwGetKey(app.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            app.stop();
        }

        if (glfwGetKey(app.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(app.window, true);

        if (glfwGetKey(app.window, GLFW_KEY_W) == GLFW_PRESS)
            new_position += cameraSpeed * app.getCamera()->getFront();
        if (glfwGetKey(app.window, GLFW_KEY_S) == GLFW_PRESS)
            new_position -= cameraSpeed * app.getCamera()->getFront();
        if (glfwGetKey(app.window, GLFW_KEY_A) == GLFW_PRESS)
            new_position -= app.getCamera()->getRight() * cameraSpeed;
        if (glfwGetKey(app.window, GLFW_KEY_D) == GLFW_PRESS)
            new_position += app.getCamera()->getRight() * cameraSpeed;
        if (glfwGetKey(app.window, GLFW_KEY_SPACE) == GLFW_PRESS)
            new_position += yc::VectorUp * cameraSpeed;
        if (glfwGetKey(app.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            new_position -= yc::VectorUp * cameraSpeed;

        app.getCamera()->setPosition(new_position);

        // app.process();
        app.getDisplay()->prepareFrame();
        app.getDisplay()->drawFrame();

        yc::Resource::GameTexure.bind();
        yc::Resource::ChunkShader.use();
        yc::Resource::ChunkShader.setMat4("projection_view", app.getCamera()->getProjectionViewMatrix());
        overworld->render();

        glfwSwapBuffers(app.window);
        glfwPollEvents();    
    }
    return 0;
}