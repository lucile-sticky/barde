#include "userMapper.h"

#include <booster/log.h>


namespace data {

    UserMapper::UserMapper(const std::string& connectionString)
        : DbMapper(connectionString)
    {}


    bool UserMapper::checkAuthentification(
            const std::string& username,
            const std::string password,
            LoginPage& login
    ) {
        login.user.isAuthenticated = false;
        login.input.clear();

        std::string query = "SELECT password, id, alias, level, privacy "
            "FROM user "
            "WHERE username = ?";

        BOOSTER_DEBUG(__func__) << query;

        cppdb::statement statement = connection() << query << username;
        cppdb::result result = statement.row();

        if(! result.empty()) {
            std::string hashedPassword = cppcms::util::md5hex(password);
            if(hashedPassword == result.get<std::string>(0)) {
                login.user.id = result.get<unsigned int>(1);
                login.user.alias = result.get<std::string>(2);
                login.user.level = result.get<unsigned int>(3);
                login.user.privacy = result.get<std::string>(4);
                login.user.isAuthenticated = true;

                BOOSTER_INFO(__func__) << login.user.id
                    << " \"" << login.user.alias << "\" authenticated with level " << login.user.level;
            } else {
                BOOSTER_WARNING(__func__) << username << " entered wrong password";
            }
        } else {
            BOOSTER_WARNING(__func__) << "Unknown user " << username << " tried to authenticate";
        }

        return login.user.isAuthenticated;
    }


    bool UserMapper::update(const User& user) {
        std::string query = "UPDATE user SET privacy = ? "
            "WHERE id = ? ";

        BOOSTER_DEBUG(__func__) << query << user.privacy << ", " << user.id;

        cppdb::statement st = connection() << query << user.privacy << user.id << cppdb::exec;

        return st.affected() >= 1;
    }

}   // namespace data
