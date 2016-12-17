//
// Created by vrenclouff on 05/12/16.
//

#ifndef NIMSERVER_GAME_H
#define NIMSERVER_GAME_H

#include <vector>
#include <string>

class Game {
private:
    static std::vector<int> matches_of_layers;
    static std::vector<int> number_of_matches;

    std::vector<int> _layers;
    int _layers_count;
    int _take_max;

    int _on_the_turn;

public:
    Game(int const player_first, int const player_second, int const layers_count, int const take_max, int const id)
    {
        this->_on_the_turn = player_first;
        this->player_first = player_first;
        this->player_second = player_second;
        this->_layers_count = layers_count - 1;
        this->_take_max = take_max;
        this->take_counter = take_max;
        this->id = id;
        this->_layers = matches_of_layers;
    }

    void resetTake()
    {
        take_counter = _take_max;
    }

    int onTheTurn()
    {
        return _on_the_turn;
    }

    int onNotTheTurn()
    {
        return _on_the_turn == player_first ? player_second : player_first;
    }

    void switchUser()
    {
        _on_the_turn = _on_the_turn == player_first ? player_second : player_first;
    }

    int id;
    int player_first;
    int player_second;
    int take_counter;

    int actualMatchesOnLayer(int const layer);
    int matches();
    void takeOnLayer(int const layer);
    std::string state();

};


#endif //NIMSERVER_GAME_H
