#ifndef POKER_TABLE_GUI
#define POKER_TABLE_GUI

#include "poker_chips_gui.hpp"
#include "poker_card_gui.hpp"
#include "poker_special_chips_gui.hpp"

namespace poker
{
    class TableGUI : public gui::Element
    {
    private:
        gui::Font *font = nullptr;
        gui::Master *master = nullptr;
        int *windowWidth = nullptr, *windowHeight = nullptr;

        int numPlayers, dealerIndex = -1, smallBlindIndex = -1, bigBlindIndex = -1;
        std::vector<gui::Constraints *> chipsConstraints, chipsLabelConstraints, cardsConstraints, specialChipsConstraints, playerInfoConstraints;
        std::vector<ChipsGUI *> chips;
        std::vector<gui::native::Text *> chipsLabel;
        std::vector<CardGUI *> cards;
        std::vector<SpecialChipsGUI *> specialChips;
        std::vector<gui::native::Container *> playerInfo;
        std::vector<gui::native::Text *> playerInfoText;

        struct Animation
        {
            gui::Constraints *constraints = nullptr;
            gui::Element *element = nullptr;
            glm::vec2 startPos, endPos;
            double time, duration;
            gui::Element *hideElement = nullptr, *showElement = nullptr;
            bool resize = false;
        };
        std::vector<Animation> animations;
        bool hideCommunityPot = false;

    public:
        TableGUI(gui::Font *font, gui::Master *master, gui::Constraints *constraints, int *windowWidth, int *windowHeight);

        void init(int numPlayers, int startMoney);

        void resizePoly(int &windowWidth, int &windowHeight) override;
        void draw() override;

        void updatePlayerCards(int playerIndex, std::vector<Card> &playerCards);
        void hidePlayerCards(int playerIndex);
        void showPlayerCards(std::vector<int> playerIndices = {});
        void resetPlayerCards();

        void updateCommunityCards(std::vector<Card> &communityCards);
        void hideCommunityCards();
        void showFlop(), showTurn(), showRiver();

        void updatePlayerChips(int playerIndex, int money, int bettingMoney);
        void hideBettingChips();

        void updateCommunityPot(std::vector<int> money = {-1});
        void updateCommunityPot(int playerMoney, int playerIndex, bool fastAnimation = false);

        void updateSpecialChips(int dealerIndex, int smallBlindIndex, int bigBlindIndex);
        void hideSpecialChips();

        void allIn(int playerIndex);
        void clearAllIn(int playerIndex = -1);
        void fold(int playerIndex);

        void updatePlayerName(int playerIndex, std::string name);
        void updatePlayerStatus(int playerIndex, std::string status, glm::vec3 color = glm::vec3(1));
        void updatePlayerStatus(int playerIndex, glm::vec3 color);
    };
};

#endif