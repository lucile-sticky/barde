#include "songValidator.h"

#include <sstream>


namespace app { namespace validator {

    SongValidator::SongValidator(const data::Song& song)
        :song_(song)
    {}

    bool SongValidator::validate() {
        bool result = false;
        std::ostringstream message;

        if (song_.title.empty()) {
            message << "Missing song title";
        } else if (song_.title.length() > 100) {
            message << "Title is too long";
        } else if (song_.artist.name.empty()) {
            message << "Missing artist";
        } else if (song_.artist.name.length() > 100) {
            message << "Artist is too long";
        } else if (song_.url.empty()) {
            message << "Missing URL";
        } else if (song_.url.length() > 200) {
            message << "URL is too long";
        } else {
            result = true;
        }

        lastMessage_ = message.str();
        return result;
    }

}}   // namespace app::validator
