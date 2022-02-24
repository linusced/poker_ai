#include "../../../header/poker/poker.hpp"

void poker::Game::newRound()
{
    roundIndex++;
    if (prevBlindsIncrement + blindsIncrementRounds == roundIndex)
    {
        smallBlindMoney *= 2;
        bigBlindMoney *= 2;
        prevBlindsIncrement = roundIndex;
    }

    for (int i = 0; i < players.size(); i++)
        table->updatePlayerStatus(i, players[i].busted ? "hide" : "", glm::vec3(1));

    std::shuffle(cards.begin(), cards.end(), randomEngine);

    dealerIndex = nextPlayer(dealerIndex);

    int cardIndex = -1, firstPlayerIndex = -1, firstPlayerCount = 0,
        smallBlindIndex = -1, bigBlindIndex = -1;
    std::vector<int> activePlayers;

    for (int playerIndex = nextPlayer(dealerIndex); true; playerIndex = nextPlayer(playerIndex))
    {
        if (firstPlayerIndex == -1)
            firstPlayerIndex = smallBlindIndex = playerIndex;
        else if (bigBlindIndex == -1)
            bigBlindIndex = playerIndex;
        else if (playerIndex == firstPlayerIndex)
            if (++firstPlayerCount > 1)
                break;

        if (players[playerIndex].cards.size() != 2)
            players[playerIndex].cards.resize(2);

        players[playerIndex].cards[firstPlayerCount] = cards[++cardIndex];

        if (firstPlayerCount == 0)
            activePlayers.push_back(playerIndex);

        if (playerIndex == 0 || devMode)
            table->updatePlayerCards(playerIndex, players[playerIndex].cards);
    }

    if (activePlayers.size() == 2)
    {
        bigBlindIndex = smallBlindIndex;
        smallBlindIndex = dealerIndex;
    }
    table->updateSpecialChips(dealerIndex, smallBlindIndex, bigBlindIndex);

    std::string betType = "Small Blinds";

    players[smallBlindIndex].bettingMoney = smallBlindMoney;
    players[smallBlindIndex].money -= smallBlindMoney;

    if (players[smallBlindIndex].money <= 0)
    {
        players[smallBlindIndex].bettingMoney += players[smallBlindIndex].money;
        players[smallBlindIndex].money = 0;
        table->allIn(smallBlindIndex);
        betType = "All-In";
    }
    table->updatePlayerChips(smallBlindIndex, players[smallBlindIndex].money, players[smallBlindIndex].bettingMoney);
    table->updatePlayerStatus(smallBlindIndex, betType, glm::vec3(0, .5, 1));

    betType = "Big Blinds";

    players[bigBlindIndex].bettingMoney = bigBlindMoney;
    players[bigBlindIndex].money -= bigBlindMoney;

    if (players[bigBlindIndex].money <= 0)
    {
        players[bigBlindIndex].bettingMoney += players[bigBlindIndex].money;
        players[bigBlindIndex].money = 0;
        table->allIn(bigBlindIndex);
        betType = "All-In";
    }
    table->updatePlayerChips(bigBlindIndex, players[bigBlindIndex].money, players[bigBlindIndex].bettingMoney);
    table->updatePlayerStatus(bigBlindIndex, betType, glm::vec3(0, .5, 1));

    table->showPlayerCards(activePlayers);

    if (communityCards.size() != 5)
        communityCards.resize(5);

    cardIndex++;
    communityCards[0] = cards[++cardIndex];
    communityCards[1] = cards[++cardIndex];
    communityCards[2] = cards[++cardIndex];
    cardIndex++;
    communityCards[3] = cards[++cardIndex];
    cardIndex++;
    communityCards[4] = cards[++cardIndex];

    table->updateCommunityCards(communityCards);

    communityPots.resize(1);

    prevBetMoney = prevRaiseMoney = bigBlindMoney;
    prevRaisePlayerIndex = -1;
    currentPlayerIndex = bigBlindIndex;
    section = PRE_FLOP;

    nextAction = CONTINUE;
    nextActionTime = glfwGetTime() + 1;
}