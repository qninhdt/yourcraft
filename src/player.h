#pragma once

#include "camera.h"
#include "world/world.h"

namespace yc {

class Player {

public:

    Player(float speed, yc::world::World* world);

    Camera* getCamera();

    glm::ivec3 getSelectingBlock();
    glm::ivec3 getSelectingFace();

    void moveFront();
    void moveBack();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    void init(int32_t width, int32_t height);

    bool isSelectingBlock();

    void update();

private:

    glm::ivec3 selectingBlockCoord;
    glm::ivec3 selectingFace;
    bool selectingBlock;
    float speed;

    Camera camera;
    yc::world::World* world;

};

}