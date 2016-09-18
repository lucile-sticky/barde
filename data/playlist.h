#ifndef DATA_PLAYLIST_H
#define DATA_PLAYLIST_H

#include <data/pages/htmlPage.h>
#include <data/song.h>
#include <data/playlistVote.h>
#include <data/forms/playlist.h>

namespace data {

    struct PlaylistItem : public cppcms::base_content {
        std::string id;
        std::string name;
        std::string image;
        std::string description;
        time_t publication;
        size_t nbSongs;
    };

}   // namespace data
#endif  // DATA_PLAYLIST_H
