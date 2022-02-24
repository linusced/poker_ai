#include "../../../header/poker/gui/poker_special_chips_gui.hpp"

Texture *poker::SpecialChipsGUI::pokerSpecialChipsTexture = nullptr;

void poker::SpecialChipsGUI::createTexture()
{
    if (!pokerSpecialChipsTexture)
        pokerSpecialChipsTexture = new Texture(filePath("textures/poker_special_chips.png"), Texture::DIFFUSE);
}
void poker::SpecialChipsGUI::deleteTexture()
{
    if (pokerSpecialChipsTexture)
        delete pokerSpecialChipsTexture;

    pokerSpecialChipsTexture = nullptr;
}

poker::SpecialChipsGUI::SpecialChipsGUI(TYPE chipType, gui::Constraints *constraints)
    : gui::Element(constraints)
{
    std::vector<Vertex2D> vertices{Vertex2D(glm::vec2(0), glm::vec2(0, 0)), Vertex2D(glm::vec2(0), glm::vec2(.5, 0)), Vertex2D(glm::vec2(0), glm::vec2(.5, .5)), Vertex2D(glm::vec2(0), glm::vec2(0, .5))};

    glm::vec2 texCoordOffset;
    switch (chipType)
    {
    case DEALER:
        texCoordOffset = {0, 0};
        break;
    case SMALL_BLIND:
        texCoordOffset = {.5, 0};
        break;
    case BIG_BLIND:
        texCoordOffset = {0, .5};
        break;
    case ALL_IN:
        texCoordOffset = {.5, .5};
        break;
    }

    for (auto &v : vertices)
        v.texture += texCoordOffset;

    poly.push_back(new Polygon(vertices, {0, 1, 2, 0, 2, 3}, pokerSpecialChipsTexture));
}

void poker::SpecialChipsGUI::resizePoly(int &windowWidth, int &windowHeight)
{
    poly[0]->vertices[0].position = {layout->x, layout->y};
    poly[0]->vertices[1].position = {layout->x + layout->w, layout->y};
    poly[0]->vertices[2].position = {layout->x + layout->w, layout->y + layout->h};
    poly[0]->vertices[3].position = {layout->x, layout->y + layout->h};

    poly[0]->applyVertexChange();
}