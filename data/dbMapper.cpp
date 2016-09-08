#include "dbMapper.h"

#include <booster/log.h>

namespace data {

    DbMapper::DbMapper(const std::string& connectionString)
        :connectionString_(connectionString)
    {
        sql_.reset(new cppdb::session());
    }

    DbMapper::~DbMapper() {
        clear();
    }

    void DbMapper::clear() {
        sql_->close();
    }

    cppdb::session& DbMapper::sql() {
        sql_->close();
        if(! sql_->is_open()) {
            sql_->open(connectionString_);
        }
        return *sql_;
    }

}   // namespace data
