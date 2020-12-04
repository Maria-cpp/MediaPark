#include "Database.h"
#include <memory>

#include "DatabaseException.h"

Database::Database(std::string_view dbpath) {
    connect(dbpath.data());
}
Database::Database(sqlite3 *pdb) :
    db_{ pdb } {

}

Database::~Database() {
    disconnect();
}

sqlite3* Database::db() const {
    return db_;
}


int Database::connect(std::string_view dbpath) {

    int rc = sqlite3_open(dbpath.data(), &db_);
    if (rc != SQLITE_OK) {
        throw DatabaseException(std::string("Failed to open database ") + dbpath.data() + ". Error code: " + std::to_string(rc) + "\n");
    }
}

int Database::disconnect() {

    int rc = SQLITE_OK;
    if (db_ != nullptr) {
        rc = sqlite3_close(db_);
        if (rc == SQLITE_OK) {
            db_ = nullptr;
        }
    }
    return rc;
}

int Database::attach(char const *dbname, char const *name) {
    return executef("ATTACH '%q' AS '%q'", dbname, name);
}

int Database::detach(char const *name) {
    return executef("DETACH '%q'", name);
}


int Database::changes() const {
    return sqlite3_changes(db_);
}

int Database::errorCode() const {
    return sqlite3_errcode(db_);
}

int Database::extendedErrorCode() const {
    return sqlite3_extended_errcode(db_);
}

char const* Database::errorMsg() const {
    return sqlite3_errmsg(db_);
}

int Database::setBusyTimeout(int ms) {
    return sqlite3_busy_timeout(db_, ms);
}

// This is the callback function to sqlite3_exec
static int callback(void *param, int argc, char **argv, char **azColName) {

    if (argc == 0) {
        return 0;
    }

    char *res = (char *)param;
    for (int i = 0; i < argc; ++i) {
        printf("%s = %s\n", azColName[i], (argv[i] != nullptr ? argv[i] : "NULL"));
    }

    printf("\n");
    return 0;
}

int Database::execute(std::string_view sql) {

    int rc = sqlite3_exec(db_, sql.data(), nullptr/*callback*/, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        throw DatabaseException(std::string("Failed to execute query:\n") + sql.data() + "\"\nError code: " + std::to_string(rc) + "\n");
    }
    return rc;
}

int Database::executef(char const *sql, ...) {
    va_list ap;
    va_start(ap, sql);
    std::shared_ptr<char> msql(sqlite3_vmprintf(sql, ap), sqlite3_free);
    va_end(ap);

    return execute(msql.get());
}

