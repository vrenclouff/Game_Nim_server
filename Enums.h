//
// Created by vrenclouff on 05/12/16.
//


#ifndef NIMSERVER_ENUMS_H
#define NIMSERVER_ENUMS_H

#include <vector>
#include <string>
#include <map>


namespace enums {

    enum user_state {LOGGED, DISCONNECTED, PLAYED};
    enum network_state {LOGIN, ALL_USERS, GAME_JOIN, GAME_TAKE, LOGOUT, HARD_LOGOUT, GAME_END, GAME_SWITCH_USER, GAME_TURN, GAME};
}

class EnumUtils {
private:
    static std::map<std::string, enums::network_state > string_to_network_state;
    static std::map<std::string, enums::user_state > string_to_user_state;

public:
    static std::vector<std::string> network_state_str;
    static std::vector<std::string> user_state_str;

    static std::string to_string(enums::user_state state)
    {
        return EnumUtils::user_state_str[state];
    }

    static std::string to_string(enums::network_state state)
    {
        return EnumUtils::network_state_str[state];
    }

    static enums::network_state to_network_state(std::string const &state_string)
    {
        return EnumUtils::string_to_network_state[state_string];
    }

    static enums::user_state to_user_state(std::string const &state_string)
    {
        return EnumUtils::string_to_user_state[state_string];
    }
};


#endif //NIMSERVER_ENUMS_H
