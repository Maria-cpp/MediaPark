#include "MyClient.h"
#include <iostream>

bool MyClient::processPacket(std::shared_ptr<Packet> const &packet) {
    Client::processPacket(packet);

    switch (packet->getPacketType()) {
    case PacketType::PT_ChatMessage: {
        std::string chatmessage;
        *packet >> chatmessage;
        std::cout << "Chat Message: " << chatmessage << std::endl;
        break;
    }
    case PacketType::PT_IntegerArray: {
        uint32_t arraySize = 0;
        *packet >> arraySize;
        std::cout << "Array Size: " << arraySize << std::endl;
        for (uint32_t i = 0; i < arraySize; i++) {
            uint32_t element = 0;
            *packet >> element;
            std::cout << "Element[" << i << "] - " << element << std::endl;
        }
        break;
    }
    default:
        std::cout << "Unrecognized packet type: " << packet->getPacketType() << std::endl;
        return false;
    }

    return true;
}

void MyClient::onConnect() {
    Client::onConnect();

    std::shared_ptr<Packet> helloPacket1 = std::make_shared<Packet>(PacketType::PT_ChatMessage);
    *helloPacket1 << std::string("Hello 1!");
    connection.pm_outgoing.push(helloPacket1);
    std::shared_ptr<Packet> helloPacket2 = std::make_shared<Packet>(PacketType::PT_ChatMessage);
    *helloPacket2 << std::string("Hello 2!");
    connection.pm_outgoing.push(helloPacket2);
}

void MyClient::onConnectFail() {
    Client::onConnectFail();

}

void MyClient::onDisconnect(std::string const &reason) {
    Client::onDisconnect(reason);

}

