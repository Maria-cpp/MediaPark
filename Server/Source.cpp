//Server Code [Tutorial 10] [Nonblocking] [Winsock]
//Author: Jacob Preston 2019-06-28

#include "MyServer.h"
#include <iostream>

#include <sys/ioctl.h>
#include <termios.h>

bool kbhit()
{
    termios term;
    tcgetattr(0, &term);

    termios term2 = term;
    term2.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &term2);

    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);

    tcsetattr(0, TCSANOW, &term);

    return byteswaiting > 0;
}

int main() {

    Network network;

    if (Network::Initialized) {
        MyServer server;
        if (server.initialize(IPEndpoint("127.0.0.1", 6112)))
        {
            int loop=0;
            std::string input;

            while (!loop)
            {
                server.frame();


                if (kbhit()) {
                    std::getline(std::cin, input, '\n');
                    if (input == "exit") {
                        printf("YOU Have entered exit...NOW QUITTING");
                        loop = 1;
                    }
                    std::shared_ptr<Packet> inputPacket = std::make_shared<Packet>(PacketType::PT_ChatMessage);
                    *inputPacket << input;
                    server.getConnection(0).pm_outgoing.push(inputPacket);
                }

            }

        }
    }

    system("pause");
    return 0;
}
