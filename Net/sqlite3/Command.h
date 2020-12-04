#pragma once

#include <string_view>

#include "sqlite3.h"
#include "../NonCopyable.h"
#include "Database.h"
#include "DatabaseException.h"
#include "Statement.h"

class Command :
    public Statement {

public:

    class BindStream {

    public:
        BindStream(Command &cmd, int idx);

        template <class T>
        BindStream &operator << (T value) {
            auto rc = cmd_.bind(idx_, value);
            if (rc != SQLITE_OK) {
                throw DatabaseException(cmd_.db_);
            }
            ++idx_;
            return *this;
        }
        BindStream &operator << (char const *value) {
            auto rc = cmd_.bind(idx_, value, Copy);
            if (rc != SQLITE_OK) {
                throw DatabaseException(cmd_.db_);
            }
            ++idx_;
            return *this;
        }
        BindStream &operator << (std::string const &value) {
            auto rc = cmd_.bind(idx_, value, Copy);
            if (rc != SQLITE_OK) {
                throw DatabaseException(cmd_.db_);
            }
            ++idx_;
            return *this;
        }

    private:

        Command &cmd_;
        int idx_;
    };

    explicit Command(Database &db, char const *stmt = nullptr);

    BindStream binder(int idx = 1);

    int execute();
    int executeAll();
};

