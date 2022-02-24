#include "../../header/app/app_gui.hpp"

void AppGUI::showPlayerHands(std::vector<std::vector<poker::Card> *> playerCards, std::vector<poker::Card> *communityCards, std::vector<poker::Player::Hand> *playerHands)
{
    playerHandsVisible = true;

    for (int i = 0; i < playerCards.size(); i++)
        getNamedChild("playerHands_" + std::to_string(i))->draw_b = false;

    for (int p = 0, guiHandIndex = 0; p < playerCards.size(); p++)
    {
        if (playerCards.at(p) == nullptr)
            continue;

        getNamedChild("playerHands_" + std::to_string(guiHandIndex))->draw_b = true;

        const poker::Player::Hand &hand = playerHands->at(p);
        std::vector<int> handCardValues = hand.top5CardValues;
        std::vector<poker::Card> outputCards(5);

        std::string handText = hand.toString();
        handText = (char)toupper(handText[0]) + handText.substr(1);

        std::string playerTextElement = "playerHandsText_" + std::to_string(guiHandIndex);
        variableText[playerTextElement]->setText(hand.playerName + ": " + handText, variableText[playerTextElement]->getFontSize(), variableText[playerTextElement]->getColor(), *windowWidth, *windowHeight);

        for (int i = 0; i < 2; i++)
        {
            auto topIndex = std::find(handCardValues.begin(), handCardValues.end(), poker::Card::INDICES.find(playerCards.at(p)->at(i).getIndex()));
            bool isCorrectSuit = poker::Card::SUITS.find(playerCards.at(p)->at(i).getSuit()) == hand.flushSuit;

            if (topIndex != handCardValues.end() && (hand.flushSuit == -1 || isCorrectSuit))
            {
                outputCards[topIndex - handCardValues.begin()] = playerCards.at(p)->at(i);
                handCardValues[topIndex - handCardValues.begin()] = -1;
            }
        }

        for (int i = 0; i < 5; i++)
        {
            auto topIndex = std::find(handCardValues.begin(), handCardValues.end(), poker::Card::INDICES.find(communityCards->at(i).getIndex()));
            bool isCorrectSuit = poker::Card::SUITS.find(communityCards->at(i).getSuit()) == hand.flushSuit;

            if (topIndex != handCardValues.end() && (hand.flushSuit == -1 || isCorrectSuit))
            {
                outputCards[topIndex - handCardValues.begin()] = communityCards->at(i);
                handCardValues[topIndex - handCardValues.begin()] = -1;
            }
        }

        for (int i = 0; i < 2 && outputCards.size() < 7; i++)
        {
            bool alreadyExists = false;
            for (auto &c : outputCards)
                if (c.getIndex() == playerCards.at(p)->at(i).getIndex() && c.getSuit() == playerCards.at(p)->at(i).getSuit())
                {
                    alreadyExists = true;
                    break;
                }

            if (!alreadyExists)
                outputCards.push_back(playerCards.at(p)->at(i));
        }

        for (int i = 0; i < 5 && outputCards.size() < 7; i++)
        {
            bool alreadyExists = false;
            for (auto &c : outputCards)
                if (c.getIndex() == communityCards->at(i).getIndex() && c.getSuit() == communityCards->at(i).getSuit())
                {
                    alreadyExists = true;
                    break;
                }

            if (!alreadyExists)
                outputCards.push_back(communityCards->at(i));
        }

        for (int i = 0; i < outputCards.size(); i++)
            cards["playerHands_" + std::to_string(guiHandIndex) + "_Card_" + std::to_string(i)]->update(&outputCards.at(i));

        guiHandIndex++;
    }

    getNamedChild("playerHandsContainer")->draw_b = true;
    getConstraints("wrapper")->setX((1 - getConstraints("tableContainer")->getW()) / -2);

    getConstraints("pauseContainer")->setConstraintX(gui::RELATIVE_CONSTRAINT_INVERSE);
    getConstraints("pauseContainer")->setConstraintY(gui::RELATIVE_CONSTRAINT);
    getConstraints("pauseContainer")->setY(0);

    resize(*windowWidth, *windowHeight);
}

void AppGUI::hidePlayerHands()
{
    playerHandsVisible = false;

    getNamedChild("playerHandsContainer")->draw_b = false;
    getConstraints("wrapper")->setX(0);

    getConstraints("pauseContainer")->setConstraintX(gui::CENTER_CONSTRAINT);
    getConstraints("pauseContainer")->setConstraintY(gui::RELATIVE_CONSTRAINT_INVERSE);
    getConstraints("pauseContainer")->setY(.57f);

    resize(*windowWidth, *windowHeight);
}