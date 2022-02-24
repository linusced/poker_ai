#ifndef POKER_COMMUNITY_POT
#define POKER_COMMUNITY_POT

#include <vector>

namespace poker
{
    struct CommunityPot
    {
        int money = 0;
        std::vector<int> players;
    };
};

#endif