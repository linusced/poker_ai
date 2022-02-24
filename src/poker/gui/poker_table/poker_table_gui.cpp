#include "../../../../header/poker/gui/poker_table_gui.hpp"

poker::TableGUI::TableGUI(gui::Font *font, gui::Master *master, gui::Constraints *constraints, int *windowWidth, int *windowHeight)
    : gui::Element(constraints)
{
    this->font = font;
    this->master = master;
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
}

void poker::TableGUI::updatePlayerCards(int playerIndex, std::vector<Card> &playerCards)
{
    cards[playerIndex * 2]->update(&playerCards[1]);
    cards[playerIndex * 2 + 1]->update(&playerCards[0]);
}
void poker::TableGUI::hidePlayerCards(int playerIndex)
{
    cards[playerIndex * 2]->draw_b = false;
    cards[playerIndex * 2 + 1]->draw_b = false;
}
void poker::TableGUI::showPlayerCards(std::vector<int> playerIndices)
{
    if (playerIndices.size() == 0)
        for (int i = 0; i < numPlayers; i++)
            playerIndices.push_back(i);

    for (int i = 0; i < numPlayers; i++)
    {
        cards[i * 2]->draw_b = false;
        cards[i * 2 + 1]->draw_b = false;
    }

    double currentTime = glfwGetTime();
    glm::vec2 layoutSize{layout->w * *windowWidth, layout->h * *windowHeight};

    for (auto &i : playerIndices)
    {
        cards[i * 2]->draw_b = true;
        cards[i * 2 + 1]->draw_b = true;

        animations.push_back(Animation{cardsConstraints[i * 2], cards[i * 2], layoutSize / glm::vec2(2), glm::vec2(cardsConstraints[i * 2]->getX(), cardsConstraints[i * 2]->getY()), currentTime, .25});
        animations.push_back(Animation{cardsConstraints[i * 2 + 1], cards[i * 2 + 1], layoutSize / glm::vec2(2), glm::vec2(cardsConstraints[i * 2 + 1]->getX(), cardsConstraints[i * 2 + 1]->getY()), currentTime, .25});
    }
}
void poker::TableGUI::resetPlayerCards()
{
    double currentTime = glfwGetTime();
    glm::vec2 layoutSize{layout->w * *windowWidth, layout->h * *windowHeight};

    for (int i = 0; i < numPlayers; i++)
    {
        cards[i * 2]->update(nullptr);
        cards[i * 2 + 1]->update(nullptr);

        if (cards[i * 2]->draw_b)
        {
            animations.push_back(Animation{cardsConstraints[i * 2], cards[i * 2], glm::vec2(cardsConstraints[i * 2]->getX(), cardsConstraints[i * 2]->getY()), layoutSize / glm::vec2(2), currentTime, .25, cards[i * 2]});
            animations.push_back(Animation{cardsConstraints[i * 2 + 1], cards[i * 2 + 1], glm::vec2(cardsConstraints[i * 2 + 1]->getX(), cardsConstraints[i * 2 + 1]->getY()), layoutSize / glm::vec2(2), currentTime, .25, cards[i * 2 + 1]});
        }
    }
}

