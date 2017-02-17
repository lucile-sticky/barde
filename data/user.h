#ifndef DATA_USER_H
#define DATA_USER_H

#include <cppcms/view.h>

#include <data/media.h>
#include <data/song.h>

#include <string>
#include <sstream>
#include <vector>

namespace data {

    struct User : public cppcms::base_content {
        static constexpr const char* ANONYMOUS_ALIAS = "Anonymous";

        static constexpr const char* PRIVACY_PRIVATE = "private";
        static constexpr const char* PRIVACY_PUBLIC = "public";

        enum Level {
            ANONYMOUS = 0,  // No authentification
            GUEST = 1,
            CITIZEN = 10,
            ADMINISTRATOR = 200,
            ROOT = 255
        };

        unsigned int id;
        std::string alias;
        Media avatar;
        unsigned int level;
        bool isAuthenticated;
        bool isAllowed;
        std::string privacy;

        // Songs the user proposed
        std::vector<Song> proposedSongs;

        unsigned int nbComments;


        User() {
            clear();
        }

        bool isPublic() {
            return privacy == PRIVACY_PUBLIC;
        }

        bool hasAvatar() {
            return avatar.id > 0;
        }

        void clear() {
            id = 0;
            alias = ANONYMOUS_ALIAS;
            avatar.clear();
            level = data::User::ANONYMOUS;
            isAuthenticated = false;
            isAllowed = false;
            privacy = PRIVACY_PRIVATE;

            nbComments = 0;
        }

        std::string toString() const {
            std::ostringstream oss;
            oss << "{ id: " << id << ", alias: " << alias << ", avatar: " << avatar.id
                << ", level: " << level << ", privacy: " << privacy << " }";
            return oss.str();
        }
    };

}   // namespace data


#endif  // DATA_USER_H;
