#include "../../../header/poker/gui/poker_card_gui.hpp"

Texture *poker::CardGUI::pokerCardsTexture = nullptr, *poker::CardGUI::pokerCardsBackTexture;

void poker::CardGUI::createTexture()
{
    if (!pokerCardsTexture)
        pokerCardsTexture = new Texture(filePath("textures/poker_cards.jpg"), Texture::DIFFUSE);
    if (!pokerCardsBackTexture)
        pokerCardsBackTexture = new Texture(filePath("textures/poker_cards_back.png"), Texture::DIFFUSE);
}
void poker::CardGUI::deleteTexture()
{
    if (pokerCardsTexture)
        delete pokerCardsTexture;
    if (pokerCardsBackTexture)
        delete pokerCardsBackTexture;

    pokerCardsTexture = nullptr;
    pokerCardsBackTexture = nullptr;
}

poker::CardGUI::CardGUI(Card *card, gui::Constraints *constraints, int *windowWidth, int *windowHeight)
    : gui::Element(constraints)
{
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;

    std::vector<Vertex2D> vertices;
    for (int i = 0; i < 4; i++)
        vertices.push_back(Vertex2D(glm::vec2(0), glm::vec2(0)));

    if (card)
    {
        card->getTextureCoords(&vertices[0].texture, &vertices[1].texture, &vertices[2].texture, &vertices[3].texture);
    }
    else
    {
        vertices[0].texture = glm::vec2(0, 0);
        vertices[1].texture = glm::vec2(1, 0);
        vertices[2].texture = glm::vec2(1, 1);
        vertices[3].texture = glm::vec2(0, 1);
    }

    poly.push_back(new Polygon(vertices, {0, 1, 2, 0, 2, 3}, card ? pokerCardsTexture : pokerCardsBackTexture));
}

void poker::CardGUI::update(Card *card)
{
    if (card)
        card->getTextureCoords(&poly[0]->vertices[0].texture, &poly[0]->vertices[1].texture, &poly[0]->vertices[2].texture, &poly[0]->vertices[3].texture);
    else
    {
        poly[0]->vertices[0].texture = glm::vec2(0, 0);
        poly[0]->vertices[1].texture = glm::vec2(1, 0);
        poly[0]->vertices[2].texture = glm::vec2(1, 1);
        poly[0]->vertices[3].texture = glm::vec2(0, 1);
    }

    poly[0]->applyVertexChange();
    poly[0]->diffuse = card ? pokerCardsTexture : pokerCardsBackTexture;
}
void poker::CardGUI::rotate(float rot)
{
    prevRot = rot;
    if (!layout)
        return;

    std::vector<Vertex2D> &v = poly[0]->vertices;

    v[0].position = {layout->x, layout->y};
    v[1].position = {layout->x + layout->w, layout->y};
    v[2].position = {layout->x + layout->w, layout->y + layout->h};
    v[3].position = {layout->x, layout->y + layout->h};

    if (rot != 0)
    {
        glm::vec2 windowSize = {*windowWidth, *windowHeight},
                  center{layout->x + layout->w / 2, layout->y + layout->h / 2};

        for (int i = 0; i < v.size(); i++)
            v[i].position = (glm::rotate((v[i].position - center) * windowSize, rot) / windowSize) + center;
    }

    poly[0]->applyVertexChange();
}

void poker::CardGUI::resizePoly(int &windowWidth, int &windowHeight)
{
    rotate(prevRot);
}