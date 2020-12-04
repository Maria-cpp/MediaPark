#pragma once
#include "Socket.h"
#include "PacketManager.h"

namespace Net {
    class Connection {

    public:
        Connection(Socket const &socket, IPEndpoint const &endpoint);
        Connection();

        void close();

        bool send();
        bool recv();

        std::string toString() const noexcept;


        Socket socket;

        PacketManager pm_outgoing;
        PacketManager pm_incoming;

        char buffer[Packet::MaxSize]; // temporary buffer to recv partial data 

    private:
        IPEndpoint endpoint;
    };
}
