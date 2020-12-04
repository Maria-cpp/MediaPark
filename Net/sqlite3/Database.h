#pragma once

#include <string_view>

#include "sqlite3.h"
#include "../NonCopyable.h"


class Database :
    NonCopyable {

public:
    Database(std::string_view dbpath);
private:
    Database(sqlite3 *pdb);
public:
    ~Database();

    sqlite3* db() const;

    int connect(std::string_view dbpath);
    int disconnect();

    int attach(char const *dbname, char const *name);
    int detach(char const *name);

    int changes() const;

    int errorCode() const;
    int extendedErrorCode() const;
    char const* errorMsg() const;

    int setBusyTimeout(int ms);

    int execute(std::string_view sql);
    int executef(char const *sql, ...);

private:

    sqlite3 *db_;
};


