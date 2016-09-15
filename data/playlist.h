#ifndef DATA_PLAYLIST_H
#define DATA_PLAYLIST_H

#include <data/htmlPage.h>
#include <data/song.h>
#include <data/playlistVote.h>
#include <forms/playlist.h>

namespace data {

    struct Playlist: public HtmlPage {
        std::string id;
        std::string name;
        std::string image;
        std::string description;
        time_t publication;
        bool votesEnabled;
        std::vector<Song> songs;
        PlaylistVote vote;

        Playlist() :id(""), votesEnabled(true) {}
    };

    struct PlaylistItem : public cppcms::base_content {
        std::string id;
        std::string name;
        std::string image;
        std::string description;
        time_t publication;
        size_t nbSongs;
    };

    struct AllPlaylists: public HtmlPage {
        std::vector<PlaylistItem> playlists;
    };

	struct PagePlaylist:public HtmlPage {
        form::PlaylistForm input;
	};

}   // namespace data
#endif  // DATA_PLAYLIST_H
