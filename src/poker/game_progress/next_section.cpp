#include "../../../header/poker/poker.hpp"

void poker::Game::nextSection()
{
    std::vector<int> activePlayers;
    int allInPlayerCount = 0, prevCommunityPotsSize = communityPots.size();

    for (int i = 0; i < players.size(); i++)
        if (!players[i].busted && !players[i].folded)
        {
            activePlayers.push_back(i);
            if (players[i].money == 0)
                allInPlayerCount++;
            else
                table->updatePlayerStatus(i, "");
        }
        else if (players[i].folded)
            table->updatePlayerStatus(i, "Folded", glm::vec3(1, 0, 0));

    if (activePlayers.size() == 1)
    {
        for (int i = 0; i < players.size(); i++)
        {
            communityPots[0].money += players[i].bettingMoney;
            players[i].bettingMoney = 0;
            table->updatePlayerChips(i, players[i].money, 0);
        }

        players[activePlayers[0]].money += communityPots[0].money;
        winnerText = players[activePlayers[0]].name + " stole the pot!";

        table->updatePlayerChips(activePlayers[0], players[activePlayers[0]].money, 0);
        table->hideBettingChips();
        table->clearAllIn();
        table->updateCommunityPot(players[activePlayers[0]].money, activePlayers[0], true);

        section = END_ROUND;
        nextAction = PAUSE;
        return;
    }

    if (allInPlayerCount == 0)
    {
        communityPots[0].players = activePlayers;

        for (int i = 0; i < players.size(); i++)
        {
            communityPots[0].money += players[i].bettingMoney;
            players[i].bettingMoney = 0;
            table->updatePlayerChips(i, players[i].money, 0);
        }
    }
    else
    {
        int highestBet = 0, secondHighestBet = 0, highestBetIndex, highestBetCount = 0;
        std::vector<int> uniqueBets;

        for (int i = 0; i < players.size(); i++)
            if (!players[i].busted && !players[i].folded)
            {
                if (players[i].bettingMoney > highestBet)
                {
                    highestBet = players[i].bettingMoney;
                    highestBetIndex = i;
                }
                if (std::find(uniqueBets.begin(), uniqueBets.end(), players[i].bettingMoney) == uniqueBets.end() && (players[i].bettingMoney > 0 || players[i].money > 0))
                    uniqueBets.push_back(players[i].bettingMoney);
            }

        for (int i = 0; i < players.size(); i++)
            if (!players[i].busted && !players[i].folded)
            {
                if (players[i].bettingMoney > secondHighestBet && players[i].bettingMoney != highestBet)
                    secondHighestBet = players[i].bettingMoney;
                else if (players[i].bettingMoney == highestBet)
                    highestBetCount++;
            }

        if (highestBetCount == 1)
        {
            players[highestBetIndex].bettingMoney = secondHighestBet;
            players[highestBetIndex].money += highestBet - secondHighestBet;

            table->updatePlayerChips(highestBetIndex, players[highestBetIndex].money, players[highestBetIndex].bettingMoney);
            table->clearAllIn(highestBetIndex);

            uniqueBets.erase(std::find(uniqueBets.begin(), uniqueBets.end(), highestBet));
        }
        std::sort(uniqueBets.begin(), uniqueBets.end(), std::less<int>());

        int firstCommunityPotIndex = communityPots.size() - 1;
        communityPotIndex = firstCommunityPotIndex;
        for (int i = 1; i < uniqueBets.size(); i++)
            communityPots.push_back(CommunityPot());

        for (int i = 0; i < uniqueBets.size(); i++, communityPotIndex++)
            for (int j = 0; j < players.size(); j++)
                if (!players[j].busted)
                {
                    int bettingMoney = players[j].bettingMoney;
                    if (bettingMoney > 0 && std::find(communityPots[communityPotIndex].players.begin(), communityPots[communityPotIndex].players.end(), j) == communityPots[communityPotIndex].players.end())
                        communityPots[communityPotIndex].players.push_back(j);

                    if (bettingMoney > uniqueBets[i])
                    {
                        bettingMoney = uniqueBets[i];
                        players[j].bettingMoney -= uniqueBets[i];
                    }
                    else
                        players[j].bettingMoney = 0;

                    communityPots[communityPotIndex].money += bettingMoney;
                }

        for (int i = 0; i < players.size(); i++)
            table->updatePlayerChips(i, players[i].money, 0);
    }

    if (activePlayers.size() > 1 && activePlayers.size() - allInPlayerCount < 2)
    {
        for (int &i : activePlayers)
            table->updatePlayerCards(i, players[i].cards);
    }
    else if (activePlayers.size() > 1 && communityPots.size() == prevCommunityPotsSize && allInPlayerCount > 0)
    {
        bool allInPlayerInLastPot = false;
        for (auto &i : communityPots.back().players)
            if (players[i].money == 0)
            {
                allInPlayerInLastPot = true;
                break;
            }

        if (allInPlayerInLastPot)
        {
            communityPots.push_back(CommunityPot());
            communityPots.back().money = 0;

            for (int &i : activePlayers)
                if (players[i].money > 0)
                    communityPots.back().players.push_back(i);
        }
    }

    std::vector<int> communityPotsMoney;
    for (auto &pot : communityPots)
        communityPotsMoney.push_back(pot.money);

    table->updateCommunityPot(communityPotsMoney);

    if (section == PRE_FLOP)
        table->showFlop();
    else if (section == FLOP)
        table->showTurn();
    else if (section == TURN)
        table->showRiver();
    else
    {
        communityPotIndex = 0;
        showPlayerHands_b = true;
        for (int i = 0; i < players.size(); i++)
        {
            if (players[i].folded)
                playerHands.push_back({Player::HAND_TYPE::FOLDED});
            else
                playerHands.push_back(players[i].getPlayerHand(communityCards));

            playerHands.back().playerName = players[i].name;
        }

        nextAction = CONTINUE;
        section = GET_WINNERS;
        return;
    }

    section = (GAME_STATE)((int)section + 1);

    prevBetMoney = 0;
    prevRaiseMoney = bigBlindMoney;
    prevRaisePlayerIndex = -1;
    currentPlayerIndex = dealerIndex;

    nextAction = CONTINUE;
    nextActionTime = glfwGetTime() + (section == FLOP ? 2.5 : 1.5);
}