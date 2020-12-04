#include "Server.h"
#include <iostream>
#include "Network.h"

namespace Net {
    bool Server::initialize(IPEndpoint const &endpoint)
    {
        master_fd.clear();
        connections.clear();

        listeningSocket = Socket{ endpoint.getIPVersion() };
        listeningSocket.create();
        if (listeningSocket.isValid())
        {
            std::cout << "Socket successfully created." << std::endl;

            if (!listeningSocket.setBlocking(false)) {
                listeningSocket.close();
                return false;
            }

            // Set the no-delay option (disable Nagle's algorithm) on the TCP socket.
            if (!listeningSocket.setOption(IPPROTO_TCP, TCP_NODELAY, TRUE)) {
                listeningSocket.close();
                return false;
            }

            if (listeningSocket.bind(endpoint) != SR_SUCCESS) {

            }

            if (listeningSocket.listen() != SR_SUCCESS) {
                std::cerr << "Failed to listen." << std::endl;
            } else {
                pollfd listeningSocketFD;
                listeningSocketFD.fd = listeningSocket.getHandle();
                listeningSocketFD.events = POLLIN;
                listeningSocketFD.revents = 0;

                master_fd.push_back(listeningSocketFD);

                std::cout << "Socket successfully listening." << std::endl;
                return true;
            }
            //connections.push_back(listeningSocket);
           // listeningSocket.close();
        } else {
            std::cerr << "Socket failed to create." << std::endl;
        }
        return false;
    }

    void Server::frame()
    {
        for (int i = 0; i < connections.size(); ++i) {
            if (!connections[i].pm_outgoing.empty()) {
                master_fd[i + 1].events = POLLIN | POLLOUT;
            }
        }

        use_fd = master_fd;
        //use_fd[0].events = POLLIN;

        int timeout = 1;
        if (poll(use_fd.data(), use_fd.size(), timeout) > 0)
        {
          //if (poll( pf, 1, timeout) > 0) {
            //std::cout << "Connected\n";

#pragma region listener
            pollfd &listeningSocketFD = use_fd[0];
            if (listeningSocketFD.revents & POLLIN) {
                Socket newConnectionSocket;
                IPEndpoint newConnectionEndpoint;
                if (listeningSocket.accept(newConnectionSocket, &newConnectionEndpoint) == SR_SUCCESS) {
                    connections.push_back(Connection(newConnectionSocket, newConnectionEndpoint));
                    Connection &acceptedConnection = connections[connections.size() - 1];

                    pollfd newConnectionFD;
                    newConnectionFD.fd = newConnectionSocket.getHandle();
                    newConnectionFD.events = POLLIN;
                    newConnectionFD.revents = 0;
                    master_fd.push_back(newConnectionFD);
                    onConnect(acceptedConnection);
                } else {
                    std::cerr << "Failed to accept new connection." << std::endl;
                }
            }
#pragma endregion

            for (int i = use_fd.size() - 1; i >= 1; --i) {
                int connectionIndex = i - 1;
                Connection &connection = connections[connectionIndex];
                // If error occurred on this socket
                if (use_fd[i].revents & POLLERR) {
                    closeConnection(connectionIndex, "POLLERR");
                    continue;
                }
                // If poll hangup occurred on this socket
                if (use_fd[i].revents & POLLHUP) {
                    closeConnection(connectionIndex, "POLLHUP");
                    continue;
                }
                // If invalid socket
                if (use_fd[i].revents & POLLNVAL) {
                    closeConnection(connectionIndex, "POLLNVAL");
                    continue;
                }
                // If normal data can be read without blocking
                if (use_fd[i].revents & POLLIN) {

                    if (!connection.recv()) {
                        closeConnection(connectionIndex, "");
                        continue;
                    }
                }
                // If normal data can be written without blocking
                if (use_fd[i].revents & POLLOUT) {

                    if (!connection.send()) {

                    }

                    if (connection.pm_outgoing.empty()) {
                        //master_fd[i].events = POLLRDNORM;
                        master_fd[i].events = POLLIN;
                    }
                }

            }
        }

        for (int i = connections.size() - 1; i >= 0; --i) {
            while (!connections[i].pm_incoming.empty()) {
                std::shared_ptr<Packet> frontPacket = connections[i].pm_incoming.front();
                if (!processPacket(frontPacket)) {
                    closeConnection(i, "Failed to process incoming packet.");
                    break;
                }
                connections[i].pm_incoming.pop();

                /*//for seding message to particular node
                std::string input;
                std::getline(std::cin, input, '\n');

                std::shared_ptr<Packet> inputPacket = std::make_shared<Packet>(PacketType::PT_ChatMessage);
                *inputPacket << input;
                connections[i].pm_outgoing.push(inputPacket);
                */
            }
        }

    }

    Connection& Server::getConnection(int i) {
        return connections[i];
    }

    bool Server::processPacket(std::shared_ptr<Packet> const &packet) {
        std::cout << "Packet received with size: " << packet->buffer.size() << std::endl;
        return true;
    }

    void Server::onConnect(Connection &newConnection) {
        std::cout << newConnection.toString() << " - New connection accepted." << std::endl;
    }

    void Server::onDisconnect(Connection &lostConnection, std::string const &reason) {
        std::cout << "[" << reason << "] Connection lost: " << lostConnection.toString() << "." << std::endl;
    }

    void Server::closeConnection(int connectionIndex, std::string const &reason) {
        Connection &connection = connections[connectionIndex];
        onDisconnect(connection, reason);
        master_fd.erase(master_fd.begin() + (connectionIndex + 1));
        use_fd.erase(use_fd.begin() + (connectionIndex + 1));
        connection.close();
        connections.erase(connections.begin() + connectionIndex);
    }

}
