#ifndef APP_MASTER_H
#define APP_MASTER_H

#include <cppcms/application.h>
#include <cppcms/http_file.h>
#include <cppcms/service.h>
#include <cppdb/frontend.h>

#include <data/htmlPage.h>
#include <data/user.h>

#include <string>

namespace app {
    class Master : public cppcms::application {
    protected:
        static const int CACHE_TTL_SHORT = 30;
        static const int CACHE_TTL_MEDIUM = 300;
        static const int CACHE_TTL_LONG = 3600;

        static constexpr const char* PLAYLISTS_IMAGE_DIR = "/images/playlists";

        std::string connectionString_;
        std::string uploadRootPath_;
        data::HtmlPage page_;

    public:
        Master(cppcms::service& s);

        static std::string getCacheKey(const std::string& prefix, const std::string& id, const data::User& user);

    protected:
        bool checkAuth(data::User::Level requiredLevel = data::User::GUEST);
        void redirectTo(const std::string& internalLocation);

        std::string composeImagePlaylistPath(const cppcms::http::file* mediaFile) const;
        std::string composeUploadDestPath(const cppcms::http::file* mediaFile) const;

    private:
        std::string composeFilePath(const std::string& dirPath, const cppcms::http::file* mediaFile) const;
    };
}   // namespace app

#endif  // APP_MASTER_H
