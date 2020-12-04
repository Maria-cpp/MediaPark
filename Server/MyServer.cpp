#include "MyServer.h"
#include <iostream>

bool MyServer::processPacket(std::shared_ptr<Packet> const &packet) {
    Server::processPacket(packet);

    switch (packet->getPacketType()) {
    case PacketType::PT_ChatMessage:
    {
        std::string chatmessage;
        *packet >> chatmessage;
        std::cout << "Chat Message: " << chatmessage << std::endl;
        break;
    }
    case PacketType::PT_IntegerArray:
    {
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

void MyServer::onConnect(Connection &newConnection) {
    Server::onConnect(newConnection);

    std::shared_ptr<Packet> welcomeMessagePacket1 = std::make_shared<Packet>(PacketType::PT_ChatMessage);
    *welcomeMessagePacket1 << std::string("Welcome! 1");
    newConnection.pm_outgoing.push(welcomeMessagePacket1);
    std::shared_ptr<Packet> welcomeMessagePacket2 = std::make_shared<Packet>(PacketType::PT_ChatMessage);
    *welcomeMessagePacket2 << std::string("Welcome! 2");
    newConnection.pm_outgoing.push(welcomeMessagePacket2);

    std::shared_ptr<Packet> userConnectedPacket = std::make_shared<Packet>(PacketType::PT_ChatMessage);
    *userConnectedPacket << std::string("New user connected!");
    for (auto &connection : connections) {
        if (&connection == &newConnection) {
            continue;
        }
        connection.pm_outgoing.push(userConnectedPacket);
    }
}

void MyServer::onDisconnect(Connection &lostConnection, std::string const &reason) {
    Server::onDisconnect(lostConnection, reason);

    std::shared_ptr<Packet> userDisconnectedPacket = std::make_shared<Packet>(PacketType::PT_ChatMessage);
    *userDisconnectedPacket << std::string("A user disconnected!");
    for (auto &connection : connections) {
        if (&connection == &lostConnection) {
            continue;
        }
        connection.pm_outgoing.push(userDisconnectedPacket);
    }
}

