#include "../../header/poker/poker_player.hpp"

std::string poker::Player::Hand::toString() const
{
    std::string handText;
    switch (this->type)
    {
    case HAND_TYPE::HIGH_CARD:
        handText = Card::getIndexNameByValue(this->value[0]) + " high";
        break;
    case HAND_TYPE::PAIR:
        handText = "pair of " + Card::getIndexNameByValue(this->value[0]) + 's';
        break;
    case HAND_TYPE::TWO_PAIRS:
        handText = "two pair " + Card::getIndexNameByValue(this->value[0]) + "s and " + Card::getIndexNameByValue(this->value[1]) + 's';
        break;
    case HAND_TYPE::TRIPLETS:
        handText = "three of a kind " + Card::getIndexNameByValue(this->value[0]) + 's';
        break;
    case HAND_TYPE::STRAIGHT:
        handText = Card::getIndexNameByValue(this->value[0]) + " high straight";
        break;
    case HAND_TYPE::FLUSH:
        handText = Card::getIndexNameByValue(this->value[0]) + " high flush";
        break;
    case HAND_TYPE::FULL_HOUSE:
        handText = "full house " + Card::getIndexNameByValue(this->value[0]) + "s over " + Card::getIndexNameByValue(this->value[1]) + 's';
        break;
    case HAND_TYPE::QUADS:
        handText = "four of a kind " + Card::getIndexNameByValue(this->value[0]) + 's';
        break;
    case HAND_TYPE::STRAIGHT_FLUSH:
        handText = Card::getIndexNameByValue(this->value[0]) + " high straight  flush";
        break;
    case HAND_TYPE::ROYAL_FLUSH:
        handText = "royal flush";
        break;
    }
    return handText;
}

poker::Player::Player(std::string name, int startMoney)
{
    this->name = name;
    this->money = startMoney;
}