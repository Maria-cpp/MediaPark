#pragma once
#include "SocketHandle.h"

namespace Net {
    class Network {

    public:
        Network();
        ~Network();

        static bool startup();
        static bool shutdown();

        static bool Initialized;

    private:


    };
}
