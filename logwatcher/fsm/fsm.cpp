#include <string>

#include "fsm.h"
#include "rcon.h"

fsm_base::fsm_base(): state(0), conn(nullptr) {}

fsm_base::fsm_base(rcon_connection *connection): state(0), conn(connection) {}

fsm_base::~fsm_base() {}

int fsm_base::update(std::string input) {
    return state;
}

int fsm_base::get_state() {
    return state;
}

fsm_manager::fsm_manager(): fsm_ptrs() {}

fsm_manager::~fsm_manager() {}

void fsm_manager::add_fsm(fsm_base *add) {
    fsm_ptrs.push_back(add);
}

void fsm_manager::rem_fsm(fsm_base *rem) {
    for (auto it = fsm_ptrs.begin(); it < fsm_ptrs.end(); it++) {
        if (*it == rem) {
            fsm_ptrs.erase(it);
            return;
        }
    }
}

void fsm_manager::update_all(std::string input) {
    for (fsm_base *fsm : fsm_ptrs) {
        fsm->update(input);
    }
}
