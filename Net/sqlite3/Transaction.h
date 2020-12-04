#pragma once

#include <string_view>

#include "sqlite3.h"
#include "../NonCopyable.h"
#include "Database.h"
#include "DatabaseException.h"

class Transaction
    : NonCopyable {

public:
    explicit Transaction(Database &db, bool fcommit = false, bool freserve = false);
    ~Transaction();

    int commit();
    int rollback();

private:
    Database *db_;
    bool fcommit_;
};


