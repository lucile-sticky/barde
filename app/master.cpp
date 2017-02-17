#include "master.h"


#include <cppcms/cache_interface.h>
#include <cppcms/http_response.h>
#include <cppcms/json.h>
#include <cppcms/session_interface.h>
#include <booster/log.h>

#include <data/mediaMapper.h>
#include <app/util/pathMethods.h>
#include <app/util/stringMethods.h>

#include <cstdlib>

using namespace cppcms::http;

namespace app {

    Master::Master(cppcms::service& s) : cppcms::application(s) {
        connectionString_ = settings().get<std::string>("database.connection_string");

        // Page data
        page_.siteTitle = settings().get<std::string>("general.site_title");
        page_.pageTitle = "";

        std::string host = settings().get<std::string>("public.host");
        if (!host.empty()) {
            host = "http://" + host;
        }

        page_.publicUrl = util::PathMethods::combine(
                host,
                settings().get<std::string>("public.path"));
        page_.publicPath = util::PathMethods::combine(
            settings().get<std::string>("public.www_root"),
            settings().get<std::string>("public.path"));
        page_.httpScript = settings().get<std::string>("http.script");
    }

    std::string Master::getCacheKey(const std::string& prefix, const std::string& id, const data::User& user) {
        std::ostringstream key;
        key << prefix << '_' << id << '_' << user.id;

        BOOSTER_DEBUG(__func__) << "Composed cache key " << key.str();

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
        if(session().is_set("avatar")) {
            deserializeMedia(user.avatar, session()["avatar"]);
        }
        if(session().is_set("privacy")) {
            user.privacy = session()["privacy"];
        }

        // Check level
        BOOSTER_DEBUG(__func__) << "Checking user -> " << user.toString();

        std::string pathInfo = request().path_info();
        if (user.level < static_cast<unsigned int>(requiredLevel)) {
            forbidAccess(user);
        } else {    // Allowed
            user.isAllowed = true;
            BOOSTER_INFO(__func__) << user.alias << " accesses " << pathInfo;
        }

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

    std::string Master::toUploadRelativePath(const cppcms::http::file* uploadedFile) const {
        std::string result = util::PathMethods::combine(
                UPLOAD_PATH,
                util::StringMethods::toSnakeCase(uploadedFile->filename()));

        BOOSTER_DEBUG(__func__) << result;
        return result;
    }

    std::string Master::toFullPath(const std::string& relativePath) const {
        std::string result = util::PathMethods::combine(
                page_.publicPath,
                relativePath);

        BOOSTER_DEBUG(__func__) << result;
        return result;
    }

    std::string Master::toPublicUrl(const std::string& relativePath) const {
        std::string result = util::PathMethods::combine(
                page_.publicUrl,
                relativePath);

        BOOSTER_DEBUG(__func__) << result;
        return result;
    }

    std::string Master::serializeMedia(unsigned int id) const {
        if (id <= 0) {
            BOOSTER_WARNING(__func__) << "Invalid media ID = " << id;
            return "";
        }

        data::Media media;
        data::MediaMapper mediaMapper(connectionString_);

        if (!mediaMapper.get(media, id)) {
            BOOSTER_ERROR(__func__) << "No media for ID: " << id << ", how possible ?";
            return "";
        }

        BOOSTER_DEBUG(__func__) << "Loaded media -> " << media.toString();
        return serializeMedia(media);
    }

    std::string Master::serializeMedia(const data::Media& src) const {
        cppcms::archive a;
        src.save(a);

        BOOSTER_DEBUG(__func__) << "Serialized media -> " << src.toString();

        return a.str();
    }

    data::Media& Master::deserializeMedia(data::Media& dest, const std::string serialized) const {
        if (serialized.empty()) {
            BOOSTER_WARNING(__func__) << "No serilialized media";
        } else {
            cppcms::archive a;
            a.str(serialized);
            dest.load(a);
            BOOSTER_DEBUG(__func__) << "Deserialized media -> " << dest.toString();
        }
        return dest;
    }

}   // namespace app

