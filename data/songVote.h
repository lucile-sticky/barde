#ifndef DATA_SONG_VOTE_H
#define DATA_SONG_VOTE_H

#include <cppcms/view.h>
#include <string>

namespace data {

    struct SongVote : public cppcms::base_content {
        short value;
        float totalValues;

        bool isLike()   { return value == 1;    }
        bool isDislike()   { return value == -1;    }

        SongVote()
            :value(0), totalValues(0)
        {}

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


#endif  // DATA_SONG_VOTE_H;
