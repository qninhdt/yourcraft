#include "player.h"
#include "application.h"
#include "util/math.h"
#include "util/aabb.h"
#include <iostream>

namespace yc {

const float PlayerSize = 0.1;

const glm::vec3 PlayerBoundBox[8] = {
    { +PlayerSize, +PlayerSize, +PlayerSize },
    { +PlayerSize, +PlayerSize, -PlayerSize },
    { +PlayerSize, -PlayerSize, +PlayerSize },
    { -PlayerSize, +PlayerSize, +PlayerSize },
    { +PlayerSize, -PlayerSize, -PlayerSize },
    { -PlayerSize, +PlayerSize, -PlayerSize },
    { -PlayerSize, -PlayerSize, +PlayerSize },
    { -PlayerSize, -PlayerSize, -PlayerSize }
};

Player::Player(float speed, yc::world::World* world):
    speed(speed),
    world(world),
    selectingBlock(false) {

}

void Player::init(int32_t width, int32_t height) {
    camera.init(width, height);
}

void Player::update() {
    auto coord = camera.getPosition();
    auto direction = camera.getDirection();
    auto raycast = world->raycastCheck(coord, direction, false, true);
    
    selectingBlock = raycast.block.getType() != yc::world::BlockType::NONE;

    if (selectingBlock) {
        selectingBlockCoord = raycast.coord;
        selectingFace = raycast.face;
    }
}

Camera* Player::getCamera() {
    return &camera;
}

glm::ivec3 Player::getSelectingBlock() {
    return selectingBlockCoord;
}

glm::ivec3 Player::getSelectingFace() {
    return selectingFace;
}

bool Player::isSelectingBlock() {
    return selectingBlock;
}

void Player::moveFront() {
    auto delta = speed * Application::GetDeltaTime() * camera.getFront();
    if (checkIntersect(delta)) return;
    camera.setPosition(camera.getPosition() + delta);
}

void Player::moveBack() {
    auto delta = - speed * Application::GetDeltaTime() * camera.getFront();
    if (checkIntersect(delta)) return;
    camera.setPosition(camera.getPosition() + delta);
}

void Player::moveRight() {
    auto delta = speed * Application::GetDeltaTime() * camera.getRight();
    if (checkIntersect(delta)) return;
    camera.setPosition(camera.getPosition() + delta);
}

void Player::moveLeft() {
    auto delta = - speed * Application::GetDeltaTime() * camera.getRight();
    if (checkIntersect(delta)) return;
    camera.setPosition(camera.getPosition() + delta);
}

bool Player::checkIntersect(const glm::vec3& delta) {
    auto currentCoord = camera.getPosition() + delta;
    auto direction = glm::normalize(delta);

    for (int i=0;i<8;++i) {
        auto raycast = world->raycastCheck(
            currentCoord + PlayerBoundBox[i],
            direction,
            true,
            true
        );

        if (raycast.block.getType() == world::BlockType::NONE) {
            continue;
        }

        bool check = util::IsInRect(
            currentCoord + PlayerBoundBox[i],
            raycast.coord + glm::ivec3(1, 1, 1),
            raycast.coord
        );

        if (check) return true;
    }

    return false;
}

void Player::moveUp() {
    auto delta = speed * Application::GetDeltaTime() * VectorUp;
    if (checkIntersect(delta)) return;
    camera.setPosition(camera.getPosition() + delta);
}

void Player::moveDown() {
    auto delta = - speed * Application::GetDeltaTime() * VectorUp;
    if (checkIntersect(delta)) return;
    camera.setPosition(camera.getPosition() + delta);
}

}