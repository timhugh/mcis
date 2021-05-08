#include <postgis/db.hpp>

using namespace postgis;

postgis::DB::DB(const std::string connectionString)
{
    this->connectionString = connectionString.c_str();
}

postgis::DB::~DB()
{
    delete this->connectionString;
}

const std::unique_ptr<Result> postgis::DB::transact(
    const const std::function<std::unique_ptr<Result>(const TransactionOperator &)> xact) const
{
    const PGconn *conn = PQconnectdb(connectionString);
    const TransactionOperator op(conn);
    auto result = xact(op);
    return result;
}

postgis::TransactionOperator::TransactionOperator()
    : conn(std::unique_ptr<const PGconn>{conn})
{
}

const std::unique_ptr<Result> postgis::TransactionOperator::executeQuery(const std::string query, const bool binaryResult = true) const
{
}