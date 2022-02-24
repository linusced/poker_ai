#include "../../../header/poker/poker.hpp"

void poker::Game::playerAction()
{
    currentPlayerIndex = nextPlayer(currentPlayerIndex);

    std::vector<int> activePlayers;
    for (int i = 0; i < players.size(); i++)
    {
        if (!players[i].busted && !players[i].folded)
        {
            if (players[i].money > 0)
                activePlayers.push_back(i);

            table->updatePlayerStatus(i, glm::vec3(1));
        }
    }

    if (currentPlayerIndex == prevRaisePlayerIndex || activePlayers.size() == 0 || (activePlayers.size() == 1 && players[activePlayers[0]].bettingMoney >= prevBetMoney))
    {
        nextSection();
        return;
    }
    else if (prevRaisePlayerIndex == -1)
        prevRaisePlayerIndex = currentPlayerIndex;

    if (players[currentPlayerIndex].money == 0)
        nextAction = CONTINUE;
    else if (currentPlayerIndex == 0)
        nextAction = PLAYER_INPUT;
    else
    {
        int botAction = computeAI();
        if (botAction == -1)
            fold();
        else
            bet(botAction);
    }
}

// Absolute betting money
void poker::Game::bet(int money)
{
    bool call = true, bet = prevBetMoney == 0;
    std::string betType;

    if (money < prevBetMoney)
        money = prevBetMoney;
    else if (money > prevBetMoney && money < prevBetMoney + prevRaiseMoney)
        money = prevBetMoney + prevRaiseMoney;

    if (money > prevBetMoney)
    {
        prevRaiseMoney = money - prevBetMoney;
        prevBetMoney = money;
        prevRaisePlayerIndex = currentPlayerIndex;
        call = false;
    }

    if (money - players[currentPlayerIndex].bettingMoney == 0)
    {
        betType = "Check";
    }
    else
    {
        players[currentPlayerIndex].money += players[currentPlayerIndex].bettingMoney;
        players[currentPlayerIndex].bettingMoney = money;
        players[currentPlayerIndex].money -= money;

        if (players[currentPlayerIndex].money <= 0)
        {
            players[currentPlayerIndex].bettingMoney += players[currentPlayerIndex].money;
            players[currentPlayerIndex].money = 0;
            table->allIn(currentPlayerIndex);
            betType = "All-In";
        }
        else
            betType = call ? "Call" : (bet ? "Bet" : "Raise");

        table->updatePlayerChips(currentPlayerIndex, players[currentPlayerIndex].money, players[currentPlayerIndex].bettingMoney);
    }

    table->updatePlayerStatus(currentPlayerIndex, betType, glm::vec3(0, .5, 1));

    nextAction = CONTINUE;
    nextActionTime = glfwGetTime() + .5;
}

void poker::Game::fold()
{
    players[currentPlayerIndex].folded = true;
    table->fold(currentPlayerIndex);

    if (currentPlayerIndex == prevRaisePlayerIndex)
        prevRaisePlayerIndex = -1;

    table->updatePlayerStatus(currentPlayerIndex, "Fold", glm::vec3(1, 0, 0));

    nextAction = CONTINUE;
    nextActionTime = glfwGetTime() + .5;
}