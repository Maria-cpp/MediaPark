#pragma once
#include "SocketHandle.h"
#include "SocketResult.h"
#include "IPVersion.h"
#include "SocketOption.h"
#include "IPEndpoint.h"
#include "Packet.h"

namespace Net {
    class Socket {

    public:

        Socket(IPVersion ipVersion, SOCKET hSocket = INVALID_SOCKET) noexcept;
        Socket() noexcept;

        SOCKET getHandle() const noexcept;
        IPVersion getIPVersion() const noexcept;

        bool create(int addressFamily, int type, int protocol) noexcept;
        bool create() noexcept;
        bool close() noexcept;

        bool isValid() const noexcept;

        SocketResult bind(IPEndpoint const &endpoint) const noexcept;
        SocketResult bind(std::string_view ip, uint16_t port) const noexcept;
        SocketResult listen(int backlog = 5) const noexcept;
        SocketResult accept(Socket &acceptedSocket, IPEndpoint *endpoint = nullptr) const noexcept;
        SocketResult connect(IPEndpoint const &endpoint) const noexcept;
        SocketResult connect(std::string_view ip, uint16_t port) const noexcept;

        SocketResult send(char const *buffer, int buffLen, int &sendLen, int flags = MSG_NOSIGNAL) const noexcept;
        SocketResult recv(char       *buffer, int buffLen, int &recvLen, int flags = MSG_DONTWAIT) const noexcept;

        SocketResult sendAll(char const *buffer, int buffLen, int flags = MSG_NOSIGNAL) const noexcept;
        SocketResult recvAll(char       *buffer, int buffLen, int flags = MSG_DONTWAIT) const noexcept;

        SocketResult send(Packet const &packet, int flags = MSG_NOSIGNAL) const noexcept;
        SocketResult recv(Packet       &packet, int flags = MSG_DONTWAIT) const noexcept;

        bool setBlocking(bool isBlocking) noexcept;
        bool setOption(int protocol, int optionName, int value) noexcept;
        bool shutdown(int how) noexcept;

        std::string getPeerName() const noexcept;

        std::string toString() const noexcept;

    private:

        IPVersion ipVersion;
        SOCKET    hSocket;
    };
}
