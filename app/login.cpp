#include "login.h"

#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>
#include <booster/log.h>

#include <data/userMapper.h>

namespace app {
    Login::Login(cppcms::service& s) :
        app::Master(s)
    {
        dispatcher().assign(".*", &Login::display, this);
        mapper().assign("");
    }

    void Login::display() {
        BOOSTER_DEBUG("display");

        session().clear();

        data::LoginPage login(page_);
        login.pageTitle = "Authentification";
        login.user.isAllowed = true;

        if(request().request_method() == "POST") {
            login.input.load(context());

            if(! login.input.validate()) {
                login.alerts.errors.push_back("Invalid or missing fields!");
            } else {
                std::string username = login.input.username.value();
                std::string password = login.input.password.value();

                data::UserMapper userMapper(connectionString_);

                if(! userMapper.checkAuthentification(username, password, login)) {
                    login.alerts.errors.push_back("Wrong authentification!");
                } else {
                    session().clear();
                    session().set<unsigned int>("id", login.user.id);
                    session()["alias"] = login.user.alias;
                    session().set<unsigned int>("level", login.user.level);
                    login.alerts.success.push_back("Authenticated");
                    redirectTo(login.user, "/playlist");
                }
            }
            login.input.clear();
        }
        render("login", login);
    }

}   // namespace app

