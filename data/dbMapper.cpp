#include "dbMapper.h"

#include <booster/log.h>

namespace data {

    DbMapper::DbMapper(const std::string& connectionString)
        :connectionString_(connectionString)
    {
        connection_.reset(new cppdb::session());
    }

    DbMapper::~DbMapper() {
        disconnect();
    }

    void DbMapper::disconnect() {
        connection_->close();
    }

    cppdb::session& DbMapper::connection() {
        if (!connection_->is_open()) {
            connection_->open(connectionString_);
        }
        return *connection_;
    }

}   // namespace data
