#pragma once

#include <queue> //for std::queue
#include <memory> //for std::shared_ptr
#include "Packet.h"

namespace Net {

    class PacketManager {
    public:
        enum Task {
            ProcessPacketSize,
            ProcessPacketContents
        };

        bool empty() const noexcept;
        size_t count() const noexcept;
        std::shared_ptr<Packet> front() const noexcept;

        void push(std::shared_ptr<Packet> p);
        void pop() noexcept;
        void clear() noexcept;


        uint16_t currentPacketSize = 0;
        int currentPacketExtractionOffset = 0;
        Task currentTask = Task::ProcessPacketSize;
 
    private:
        std::queue<std::shared_ptr<Packet>> packets;
    };
}
