#ifndef POKER_CHIPS_GUI
#define POKER_CHIPS_GUI

#include "../../gl/gui.hpp"
#include "../../files.hpp"

namespace poker
{
    class ChipsGUI : public gui::Element
    {
    private:
        static Texture *pokerChipsTextureLarge, *pokerChipsTextureSmall;

    public:
        static void createTexture();
        static void deleteTexture();

        ChipsGUI(bool isLarge, gui::Constraints *constraints);

        void resizePoly(int &windowWidth, int &windowHeight) override;
    };
};

#endif