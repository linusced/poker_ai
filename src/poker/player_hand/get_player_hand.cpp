#include "../../../header/poker/poker_player.hpp"

poker::Player::Hand poker::Player::getPlayerHand(std::vector<Card> &communityCards) const
{
    Hand result;
    int amountOfCards = communityCards.size() + 2, i, j;

    std::vector<int> suits, indices;
    for (auto &c : this->cards)
    {
        suits.push_back(Card::SUITS.find(c.getSuit()));
        indices.push_back(Card::INDICES.find(c.getIndex()));
    }
    for (auto &c : communityCards)
    {
        suits.push_back(Card::SUITS.find(c.getSuit()));
        indices.push_back(Card::INDICES.find(c.getIndex()));
    }

    if (amountOfCards == 2)
    {
        result.type = (indices[0] == indices[1]) ? HAND_TYPE::PAIR : HAND_TYPE::HIGH_CARD;
        std::sort(indices.begin(), indices.end(), std::greater<int>());
        result.value = {indices[0]};
        result.top5CardValues = indices;
    }
    else
    {
        std::map<int, int> cardsOfSuit, cardsOfIndex;
        for (i = 0; i < amountOfCards; i++)
        {
            cardsOfSuit[suits[i]]++;
            cardsOfIndex[indices[i]]++;
        }

        for (auto p : cardsOfSuit)
            if (p.second >= 5)
            {
                result.type = HAND_TYPE::FLUSH;
                result.flushSuit = p.first;

                for (i = 0; i < amountOfCards; i++)
                    if (suits[i] == p.first)
                        result.value.push_back(indices[i]);

                std::sort(result.value.begin(), result.value.end(), std::greater<int>());
                result.top5CardValues = result.value;
                break;
            }

        std::vector<int> indicesSorted = result.type == HAND_TYPE::FLUSH ? result.value : indices;
        std::sort(indicesSorted.begin(), indicesSorted.end(), std::greater<int>());
        indicesSorted.erase(std::unique(indicesSorted.begin(), indicesSorted.end()), indicesSorted.end());
        if (indicesSorted.size() > 0 && indicesSorted[0] == Card::INDICES.find('A'))
            indicesSorted.push_back(-1);

        bool isStraight = false;
        std::vector<int> straightValues;
        for (i = 0; i < indicesSorted.size(); i++)
        {
            if (indicesSorted.size() - i < 5)
                break;

            straightValues = {indicesSorted[i]};
            for (j = i + 1; j < indicesSorted.size(); j++)
            {
                if (indicesSorted[j] == indicesSorted[j - 1] - 1)
                {
                    straightValues.push_back(indicesSorted[j]);
                    if (straightValues.size() == 5)
                    {
                        isStraight = true;
                        break;
                    }
                }
                else
                    break;
            }
            if (isStraight)
                break;
        }
        if (isStraight)
        {
            if (result.type == HAND_TYPE::FLUSH)
                result.type = straightValues[0] == Card::INDICES.find('A') ? HAND_TYPE::ROYAL_FLUSH : HAND_TYPE::STRAIGHT_FLUSH;
            else
                result.type = HAND_TYPE::STRAIGHT;
            result.value = straightValues;
            result.top5CardValues = result.value;
        }

        if (result.value.size() == 0)
        {
            bool possibleTwoPairs = false, possibleFullHouse = false;
            for (auto it = cardsOfIndex.rbegin(); it != cardsOfIndex.rend(); it++)
            {
                if (it->second == 4)
                {
                    result.type = HAND_TYPE::QUADS;
                    result.value = {it->first};
                    break;
                }
                else if (it->second == 3)
                {
                    if (possibleFullHouse)
                    {
                        result.type = HAND_TYPE::FULL_HOUSE;
                        result.value = {it->first, result.value[0]};
                        break;
                    }
                    result.type = HAND_TYPE::TRIPLETS;
                    result.value = {it->first};
                    possibleFullHouse = true;
                }
                else if (it->second == 2)
                {
                    if (possibleTwoPairs)
                    {
                        result.type = HAND_TYPE::TWO_PAIRS;
                        result.value.push_back(it->first);
                        break;
                    }
                    if (possibleFullHouse)
                    {
                        result.type = HAND_TYPE::FULL_HOUSE;
                        result.value = {result.value[0], it->first};
                        break;
                    }
                    result.type = HAND_TYPE::PAIR;
                    result.value = {it->first};
                    possibleTwoPairs = possibleFullHouse = true;
                }
            }

            std::sort(indices.begin(), indices.end(), std::greater<int>());
            if (result.value.size() == 0)
            {
                result.type = HAND_TYPE::HIGH_CARD;
                result.value = {indices[0]};
                result.top5CardValues = indices;
            }
            else if (result.type == HAND_TYPE::FULL_HOUSE)
            {
                result.top5CardValues = {result.value[0], result.value[0], result.value[0], result.value[1], result.value[1], result.value[1]};
            }
            else
            {
                for (i = 0; i < indices.size(); i++)
                    if (std::find(result.value.begin(), result.value.end(), indices[i]) != result.value.end())
                    {
                        result.top5CardValues.push_back(indices[i]);
                        indices.erase(indices.begin() + i);
                        i--;
                    }

                for (i = 0; i < indices.size(); i++)
                    result.top5CardValues.push_back(indices[i]);
            }
        }
    }

    if (result.value.size() > 5)
        result.value.erase(result.value.begin() + 5, result.value.end());
    if (result.value.back() == -1)
        result.value.back() = Card::INDICES.find('A');

    if (result.top5CardValues.size() > 5)
        result.top5CardValues.erase(result.top5CardValues.begin() + 5, result.top5CardValues.end());
    if (result.top5CardValues.back() == -1)
        result.top5CardValues.back() = Card::INDICES.find('A');

    return result;
}