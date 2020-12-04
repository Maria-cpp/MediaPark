#pragma once

#include <string_view>
#include "Connection.h"
#include <poll.h>


using namespace Net;

namespace Net {
    class Server {

    public:
        bool initialize(IPEndpoint const &endpoint);
        bool initialize(std::string_view ip, uint16_t port);

        void frame();

        Connection& getConnection(int i);

    protected:

        virtual bool processPacket(std::shared_ptr<Packet> const &packet);
        virtual void onConnect(Connection &newConnection);
        virtual void onDisconnect(Connection &lostConnection, std::string const &reason);

        void closeConnection(int connectionIndex, std::string const &reason);

        Socket listeningSocket;

        std::vector<Connection> connections;

        std::vector<pollfd> master_fd;
        std::vector<pollfd> use_fd;
    };
}
