#ifndef DATA_USER_H
#define DATA_USER_H

#include <cppcms/view.h>

#include <data/song.h>

#include <string>
#include <vector>

namespace data {

    struct User : public cppcms::base_content {
        enum Level {
            ANONYMOUS = 0,  // No authentification
            GUEST = 1,
            CITIZEN = 10,
            ADMINISTRATOR = 200,
            ROOT = 255
        };

        unsigned int id;
        std::string alias;
        unsigned int level;
        bool isAuthenticated;
        bool isAllowed;

        // Songs the user proposed
        std::vector<Song> proposedSongs;

        unsigned int nbComments;

        User() {
            clear();
        }

        void clear() {
            id = 0;
            alias = "Anonymous";
            level = data::User::ANONYMOUS;
            isAuthenticated = false;
            isAllowed = true;

            nbComments = 0;
        }
    };

}   // namespace data


#endif  // DATA_USER_H;
