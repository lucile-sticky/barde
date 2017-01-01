#include "master.h"


#include <cppcms/cache_interface.h>
#include <cppcms/http_response.h>
#include <cppcms/json.h>
#include <cppcms/session_interface.h>
#include <booster/log.h>

#include <app/util/stringMethods.h>

#include <cstdlib>

using namespace cppcms::http;

namespace app {

    Master::Master(cppcms::service& s) : cppcms::application(s) {
        connectionString_ = settings().get<std::string>("database.connection_string");

        uploadRootPath_ = settings().get<std::string>("general.upload_root_path");

        // Page data
        page_.siteTitle = settings().get<std::string>("general.site_title");
        page_.pageTitle = "";
        page_.mediaRootUrl = settings().get<std::string>("general.media_root_url");
        page_.httpScript = settings().get<std::string>("http.script");
    }

    std::string Master::getCacheKey(const std::string& prefix, const std::string& id, const data::User& user) {
        std::ostringstream key;
        key << prefix << '_' << id << '_' << user.id;
        return key.str();
    }

    bool Master::checkAuth(data::User& user, data::User::Level requiredLevel) {
        user.clear();

        // Load user data from session
        if(session().is_set("level")) {
            user.isAuthenticated = true;
            user.level = session().get<unsigned int>("level");
        }
        if(session().is_set("id")) {
            user.id = session().get<unsigned int>("id");
        }
        if(session().is_set("alias")) {
            user.alias = session()["alias"];
        }
        if(session().is_set("privacy")) {
            user.privacy = session()["privacy"];
        }

        // Check level
        std::string pathInfo = request().path_info();
        if (user.level < static_cast<unsigned int>(requiredLevel)) {
            forbidAccess(user);
        } else {    // Allowed
            user.isAllowed = true;
            BOOSTER_INFO(__func__) << user.alias << " accesses " << pathInfo;
        }
        BOOSTER_DEBUG(__func__) << "User -> " << user.toString();

        return user.isAllowed;
    }

    void Master::redirectTo(const data::User& user, const std::string& internalLocation) {
        std::string location = page_.httpScript + internalLocation;
        BOOSTER_INFO(__func__) << "Redirect user " << user.alias << " to internal location " << location;
        response().set_redirect_header(location);
    }

    void Master::forbidAccess(const data::User& user) {
        std::string internalLocation = request().path_info();
        BOOSTER_WARNING(__func__) << "Forbid " << user.alias << " to access " << internalLocation;

        session()["error"] = translate("You have to authenticate to access the requested page!");
        session()["location"] = internalLocation;
        redirectTo(user, "/login");
    }

    std::string Master::composeImagePlaylistPath(const cppcms::http::file* mediaFile) const {
        return composeFilePath(PLAYLISTS_IMAGE_DIR, mediaFile);
    }

    std::string Master::composeUploadDestPath(const cppcms::http::file* mediaFile) const {
        return composeFilePath(uploadRootPath_, mediaFile);
    }

    std::string Master::composeFilePath(const std::string& dirPath, const cppcms::http::file* mediaFile) const {
        std::string fileName = std::string(dirPath);
        if (fileName[fileName.length() - 1] != '/') {
            fileName += '/';
        }
        fileName += app::util::StringMethods::toSnakeCase(mediaFile->filename());
        return fileName;
    }

}   // namespace app
