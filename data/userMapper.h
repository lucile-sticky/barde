#ifndef USER_MAPPER_H
#define USER_MAPPER_H


#include "dbMapper.h"
#include "user.h"
#include <data/pages/login.h>


namespace data {

    class UserMapper : public DbMapper {
    public:
        UserMapper(const std::string& connectionString);

        bool checkAuthentification(
                const std::string& username,
                const std::string password,
                LoginPage& login);

        bool update(const User& user);

    };


}   // namespace data
#endif  // USER_MAPPER_H
