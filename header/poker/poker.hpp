#ifndef POKER
#define POKER

#include "poker_card.hpp"
#include "poker_player.hpp"
#include "poker_community_pot.hpp"
#include "gui/poker_table_gui.hpp"

#include <algorithm>
#include <random>

namespace poker
{
    class Game
    {
    public:
        enum GAME_ACTION
        {
            NONE,
            START,
            CONTINUE,
            PAUSE,
            PLAYER_INPUT,
            END
        };
        enum GAME_STATE
        {
            PRE_FLOP,
            FLOP,
            TURN,
            RIVER,
            GET_WINNERS,
            END_ROUND,
            NEW_ROUND
        };

    private:
        TableGUI *table = nullptr;
        bool devMode = false;

        std::random_device randomDevice;
        std::default_random_engine randomEngine{randomDevice()};

        std::vector<Card> cards, communityCards;
        std::vector<Player> players;
        std::vector<CommunityPot> communityPots;

        int startMoney, smallBlindMoney, bigBlindMoney, prevBetMoney, prevRaiseMoney,
            dealerIndex = -1, currentPlayerIndex, prevRaisePlayerIndex, communityPotIndex,
            roundIndex = -1, blindsIncrementRounds, prevBlindsIncrement = 0,
            numWeightsAI;

        GAME_STATE section = PRE_FLOP;
        GAME_ACTION nextAction = START;
        double nextActionTime = 0;

        std::string winnerText;
        std::vector<Player::Hand> playerHands;
        bool showPlayerHands_b = false;

        int nextPlayer(int x);

        void newRound();
        void playerAction();
        void nextSection();
        void getWinners();
        void endRound();

        int computeAI();
        void getWorstAndBestHands(std::vector<Card> *currentCommunityCards, Player::Hand *worstHand, Player::Hand *bestHand);

    public:
        Game(TableGUI *table, bool devMode, int numPlayers, int startMoney, int bigBlindMoney, int blindsIncrementRounds);

        GAME_ACTION getNextAction();
        double getNextActionTime();

        void startGame();
        void continueGame();

        void bet(int money);
        void fold();

        int getMinBetMoney(), getPrevRaiseMoney(), getMaxBetMoney(), getCurrentBetMoney();
        std::string getWinnerText();

        bool showPlayerHands();
        std::vector<std::vector<Card> *> getPlayerCards();
        std::vector<Card> *getCommunityCards();
        std::vector<Player::Hand> *getPlayerHands();

        int getRoundIndex(), getSmallBlindMoney(), getBigBlindMoney(), getBlindsIncrementRounds(), getPrevBlindsIncrement(), getTotalBettingMoney();
        std::vector<CommunityPot> *getCommunityPots();
    };
};

#endif