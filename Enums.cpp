//
// Created by vrenclouff on 05/12/16.
//

#include "Enums.h"

std::vector<std::string> EnumUtils::network_state_str = {"LOGIN", "ALL_USERS", "GAME_JOIN", "GAME_TAKE", "LOGOUT", "HARD_LOGOUT", "GAME_END", "GAME_SWITCH_USER", "GAME_TURN", "GAME"};
std::vector<std::string> EnumUtils::user_state_str = {"LOGGED", "DISCONNECTED", "PLAYED"};

std::map<std::string, enums::network_state > EnumUtils::string_to_network_state = {
        {EnumUtils::network_state_str[enums::LOGIN],            enums::LOGIN},
        {EnumUtils::network_state_str[enums::ALL_USERS],        enums::ALL_USERS},
        {EnumUtils::network_state_str[enums::GAME_JOIN],        enums::GAME_JOIN},
        {EnumUtils::network_state_str[enums::GAME_TAKE],        enums::GAME_TAKE},
        {EnumUtils::network_state_str[enums::LOGOUT],           enums::LOGOUT},
        {EnumUtils::network_state_str[enums::HARD_LOGOUT],      enums::HARD_LOGOUT},
        {EnumUtils::network_state_str[enums::GAME_END],         enums::GAME_END},
        {EnumUtils::network_state_str[enums::GAME_SWITCH_USER], enums::GAME_SWITCH_USER},
        {EnumUtils::network_state_str[enums::GAME_TURN],        enums::GAME_TURN},
        {EnumUtils::network_state_str[enums::GAME],             enums::GAME}
};

std::map<std::string, enums::user_state > EnumUtils::string_to_user_state = {
        {EnumUtils::user_state_str[enums::LOGGED],      enums::LOGGED},
        {EnumUtils::user_state_str[enums::DISCONNECTED],enums::DISCONNECTED},
        {EnumUtils::user_state_str[enums::PLAYED],      enums::PLAYED}
};