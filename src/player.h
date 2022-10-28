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
    yc::world::BlockType getSelectingBlockType();

    void moveFront();
    void moveBack();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    bool checkIntersect(const glm::vec3& delta);

    void init();

    bool isSelectingBlock();

    void update();

    void nextSlot();

    void prevSlot();

    uint32_t getCurrentSlot();

    world::BlockType getCurrentBlockType();

    std::vector<yc::world::BlockType> getInventory();

private:

    glm::ivec3 selectingBlockCoord;
    glm::ivec3 selectingFace;
    yc::world::BlockType selectingBlockType;

    bool selectingBlock;
    float speed;

    Camera camera;
    yc::world::World* world;
    std::vector<yc::world::BlockType> inventory;
    uint32_t currentSlot;
};

}