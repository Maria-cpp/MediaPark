#pragma once

#include <string_view>

#include "sqlite3.h"
#include "../NonCopyable.h"
#include "Database.h"

class NullType {};

enum CopySemantic { Copy, NoCopy };

class Statement :
    NonCopyable {

public:
    int prepare(char const *stmt);
    int finish();

    int bind(int idx, int value);
    int bind(int idx, double value);
    int bind(int idx, long long int value);
    int bind(int idx, char const *value, CopySemantic copy);
    int bind(int idx, void const *value, int n, CopySemantic copy);
    int bind(int idx, std::string const &value, CopySemantic copy);
    int bind(int idx);
    int bind(int idx, NullType);

    int bind(char const *name, int value);
    int bind(char const *name, double value);
    int bind(char const *name, long long int value);
    int bind(char const *name, char const *value, CopySemantic copy);
    int bind(char const *name, void const *value, int n, CopySemantic copy);
    int bind(char const *name, std::string const &value, CopySemantic copy);
    int bind(char const *name);
    int bind(char const *name, NullType);

    int step();
    int reset();

protected:
    explicit Statement(Database &db, char const *stmt = nullptr);
    ~Statement();

    int prepare_impl(char const *stmt);
    int finish_impl(sqlite3_stmt *stmt);

protected:

    Database &db_;
    sqlite3_stmt *stmt_;
    char const *tail_;
};

extern NullType ignore;
