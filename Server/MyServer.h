#pragma once

#include "../Net/IncludeMe.h"

using namespace std;

class MyServer :
    public Server {
private:

    bool processPacket(std::shared_ptr<Packet> const &packet) override;
    void onConnect(Connection &newConnection) override;
    void onDisconnect(Connection &lostConnection, std::string const &reason) override;
    
};
