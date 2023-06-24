#include <string>
#include <iostream>

#include "parser.h"

/*
 * Detect if the server log line represents a chat message and enters that 
 * information in the chat_message struct pointed at by ret.
 */
int parse_chat_message(std::string logline, struct chat_message *ret) {
    //example chat message log line:
    //[2021.05.05-05.26.17:149][474]LogChat: Display: SENDER(SENDERSTEAM64) Global Chat: MESSAGE

    //discard log time and number (two ']'s)
    size_t iter;
    size_t user_start, user_end, msg_start;

    iter = logline.find(']');
    if (iter == std::string::npos) {
        return 1;
    }
    iter++;

    iter = logline.find(']', iter);
    if (iter == std::string::npos) {
        return 1;
    }
    iter++;

    //check logtype
    if (logline.find("LogChat: Display: ", iter) != iter) {
        return 1;
    } else {
        //move iter to the start of the sender's username
        iter += std::string("LogChat: Display: ").length();
        user_start = iter;
    }

    //check chat type as separator
    size_t team_0 = logline.find(" Team 0 Chat: ", iter);
    size_t team_1 = logline.find(" Team 1 Chat: ", iter);
    size_t global = logline.find(" Global Chat: ", iter);

    if (team_0 != std::string::npos) {
        iter = team_0;
        user_end = iter - 1;
        msg_start = iter + std::string(" Team 0 Chat: ").length();
        ret->channel = 0;
    } else if (team_1 != std::string::npos) {
        iter = team_1;
        user_end = iter - 1;
        msg_start = iter + std::string(" Team 1 Chat: ").length();
        ret->channel = 1;
    } else if (global != std::string::npos) {
        iter = global;
        user_end = iter - 1;
        msg_start = iter + std::string(" Global Chat: ").length();
        ret->channel = 2;
    } else {
        return 1;
    }


    //split username
    iter = user_end;
    iter = logline.rfind('(', iter);

    ret->name = logline.substr(user_start, iter - user_start);
    ret->id = logline.substr(iter + 1, user_end - iter - 1);
    ret->contents = logline.substr(msg_start, logline.length() - msg_start);

    return 0;
}

/*
 * Detect if the server log line represents a player join, and enters that 
 * information in the chat_message struct pointed at by ret.
 */
int parse_player_join(std::string logline, struct player_join *ret) {
    //example player join log line:
    //[2023.01.13-05.07.26:027][471]LogNet: Login request: ?Name=PLAYERNAME userId: SteamNWI:STEAM64 platform: SteamNWI

    //discard log time and number (two ']'s)
    size_t iter;

    iter = logline.find(']');
    if (iter == std::string::npos) {
        return 1;
    }
    iter++;

    iter = logline.find(']', iter);
    if (iter == std::string::npos) {
        return 1;
    }
    iter++;

    //check logtype
    if (logline.find("LogNet: Login request: ", iter) != iter) {
        return 1;
    } else {
        //move iter to point after the logtype
        iter += std::string("LogNet: Login request: ").length();
    }

    //parse username
    iter = logline.find("userId: SteamNWI:");
    if (iter != std::string::npos) {
        iter += std::string("userId: SteamNWI:").length();
        size_t end = logline.find(' ', iter);
        ret->id = logline.substr(iter, end - iter);
    } else {
        return 1;
    }

    return 0;
}

/*
 * Detect if the server log line represents a state change, which is a change
 * in the playing state of the round, game, or match (i.e. from waiting to start
 * to an active round) and enters that information in the chat_message struct 
 * pointed at by ret.
 */
int parse_state_change(std::string logline, struct state_change *ret) {
    //example state change log line:
    //[2023.01.13-05.12.49:664][823]LogGameMode: Display: State: RoundWon -> PostRound

    //discard log time and number (two ']'s)
    size_t iter, arrow;

    iter = logline.find(']');
    if (iter == std::string::npos) {
        return 1;
    }
    iter++;

    iter = logline.find(']', iter);
    if (iter == std::string::npos) {
        return 1;
    }
    iter++;

    //check logtype
    if (logline.find("LogGameMode: Display: State: ", iter) != iter) {
        return 1;
    } else {
        //move iter to point after the logtype
        iter += std::string("LogGameMode: Display: State: ").length();
    }

    //split into before and after states
    arrow = logline.find(" -> ", iter);
    if (arrow == std::string::npos) return 1;
    
    ret->before = logline.substr(iter, arrow - iter);
    ret->after = logline.substr(arrow + 4);

    return 0;
}