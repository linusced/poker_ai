#include "../../../header/poker/poker.hpp"

int poker::Game::computeAI()
{
    std::vector<Card> currentCommunityCards;
    if (section >= FLOP)
        for (int i = 0; i < 3 + (section - FLOP); i++)
            currentCommunityCards.push_back(communityCards[i]);

    const Player &botPlayer = players[currentPlayerIndex];
    Player::Hand hand = botPlayer.getPlayerHand(currentCommunityCards),
                 worstHand, bestHand;

    getWorstAndBestHands(&currentCommunityCards, &worstHand, &bestHand);

    int worstHandScore = (int)worstHand.type * Card::INDICES.find('A') + worstHand.value.back(),
        bestHandScore = (int)bestHand.type * Card::INDICES.find('A') + bestHand.value.front(),
        handScore = (int)hand.type * Card::INDICES.find('A') + hand.value.front();

    float handInput = (handScore - worstHandScore) / (float)(bestHandScore - worstHandScore) * 4 - 2;
    handInput *= botPlayer.weightsAI[0];

    if (communityCards.size() == 0 && (botPlayer.cards[0].getSuit() == botPlayer.cards[1].getSuit() || abs((int)(Card::INDICES.find(botPlayer.cards[0].getIndex()) - Card::INDICES.find(botPlayer.cards[1].getIndex()))) < 4))
        handInput = 5;
    else if (hand.type == Player::HAND_TYPE::TRIPLETS || hand.type > Player::HAND_TYPE::FLUSH)
        handInput = 5;

    int totalPotMoney = 0;
    for (auto &i : communityPots)
        totalPotMoney += i.money;

    int communityPotMoney = totalPotMoney;

    for (auto &i : players)
        totalPotMoney += i.bettingMoney;

    float potInput = 1;
    if (totalPotMoney - communityPotMoney > 0)
    {
        potInput = 1.0f - ((prevBetMoney - botPlayer.bettingMoney) / (float)(totalPotMoney));
        potInput *= botPlayer.weightsAI[1] * 6;
        std::cout << potInput << '\n';
    }

    std::uniform_real_distribution<float> randomRaiseMult(1.f, 2.5f), randomPercentage(0.f, 1.f);
    int botRaiseMoney = prevRaiseMoney * randomRaiseMult(randomEngine);

    if (handInput + potInput > 0.f)
    {
        if (handInput > 2 * botPlayer.weightsAI[2] && prevBetMoney < bigBlindMoney * 3)
            return prevBetMoney + botRaiseMoney;
        return 0;
    }
    else if (botPlayer.weightsAI[3] * randomPercentage(randomEngine) > .7f)
        return prevBetMoney + botRaiseMoney * (randomPercentage(randomEngine) + .6f);

    if (prevBetMoney == botPlayer.bettingMoney)
        return 0;
    return -1;
}