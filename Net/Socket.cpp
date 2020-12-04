#include "Socket.h"

#include <cstring>
#include <cassert>
#include <iostream>
#include <sstream>

namespace Net {

    Socket::Socket(IPVersion ipVersion, SOCKET hSocket) noexcept {

        this->ipVersion = ipVersion;

        if (ipVersion == IP_UNKNOWN) {
            hSocket = INVALID_SOCKET;
        } else {
            /*
            if (!socket_isValid(hSocket)) {
                create();
                return;
            }
            */
        }
        this->hSocket = hSocket;
    }
    Socket::Socket() noexcept :
        Socket{ IP_UNKNOWN, INVALID_SOCKET } {
    }

    SOCKET Socket::getHandle() const noexcept {
        return hSocket;
    }

    IPVersion Socket::getIPVersion() const noexcept {
        return ipVersion;
    }

    bool Socket::create(int addressFamily, int type, int protocol) noexcept {

        hSocket = socket_create(addressFamily, type, protocol);
        return isValid();
    }

    bool Socket::create() noexcept {

        int addressFamily; // domain
        switch (ipVersion) {
        case IP_V4:
            addressFamily = AF_INET;
            break;
        case IP_V6:
            addressFamily = AF_INET6;
            break;
        default:
            ipVersion = IP_V4;
            addressFamily = AF_INET;
            break;
        }

        int type = SOCK_STREAM;
        int protocol = IPPROTO_TCP;

        return create(addressFamily, type, protocol);
    }

    bool Socket::close() noexcept {
        return socket_close(hSocket);
    }

    bool Socket::isValid() const noexcept {
        return socket_isValid(hSocket);
    }

    SocketResult Socket::bind(IPEndpoint const &endpoint) const noexcept {
        //assert(ipVersion == endpoint.getIPVersion());

        sockaddr_in addr4;
        sockaddr_in6 addr6;
        sockaddr *addrPtr = nullptr;
        socklen_t addrLen = 0;

        switch (ipVersion) {
        case IP_V4: {
            addr4 = endpoint.getSockaddrIPv4();
            addrPtr = (sockaddr*)&addr4;
            addrLen = sizeof(addr4);
        }
            break;
        case IP_V6: {
            addr6 = endpoint.getSockaddrIPv6();
            addrPtr = (sockaddr*)&addr6;
            addrLen = sizeof(addr6);

            if (!socket_setOption(hSocket, IPPROTO_IPV6, IPV6_V6ONLY, FALSE)) {
                return SR_ERROR;
            }
        }
            break;
        default:
            break;
        }

        if (!socket_bind(hSocket, addrPtr, addrLen)) {
            return SR_ERROR;
        }

        return SR_SUCCESS;
    }

    SocketResult Socket::bind(std::string_view ip, uint16_t port) const noexcept {
        return bind(IPEndpoint(ip, port));
    }

    SocketResult Socket::listen(int backlog) const noexcept {

        if (!socket_listen(hSocket, backlog)) {
            return SR_ERROR;
        }

        return SR_SUCCESS;
    }

    SocketResult Socket::accept(Socket &acceptedSocket, IPEndpoint *endpoint) const noexcept {
        assert(ipVersion == IP_V4
            || ipVersion == IP_V6);

        sockaddr_in addr4;
        sockaddr_in6 addr6;
        sockaddr *addrPtr = nullptr;
        socklen_t addrLen = 0;

        switch (ipVersion) {
        case IP_V4: {
            std::memset(&addr4, 0, sizeof(addr4));
            addrPtr = (sockaddr*)&addr4;
            addrLen = sizeof(addr4);
        }
            break;
        case IP_V6: {
            std::memset(&addr6, 0, sizeof(addr6));
            addrPtr = (sockaddr*)&addr6;
            addrLen = sizeof(addr6);
        }
            break;
        default:
            break;
        }

        SOCKET acceptedHandle = socket_accept(hSocket, addrPtr, &addrLen);
        if (!socket_isValid(acceptedHandle)) {
            return SR_ERROR;
        }

        if (endpoint != nullptr) {
            *endpoint = IPEndpoint(addrPtr);
        }

        acceptedSocket = Socket{ IP_V6, acceptedHandle };

        return SR_SUCCESS;
    }

    SocketResult Socket::connect(IPEndpoint const &endpoint) const noexcept {
       // assert(ipVersion == endpoint.getIPVersion());

        sockaddr_in addr4;
        sockaddr_in6 addr6;
        sockaddr *addrPtr = nullptr;
        socklen_t addrLen = 0;

        switch (ipVersion) {
        case IP_V4: {
            addr4 = endpoint.getSockaddrIPv4();
            addrPtr = (sockaddr*)&addr4;
            addrLen = sizeof(addr4);
        }
            break;
        case IP_V6: {
            addr6 = endpoint.getSockaddrIPv6();
            addrPtr = (sockaddr*)&addr6;
            addrLen = sizeof(addr6);
        }
            break;
        default:
            break;
        }

        if (!socket_connect(hSocket, addrPtr, addrLen)) {
            return SR_CONNECT;
        }

        return SR_SUCCESS;
    }

    SocketResult Socket::connect(std::string_view ip, uint16_t port) const noexcept {
        return connect(IPEndpoint(ip, port));
    }

