#include "Network.h"
#include <iostream>

namespace Net {

    bool Network::Initialized = false;

    Network::Network() {
        if (!Initialized) {
            Initialized = startup();
        }
    }
    Network::~Network() {
        if (Initialized) {
            shutdown();
            Initialized = false;
        }
    }

    bool Network::startup() {

    #if defined(_WIN32)
        int versionLo = 2;
        int versionHi = 2;
        WSADATA wsadata;
        int ret = WSAStartup(MAKEWORD(versionLo, versionHi), &wsadata);
        // If winsock API failed to start up
        if (ret != 0) {
            std::cerr << "Failed to start up the winsock API." << std::endl;
            return false;
        }
        // If version received does not match version requested (2.2)
        if (LOBYTE(wsadata.wVersion) != versionLo || HIBYTE(wsadata.wVersion) != versionHi) {
            std::cerr << "Could not find a usable version of the winsock api dll." << std::endl;
            return false;
        } else {
            //std::cerr << "The Winsock dll was found okay." << std::endl;
        }
    #else

    #endif

        return true;
    }

    bool Network::shutdown() {

    #if defined(_WIN32)
        int ret = WSACleanup();
        return ret == NO_ERROR;
    #else
        return true;
    #endif

    }
}