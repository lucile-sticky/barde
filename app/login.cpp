#include "login.h"

#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>
#include <booster/log.h>


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
        login_.user.clear();
        login_.alerts.clear();

        if(request().request_method() == "POST") {
            login_.input.load(context());

            if(! login_.input.validate()) {
                login_.alerts.errors.push_back("Invalid or missing fields!");
            } else {
                std::string username = login_.input.username.value();
                std::string password = login_.input.password.value();

                data::LoginMapper loginMapper(connectionString_);

                if(! loginMapper.checkAuthentification(username, password, login_)) {
                    login_.alerts.errors.push_back("Wrong authentification!");
                } else {
                    session().clear();
                    session().set<unsigned int>("id", login_.user.id);
                    session()["alias"] = login_.user.alias;
                    session().set<unsigned int>("level", login_.user.level);
                    login_.alerts.success.push_back("Authenticated");
                    redirectTo("/playlist");
                }
            }
            login_.input.clear();
        }

        login_.resetFrom(page_);
        login_.pageTitle = "Authentification";

        render("login", login_);
    }

}   // namespace app

