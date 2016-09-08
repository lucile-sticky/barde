#include "master.h"


#include <cppcms/json.h>
#include <cppcms/session_interface.h>
#include <cppcms/cache_interface.h>
#include <booster/log.h>

#include <app/util/stringMethods.h>

#include <cstdlib>

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

    bool Master::checkAuth(data::User::Level requiredLevel) {
        page_.user.clear();
        page_.user.isAllowed = false;

        // Load user data from session
        if(session().is_set("level")) {
            page_.user.isAuthenticated = true;
            page_.user.level = session().get<unsigned int>("level");
        }
        if(session().is_set("id")) {
            page_.user.id = session().get<unsigned int>("id");
        }
        if(session().is_set("alias")) {
            page_.user.alias = session()["alias"];
        }

        // Check level
        if (page_.user.level < static_cast<unsigned int>(requiredLevel)) {
            BOOSTER_WARNING("checkAuth") << page_.user.alias << " is trying to access unauthorized content";
        } else {
            page_.user.isAllowed = true;
        }

        return page_.user.isAllowed;
    }

    void Master::redirectTo(const std::string& internalLocation) {
        std::string location = page_.httpScript + internalLocation;
        BOOSTER_INFO("redirectTo") << "Redirect user " << page_.user.alias << " to internal location " << location;
        response().set_redirect_header(location);
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
