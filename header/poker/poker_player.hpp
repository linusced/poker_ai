#ifndef POKER_PLAYER
#define POKER_PLAYER

#include "poker_card.hpp"
#include <vector>
#include <map>

namespace poker
{
    class Player
    {
    public:
        enum class HAND_TYPE
        {
            FOLDED = -1,
            HIGH_CARD,
            PAIR,
            TWO_PAIRS,
            TRIPLETS,
            STRAIGHT,
            FLUSH,
            FULL_HOUSE,
            QUADS,
            STRAIGHT_FLUSH,
            ROYAL_FLUSH
        };
        struct Hand
        {
            HAND_TYPE type = HAND_TYPE::HIGH_CARD;
            std::vector<int> value, top5CardValues;
            int flushSuit = -1;
            std::string playerName;

            std::string toString() const;
        };

        std::string name;
        int money, bettingMoney = 0;
        bool folded = false, busted = false;
        std::vector<Card> cards;
        std::vector<float> weightsAI;

        Player(std::string name, int startMoney);

        Hand getPlayerHand(std::vector<Card> &communityCards) const;

        static std::vector<int> getBestHand(std::vector<Hand> &hands);
    };
};

#endif