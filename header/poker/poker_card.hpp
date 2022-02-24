#ifndef POKER_CARD
#define POKER_CARD

#include <string>
#include <stdexcept>
#include <glm/glm.hpp>
#include <iostream>

namespace poker
{
    class Card
    {
    private:
        char suit, index;

    public:
        static const std::string SUITS, INDICES;
        static std::string getSuitNameByValue(int suitValue), getIndexNameByValue(int indexValue);

        Card();
        Card(char suit, char index);
        Card(std::string suit, std::string index);

        void setSuit(char newSuit), setIndex(char newIndex);
        void setSuit(std::string newSuit), setIndex(std::string newIndex);

        char getSuit() const, getIndex() const;
        std::string getSuitName() const, getIndexName() const;

        void getTextureCoords(glm::vec2 *_0, glm::vec2 *_1, glm::vec2 *_2, glm::vec2 *_3) const;
    };
};

#endif