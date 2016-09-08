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
        } else if (song_.title.length() > 45) {
            message << "Song title value " << song_.title << " is too long";
        } else if (song_.artist.empty()) {
            message << "Missing artist";
        } else if (song_.artist.length() > 50) {
            message << "Artist value " << song_.artist << " is too long";
        } else if (song_.url.empty()) {
            message << "Missing URL";
        } else if (song_.url.length() > 200) {
            message << "URL value " << song_.url << " is too long";
        } else {
            result = true;
        }

        lastMessage_ = message.str();
        return result;
    }

}}   // namespace app::validator
