#ifndef POKER_CARD_GUI
#define POKER_CARD_GUI

#include "../poker_card.hpp"
#include "../../gl/gui.hpp"
#include "../../files.hpp"
#include <glm/gtx/rotate_vector.hpp>

namespace poker
{
    class CardGUI : public gui::Element
    {
    private:
        static Texture *pokerCardsTexture, *pokerCardsBackTexture;

        int *windowWidth = nullptr, *windowHeight = nullptr;
        float prevRot = 0;

    public:
        static void createTexture();
        static void deleteTexture();

        CardGUI(Card *card, gui::Constraints *constraints, int *windowWidth, int *windowHeight);

        void update(Card *card);
        void rotate(float rot);

        void resizePoly(int &windowWidth, int &windowHeight) override;
    };
};

#endif