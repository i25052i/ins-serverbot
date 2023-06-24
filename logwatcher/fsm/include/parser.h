#ifndef PARSER_H
#define PARSER_H

#include <string>

struct chat_message {
    std::string name;
    std::string id;
    std::string contents;
    unsigned channel; //0: blufor team chat, 1: redfor team chat, 2: global
};

struct player_join {
    std::string id;
};

struct state_change {
    std::string before;
    std::string after;
};

int parse_chat_message(std::string logline, struct chat_message *ret);

int parse_player_join(std::string logline, struct player_join *ret);

int parse_state_change(std::string logline, struct state_change *ret);

#endif