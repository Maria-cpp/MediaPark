cmake_minimum_required(VERSION 3.17)
project(TestLive)

set(CMAKE_CXX_STANDARD 17)

add_executable(Server

        Net/SocketHandle.h
        Net/Socket.h
        Net/Socket.cpp
        Net/IPEndpoint.h
        Net/IPEndpoint.cpp
        Net/Network.h
        Net/Network.cpp
        Net/Connection.h
        Net/Connection.cpp
        Net/Packet.h
        Net/Packet.cpp
        Net/PacketManager.h
        Net/PacketManager.cpp
        Net/PacketException.h
        Net/IncludeMe.h

        Net/Server.h
        Net/Server.cpp

        Server/MyServer.h
        Server/MyServer.cpp
        Server/Source.cpp


        )

add_executable(client

        Net/SocketHandle.h
        Net/Socket.h
        Net/Socket.cpp
        Net/IPEndpoint.h
        Net/IPEndpoint.cpp
        Net/Network.h
        Net/Network.cpp
        Net/Connection.h
        Net/Connection.cpp
        Net/Packet.h
        Net/Packet.cpp
        Net/PacketManager.h
        Net/PacketManager.cpp
        Net/PacketException.h
        Net/IncludeMe.h

        Net/Client.h
        Net/Client.cpp

        Client/Source.cpp
        Client/MyClient.h
        Client/MyClient.cpp

        )