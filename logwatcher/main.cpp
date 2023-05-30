#include <iostream>
#include <string>

#include "logwatcher.h"
#include "rcon.h"

int main(int argc, char* argv[]) {

    /* Startup Messages */
    std::cout << "ins-serverbot" << std::endl;
    std::cout << "Version " << logwatcher_VERSION_MAJOR << "." 
              << logwatcher_VERSION_MINOR << std::endl;
    std::cout << "Starting..." << std::endl;

    for (int i = 0; i < argc; i++) {
        std::cout << i << ": " << argv[i] << std::endl;
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

    std::cout << "Instantiating connection" << std::endl;
    rcon_connection rc = rcon_connection("localhost", 27015, "gavinmeme");
    std::cout << "Connected? " << rc.is_connected() << " Authenticated? " << rc.is_authenticated() << std::endl;

    if (rc.is_authenticated()) {
        std::string response, command;

        command = "say test";
        std::cout << "Testing: " << command << std::endl;
        rc.send(response, command);
        std::cout << "Response: " << response << std::endl;

        command = "scenarios";
        std::cout << "Testing: " << command << std::endl;
        rc.send(response, command);
        std::cout << "Response: " << response << std::endl;

        command = "travelscenario Scenario_Crossing_Checkpoint_Security";
        std::cout << "Testing: " << command << std::endl;
        rc.send(response, command);
        std::cout << "Response: " << response << std::endl;
    }

    return 0;
}
