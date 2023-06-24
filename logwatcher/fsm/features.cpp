#include <string>
#include <set>
#include <iostream>

#include "fsm.h"
#include "rcon.h"
#include "features.h"
#include "parser.h"

/* default chat message handler */

fsm_cmhandler::fsm_cmhandler(rcon_connection *connection):
    fsm_base::fsm_base(connection) {}

fsm_cmhandler::~fsm_cmhandler() {}

int fsm_cmhandler::update(std::string input) {
    int result;
    struct chat_message cm;

    result = parse_chat_message(input, &cm);
    if (result) return state; //do nothing

    //report event
    std::cout << "chat_message: " << cm.name << " (" << cm.id << ")[" << cm.channel << "]: " << cm.contents << std::endl;

    state = 0;
    return state;
}

/* default player join handler */

fsm_pjhandler::fsm_pjhandler(rcon_connection *connection):
    fsm_base::fsm_base(connection) {}

fsm_pjhandler::~fsm_pjhandler() {}

int fsm_pjhandler::update(std::string input) {
    int result;
    struct player_join pj;

    result = parse_player_join(input, &pj);
    if (result) return state; //do nothing

    //report event
    std::cout << "player_join: " << pj.id << std::endl;

    state = 0;
    return state;
}

/* deafult state change handler */

fsm_schandler::fsm_schandler(rcon_connection *connection):
    fsm_base::fsm_base(connection) {}

fsm_schandler::~fsm_schandler() {}

int fsm_schandler::update(std::string input) {
    int result;
    struct state_change sc;

    result = parse_state_change(input, &sc);
    if (result) return state; //do nothing

    //report event
    std::cout << "state_change: " << sc.before << " -> " << sc.after << std::endl;

    state = 0;
    return state;
}

/* whitelist */

fsm_whitelist::fsm_whitelist(rcon_connection *connection):
    fsm_base::fsm_base(connection) {}

fsm_whitelist::~fsm_whitelist() {}

int fsm_whitelist::update(std::string input) {
    //parse input
    return 0;
}

void fsm_whitelist::whitelist_add(std::string id) {
    allowed.insert(id);
}

void fsm_whitelist::whitelist_rem(std::string id) {
    allowed.erase(id);
}
