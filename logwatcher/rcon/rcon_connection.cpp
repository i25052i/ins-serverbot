#include <string>
#include <stdio.h>
#include <sys/types.h>

#include "rcon.h"

/* rcon connection class implementation */

rcon_connection::rcon_connection(const std::string address, const int port, const std::string password, const double timeout):
    address(address),
    port(port),
    password(password),
    counter(0),
    //socket_fd(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)),
    connected(false),
    authenticated(false)
{
    
}