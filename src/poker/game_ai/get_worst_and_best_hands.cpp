#include "../../../header/poker/poker.hpp"

void poker::Game::getWorstAndBestHands(std::vector<Card> *currentCommunityCards, Player::Hand *worstHand, Player::Hand *bestHand)
{
    if (currentCommunityCards->size() == 0)
    {
        worstHand->type = Player::HAND_TYPE::HIGH_CARD;
        worstHand->value.push_back(Card::INDICES.find('3'));

        bestHand->type = Player::HAND_TYPE::HIGH_CARD;
        bestHand->value.push_back(Card::INDICES.find('A'));
        return;
    }

    int amountOfCards = currentCommunityCards->size(), i, j, h;
    std::vector<int> suits, indices;
    for (i = 0; i < currentCommunityCards->size(); i++)
    {
        suits.push_back(Card::SUITS.find(currentCommunityCards->at(i).getSuit()));
        indices.push_back(Card::INDICES.find(currentCommunityCards->at(i).getIndex()));
    }

    std::map<int, int> cardsOfSuit, cardsOfIndex;
    for (i = 0; i < amountOfCards; i++)
    {
        cardsOfSuit[suits[i]]++;
        cardsOfIndex[indices[i]]++;
    }

    for (auto p : cardsOfSuit)
        if (p.second == 5)
        {
            worstHand->type = Player::HAND_TYPE::FLUSH;
            worstHand->flushSuit = p.first;

            for (i = 0; i < amountOfCards; i++)
                if (suits[i] == p.first)
                    worstHand->value.push_back(indices[i]);

            std::sort(worstHand->value.begin(), worstHand->value.end(), std::greater<int>());
            break;
        }
        else if (p.second >= 3)
        {
            bestHand->type = Player::HAND_TYPE::FLUSH;
            bestHand->flushSuit = p.first;

            for (i = 0; i < amountOfCards; i++)
                if (suits[i] == p.first)
                    bestHand->value.push_back(indices[i]);

            std::sort(bestHand->value.begin(), bestHand->value.end(), std::greater<int>());
            break;
        }

    for (h = 0; h < 2; h++)
    {
        Player::Hand *result = h == 0 ? worstHand : bestHand;
        std::vector<int> indicesSorted = result->type == Player::HAND_TYPE::FLUSH ? result->value : indices;
        std::sort(indicesSorted.begin(), indicesSorted.end(), std::greater<int>());
        indicesSorted.erase(std::unique(indicesSorted.begin(), indicesSorted.end()), indicesSorted.end());
        if (indicesSorted.size() > 0 && indicesSorted[0] == Card::INDICES.find('A'))
            indicesSorted.push_back(-1);

        int errorCount = 0, maxError = h * 2;
        bool isStraight = false;
        std::vector<int> straightValues;
        for (i = 0; i < indicesSorted.size(); i++)
        {
            errorCount = 0;
            if (indicesSorted.size() - i < 5 - maxError)
                break;

            straightValues = {indicesSorted[i]};
            for (j = i + 1; j < indicesSorted.size(); j++)
            {
                if (indicesSorted[j] == indicesSorted[j - 1] - 1)
                {
                    straightValues.push_back(indicesSorted[j]);
                    if (straightValues.size() == 5 - maxError)
                    {
                        isStraight = true;
                        break;
                    }
                }
                else if (++errorCount <= maxError && straightValues.front() - indicesSorted[j] < 4)
                {
                    straightValues.push_back(straightValues.back() - 1);
                    if (straightValues.size() == 5 - maxError)
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
            if (result->type == Player::HAND_TYPE::FLUSH)
                result->type = straightValues[0] == Card::INDICES.find('A') ? Player::HAND_TYPE::ROYAL_FLUSH : Player::HAND_TYPE::STRAIGHT_FLUSH;
            else
                result->type = Player::HAND_TYPE::STRAIGHT;
            result->value = {0};
        }

        if (result->value.size() == 0)
        {
            bool possibleTwoPairs = false, possibleFullHouse = false;
            for (auto it = cardsOfIndex.rbegin(); it != cardsOfIndex.rend(); it++)
            {
                if (it->second == 4)
                {
                    result->type = Player::HAND_TYPE::QUADS;
                    result->value = {it->first};
                    break;
                }
                else if (it->second == 3)
                {
                    if (possibleFullHouse)
                    {
                        result->type = Player::HAND_TYPE::FULL_HOUSE;
                        result->value = {it->first, result->value[0]};
                        break;
                    }
                    result->type = Player::HAND_TYPE::TRIPLETS;
                    result->value = {it->first};
                    possibleFullHouse = true;
                }
                else if (it->second == 2)
                {
                    if (possibleTwoPairs)
                    {
                        result->type = Player::HAND_TYPE::TWO_PAIRS;
                        result->value.push_back(it->first);
                        break;
                    }
                    if (possibleFullHouse)
                    {
                        result->type = Player::HAND_TYPE::FULL_HOUSE;
                        result->value = {result->value[0], it->first};
                        break;
                    }
                    result->type = Player::HAND_TYPE::PAIR;
                    result->value = {it->first};
                    possibleTwoPairs = possibleFullHouse = true;
                }
            }

            if (result->value.size() == 0)
            {
                result->type = (Player::HAND_TYPE)((int)Player::HAND_TYPE::HIGH_CARD + h);
                std::sort(indices.begin(), indices.end(), std::greater<int>());
                result->value = {indices[0]};
            }
            else if (result->value.size() <= 2 && h == 1)
            {
                result->type = Player::HAND_TYPE::FULL_HOUSE;
                result->value = {0, 1};
            }
            else if (result->value.size() == 5 && h == 1)
            {
                result->value = {0};
            }
        }
    }
}