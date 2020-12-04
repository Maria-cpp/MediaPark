#include "Transaction.h"

Transaction::Transaction(Database &db, bool fcommit, bool freserve) :
    db_{ &db },
    fcommit_{ fcommit } {
    int rc = db_->execute(freserve ? "BEGIN IMMEDIATE" : "BEGIN");
    if (rc != SQLITE_OK) {
        throw DatabaseException(*db_);
    }
}

Transaction::~Transaction() {

    if (db_ != nullptr) {
        // execute() can return error. If you want to check the error,
        // call commit() or rollback() explicitly before this object is
        // destructed.
        db_->execute(fcommit_ ? "COMMIT" : "ROLLBACK");
    }
}

int Transaction::commit() {
    auto db = db_;
    db_ = nullptr;
    int rc = db->execute("COMMIT");
    return rc;
}

int Transaction::rollback() {
    auto db = db_;
    db_ = nullptr;
    int rc = db->execute("ROLLBACK");
    return rc;
}

