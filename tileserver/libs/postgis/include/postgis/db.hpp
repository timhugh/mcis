#pragma once

#include <postgresql/libpq-fe.h>
#include <memory>
#include <functional>

namespace postgis
{
    struct Result
    {
        const std::string value;
        const int length;
    };

    class TransactionOperator
    {
    private:
        const PGconn *conn;

    public:
        TransactionOperator(const PGconn *conn);
        ~TransactionOperator();
        const std::unique_ptr<Result> executeQuery(
            const std::string query, const bool binaryResult) const;
    };

    class DB
    {
    private:
        const char *connectionString;

    public:
        DB(const std::string connectionString);
        ~DB();
        const std::unique_ptr<Result> transact(
            const std::function<std::unique_ptr<Result>(const TransactionOperator &)>) const;
    };

}
