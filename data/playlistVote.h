#ifndef DATA_PLAYLIST_VOTE_H
#define DATA_PLAYLIST_VOTE_H

#include <cppcms/view.h>
#include <string>

namespace data {

    struct PlaylistVote : public cppcms::base_content {
        short value;

        bool isLike()   { return value == 1;    }
        bool isDislike()   { return value == -1;    }

        static short stringToVote(const std::string& s) {
            if (s == "like") {
                return 1;
            }
            if (s == "dislike") {
                return -1;
            }
            return 0;
        }
    };

}   // namespace data


#endif  // DATA_PLAYLIST_VOTE_H;
