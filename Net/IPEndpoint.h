#pragma once
#include "IPVersion.h"

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include "SocketHandle.h"

namespace Net {

    class IPEndpoint {

    public:

        IPEndpoint();
        IPEndpoint(std::string_view ip, uint16_t port);
        IPEndpoint(sockaddr *addr);

        IPVersion getIPVersion() const noexcept;
        std::string getHostname() const noexcept;
        std::string getIPString() const noexcept;
        std::vector<uint8_t> getIPBytes() const noexcept;
        uint16_t getPort() const noexcept;

        sockaddr_in getSockaddrIPv4() const noexcept;
        sockaddr_in6 getSockaddrIPv6() const noexcept;

        std::string toString() const noexcept;

    private:

        IPVersion ipVersion;
        std::string hostname;
        std::string ipString;
        std::vector<uint8_t> ipBytes;
        uint16_t port;
    };
}