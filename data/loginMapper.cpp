#include "loginMapper.h"

#include <booster/log.h>


namespace data {

    LoginMapper::LoginMapper(const std::string& connectionString)
        : DbMapper(connectionString)
    {}


    bool LoginMapper::checkAuthentification(
            const std::string& username,
            const std::string password,
            PageLogin& login
    ) {
        login.user.isAuthenticated = false;
        login.input.clear();

        std::string query = "SELECT password, id, alias, level "
            "FROM user "
            "WHERE username = ?";

        BOOSTER_DEBUG("checkAuthentification") << query;

        cppdb::statement statement = connection() << query << username;
        cppdb::result result = statement.row();

        if(! result.empty()) {
            std::string hashedPassword = cppcms::util::md5hex(password);
            if(hashedPassword == result.get<std::string>(0)) {
                login.user.id = result.get<unsigned int>(1);
                login.user.alias = result.get<std::string>(2);
                login.user.level = result.get<unsigned int>(3);
                login.user.isAuthenticated = true;

                BOOSTER_INFO("checkAuthentification") << login.user.id
                    << " - " << login.user.alias
                    << " authenticated with level " << login.user.level;
            } else {
                BOOSTER_WARNING("checkAuthentification") << username << " entered wrong password";
            }
        } else {
            BOOSTER_WARNING("checkAuthentification") << "Unknown user " << username << " tried to authenticate";
        }

        return login.user.isAuthenticated;
    }


}   // namespace data
