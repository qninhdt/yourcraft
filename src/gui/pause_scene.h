#pragma once

namespace yc {
    class Application;
}

namespace yc::gui {

class PauseScene {

public:

    PauseScene(Application* application);

    void render();

private:

    Application* application;
    bool isResumeButtonHovered;
    bool isBackButtonHovered;

};

}