#include "../../../header/poker/poker.hpp"

void poker::Game::getWinners()
{
    for (int i = 0; i < players.size(); i++)
        if (!players[i].busted && !players[i].folded)
            table->updatePlayerStatus(i, players[i].money == 0 ? "All-In" : "");

    std::vector<Player::Hand> hands;
    for (auto &i : communityPots[communityPotIndex].players)
    {
        hands.push_back(playerHands[i]);
        if (!players[i].folded)
            table->updatePlayerCards(i, players[i].cards);
    }

    winnerText = "";
    std::vector<int> winnerIndices = Player::getBestHand(hands);
    for (auto &i : winnerIndices)
    {
        int playerIndex = communityPots[communityPotIndex].players[i];
        players[playerIndex].money += communityPots[communityPotIndex].money / winnerIndices.size();
        table->updateCommunityPot(players[playerIndex].money, playerIndex);

        table->updatePlayerStatus(playerIndex, "Winner", glm::vec3(0, 1, 0));

        if (winnerText.size() == 0)
            winnerText = players[playerIndex].name;
        else
            winnerText += " and " + players[playerIndex].name;
    }

    winnerText += winnerIndices.size() == 1 ? " won" : " split";
    if (communityPots.size() == 1)
        winnerText += " the pot";
    else
        winnerText += " community pot " + std::to_string(communityPotIndex + 1);
    winnerText += " with " + hands[winnerIndices[0]].toString() + '!';

    communityPotIndex++;
    section = communityPotIndex < communityPots.size() ? GET_WINNERS : END_ROUND;
    nextAction = PAUSE;
}

void poker::Game::endRound()
{
    showPlayerHands_b = false;

    playerHands.clear();
    communityPots.clear();
    table->updateCommunityPot();
    table->resetPlayerCards();
    table->hideCommunityCards();
    table->hideSpecialChips();
    table->clearAllIn();

    std::vector<int> activePlayers;
    for (int i = 0; i < players.size(); i++)
        if (!players[i].busted)
        {
            table->updatePlayerChips(i, players[i].money, players[i].bettingMoney);
            players[i].folded = false;
            players[i].busted = players[i].money == 0;
            if (!players[i].busted)
                activePlayers.push_back(i);
        }

    if (activePlayers.size() == 1)
    {
        for (int i = 0; i < players.size(); i++)
            if (i != activePlayers[0])
                table->updatePlayerStatus(i, "Busted", glm::vec3(1, 0, 0));

        winnerText = players[activePlayers[0]].name + " won the game!";
        nextAction = END;
    }
    else
    {
        section = NEW_ROUND;
        nextAction = CONTINUE;
        nextActionTime = glfwGetTime() + 1;
    }
}