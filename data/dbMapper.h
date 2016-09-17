#ifndef DB_MAPPER_H
#define DB_MAPPER_H

#include <cppcms/view.h>
#include <cppdb/frontend.h>

#include <string>


namespace data {

    class DbMapper {

        std::string connectionString_;
        std::unique_ptr<cppdb::session> connection_;

    public:
        DbMapper(const std::string& connectionString);
        ~DbMapper();

        /*
         * Called by desctructor.
         */
        void disconnect();

    protected:
        cppdb::session& connection();

    };

}   // namespace data
#endif  // DB_MAPPER_H
