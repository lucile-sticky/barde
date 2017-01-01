#include "user.h"

#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>
#include <booster/log.h>

#include <data/userMapper.h>

using namespace cppcms::http;

namespace app {
    User::User(cppcms::service& s) :
        app::Master(s)
    {
        dispatcher().assign("/ajax-update", &User::ajaxUpdate, this);
        mapper().assign("");
    }

    void User::ajaxUpdate() {
        data::User user;

        if (! checkAuth(user, data::User::CITIZEN)) {
            response().make_error_response(response::forbidden);
            BOOSTER_WARNING(__func__) << "Forbid user "
                << user.alias << " to update user";
            return;
        }

        BOOSTER_DEBUG(__func__);

        data::User updateUser;
        updateUser.id = user.id;
        updateUser.privacy = request().post("privacy");

        data::UserMapper userMapper(connectionString_);
        bool success = userMapper.update(updateUser);
        if (success) {
            session()["privacy"] = updateUser.privacy;
        }

        cppcms::json::value jsonOutput;
        jsonOutput["success"] = success;
        response().out() << jsonOutput;
    }

}   // namespace app

