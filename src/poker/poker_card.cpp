#include "../../header/poker/poker_card.hpp"

const std::string poker::Card::SUITS = "CDHS", poker::Card::INDICES = "23456789TJQKA";

poker::Card::Card() {}
poker::Card::Card(char suit, char index)
{
    setSuit(suit);
    setIndex(index);
}
poker::Card::Card(std::string suit, std::string index)
{
    setSuit(suit);
    setIndex(index);
}

void poker::Card::setSuit(char newSuit)
{
    newSuit = toupper(newSuit);

    if (SUITS.find(newSuit) != std::string::npos)
        this->suit = newSuit;
    else
        throw std::invalid_argument(std::string("Invalid poker card suit: ") + newSuit);
}
void poker::Card::setIndex(char newIndex)
{
    newIndex = toupper(newIndex);

    if (INDICES.find(newIndex) != std::string::npos)
        this->index = newIndex;
    else
        throw std::invalid_argument(std::string("Invalid poker card index: ") + newIndex);
}

void poker::Card::setSuit(std::string newSuit)
{
    char newSuitChar = toupper(newSuit[0]);

    if (SUITS.find(newSuitChar) != std::string::npos)
        this->suit = newSuitChar;
    else
        throw std::invalid_argument(std::string("Invalid poker card suit: ") + newSuit);
}
void poker::Card::setIndex(std::string newIndex)
{
    char newIndexChar;
    if (newIndex == "10")
        newIndexChar = 'T';
    else
        newIndexChar = toupper(newIndex[0]);

    if (INDICES.find(newIndexChar) != std::string::npos)
        this->index = newIndexChar;
    else
        throw std::invalid_argument(std::string("Invalid poker card index: ") + newIndex);
}

char poker::Card::getSuit() const
{
    return suit;
}
char poker::Card::getIndex() const
{
    return index;
}

std::string poker::Card::getSuitName() const
{
    std::string name;
    switch (suit)
    {
    case 'C':
        name = "club";
        break;
    case 'D':
        name = "diamond";
        break;
    case 'H':
        name = "heart";
        break;
    case 'S':
        name = "spade";
        break;
    }
    return name;
}
std::string poker::Card::getIndexName() const
{
    std::string name;
    switch (index)
    {
    case 'T':
        name = "10";
        break;
    case 'J':
        name = "jack";
        break;
    case 'Q':
        name = "queen";
        break;
    case 'K':
        name = "king";
        break;
    case 'A':
        name = "ace";
        break;
    default:
        name = index;
        break;
    }
    return name;
}

void poker::Card::getTextureCoords(glm::vec2 *_0, glm::vec2 *_1, glm::vec2 *_2, glm::vec2 *_3) const
{
    size_t suitValue = SUITS.find(suit), indexValue = INDICES.find(index);

    float offsetX = (suitValue % 2) * .5f, offsetY = (suitValue > 1) * .5f;

    if (indexValue < 6)
    {
        offsetY += .25f;
        indexValue += 2;
    }
    else
        indexValue -= 5;

    offsetX += (7 - indexValue) * (.5f / 7);

    _0->x = _3->x = offsetX;
    _0->y = _1->y = offsetY;
    _1->x = _2->x = offsetX + (.5f / 7);
    _2->y = _3->y = offsetY + .25f;
}

std::string poker::Card::getSuitNameByValue(int suitValue)
{
    char suit = SUITS[suitValue];
    std::string name;
    switch (suit)
    {
    case 'C':
        name = "club";
        break;
    case 'D':
        name = "diamond";
        break;
    case 'H':
        name = "heart";
        break;
    case 'S':
        name = "spade";
        break;
    }
    return name;
}
std::string poker::Card::getIndexNameByValue(int indexValue)
{
    char index = INDICES[indexValue];
    std::string name;
    switch (index)
    {
    case 'T':
        name = "10";
        break;
    case 'J':
        name = "jack";
        break;
    case 'Q':
        name = "queen";
        break;
    case 'K':
        name = "king";
        break;
    case 'A':
        name = "ace";
        break;
    default:
        name = index;
        break;
    }
    return name;
}