    SocketResult Socket::send(char const *buffer, int buffLen, int &sendLen, int flags) const noexcept {

        sendLen = socket_send(hSocket, buffer, buffLen, flags);

        switch (sendLen) {
        //case 0: // probably should not happen but in any case
        //    return SR_ERROR;
        case SOCKET_ERROR:
            return SR_ERROR;
        }
        return SR_SUCCESS;
    }
    SocketResult Socket::recv(char *buffer, int buffLen, int &recvLen, int flags) const noexcept {

        recvLen = socket_recv(hSocket, buffer, buffLen, flags);

        switch (recvLen) {
        case 0: // If connection was gracefully closed
            return SR_CLOSED;
        case SOCKET_ERROR:
            return SR_ERROR;
        }
        return SR_SUCCESS;
    }

    SocketResult Socket::sendAll(char const *buffer, int buffLen, int flags) const noexcept {

        int totalSendLen = 0;
        while (totalSendLen < buffLen) {
            int remainingLen = buffLen - totalSendLen;
            auto *bufferOffset = buffer + totalSendLen;

            int sendLen;
            if (send(bufferOffset, remainingLen, sendLen, flags) != SR_SUCCESS) {
                std::cerr << "sent bytes: " << totalSendLen << '\n';
                return SR_ERROR;
            }
            totalSendLen += sendLen;
        }

        return SR_SUCCESS;
    }
    SocketResult Socket::recvAll(char       *buffer, int buffLen, int flags) const noexcept {

        int totalRecvLen = 0;
        while (totalRecvLen < buffLen) {
            int remainingLen = buffLen - totalRecvLen;
            auto *bufferOffset = buffer + totalRecvLen;

            int recvLen;
            if (recv(bufferOffset, remainingLen, recvLen, flags) != SR_SUCCESS) {
                std::cerr << "recv bytes: " << totalRecvLen << '\n';
                return SR_ERROR;
            }
            totalRecvLen += recvLen;
        }

        return SR_SUCCESS;
    }

    SocketResult Socket::send(Packet const &packet, int flags) const noexcept {

        if (packet.buffer.size() > Packet::MaxSize) {
            return SR_ERROR;
        }
        uint16_t packetSize = htons(packet.buffer.size());
        if (sendAll((char const*)&packetSize, sizeof(packetSize), flags) != SR_SUCCESS) {
            return SR_ERROR;
        }
        if (sendAll((char const*)packet.buffer.data(), packet.buffer.size(), flags) != SR_SUCCESS) {
            return SR_ERROR;
        }

        return SR_SUCCESS;
    }
    SocketResult Socket::recv(Packet &packet, int flags) const noexcept {
        packet.clear();

        uint16_t packetSize;
        if (recvAll((char*)&packetSize, sizeof(packetSize), flags) != SR_SUCCESS) {
            return SR_ERROR;
        }
        packetSize = ntohs(packetSize);
        if (packetSize > Packet::MaxSize) {
            return SR_ERROR;
        }
        packet.buffer.resize(packetSize);
        if (recvAll((char*)&packet.buffer[0], packetSize, flags) != SR_SUCCESS) {
            return SR_ERROR;
        }

        return SR_SUCCESS;
    }

    bool Socket::setBlocking(bool isBlocking) noexcept {
        return socket_setBlocking(hSocket, isBlocking);
    }

    // Example:
    // setOption(IPPROTO_TCP, TCP_NODELAY, TRUE);
    // setOption(IPPROTO_IPV6, IPV6_V6ONLY, FALSE);
    bool Socket::setOption(int protocol, int optionName, int optionValue) noexcept {
        return socket_setOption(hSocket, protocol, optionName, optionValue);
    }

    // SD_RECEIVE SD_SEND SD_BOTH
    bool Socket::shutdown(int how) noexcept {
        return socket_shutdown(hSocket, how);
    }

    std::string Socket::getPeerName() const noexcept {

        sockaddr_in addr4;
        sockaddr_in6 addr6;
        sockaddr *addrPtr = nullptr;
        socklen_t addrLen = 0;

        switch (ipVersion) {
        case IP_V4: {
            std::memset(&addr4, 0, sizeof(addr4));
            addrPtr = (sockaddr *)&addr4;
            addrLen = sizeof(addr4);
        }
            break;
        case IP_V6: {
            std::memset(&addr6, 0, sizeof(addr6));
            addrPtr = (sockaddr *)&addr6;
            addrLen = sizeof(addr6);
        }
            break;
        }

        std::string name;

        if (socket_getPeerName(hSocket, addrPtr, &addrLen)) {

            int port;
            std::string ip;

            switch (ipVersion) {
            case IP_V4: {
                port = ntohs(addr4.sin_port);
                ip = ::inet_ntoa(addr4.sin_addr);
            }
                break;
            case IP_V6: {
                //port = ntohs(addr6.sin6_port);
                //ip = ::inet_ntoa(addr6.sin6_addr);
            }
                break;
            }
            name = ip + ":" + std::to_string(port);
        }

        return name;
    }

    std::string Socket::toString() const noexcept {
        std::ostringstream oss;

        oss << "IP Version: ";
        switch (ipVersion) {
        case IP_V4:
            oss << "IPv4" << '\n';
            break;
        case IP_V6:
            oss << "IPv6" << '\n';
            break;
        default:
            oss << "Unknown" << '\n';
            break;
        }

        oss << "NAME: " << getPeerName() << '\n'
            << "SOCKET: " << hSocket << '\n';

        return oss.str();
    }

}
