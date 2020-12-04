#pragma once

#include <exception>
#include <string>

namespace Net {

    class PacketException :
        virtual public std::exception {

    public:

        PacketException(std::string const &message) noexcept :
            message(message) {}
        virtual ~PacketException() throw() {}

        const char *what() const noexcept override {
            return message.c_str();
        }

        std::string toString() const noexcept {
            return message;
        }

    private:

        std::string message;
    };

}