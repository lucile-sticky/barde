#ifndef APP_MASTER_H
#define APP_MASTER_H

#include <cppcms/application.h>
#include <cppcms/http_file.h>
#include <cppcms/service.h>
#include <cppdb/frontend.h>

#include <data/pages/htmlPage.h>
#include <data/user.h>

#include <string>

namespace app {
    class Master : public cppcms::application {
    protected:
        static const int CACHE_TTL_SHORT = 30;
        static const int CACHE_TTL_MEDIUM = 60;
        static const int CACHE_TTL_LONG = 300;

        static constexpr const char* UPLOAD_PATH = "/upload";

        std::string connectionString_;
        data::HtmlPage page_;

    public:
        Master(cppcms::service& s);

        static std::string getCacheKey(const std::string& prefix, const std::string& id, const data::User& user);

    protected:
        bool checkAuth(data::User& user, data::User::Level requiredLevel = data::User::GUEST);
        void redirectTo(const data::User& user, const std::string& internalLocation);
        void forbidAccess(const data::User& user);

        std::string toUploadRelativePath(const cppcms::http::file* uploadedFile) const;
        std::string toUploadRelativePath(const cppcms::http::file* uploadedFile, const std::string forceFilename) const;
        std::string toFullPath(const std::string& relativePath) const;
        std::string toPublicUrl(const std::string& relativePath) const;

        std::string serializeMedia(unsigned int id) const;
        std::string serializeMedia(const data::Media& src) const;
        data::Media& deserializeMedia(data::Media& dest, const std::string serialized) const;
    };
}   // namespace app

#endif  // APP_MASTER_H
