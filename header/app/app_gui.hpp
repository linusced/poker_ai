#ifndef APP_GUI
#define APP_GUI

#include "../poker/poker.hpp"
#include "../custom_gui/custom_gui.hpp"

class AppGUI : public gui::Master
{
private:
    GLFWwindow *window = nullptr;
    int *windowWidth = nullptr, *windowHeight = nullptr;

    gui::Font *Arial = nullptr;

    std::map<std::string, gui::native::Text *> variableText;
    std::map<std::string, gui::native::Button *> buttons;
    std::map<std::string, std::function<void()>> clickCallbacks;
    std::map<std::string, customGUI::Slider *> sliders;
    std::map<std::string, std::function<void(double &)>> sliderCallbacks;
    std::map<std::string, poker::CardGUI *> cards;

    int minBetMoney, prevRaiseMoney, maxBetMoney, currentBetMoney;

    void createCallbacks();

public:
    poker::TableGUI *table = nullptr;

    bool playerInput = false, bet = false, fold = false,
         startGame = false, continuePausedGame = false, isPaused = false,
         isGameOver = false, restartGame = false,
         playerHandsVisible = false;

    int betMoney;

    AppGUI(GLFWwindow *window, int *windowWidth, int *windowHeight, bool skipStartGamePopup, int numPlayers);

    void getPlayerInput(int minBetMoney, int prevRaiseMoney, int maxBetMoney, int currentBetMoney);
    void pauseGame(std::string title);
    void gameOver(std::string title);

    void showPlayerHands(std::vector<std::vector<poker::Card> *> playerCards, std::vector<poker::Card> *communityCards, std::vector<poker::Player::Hand> *playerHands);
    void hidePlayerHands();

    void updateGameInfo(int roundIndex, int smallBlindMoney, int bigBlindMoney, int blindsIncrementRounds, int prevBlindsIncrement, int totalBettingMoney, std::vector<poker::CommunityPot> *communityPots);
};

#endif