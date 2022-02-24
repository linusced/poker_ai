#include "../../../header/poker/poker_player.hpp"

std::vector<int> poker::Player::getBestHand(std::vector<Hand> &hands)
{
    std::vector<int> highestHandPlayers;
    int i, j;

    HAND_TYPE highestHandType = HAND_TYPE::HIGH_CARD;
    for (auto &h : hands)
        if (h.type > highestHandType)
            highestHandType = h.type;

    for (i = 0; i < hands.size(); i++)
        if (hands[i].type == highestHandType)
            highestHandPlayers.push_back(i);

    if (highestHandPlayers.size() == 1)
        return highestHandPlayers;

    int valueSize = hands[highestHandPlayers[0]].value.size();
    for (j = 0; j < valueSize; j++)
    {
        int prevValue = hands[highestHandPlayers[0]].value[j];
        for (i = 1; i < highestHandPlayers.size(); i++)
        {
            int value = hands[highestHandPlayers[i]].value[j];
            if (value < prevValue)
            {
                highestHandPlayers.erase(highestHandPlayers.begin() + i);
                i--;
            }
            else if (value > prevValue)
            {
                highestHandPlayers.erase(highestHandPlayers.begin(), highestHandPlayers.begin() + i);
                i = 0;
                prevValue = value;
            }
        }
        if (highestHandPlayers.size() == 1)
            return highestHandPlayers;
    }

    valueSize = hands[highestHandPlayers[0]].top5CardValues.size();
    for (j = 0; j < valueSize; j++)
    {
        int prevValue = hands[highestHandPlayers[0]].top5CardValues[j];
        for (i = 1; i < highestHandPlayers.size(); i++)
        {
            int value = hands[highestHandPlayers[i]].top5CardValues[j];
            if (value < prevValue)
            {
                highestHandPlayers.erase(highestHandPlayers.begin() + i);
                i--;
            }
            else if (value > prevValue)
            {
                highestHandPlayers.erase(highestHandPlayers.begin(), highestHandPlayers.begin() + i);
                i = 0;
                prevValue = value;
            }
        }
        if (highestHandPlayers.size() == 1)
            return highestHandPlayers;
    }

    return highestHandPlayers;
}