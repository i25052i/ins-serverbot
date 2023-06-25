#include <iostream>
#include <string>

#include "logwatcher.h"
#include "rcon.h"
#include "fsm.h"
#include "features.h"
#include "parser.h"

int main(int argc, char* argv[]) {

    /* Startup messages */
    std::cout << "ins-serverbot" << std::endl;
    std::cout << "Starting..." << std::endl;

    /* Obtain command-line arguments and options */
    std::string hostname; //server hostname
    int port; //server port
    std::string password; //rcon password

    if (argc < 3) {
        //report version
        std::cout << argv[0] << " Version " << logwatcher_VERSION_MAJOR << "." 
                  << logwatcher_VERSION_MINOR << std::endl;
        //usage instructions
        std::cout << "Usage: " << argv[0] << " hostname port password" << std::endl;
        return 1;
    }

    hostname = std::string(argv[1]);
    port = atoi(argv[2]);
    password = std::string(argv[3]);

    for (int i = 0; i < argc; i++) {
        std::cout << i << ": " << argv[i] << std::endl;
    }
    std::cout << std::endl;

    /* Wait for server to open RCON port */
    while (true) {
        std::string line;

        std::getline(std::cin, line);

        if (line.empty()) {
            continue;
        }

        if (line.find("LogRcon: Rcon listening at ") != std::string::npos) {
            break;
        }
    }
    

    /* Initialize connection to server */
    std::cout << "Connecting to server " << hostname << ':' << port << std::endl;
    rcon_connection rc = rcon_connection(hostname, port, password);

    if (!rc.is_connected() || !rc.is_authenticated()) {
        //check connection
        std::cout << "Error connecting to server" << std::endl;
        return 1;
    } else {
        std::cout << "Connection successful" << std::endl;
    }
    std::cout << std::endl;

    /* Initialize logic for features */
    std::cout << "Initializing features" << std::endl;
    fsm_manager manager = fsm_manager();

    fsm_base *fsm_cmh = new fsm_cmhandler(&rc);
    manager.add_fsm(fsm_cmh);
    fsm_base *fsm_pjh = new fsm_pjhandler(&rc);
    manager.add_fsm(fsm_pjh);
    fsm_base *fsm_sch = new fsm_schandler(&rc);
    manager.add_fsm(fsm_sch);

    /* Watch server logs on stdin */
    while (true) {
        std::string line;

        std::getline(std::cin, line);

        if (line.empty()) {
            continue;
        }

        manager.update_all(line);
    }

    return 0;
}