void poker::TableGUI::updateCommunityCards(std::vector<Card> &communityCards)
{
    cards[numPlayers * 2 + 1]->update(&communityCards[0]);
    cards[numPlayers * 2 + 2]->update(&communityCards[1]);
    cards[numPlayers * 2 + 3]->update(&communityCards[2]);
    cards[numPlayers * 2 + 5]->update(&communityCards[3]);
    cards[numPlayers * 2 + 7]->update(&communityCards[4]);
}
void poker::TableGUI::hideCommunityCards()
{
    for (int i = 0; i < 8; i++)
        cards[numPlayers * 2 + i]->draw_b = false;
}
void poker::TableGUI::showFlop()
{
    double currentTime = glfwGetTime() + .5;
    for (int i = 0; i < 4; i++)
    {
        cards[numPlayers * 2 + i]->draw_b = true;
        if (i > 0)
            animations.push_back(Animation{cardsConstraints[numPlayers * 2 + i], cards[numPlayers * 2 + i], glm::vec2(cardsConstraints[numPlayers * 2]->getX(), cardsConstraints[numPlayers * 2]->getY()), glm::vec2(cardsConstraints[numPlayers * 2 + i]->getX(), cardsConstraints[numPlayers * 2 + i]->getY()), currentTime + (i - 1) * .5, .5, nullptr, nullptr, i == 3});
    }
}
void poker::TableGUI::showTurn()
{
    double currentTime = glfwGetTime() + .5;
    for (int i = 4; i < 6; i++)
    {
        cards[numPlayers * 2 + i]->draw_b = true;
        if (i > 4)
            animations.push_back(Animation{cardsConstraints[numPlayers * 2 + i], cards[numPlayers * 2 + i], glm::vec2(cardsConstraints[numPlayers * 2]->getX(), cardsConstraints[numPlayers * 2]->getY()), glm::vec2(cardsConstraints[numPlayers * 2 + i]->getX(), cardsConstraints[numPlayers * 2 + i]->getY()), currentTime + (i - 5) * .5, .5, nullptr, nullptr, i == 5});
    }
}
void poker::TableGUI::showRiver()
{
    double currentTime = glfwGetTime() + .5;
    for (int i = 6; i < 8; i++)
    {
        cards[numPlayers * 2 + i]->draw_b = true;
        if (i > 6)
            animations.push_back(Animation{cardsConstraints[numPlayers * 2 + i], cards[numPlayers * 2 + i], glm::vec2(cardsConstraints[numPlayers * 2]->getX(), cardsConstraints[numPlayers * 2]->getY()), glm::vec2(cardsConstraints[numPlayers * 2 + i]->getX(), cardsConstraints[numPlayers * 2 + i]->getY()), currentTime + (i - 7) * .5, .5, nullptr, nullptr, i == 7});
    }
}

void poker::TableGUI::updatePlayerChips(int playerIndex, int money, int bettingMoney)
{
    chipsLabel[playerIndex * 2]->setText(std::to_string(money) + '$', chipsLabel[0]->getFontSize(), chipsLabel[0]->getColor(), *windowWidth, *windowHeight);
    chipsLabel[playerIndex * 2]->draw_b = money > 0;
    chips[playerIndex * 2]->draw_b = money > 0;

    chipsLabel[playerIndex * 2 + 1]->setText(std::to_string(bettingMoney) + '$', chipsLabel[0]->getFontSize(), chipsLabel[0]->getColor(), *windowWidth, *windowHeight);
    chipsLabel[playerIndex * 2 + 1]->draw_b = bettingMoney > 0;

    if (bettingMoney > 0)
        animations.push_back(Animation{chipsConstraints[playerIndex * 2 + 1], chips[playerIndex * 2 + 1], glm::vec2(chipsConstraints[playerIndex * 2]->getX(), chipsConstraints[playerIndex * 2]->getY()), glm::vec2(chipsConstraints[playerIndex * 2 + 1]->getX(), chipsConstraints[playerIndex * 2 + 1]->getY()), glfwGetTime(), .25});
}
void poker::TableGUI::hideBettingChips()
{
    for (int i = 0; i < numPlayers; i++)
    {
        chipsLabel[i * 2 + 1]->draw_b = false;
        chips[i * 2 + 1]->draw_b = false;
    }
}

void poker::TableGUI::updateCommunityPot(std::vector<int> money)
{
    hideCommunityPot = false;

    std::string moneyStr;
    for (auto &m : money)
        moneyStr += std::string(" ") + std::to_string(m) + '$';

    chipsLabel[numPlayers * 2]->setText(moneyStr, chipsLabel[0]->getFontSize(), chipsLabel[0]->getColor(), *windowWidth, *windowHeight);
    chipsLabel[numPlayers * 2]->draw_b = money[0] > 0;

    if (money[0] <= 0)
        chips[numPlayers * 2]->draw_b = false;
    if (money[0] == -1)
        return;

    double currentTime = glfwGetTime();
    for (int i = 0; i < numPlayers; i++)
        if (chips[i * 2 + 1]->draw_b)
            animations.push_back(Animation{chipsConstraints[i * 2 + 1], chips[i * 2 + 1], glm::vec2(chipsConstraints[i * 2 + 1]->getX(), chipsConstraints[i * 2 + 1]->getY()), glm::vec2(chipsConstraints[numPlayers * 2]->getX(), chipsConstraints[numPlayers * 2]->getY()), currentTime, .5, chips[i * 2 + 1], chips[numPlayers * 2]});
}
void poker::TableGUI::updateCommunityPot(int playerMoney, int playerIndex, bool fastAnimation)
{
    hideCommunityPot = true;
    chips[numPlayers * 2]->draw_b = false;

    chipsLabel[playerIndex * 2]->setText(std::to_string(playerMoney) + '$', chipsLabel[0]->getFontSize(), chipsLabel[0]->getColor(), *windowWidth, *windowHeight);
    chipsLabel[playerIndex * 2]->draw_b = true;

    animations.push_back(Animation{chipsConstraints[playerIndex * 2], chips[playerIndex * 2], glm::vec2(chipsConstraints[numPlayers * 2]->getX(), chipsConstraints[numPlayers * 2]->getY()), glm::vec2(chipsConstraints[playerIndex * 2]->getX(), chipsConstraints[playerIndex * 2]->getY()), glfwGetTime(), fastAnimation ? .5 : 1.5, specialChips[playerIndex]});
}

