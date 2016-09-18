#ifndef LOGIN_MAPPER_H
#define LOGIN_MAPPER_H


#include "dbMapper.h"
#include <data/pages/login.h>


namespace data {

    class LoginMapper : public DbMapper {
    public:
        LoginMapper(const std::string& connectionString);
        bool checkAuthentification(
                const std::string& username,
                const std::string password,
                LoginPage& login);
    };


}   // namespace data
#endif  // LOGIN_MAPPER_H
