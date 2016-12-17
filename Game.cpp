//
// Created by vrenclouff on 05/12/16.
//

#include <sstream>
#include "Game.h"

std::vector<int> Game::matches_of_layers = {1, 3, 5, 7, 9, 11};
std::vector<int> Game::number_of_matches = {1, 4, 9, 16, 25, 36};


int Game::actualMatchesOnLayer(int const layer)
{
    return layer <= _layers_count ? _layers[layer] : -1;
}

int Game::matches()
{
    int result = 0;
    for (int i = 0; i <= _layers_count; i++)
    {
        result += _layers[i];
    }
    return result;
}

void Game::takeOnLayer(int const layer)
{
    if (layer <= _layers_count)
    {
        _layers[layer]--;
    }
}

std::string Game::state()
{
    std::stringstream ss;
    for (int i = 0; i <= _layers_count; i++)
    {
        if(i != 0) { ss << ","; }
        ss << std::to_string(_layers[i]);
    }
    std::string result = ss.str();
    return result;
}
