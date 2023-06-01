#include <iostream>
#include <string>

#include "logwatcher.h"
#include "rcon.h"

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

    /* Watch server logs on stdin */
    // while (true) {
    //     std::string line;

    //     std::getline(std::cin, line);

    //     if (line.empty()) {
    //         continue;
    //     }

    //     //std::cout << line << std::endl;
    //     break;
    // }

    std::string response, command;

    command = "say test";
    std::cout << "Testing: " << command << std::endl;
    rc.send(response, command);
    std::cout << "Response: " << response << std::endl;

    command = "scenarios";
    std::cout << "Testing: " << command << std::endl;
    rc.send(response, command);
    std::cout << "Response: " << response << std::endl;

    return 0;
}
