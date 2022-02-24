#ifndef APP
#define APP

#include "../gl/draw_loop.hpp"
#include "app_gui.hpp"

class App : public DrawLoop
{
private:
    AppGUI *_gui = nullptr;
    poker::Game *game = nullptr;

    bool devMode = false;
    int numPlayers, startMoney, bigBlindMoney, blindsIncrementRounds;

    void init() override;
    void inputs() override;

public:
    App(GLFWwindow *window, int *windowWidth, int *windowHeight, bool devMode);
    ~App();
};

#endif