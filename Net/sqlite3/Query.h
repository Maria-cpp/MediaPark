#pragma once

#include <string_view>

#include "sqlite3.h"
#include "../NonCopyable.h"
#include "Database.h"
#include "DatabaseException.h"
#include "Statement.h"


class Query :
    public Statement {

public:

    class Rows {
    public:

        class GetStream {
        public:
            GetStream(Rows *rws, int idx);

            template <class T>
            GetStream &operator >> (T &value) {
                value = rws_->get(idx_, T());
                ++idx_;
                return *this;
            }

        private:
            Rows *rws_;
            int idx_;
        };

        explicit Rows(sqlite3_stmt *stmt);

        int data_count() const;
        int column_type(int idx) const;

        int column_bytes(int idx) const;

        template <class T> T get(int idx) const {
            return get(idx, T());
        }

        //template <class... Ts>
        //std::tuple<Ts...> get_columns(typename convert<Ts>::to_int... idxs) const {
        //    return std::make_tuple(get(idxs, Ts())...);
        //}

        GetStream getter(int idx = 0);

    private:
        int get(int idx, int) const;
        double get(int idx, double) const;
        long long int get(int idx, long long int) const;
        char const *get(int idx, char const *) const;
        std::string get(int idx, std::string) const;
        void const *get(int idx, void const *) const;
        NullType get(int idx, NullType) const;

    private:
        sqlite3_stmt *stmt_;

    };

    class QueryIterator
        : public std::iterator<std::input_iterator_tag, Rows> {
    public:
        QueryIterator();
        explicit QueryIterator(Query *cmd);

        bool operator==(QueryIterator const &) const;
        bool operator!=(QueryIterator const &) const;

        QueryIterator &operator++();

        value_type operator*() const;

    private:
        Query *cmd_;
        int rc_;
    };

    explicit Query(Database &db, char const *stmt = nullptr);

    int column_count() const;

    char const *column_name(int idx) const;
    char const *column_decltype(int idx) const;

    using iterator = QueryIterator;

    iterator begin();
    iterator end();
};




