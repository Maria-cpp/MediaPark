#pragma once

#include <stdexcept>
#include <string>
#include "sqlite3.h"
#include "Database.h"

class DatabaseException :
    virtual public std::exception {

public:

    explicit DatabaseException(const std::string &message) noexcept :
        message(message) {

    }
    explicit DatabaseException(Database &db) :
        message(db.errorMsg()) {

    }

    virtual ~DatabaseException() throw() {}


    const char *what() const noexcept override {
        return message.c_str();
    }

    std::string toString() const noexcept {
        return message;
    }

private:

    std::string message;

};

