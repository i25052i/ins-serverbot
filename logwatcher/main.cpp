#include <iostream>
#include <string>

#include "logwatcher.h"
#include "rcon.h"

int main(int argc, char** argv[]) {

    /* Startup Messages */
    std::cout << "ins-serverbot" << std::endl;
    std::cout << "Version " << logwatcher_VERSION_MAJOR << "." 
              << logwatcher_VERSION_MINOR << std::endl;
    std::cout << "Starting..." << std::endl;

    //

    /* Watch server logs on stdin */
    while (true) {
        std::string line;

        std::getline(std::cin, line);

        if (line.empty()) {
            continue;
        }

        //std::cout << line << std::endl;
        break;
    }

    return 0;
}
