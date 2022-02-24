#include "../../../header/poker/gui/poker_chips_gui.hpp"

Texture *poker::ChipsGUI::pokerChipsTextureLarge = nullptr, *poker::ChipsGUI::pokerChipsTextureSmall = nullptr;

void poker::ChipsGUI::createTexture()
{
    if (!pokerChipsTextureLarge)
        pokerChipsTextureLarge = new Texture(filePath("textures/poker_chips_large.png"), Texture::DIFFUSE);
    if (!pokerChipsTextureSmall)
        pokerChipsTextureSmall = new Texture(filePath("textures/poker_chips_small.png"), Texture::DIFFUSE);
}
void poker::ChipsGUI::deleteTexture()
{
    if (pokerChipsTextureLarge)
        delete pokerChipsTextureLarge;
    if (pokerChipsTextureSmall)
        delete pokerChipsTextureSmall;

    pokerChipsTextureLarge = pokerChipsTextureSmall = nullptr;
}

poker::ChipsGUI::ChipsGUI(bool isLarge, gui::Constraints *constraints)
    : gui::Element(constraints)
{
    std::vector<Vertex2D> vertices{Vertex2D(glm::vec2(0), glm::vec2(0, 0)), Vertex2D(glm::vec2(0), glm::vec2(1, 0)), Vertex2D(glm::vec2(0), glm::vec2(1, 1)), Vertex2D(glm::vec2(0), glm::vec2(0, 1))};
    poly.push_back(new Polygon(vertices, {0, 1, 2, 0, 2, 3}, isLarge ? pokerChipsTextureLarge : pokerChipsTextureSmall));
}

void poker::ChipsGUI::resizePoly(int &windowWidth, int &windowHeight)
{
    poly[0]->vertices[0].position = {layout->x, layout->y};
    poly[0]->vertices[1].position = {layout->x + layout->w, layout->y};
    poly[0]->vertices[2].position = {layout->x + layout->w, layout->y + layout->h};
    poly[0]->vertices[3].position = {layout->x, layout->y + layout->h};

    poly[0]->applyVertexChange();
}