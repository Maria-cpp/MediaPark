#include "Statement.h"

#include "DatabaseException.h"

NullType ignore;


Statement::Statement(Database &db, char const *stmt) :
    db_{ db },
    stmt_{ nullptr },
    tail_{ nullptr } {

    if (stmt != nullptr) {
        auto rc = prepare(stmt);
        if (rc != SQLITE_OK) {
            throw DatabaseException(db_);
        }
    }
}

Statement::~Statement() {
    // finish() can return error. If you want to check the error, call
    // finish() explicitly before this object is destructed.
    finish();
}

int Statement::prepare(char const *stmt) {
    auto rc = finish();
    if (rc != SQLITE_OK)
        return rc;

    return prepare_impl(stmt);
}

int Statement::prepare_impl(char const *stmt) {
    return sqlite3_prepare_v2(db_.db(), stmt, std::strlen(stmt), &stmt_, &tail_);
}

int Statement::finish() {
    auto rc = SQLITE_OK;
    if (stmt_ != nullptr) {
        rc = finish_impl(stmt_);
        stmt_ = nullptr;
    }
    tail_ = nullptr;

    return rc;
}

int Statement::finish_impl(sqlite3_stmt *stmt) {
    return sqlite3_finalize(stmt);
}

int Statement::step() {
    return sqlite3_step(stmt_);
}

int Statement::reset() {
    return sqlite3_reset(stmt_);
}

int Statement::bind(int idx, int value) {
    return sqlite3_bind_int(stmt_, idx, value);
}

int Statement::bind(int idx, double value) {
    return sqlite3_bind_double(stmt_, idx, value);
}

int Statement::bind(int idx, long long int value) {
    return sqlite3_bind_int64(stmt_, idx, value);
}

int Statement::bind(int idx, char const *value, CopySemantic copy) {
    return sqlite3_bind_text(stmt_, idx, value, std::strlen(value), copy == Copy ? SQLITE_TRANSIENT : SQLITE_STATIC);
}

int Statement::bind(int idx, void const *value, int n, CopySemantic copy) {
    return sqlite3_bind_blob(stmt_, idx, value, n, copy == Copy ? SQLITE_TRANSIENT : SQLITE_STATIC);
}

int Statement::bind(int idx, std::string const &value, CopySemantic copy) {
    return sqlite3_bind_text(stmt_, idx, value.c_str(), value.size(), copy == Copy ? SQLITE_TRANSIENT : SQLITE_STATIC);
}

int Statement::bind(int idx) {
    return sqlite3_bind_null(stmt_, idx);
}

int Statement::bind(int idx, NullType) {
    return bind(idx);
}

int Statement::bind(char const *name, int value) {
    auto idx = sqlite3_bind_parameter_index(stmt_, name);
    return bind(idx, value);
}

int Statement::bind(char const *name, double value) {
    auto idx = sqlite3_bind_parameter_index(stmt_, name);
    return bind(idx, value);
}

int Statement::bind(char const *name, long long int value) {
    auto idx = sqlite3_bind_parameter_index(stmt_, name);
    return bind(idx, value);
}

int Statement::bind(char const *name, char const *value, CopySemantic copy) {
    auto idx = sqlite3_bind_parameter_index(stmt_, name);
    return bind(idx, value, copy);
}

int Statement::bind(char const *name, void const *value, int n, CopySemantic copy) {
    auto idx = sqlite3_bind_parameter_index(stmt_, name);
    return bind(idx, value, n, copy);
}

int Statement::bind(char const *name, std::string const &value, CopySemantic copy) {
    auto idx = sqlite3_bind_parameter_index(stmt_, name);
    return bind(idx, value, copy);
}

int Statement::bind(char const *name) {
    auto idx = sqlite3_bind_parameter_index(stmt_, name);
    return bind(idx);
}

int Statement::bind(char const *name, NullType) {
    return bind(name);
}


