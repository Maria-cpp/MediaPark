#include "IPEndpoint.h"

#include <cstring>
#include <cassert>
#include <iostream>
#include <sstream>
#include "Helpers.h"

namespace Net {


    IPEndpoint::IPEndpoint() :
        ipVersion{ IP_UNKNOWN },
        hostname{ "" },
        ipString{ "" },
        port{ port } {
    }

    IPEndpoint::IPEndpoint(std::string_view ip, uint16_t port) :
        port{ port } {

        hostname = ip;
        Helpers::trim(hostname);

        int ret;
        //IPv4
        {
            in_addr in4Addr; //location to store the ipv4 address
            ret = inet_pton(AF_INET, hostname.c_str(), &in4Addr);


            if (ret == 1) {
                ULONG addr = in4Addr.s_addr;
                if (addr != INADDR_NONE) {
                    ipVersion = IP_V4;
                    ipString = hostname;

                    ipBytes.resize(sizeof(addr));
                    std::memcpy(&ipBytes[0], &addr, sizeof(addr));
                    return;
                }
            }

            //Attempt to resolve hostname to ipv4 address
            addrinfo hintInfo; //hints will filter the results we get back for getaddrinfo
            hintInfo.ai_family = AF_INET; //ipv4 addresses only
            addrinfo *hostInfo = nullptr;
            ret = getaddrinfo(hostname.c_str(), NULL, &hintInfo, &hostInfo);
            if (ret == 0) {
                ipVersion = IP_V4;

                sockaddr_in *hostAddrIn4 = reinterpret_cast<sockaddr_in*>(hostInfo->ai_addr);
                ipString.resize(16);
                inet_ntop(AF_INET, &hostAddrIn4->sin_addr, &ipString[0], 16);

                ULONG ip_long = hostAddrIn4->sin_addr.s_addr; // get ip address as unsigned long
                ipBytes.resize(sizeof(ip_long));
                std::memcpy(&ipBytes[0], &ip_long, sizeof(ip_long)); // copy bytes into our array of bytes representing ip address
                freeaddrinfo(hostInfo); // memory cleanup from getaddrinfo call

                return;
            }
        }

        //IPv6
        {
            in6_addr in6Addr; //location to store the ipv6 address
            ret = inet_pton(AF_INET6, hostname.c_str(), &in6Addr);

            if (ret == 1) {
                ipVersion = IP_V6;
                ipString = hostname;

                ipBytes.resize(16);
                std::memcpy(&ipBytes[0], &in6Addr, 16);
                return;
            }

            //Attempt to resolve hostname to ipv6 address
            addrinfo hintInfo; //hints will filter the results we get back for getaddrinfo
            hintInfo.ai_family = AF_INET6; //ipv6 addresses only
            addrinfo *hostInfo = nullptr;
            ret = getaddrinfo(hostname.c_str(), NULL, &hintInfo, &hostInfo);
            if (ret == 0) {
                ipVersion = IP_V6;

                sockaddr_in6 *hostAddrIn6 = reinterpret_cast<sockaddr_in6*>(hostInfo->ai_addr);
                ipString.resize(46);
                inet_ntop(AF_INET6, &hostAddrIn6->sin6_addr, &ipString[0], 46);

                ipBytes.resize(16);
                std::memcpy(&ipBytes[0], &hostAddrIn6->sin6_addr, 16); //copy bytes into our array of bytes representing ip address
                freeaddrinfo(hostInfo); // [Fixed 2019-04-30] - had wrong variable name
                return;
            }
        }

    }

    IPEndpoint::IPEndpoint(sockaddr *addr) {
        assert(addr->sa_family == AF_INET || addr->sa_family == AF_INET6);

        switch (addr->sa_family) {
        case AF_INET: {
            sockaddr_in *addrv4 = reinterpret_cast<sockaddr_in*>(addr);
            ipVersion = IP_V4;

            port = ntohs(addrv4->sin_port);

            ipBytes.resize(sizeof(ULONG));
            std::memcpy(&ipBytes[0], &addrv4->sin_addr, sizeof(ULONG));
            ipString.resize(16);
            inet_ntop(AF_INET, &addrv4->sin_addr, &ipString[0], 16);
        }
            break;
        case AF_INET6: {
            sockaddr_in6 *addrv6 = reinterpret_cast<sockaddr_in6*>(addr);
            ipVersion = IP_V6;

            port = ntohs(addrv6->sin6_port);

            ipBytes.resize(16);
            std::memcpy(&ipBytes[0], &addrv6->sin6_addr, 16);
            ipString.resize(46);
            inet_ntop(AF_INET6, &addrv6->sin6_addr, &ipString[0], 46);
        }
            break;
        default:
            break;
        }

        Helpers::trim(ipString);
        hostname = ipString;
    }


    IPVersion IPEndpoint::getIPVersion() const noexcept {
        return ipVersion;
    }
    std::vector<uint8_t> IPEndpoint::getIPBytes() const noexcept {
        return ipBytes;
    }
    std::string IPEndpoint::getHostname() const noexcept {
        return hostname;
    }
    std::string IPEndpoint::getIPString() const noexcept {
        return ipString;
    }
    uint16_t IPEndpoint::getPort() const noexcept {
        return port;
    }

    sockaddr_in IPEndpoint::getSockaddrIPv4() const noexcept {
     //   assert(ipVersion == IP_V4);

        sockaddr_in addr4;
        std::memset(&addr4, 0, sizeof(addr4));
        addr4.sin_family = AF_INET;
        std::memcpy(&addr4.sin_addr, &ipBytes[0], sizeof(ULONG));
        addr4.sin_port = htons(port);
        return addr4;
    }

    sockaddr_in6 IPEndpoint::getSockaddrIPv6() const noexcept {
        assert(ipVersion == IP_V6);

        sockaddr_in6 addr6;
        std::memset(&addr6, 0, sizeof(addr6));
        addr6.sin6_family = AF_INET6;
        std::memcpy(&addr6.sin6_addr, &ipBytes[0], 16);
        addr6.sin6_port = htons(port);
        return addr6;
    }

    std::string IPEndpoint::toString() const noexcept {
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

        oss << "Hostname: " << hostname << '\n';
        oss << "IP: " << ipString << '\n';
        oss << "IP bytes..." << '\n';
        for (auto &bytes : ipBytes) {
            oss << (int)bytes << '\n';
        }
        oss << "Port: " << port << '\n';

        return oss.str();
    }
}