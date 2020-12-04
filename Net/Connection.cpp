#include "Connection.h"

#include <cstring>
#include <sstream>

namespace Net {

    Connection::Connection(Socket const &socket, IPEndpoint const &endpoint) :
        socket{ socket },
        endpoint{ endpoint } {

        std::memset(buffer, 0, sizeof(buffer));
    }

    Connection::Connection() :
        Connection{ Socket{}, IPEndpoint{} } {
    }

    void Connection::close() {
        socket.close();
    }

    bool Connection::send() {

        while (!pm_outgoing.empty()) {

            // Sending packet size
            if (pm_outgoing.currentTask == PacketManager::Task::ProcessPacketSize) {
                pm_outgoing.currentPacketSize = pm_outgoing.front()->buffer.size();
                uint16_t bigEndianPacketSize = htons(pm_outgoing.currentPacketSize);

                int sendLen;
                socket.send((const char *)(&bigEndianPacketSize + pm_outgoing.currentPacketExtractionOffset), sizeof(uint16_t) - pm_outgoing.currentPacketExtractionOffset, sendLen);
                if (sendLen > 0) {
                    pm_outgoing.currentPacketExtractionOffset += sendLen;
                }

                // If full packet size was sent
                if (pm_outgoing.currentPacketExtractionOffset == sizeof(uint16_t)) {
                    pm_outgoing.currentPacketExtractionOffset = 0;
                    pm_outgoing.currentTask = PacketManager::Task::ProcessPacketContents;
                } else {
                    // If full packet size was not sent, break out of the loop for sending outgoing packets for this connection - we'll have to try again next time we are able to write normal data without blocking
                    break;
                }
            } else {
                // Sending packet contents
                unsigned char *bufferPtr = &pm_outgoing.front()->buffer[0];

                int sendLen;
                socket.send((const char*)(bufferPtr + pm_outgoing.currentPacketExtractionOffset), pm_outgoing.currentPacketSize - pm_outgoing.currentPacketExtractionOffset, sendLen);
                if (sendLen > 0) {
                    pm_outgoing.currentPacketExtractionOffset += sendLen;
                }

                // If full packet contents was sent
                if (pm_outgoing.currentPacketExtractionOffset == pm_outgoing.currentPacketSize) {
                    pm_outgoing.pop(); // Remove packet from queue after finished processing

                    pm_outgoing.currentPacketExtractionOffset = 0;
                    pm_outgoing.currentTask = PacketManager::Task::ProcessPacketSize;
                } else {
                    // If full packet contents was not sent
                    break;
                }
            }


            /*
            uint16_t bigEndianPacketSize = htons(pm_outgoing.currentPacketSize);
            socket.sendAll((const char*)(&bigEndianPacketSize), sizeof(bigEndianPacketSize));
            unsigned char *bufferPtr = &pm_outgoing.front()->buffer[0];
            socket.sendAll((const char *)(bufferPtr), pm_outgoing.currentPacketSize);
            pm_outgoing.pop();
            */
        }
        return true;
    }

    bool Connection::recv() {

        int recvLen;
        switch (pm_incoming.currentTask) {
        case PacketManager::Task::ProcessPacketSize:
        {
            // Process Packet Size
            socket.recv((char *)(&pm_incoming.currentPacketSize + pm_incoming.currentPacketExtractionOffset), sizeof(uint16_t) - pm_incoming.currentPacketExtractionOffset, recvLen);
        }
        break;
        case PacketManager::Task::ProcessPacketContents:
        {
            // Process Packet Contents
            socket.recv((char *)(&buffer + pm_incoming.currentPacketExtractionOffset), pm_incoming.currentPacketSize - pm_incoming.currentPacketExtractionOffset, recvLen);
        }
        break;
        default:
            recvLen = 0;
            break;
        }

        // If connection was lost
        if (recvLen == 0) {
            //closeConnection("Recv==0");
            return false;
        }
        // If error occurred on socket
        if (recvLen == SOCKET_ERROR) {
            int error = socket_getError();
            std::cerr << error << '\n';
            /*
            if (error != WSAEWOULDBLOCK) {
                //closeConnection("Recv==SOCKET_ERROR");
                return false;
            }
             */
        }

        if (recvLen > 0) {

            pm_incoming.currentPacketExtractionOffset += recvLen;

            switch (pm_incoming.currentTask) {
            case PacketManager::Task::ProcessPacketSize:
            {
                // Processing packet size
                if (pm_incoming.currentPacketExtractionOffset == sizeof(uint16_t)) {
                    pm_incoming.currentPacketSize = ntohs(pm_incoming.currentPacketSize);
                    if (pm_incoming.currentPacketSize > Packet::MaxSize) {
                        //closeConnection("Packet size too large.");
                        return false;
                    }
                    pm_incoming.currentPacketExtractionOffset = 0;
                    pm_incoming.currentTask = PacketManager::Task::ProcessPacketContents;
                }
            }
            break;
            case PacketManager::Task::ProcessPacketContents:
            {
                // Processing packet contents
                if (pm_incoming.currentPacketExtractionOffset == pm_incoming.currentPacketSize) {
                    std::shared_ptr<Packet> packet = std::make_shared<Packet>();
                    packet->buffer.resize(pm_incoming.currentPacketSize);
                    std::memcpy(&packet->buffer[0], buffer, pm_incoming.currentPacketSize);
                    std::memset(buffer, 0, sizeof(buffer));

                    pm_incoming.push(packet);

                    pm_incoming.currentPacketSize = 0;
                    pm_incoming.currentPacketExtractionOffset = 0;
                    pm_incoming.currentTask = PacketManager::Task::ProcessPacketSize;
                }
            }
            break;
            default:
                break;
            }
        }
        return true;
    }

    std::string Connection::toString() const noexcept {
        std::ostringstream oss;
        oss << "[" << endpoint.getIPString()
            << ":" << std::to_string(endpoint.getPort())
            << "]";
        return oss.str();
    }
}
