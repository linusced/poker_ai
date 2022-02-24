#include "../../../../header/poker/gui/poker_table_gui.hpp"

void poker::TableGUI::init(int numPlayers, int startMoney)
{
    this->numPlayers = numPlayers;

    std::vector<Vertex2D> vertices{Vertex2D(glm::vec2(0), glm::vec2(.5, .5))};
    std::vector<unsigned int> indices;
    unsigned int size = 64;
    float angleStep = (M_PI * 2) / (size - 2), iAngle;

    for (unsigned int i = 1; i < size; i++)
    {
        iAngle = (i - 1) * angleStep;
        vertices.push_back(Vertex2D(glm::vec2(0), glm::vec2(sin(iAngle) * .5f + .5f, cos(iAngle) * .5f + .5f)));

        if (i > 1)
            indices.insert(indices.end(), {0, i - 1, i});
    }

    poly.push_back(new Polygon({}, indices));
    poly.push_back(new Polygon(vertices, indices, filePath("textures/poker_table_green.jpg")));

    for (auto &v : vertices)
        v.color = glm::vec4(glm::vec3(.15), 1);

    poly[0]->vertices = vertices;

    master->saveConstraints("playerInfoText_0", new gui::Constraints(gui::CENTER_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT, .05, gui::RELATIVE_CONSTRAINT, .9, gui::RELATIVE_CONSTRAINT, .4));
    master->saveConstraints("playerInfoText_1", new gui::Constraints(gui::CENTER_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT_INVERSE, .05, gui::RELATIVE_CONSTRAINT, .9, gui::RELATIVE_CONSTRAINT, .4));

    for (int i = 0; i < numPlayers; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            chipsConstraints.push_back(new gui::Constraints(gui::PIXEL_CONSTRAINT, 0, gui::PIXEL_CONSTRAINT, 0, gui::PIXEL_CONSTRAINT, 0, gui::ASPECT_CONSTRAINT, 1));
            master->saveConstraints("chips_" + std::to_string(i) + '_' + std::to_string(j), chipsConstraints.back());
            chips.push_back(new ChipsGUI(j == 0, chipsConstraints.back()));
            chips.back()->draw_b = j == 0;
            addChild(chips.back());

            chipsLabelConstraints.push_back(new gui::Constraints(gui::PIXEL_CONSTRAINT, 0, gui::PIXEL_CONSTRAINT, 0, gui::PIXEL_CONSTRAINT, 0, gui::PIXEL_CONSTRAINT, 0));
            master->saveConstraints("chipsLabel_" + std::to_string(i) + '_' + std::to_string(j), chipsLabelConstraints.back());
            chipsLabel.push_back(new gui::native::Text(font, j == 0 ? (std::to_string(startMoney) + '$') : "", 28, glm::vec3(1), false, gui::ALIGNMENT_CENTER_H, gui::ALIGNMENT_TOP_V, chipsLabelConstraints.back()));
            chipsLabel.back()->draw_b = j == 0;
            addChild(chipsLabel.back());

            cardsConstraints.push_back(new gui::Constraints(gui::PIXEL_CONSTRAINT, 0, gui::PIXEL_CONSTRAINT, 0, gui::PIXEL_CONSTRAINT, 0, gui::ASPECT_CONSTRAINT, 1.5));
            master->saveConstraints("cards_" + std::to_string(i) + '_' + std::to_string(j), cardsConstraints.back());
            cards.push_back(new CardGUI(nullptr, cardsConstraints.back(), windowWidth, windowHeight));
            cards.back()->draw_b = false;
            addChild(cards.back());
        }

        specialChipsConstraints.push_back(new gui::Constraints(gui::PIXEL_CONSTRAINT, 0, gui::PIXEL_CONSTRAINT, 0, gui::PIXEL_CONSTRAINT, 0, gui::ASPECT_CONSTRAINT, 1));
        master->saveConstraints("allIn_" + std::to_string(i), specialChipsConstraints.back());
        specialChips.push_back(new SpecialChipsGUI(SpecialChipsGUI::ALL_IN, specialChipsConstraints.back()));
        specialChips.back()->draw_b = false;
        addChild(specialChips.back());

        playerInfoConstraints.push_back(new gui::Constraints(gui::PIXEL_CONSTRAINT, 0, gui::PIXEL_CONSTRAINT, 0, gui::PIXEL_CONSTRAINT, 0, gui::ASPECT_CONSTRAINT, .5));
        master->saveConstraints("playerInfo_" + std::to_string(i), playerInfoConstraints.back());
        playerInfo.push_back(new gui::native::Container({.2, .2, .2, .6}, 20, playerInfoConstraints.back()));
        addChild(playerInfo.back());

        for (int j = 0; j < 2; j++)
        {
            playerInfoText.push_back(new gui::native::Text(font, "", 30, glm::vec3(1), false, gui::ALIGNMENT_CENTER_H, gui::ALIGNMENT_CENTER_V, master->getConstraints("playerInfoText_" + std::to_string(j))));
            playerInfo.back()->addChild(playerInfoText.back());
        }
    }

    for (int i = 0; i < 8; i++)
    {
        cardsConstraints.push_back(new gui::Constraints(gui::PIXEL_CONSTRAINT, 0, gui::PIXEL_CONSTRAINT, 0, gui::PIXEL_CONSTRAINT, 0, gui::ASPECT_CONSTRAINT, 1.5));
        master->saveConstraints("communityCards_" + std::to_string(i), cardsConstraints.back());
        cards.push_back(new CardGUI(nullptr, cardsConstraints.back(), windowWidth, windowHeight));
        cards.back()->draw_b = false;
        addChild(cards.back());
    }

    chipsConstraints.push_back(new gui::Constraints(gui::PIXEL_CONSTRAINT, 0, gui::PIXEL_CONSTRAINT, 0, gui::PIXEL_CONSTRAINT, 0, gui::ASPECT_CONSTRAINT, 1));
    master->saveConstraints("communityChips", chipsConstraints.back());
    chips.push_back(new ChipsGUI(true, chipsConstraints.back()));
    chips.back()->draw_b = false;
    addChild(chips.back());

    chipsLabelConstraints.push_back(new gui::Constraints(gui::PIXEL_CONSTRAINT, 0, gui::PIXEL_CONSTRAINT, 0, gui::PIXEL_CONSTRAINT, 0, gui::PIXEL_CONSTRAINT, 0));
    master->saveConstraints("communityChipsLabel", chipsLabelConstraints.back());
    chipsLabel.push_back(new gui::native::Text(font, "", 28, glm::vec3(1), false, gui::ALIGNMENT_CENTER_H, gui::ALIGNMENT_TOP_V, chipsLabelConstraints.back()));
    chipsLabel.back()->draw_b = false;
    addChild(chipsLabel.back());

    specialChipsConstraints.push_back(new gui::Constraints(gui::PIXEL_CONSTRAINT, 0, gui::PIXEL_CONSTRAINT, 0, gui::PIXEL_CONSTRAINT, 0, gui::ASPECT_CONSTRAINT, 1));
    master->saveConstraints("dealerChip", specialChipsConstraints.back());
    specialChips.push_back(new SpecialChipsGUI(SpecialChipsGUI::DEALER, specialChipsConstraints.back()));
    specialChips.back()->draw_b = false;
    addChild(specialChips.back());

    specialChipsConstraints.push_back(new gui::Constraints(gui::PIXEL_CONSTRAINT, 0, gui::PIXEL_CONSTRAINT, 0, gui::PIXEL_CONSTRAINT, 0, gui::ASPECT_CONSTRAINT, 1));
    master->saveConstraints("smallBlindChip", specialChipsConstraints.back());
    specialChips.push_back(new SpecialChipsGUI(SpecialChipsGUI::SMALL_BLIND, specialChipsConstraints.back()));
    specialChips.back()->draw_b = false;
    addChild(specialChips.back());

    specialChipsConstraints.push_back(new gui::Constraints(gui::PIXEL_CONSTRAINT, 0, gui::PIXEL_CONSTRAINT, 0, gui::PIXEL_CONSTRAINT, 0, gui::ASPECT_CONSTRAINT, 1));
    master->saveConstraints("bigBlindChip", specialChipsConstraints.back());
    specialChips.push_back(new SpecialChipsGUI(SpecialChipsGUI::BIG_BLIND, specialChipsConstraints.back()));
    specialChips.back()->draw_b = false;
    addChild(specialChips.back());

    master->resize(*windowWidth, *windowHeight);
}