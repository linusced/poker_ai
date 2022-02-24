#include "../../../../header/poker/gui/poker_table_gui.hpp"

void poker::TableGUI::draw()
{
    double currentTime = glfwGetTime(), animationTime;
    glm::vec2 currentPos;
    bool resizePoly_b = false;

    for (int i = animations.size() - 1; i >= 0; i--)
        if (currentTime < animations[i].time)
        {
            animations[i].element->draw_b = false;
        }
        else if (currentTime > animations[i].time + animations[i].duration)
        {
            animations[i].constraints->setX(animations[i].endPos.x);
            animations[i].constraints->setY(animations[i].endPos.y);
            animations[i].element->resize(layout, *windowWidth, *windowHeight);

            if (animations[i].hideElement)
                animations[i].hideElement->draw_b = false;

            if (animations[i].showElement && !(hideCommunityPot && animations[i].showElement == chips[numPlayers * 2]))
                animations[i].showElement->draw_b = true;

            if (animations[i].resize)
                resizePoly_b = true;

            animations.erase(animations.begin() + i);
        }
        else
        {
            animations[i].element->draw_b = true;

            animationTime = currentTime - animations[i].time;
            currentPos = glm::mix(animations[i].startPos, animations[i].endPos, animationTime / animations[i].duration);

            animations[i].constraints->setX(currentPos.x);
            animations[i].constraints->setY(currentPos.y);
            animations[i].element->resize(layout, *windowWidth, *windowHeight);
        }

    if (resizePoly_b)
        resizePoly(*windowWidth, *windowHeight);

    for (Polygon *p : poly)
        p->draw();
    for (Element *ch : children)
        if (ch->draw_b)
            ch->draw();
}