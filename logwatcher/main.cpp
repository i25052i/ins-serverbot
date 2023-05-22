#include <iostream>
#include <string>

#include "logwatcher.h"

int main(int argc, char** argv[]) {

    /* Startup Messages */
    std::cout << "ins-serverbot" << std::endl;
    std::cout << "Version " << logwatcher_VERSION_MAJOR << "." 
              << logwatcher_VERSION_MINOR << std::endl;
    std::cout << "Starting..." << std::endl;

    /* Consume server logs */
    while (true) {
        std::string line;

        std::getline(std::cin, line);

        if (line.empty()) {
            continue;
        }

        std::cout << line << std::endl;
    }

    return 0;
}
