#include "../../header/poker/poker.hpp"

poker::Game::Game(TableGUI *table, bool devMode, int numPlayers, int startMoney, int bigBlindMoney, int blindsIncrementRounds)
{
    this->table = table;
    this->devMode = devMode;
    this->startMoney = startMoney;
    this->smallBlindMoney = bigBlindMoney / 2;
    this->bigBlindMoney = bigBlindMoney;
    this->blindsIncrementRounds = blindsIncrementRounds;
    this->numWeightsAI = 4;

    for (auto &suit : Card::SUITS)
        for (auto &index : Card::INDICES)
            cards.push_back(Card(suit, index));

    table->init(numPlayers, startMoney);

    std::uniform_real_distribution<float> randomFloat(.1f, 1.f);

    for (int i = 0; i < numPlayers; i++)
    {
        players.push_back(Player(i == 0 ? "You" : ("Bot " + std::to_string(i)), startMoney));
        table->updatePlayerName(i, players.back().name);

        players.back().weightsAI.resize(numWeightsAI);
        for (auto &f : players.back().weightsAI)
            f = randomFloat(randomEngine);
    }
}

poker::Game::GAME_ACTION poker::Game::getNextAction()
{
    return nextAction;
}
double poker::Game::getNextActionTime()
{
    return nextActionTime;
}

int poker::Game::nextPlayer(int x)
{
    do
    {
        x = (x + 1) % players.size();
    } while (players[x].busted || players[x].folded);
    return x;
}

void poker::Game::startGame()
{
    nextAction = NONE;
    newRound();
}
void poker::Game::continueGame()
{
    nextAction = NONE;

    if (section == GET_WINNERS)
        getWinners();
    else if (section == END_ROUND)
        endRound();
    else if (section == NEW_ROUND)
        newRound();
    else
        playerAction();
}

int poker::Game::getMinBetMoney()
{
    return prevBetMoney;
}
int poker::Game::getPrevRaiseMoney()
{
    return prevRaiseMoney;
}
int poker::Game::getMaxBetMoney()
{
    return players[0].money + players[0].bettingMoney;
}
int poker::Game::getCurrentBetMoney()
{
    return players[0].bettingMoney;
}

std::string poker::Game::getWinnerText()
{
    return winnerText;
}

bool poker::Game::showPlayerHands()
{
    return showPlayerHands_b;
}
std::vector<std::vector<poker::Card> *> poker::Game::getPlayerCards()
{
    std::vector<std::vector<poker::Card> *> result;
    for (auto &p : players)
        if (p.busted || p.folded)
            result.push_back(nullptr);
        else
            result.push_back(&p.cards);

    return result;
}
std::vector<poker::Card> *poker::Game::getCommunityCards()
{
    return &communityCards;
}
std::vector<poker::Player::Hand> *poker::Game::getPlayerHands()
{
    return &playerHands;
}

int poker::Game::getRoundIndex()
{
    return roundIndex;
}
int poker::Game::getSmallBlindMoney()
{
    return smallBlindMoney;
}
int poker::Game::getBigBlindMoney()
{
    return bigBlindMoney;
}
int poker::Game::getBlindsIncrementRounds()
{
    return blindsIncrementRounds;
}
int poker::Game::getPrevBlindsIncrement()
{
    return prevBlindsIncrement;
}
int poker::Game::getTotalBettingMoney()
{
    int result = 0;
    for (auto &p : players)
        result += p.bettingMoney;

    return result;
}
std::vector<poker::CommunityPot> *poker::Game::getCommunityPots()
{
    return &communityPots;
}