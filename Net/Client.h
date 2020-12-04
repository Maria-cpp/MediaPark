#pragma once

#include <string_view>
#include "Connection.h"
#include <poll.h>

namespace Net {
    class Client {

    public:
        bool connect(IPEndpoint const &endpoint);
        bool connect(std::string_view ip, uint16_t port);

        bool isConnected();
        bool frame();

    protected:
        virtual bool processPacket(std::shared_ptr<Packet> const &packet);
        virtual void onConnect();
        virtual void onConnectFail();
        virtual void onDisconnect(std::string const &reason);
        void closeConnection(std::string const &reason);

    public:
        Connection connection;

    private:
        bool _isConnected = false;
        pollfd master_fd;
    };
}
