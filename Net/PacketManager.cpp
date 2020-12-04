#include "PacketManager.h"

namespace Net {

    bool PacketManager::empty() const noexcept {
        return packets.empty();
    }

    size_t PacketManager::count() const noexcept {
        return packets.size();
    }

    std::shared_ptr<Packet> PacketManager::front() const noexcept {
        return packets.front(); //Return packet that was removed from the queue
    }


    void PacketManager::push(std::shared_ptr<Packet> p) {
        packets.push(std::move(p)); //Add packet to queue
    }

    void PacketManager::pop() noexcept {
        packets.pop(); //Remove packet from front of queue
    }

    void PacketManager::clear() noexcept {
        while (!empty()) {
            pop();
        }
    }

}