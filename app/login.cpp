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
        BOOSTER_DEBUG(__func__);

        data::LoginPage login(page_);
        login.pageTitle = translate("Authentification");
        login.user.isAllowed = true;

        session().erase("level");
        session().erase("id");
        session().erase("alias");

        if(session().is_set("error")) {
            login.alerts.errors.push_back(session()["error"]);
        }

        if(request().request_method() == "POST") {
            login.input.load(context());

            if(! login.input.validate()) {
                login.alerts.errors.push_back(translate("Invalid or missing fields!"));
            } else {
                std::string username = login.input.username.value();
                std::string password = login.input.password.value();

                data::UserMapper userMapper(connectionString_);

                if(! userMapper.checkAuthentification(username, password, login)) {
                    login.alerts.errors.push_back(translate("Wrong authentification!"));
                } else {
                    std::string location = session().is_set("location") ? session()["location"] : "/playlist";

                    session().clear();
                    session().set<unsigned int>("id", login.user.id);
                    session()["alias"] = login.user.alias;
                    session()["avatar"] = serializeMedia(login.user.avatar.id);
                    session().set<unsigned int>("level", login.user.level);
                    session()["privacy"] = login.user.privacy;

                    login.alerts.clear();
                    login.alerts.success.push_back(translate("Authenticated"));

                    redirectTo(login.user, location);
                }
            }
            login.input.clear();
        }
        render("login", login);
    }

}   // namespace app

