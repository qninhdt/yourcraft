#pragma once 

#include "player.h"

namespace yc::gui {

class GameScene {

public:

    GameScene(Player* player);

    void render();

private:

    Player* player;

};

}