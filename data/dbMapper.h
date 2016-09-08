#ifndef DB_MAPPER_H
#define DB_MAPPER_H

#include <cppcms/view.h>
#include <cppdb/frontend.h>

#include <string>


namespace data {

    class DbMapper {

        std::string connectionString_;
        std::unique_ptr<cppdb::session> sql_;

    public:
        DbMapper(const std::string& connectionString);
        ~DbMapper();
        void clear();

    protected:
        cppdb::session& sql();
    };

}   // namespace data
#endif  // DB_MAPPER_H
