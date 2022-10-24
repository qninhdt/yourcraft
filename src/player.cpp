#include "player.h"
#include "application.h"
#include "util/math.h"

namespace yc {

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
        yc::world::BlockData block = world->getBlockDataIfLoadedAt({ x, y, z });
        yc::world::BlockType blockType = block.getType();

        if (blockType == yc::world::BlockType::NONE) {
            this->selectingBlock = false;
            break;
        } else if (blockType != yc::world::BlockType::AIR && blockType != yc::world::BlockType::WATER) {
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
                if (tMaxZ > radius) { this->selectingBlock = false; break; }
                z += stepZ;
                tMaxZ += tDeltaZ;
                this->selectingFace[0] = 0;
                this->selectingFace[1] = 0;
                this->selectingFace[2] = -stepZ;
            }
        }
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
    camera.setPosition(
        camera.getPosition() + 
        speed * Application::GetDeltaTime() * camera.getFront()
    );
}

void Player::moveBack() {
    camera.setPosition(
        camera.getPosition() - 
        speed * Application::GetDeltaTime() * camera.getFront()
    );
}

void Player::moveRight() {
    camera.setPosition(
        camera.getPosition() + 
        speed * Application::GetDeltaTime() * camera.getRight()
    );
}

void Player::moveLeft() {
    camera.setPosition(
        camera.getPosition() - 
        speed * Application::GetDeltaTime() * camera.getRight()
    );
}

void Player::moveUp() {
    camera.setPosition(
        camera.getPosition() + 
        speed * Application::GetDeltaTime() * VectorUp
    );
}

void Player::moveDown() {
    camera.setPosition(
        camera.getPosition() - 
        speed * Application::GetDeltaTime() * VectorUp
    );
}

}