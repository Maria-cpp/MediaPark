#pragma once

#include <vector>
#include "SocketHandle.h"
#include "PacketException.h"
#include "PacketType.h"

namespace Net {
    class Packet {

    public:

        static constexpr size_t MaxSize = 8192;


        Packet(PacketType packetType = PacketType::PT_Invalid);

        PacketType getPacketType();
        void setPacketType(PacketType packetType);

        void clear();
        void push(char const *data, uint32_t size);

        Packet& operator<<(uint16_t data);
        Packet& operator>>(uint16_t &data);

        Packet& operator<<(uint32_t data);
        Packet& operator>>(uint32_t &data);

        Packet& operator<<(std::string const &data);
        Packet& operator>>(std::string       &data);

        uint32_t extractionOffset = 0;
        std::vector<unsigned char> buffer;
    };
}
