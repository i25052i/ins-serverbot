#ifndef FSM_H
#define FSM_H

#include <vector>
#include <string>

#include "rcon.h"

class fsm_base {
protected:
    unsigned state;
    rcon_connection *conn;

public:
    fsm_base();
    fsm_base(rcon_connection *connection);
    virtual ~fsm_base();

    virtual int update(std::string input);
    int get_state();
};

class fsm_manager {
    std::vector<fsm_base *> fsm_ptrs;

public:
    fsm_manager();
    ~fsm_manager();

    void add_fsm(fsm_base *add);
    void rem_fsm(fsm_base *rem);
    void update_all(std::string input);
};

#endif