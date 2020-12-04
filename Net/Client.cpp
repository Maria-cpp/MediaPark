#include "Client.h"
#include <iostream>

namespace Net {

    bool Client::connect(IPEndpoint const &endpoint) {

        _isConnected = false;
        Socket socket = Socket{ endpoint.getIPVersion() };
        socket.create();
        if (socket.isValid()) {

            if (!socket.setBlocking(true)) {
                std::cerr << "Socket failed to set blocking (true).\n";
                socket.close();
                return false;
            }

            std::cout << "Socket successfully created." << std::endl;

            if (socket.connect(endpoint) != SR_SUCCESS) {
                std::cerr << "Socket failed to connect.\n";
            } else {

                if (!socket.setBlocking(false)) {
                    std::cerr << "Socket failed to set blocking (false).\n";
                    socket.close();
                    return false;
                }

                connection = Connection(socket, endpoint);
                master_fd.fd = connection.socket.getHandle();
                master_fd.events = POLLIN;
                master_fd.revents = 0;
                _isConnected = true;
                onConnect();
                return true;
            }
            
        } else {
            std::cerr << "Socket failed to create.\n";
        }
        onConnectFail();
        return false;
    }

    bool Client::connect(std::string_view ip, uint16_t port) {
        return connect(IPEndpoint(ip, port));
    }

    bool Client::isConnected() {
        return _isConnected;
    }

    bool Client::frame() {
        if (!connection.pm_outgoing.empty()) {
            master_fd.events = POLLIN | POLLOUT;
        }

        pollfd use_fd = master_fd;

        int timeout = 1;
        if (poll(&use_fd, 1, timeout) > 0) {
            // If error occurred on this socket
            if (use_fd.revents & POLLERR) {
                closeConnection("POLLERR");
                return false;
            }
            // If poll hangup occurred on this socket
            if (use_fd.revents & POLLHUP) {
                closeConnection("POLLHUP");
                return false;
            }
            // If invalid socket
            if (use_fd.revents & POLLNVAL) {
                closeConnection("POLLNVAL");
                return false;
            }

            // If normal data can be read without blocking
            if (use_fd.revents & POLLIN) {

                if (!connection.recv()) {
                    closeConnection("");
                    return false;
                }
            }
            // If normal data can be written without blocking
            if (use_fd.revents & POLLOUT) {
                
                if (!connection.send()) {

                }

                if (connection.pm_outgoing.empty()) {
                    master_fd.events = POLLIN;
                }
            }
        }

        while (!connection.pm_incoming.empty()) {
            std::shared_ptr<Packet> frontPacket = connection.pm_incoming.front();
            if (!processPacket(frontPacket)) {
                closeConnection("Failed to process incoming packet.");
                return false;
            }
            connection.pm_incoming.pop();
        }

        return true;
    }

    bool Client::processPacket(std::shared_ptr<Packet> const &packet) {
        std::cout << "Packet received with size: " << packet->buffer.size() << std::endl;
        return true;
    }

    void Client::onConnect() {
        std::cout << "Successfully connected to the server!" << std::endl;
    }

    void Client::onConnectFail() {
        std::cout << "Failed to connect." << std::endl;
    }

    void Client::onDisconnect(std::string const &reason) {
        std::cout << "Lost connection. Reason: " << reason << "." << std::endl;
    }

    void Client::closeConnection(std::string const &reason) {
        onDisconnect(reason);
        master_fd.fd = 0;
        _isConnected = false;
        connection.close();
    }
}
