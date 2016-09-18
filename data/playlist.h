#ifndef DATA_PLAYLIST_H
#define DATA_PLAYLIST_H

#include <cppcms/view.h>

namespace data {

    struct PlaylistItem : public cppcms::base_content {
        std::string id;
        std::string name;
        std::string image;
        std::string description;
        time_t publication;
        size_t nbSongs;
        bool enabled;

        PlaylistItem() : enabled(0) {}
    };

}   // namespace data
#endif  // DATA_PLAYLIST_H
