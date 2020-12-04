#pragma once
#include "../Net/IncludeMe.h"

class MyClient :
    public Client {

    bool processPacket(std::shared_ptr<Packet> const &packet) override;
    void onConnect() override;
    void onConnectFail() override;
    void onDisconnect(std::string const &reason) override;
};
