#include "Packet.h"

namespace Net {
    Packet::Packet(PacketType packetType) {
        clear();
        setPacketType(packetType);
    }

    PacketType Packet::getPacketType() {
        auto *packetTypePtr = reinterpret_cast<PacketType*>(&buffer[0]);
        return static_cast<PacketType>(ntohs(*packetTypePtr));
    }

    void Packet::setPacketType(PacketType packetType) {
        auto *packetTypePtr = reinterpret_cast<PacketType*>(&buffer[0]);
        *packetTypePtr = static_cast<PacketType>(htons(packetType));
    }

    void Packet::clear() {
        buffer.resize(sizeof(PacketType));
        setPacketType(PacketType::PT_Invalid);
        extractionOffset = sizeof(PacketType);
    }

    void Packet::push(char const *data, uint32_t size) {
        if ((buffer.size() + size) > Packet::MaxSize) {
            throw PacketException("[Packet::Append(const void*, uint32_t)] - Packet size exceeded max packet size.");
        }
        buffer.insert(buffer.end(), data, data + size);
    }

    Packet& Packet::operator<<(uint16_t data) {
        data = htons(data);
        push((char const*)&data, sizeof(data));
        return *this;
    }
    Packet& Packet::operator>>(uint16_t &data) {
        if ((extractionOffset + sizeof(data)) > buffer.size()) {
            throw PacketException("[Packet::operator >>(uint16_t &)] - Extraction offset exceeded buffer size.");
        }

        data = *reinterpret_cast<uint16_t*>(&buffer[extractionOffset]);
        data = ntohs(data);
        extractionOffset += sizeof(data);
        return *this;
    }

    Packet& Packet::operator<<(uint32_t data) {
        data = htonl(data);
        push((char const*)&data, sizeof(data));
        return *this;
    }
    Packet& Packet::operator>>(uint32_t &data) {
        if ((extractionOffset + sizeof(data)) > buffer.size()) {
            throw PacketException("[Packet::operator >>(uint32_t &)] - Extraction offset exceeded buffer size.");
        }

        data = *reinterpret_cast<uint32_t*>(&buffer[extractionOffset]);
        data = ntohl(data);
        extractionOffset += sizeof(data);
        return *this;
    }

    Packet& Packet::operator<<(std::string const &data) {
        *this << (uint16_t)data.size();
        push(data.data(), data.size());
        return *this;
    }
    Packet& Packet::operator>>(std::string       &data) {
        data.clear();

        uint16_t dataSize;
        *this >> dataSize;

        if ((extractionOffset + dataSize) > buffer.size()) {
            throw PacketException("[Packet::operator >>(std::string &)] - Extraction offset exceeded buffer size.");
        }
        data.resize(dataSize);
        data.assign((char const*)&buffer[extractionOffset], dataSize);
        extractionOffset += dataSize;
        return *this;
    }
}