void poker::TableGUI::updateSpecialChips(int dealerIndex, int smallBlindIndex, int bigBlindIndex)
{
    this->dealerIndex = dealerIndex;
    this->smallBlindIndex = smallBlindIndex;
    this->bigBlindIndex = bigBlindIndex;

    specialChips[numPlayers]->draw_b = true;
    specialChips[numPlayers + 1]->draw_b = true;
    specialChips[numPlayers + 2]->draw_b = true;

    resizePoly(*windowWidth, *windowHeight);

    specialChips[numPlayers]->resize(layout, *windowWidth, *windowHeight);
    specialChips[numPlayers + 1]->resize(layout, *windowWidth, *windowHeight);
    specialChips[numPlayers + 2]->resize(layout, *windowWidth, *windowHeight);
}
void poker::TableGUI::hideSpecialChips()
{
    specialChips[numPlayers]->draw_b = false;
    specialChips[numPlayers + 1]->draw_b = false;
    specialChips[numPlayers + 2]->draw_b = false;
}

void poker::TableGUI::allIn(int playerIndex)
{
    specialChips[playerIndex]->draw_b = true;
}
void poker::TableGUI::clearAllIn(int playerIndex)
{
    if (playerIndex == -1)
        for (int i = 0; i < numPlayers; i++)
            specialChips[i]->draw_b = false;
    else
        specialChips[playerIndex]->draw_b = false;
}
void poker::TableGUI::fold(int playerIndex)
{
    cards[playerIndex * 2]->update(nullptr);
    cards[playerIndex * 2 + 1]->update(nullptr);

    double currentTime = glfwGetTime();
    animations.push_back(Animation{cardsConstraints[playerIndex * 2], cards[playerIndex * 2], glm::vec2(cardsConstraints[playerIndex * 2]->getX(), cardsConstraints[playerIndex * 2]->getY()), glm::vec2(cardsConstraints[numPlayers * 2]->getX(), cardsConstraints[numPlayers * 2]->getY()), currentTime, .25, cards[playerIndex * 2]});
    animations.push_back(Animation{cardsConstraints[playerIndex * 2 + 1], cards[playerIndex * 2 + 1], glm::vec2(cardsConstraints[playerIndex * 2 + 1]->getX(), cardsConstraints[playerIndex * 2 + 1]->getY()), glm::vec2(cardsConstraints[numPlayers * 2]->getX(), cardsConstraints[numPlayers * 2]->getY()), currentTime, .25, cards[playerIndex * 2 + 1]});
}

void poker::TableGUI::updatePlayerName(int playerIndex, std::string name)
{
    playerInfoText[playerIndex * 2]->setText(name, playerInfoText[0]->getFontSize(), playerInfoText[0]->getColor(), *windowWidth, *windowHeight);
}
void poker::TableGUI::updatePlayerStatus(int playerIndex, std::string status, glm::vec3 color)
{
    playerInfoText[playerIndex * 2 + 1]->setText(status, playerInfoText[1]->getFontSize(), color, *windowWidth, *windowHeight);
    playerInfo[playerIndex]->draw_b = status != "hide";
}
void poker::TableGUI::updatePlayerStatus(int playerIndex, glm::vec3 color)
{
    playerInfoText[playerIndex * 2 + 1]->setText(playerInfoText[playerIndex * 2 + 1]->getText(), playerInfoText[1]->getFontSize(), color, *windowWidth, *windowHeight);
}
