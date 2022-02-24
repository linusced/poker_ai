#ifndef POKER_SPECIAL_CHIPS_GUI
#define POKER_SPECIAL_CHIPS_GUI

#include "../../gl/gui.hpp"
#include "../../files.hpp"

namespace poker
{
    class SpecialChipsGUI : public gui::Element
    {
    private:
        static Texture *pokerSpecialChipsTexture;

    public:
        static void createTexture();
        static void deleteTexture();

        enum TYPE
        {
            DEALER,
            SMALL_BLIND,
            BIG_BLIND,
            ALL_IN
        };

        SpecialChipsGUI(TYPE chipType, gui::Constraints *constraints);

        void resizePoly(int &windowWidth, int &windowHeight) override;
    };
};

#endif