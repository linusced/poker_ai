#include "../../../../header/poker/gui/poker_table_gui.hpp"

void poker::TableGUI::resizePoly(int &windowWidth, int &windowHeight)
{
    for (auto &a : animations)
    {
        a.element->draw_b = true;

        if (a.hideElement)
            a.hideElement->draw_b = false;

        if (a.showElement && !(hideCommunityPot && a.showElement == chips[numPlayers * 2]))
            a.showElement->draw_b = true;
    }
    animations.clear();

    glm::vec2 windowSize{windowWidth, windowHeight}, layoutSize{layout->w * windowWidth, layout->h * windowHeight},
        center{layout->x + layout->w / 2, layout->y + layout->h / 2};

    float radius = layoutSize.x < layoutSize.y ? (layoutSize.x / 2 - 50) : (layoutSize.y / 2 - 50),
          angleStep = (M_PI * 2) / (poly[0]->vertices.size() - 2), iAngle;

    poly[0]->vertices[0].position = center;
    for (int i = 1; i < poly[0]->vertices.size(); i++)
    {
        iAngle = (i - 1) * angleStep;
        poly[0]->vertices[i].position = (glm::vec2(radius * sin(iAngle), radius * cos(iAngle)) / windowSize) + center;
    }

    radius -= 15;

    poly[1]->vertices[0].position = center;
    for (int i = 1; i < poly[1]->vertices.size(); i++)
    {
        iAngle = (i - 1) * angleStep;
        poly[1]->vertices[i].position = (glm::vec2(radius * sin(iAngle), radius * cos(iAngle)) / windowSize) + center;
    }

    poly[0]->applyVertexChange();
    poly[1]->applyVertexChange();

    float chipOffset = radius * .85f, bettingChipOffset = radius * .6f, chipRadOffset = .4f, chipSize = radius * .4f,
          cardOffset = radius * .8f, cardSize = radius * .2f,
          specialChipSize = radius * .15f, specialChipRadOffset = -.35f,
          blindsChipOffset = radius * .75f, dealerChipOffset = radius * .9f,
          playerInfoOffset = radius * 1.2f, playerInfoWidth = radius * .4f;

    angleStep = (M_PI * 2) / -numPlayers;
    for (int i = 0; i < numPlayers; i++)
    {
        iAngle = i * angleStep + chipRadOffset;
        for (int j = 0; j < 2; j++)
        {
            chipsConstraints[i * 2 + j]->setX((j == 0 ? chipOffset : bettingChipOffset) * sin(iAngle) + layoutSize.x / 2 - chipSize / 2);
            chipsConstraints[i * 2 + j]->setY((j == 0 ? chipOffset : bettingChipOffset) * cos(iAngle) + layoutSize.y / 2 - chipSize / 2);
            chipsConstraints[i * 2 + j]->setW(chipSize);

            chipsLabelConstraints[i * 2 + j]->setX((j == 0 ? chipOffset : bettingChipOffset) * sin(iAngle) + layoutSize.x / 2 - 50);
            chipsLabelConstraints[i * 2 + j]->setY((j == 0 ? chipOffset : bettingChipOffset) * cos(iAngle) + layoutSize.y / 2 + chipSize / 4);
            chipsLabelConstraints[i * 2 + j]->setW(100);
            chipsLabelConstraints[i * 2 + j]->setH(chipSize);
        }

        iAngle -= chipRadOffset;
        for (int j = 0; j < 2; j++)
        {
            cardsConstraints[i * 2 + j]->setX(cardOffset * sin(iAngle) + layoutSize.x / 2 - cardSize / 2);
            cardsConstraints[i * 2 + j]->setY(cardOffset * cos(iAngle) + layoutSize.y / 2 - (cardSize * 1.5f) / 2);
            cardsConstraints[i * 2 + j]->setW(cardSize);
            cards[i * 2 + j]->rotate(-iAngle);
            iAngle -= .08f;
        }

        iAngle = i * angleStep + chipRadOffset * .6f;
        specialChipsConstraints[i]->setX(chipOffset * sin(iAngle) + layoutSize.x / 2 - specialChipSize / 2);
        specialChipsConstraints[i]->setY(chipOffset * cos(iAngle) + layoutSize.y / 2 - specialChipSize / 2);
        specialChipsConstraints[i]->setW(specialChipSize);

        iAngle = i * angleStep + specialChipRadOffset;
        if (i == dealerIndex)
        {
            specialChipsConstraints[numPlayers]->setX(dealerChipOffset * sin(iAngle) + layoutSize.x / 2 - specialChipSize / 2);
            specialChipsConstraints[numPlayers]->setY(dealerChipOffset * cos(iAngle) + layoutSize.y / 2 - specialChipSize / 2);
            specialChipsConstraints[numPlayers]->setW(specialChipSize);
        }

        if (i == smallBlindIndex)
        {
            specialChipsConstraints[numPlayers + 1]->setX(blindsChipOffset * sin(iAngle) + layoutSize.x / 2 - specialChipSize / 2);
            specialChipsConstraints[numPlayers + 1]->setY(blindsChipOffset * cos(iAngle) + layoutSize.y / 2 - specialChipSize / 2);
            specialChipsConstraints[numPlayers + 1]->setW(specialChipSize);
        }
        else if (i == bigBlindIndex)
        {
            specialChipsConstraints[numPlayers + 2]->setX(blindsChipOffset * sin(iAngle) + layoutSize.x / 2 - specialChipSize / 2);
            specialChipsConstraints[numPlayers + 2]->setY(blindsChipOffset * cos(iAngle) + layoutSize.y / 2 - specialChipSize / 2);
            specialChipsConstraints[numPlayers + 2]->setW(specialChipSize);
        }

        iAngle = i * angleStep;
        playerInfoConstraints[i]->setX(playerInfoOffset * sin(iAngle) + layoutSize.x / 2 - playerInfoWidth / 2);
        playerInfoConstraints[i]->setY(playerInfoOffset * cos(iAngle) + layoutSize.y / 2 - (playerInfoWidth * .5f) / 2);
        playerInfoConstraints[i]->setW(playerInfoWidth);

        if (playerInfoConstraints[i]->getX() < 0)
            playerInfoConstraints[i]->setX(0);
        else if (playerInfoConstraints[i]->getY() < 0)
            playerInfoConstraints[i]->setY(0);
        else if (playerInfoConstraints[i]->getX() + playerInfoConstraints[i]->getW() > layoutSize.x)
            playerInfoConstraints[i]->setX(layoutSize.x - playerInfoConstraints[i]->getW());
        else if (playerInfoConstraints[i]->getY() + (playerInfoConstraints[i]->getH() * playerInfoConstraints[i]->getW()) > layoutSize.y)
            playerInfoConstraints[i]->setY(layoutSize.y - (playerInfoConstraints[i]->getH() * playerInfoConstraints[i]->getW()));
    }

    float communityCardOffset = cardSize * 1.1f;
    std::vector<float> communityCardPosX{0, -communityCardOffset * 2, -communityCardOffset, 0, 0, communityCardOffset, 0, communityCardOffset * 2},
        communityCardPosY{cardSize * 1.5f, 0, 0, 0, cardSize * 1.5f, 0, cardSize * 1.5f, 0},
        communityCardRot{M_PI / 2, 0, 0, 0, M_PI / 2 + .2f, 0, M_PI / 2 - .2f, 0};

    for (int i = 0; i < 8; i++)
    {
        cardsConstraints[numPlayers * 2 + i]->setX(communityCardPosX[i] + layoutSize.x / 2 - cardSize / 2);
        cardsConstraints[numPlayers * 2 + i]->setY(communityCardPosY[i] + layoutSize.y / 2 - (cardSize * 1.5f) / 2);
        cardsConstraints[numPlayers * 2 + i]->setW(cardSize);
        cards[numPlayers * 2 + i]->rotate(communityCardRot[i]);
    }

    chipsConstraints[numPlayers * 2]->setX(layoutSize.x / 2 - chipSize / 2);
    chipsConstraints[numPlayers * 2]->setY(-chipSize + layoutSize.y / 2 - chipSize * .3f);
    chipsConstraints[numPlayers * 2]->setW(chipSize);

    chipsLabelConstraints[numPlayers * 2]->setX(layoutSize.x / 2 - radius);
    chipsLabelConstraints[numPlayers * 2]->setY(-chipSize + layoutSize.y / 2 + chipSize * .4f);
    chipsLabelConstraints[numPlayers * 2]->setW(radius * 2);
    chipsLabelConstraints[numPlayers * 2]->setH(chipSize);
}