#ifndef LOGIC_H
#define LOGIC_H

#include <string>
#include <set>

#include "fsm.h"
#include "rcon.h"

/* default chat message handler */
class fsm_cmhandler : public fsm_base {
public:
    fsm_cmhandler(rcon_connection *connection);
    ~fsm_cmhandler();

    int update(std::string input);
};

/* default player join handler */
class fsm_pjhandler : public fsm_base {
public:
    fsm_pjhandler(rcon_connection *connection);
    ~fsm_pjhandler();

    int update(std::string input);
};

/* deafult state change handler */
class fsm_schandler : public fsm_base {
public:
    fsm_schandler(rcon_connection *connection);
    ~fsm_schandler();

    int update(std::string input);
};

/* whitelist */
class fsm_whitelist : public fsm_base {
    std::set<std::string> allowed;

public:
    fsm_whitelist(rcon_connection *connection);
    ~fsm_whitelist();
    
    int update(std::string input);

    void whitelist_add(std::string id);
    void whitelist_rem(std::string id);
};

#endif