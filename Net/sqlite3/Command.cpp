#include "Command.h"

Command::BindStream::BindStream(Command &cmd, int idx) :
    cmd_{ cmd },
    idx_{ idx } {
}


Command::Command(Database &db, char const *stmt) :
    Statement(db, stmt) {
}

Command::BindStream Command::binder(int idx) {
    return BindStream(*this, idx);
}

int Command::execute() {
    auto rc = step();
    if (rc == SQLITE_DONE) rc = SQLITE_OK;

    return rc;
}

int Command::executeAll() {
    auto rc = execute();
    if (rc != SQLITE_OK) return rc;

    char const *sql = tail_;

    while (std::strlen(sql) > 0) { // sqlite3_complete() is broken.
        sqlite3_stmt *old_stmt = stmt_;

        if ((rc = prepare_impl(sql)) != SQLITE_OK) return rc;

        if ((rc = sqlite3_transfer_bindings(old_stmt, stmt_)) != SQLITE_OK) return rc;

        finish_impl(old_stmt);

        if ((rc = execute()) != SQLITE_OK) return rc;

        sql = tail_;
    }

    return rc;
}


