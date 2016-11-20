#ifndef DATA_MASTER_H
#define DATA_MASTER_H

#include <cppcms/view.h>
#include <booster/function.h>

#include <data/user.h>

#include <string>
#include <vector>

namespace data {

    struct AlertMessages : public cppcms::base_content {
        std::vector<std::string> errors;
        std::vector<std::string> warnings;
        std::vector<std::string> infos;
        std::vector<std::string> success;

        void clear() {
            errors.clear();
            warnings.clear();
            infos.clear();
            success.clear();
        }
    };

    /*
     * Base class for the HTML pages
     */
    struct HtmlPage : public cppcms::base_content {
        struct ComingPlaylist {
            unsigned int id;
            std::string name;
            unsigned int nbSongs;
        };

        // HTML data
        std::string siteTitle;
        std::string pageTitle;
        std::string mediaRootUrl;
        std::string httpScript;

        // User data
        User user;

        // Alert messages  - displayed on top of main content
        AlertMessages alerts;

        // Coming playlists - used in various pages
        std::vector<ComingPlaylist> comingPlaylists;


        HtmlPage() {}

        HtmlPage(const HtmlPage& other) :
            siteTitle(other.siteTitle),
            pageTitle(other.pageTitle),
            mediaRootUrl(other.mediaRootUrl),
            httpScript(other.httpScript)
        {}
    };

}   // namespace data
#endif  // DATA_MASTER_H
