#pragma once
#include <stdint.h>

namespace Net {
    enum PacketType : uint16_t {
        PT_Invalid,
        PT_ChatMessage,
        PT_IntegerArray,
        PT_Test
    };
}
