#ifndef DATA_PAGE_PLAYLIST_H
#define DATA_PAGE_PLAYLIST_H

#include <data/playlist.h>
#include <data/song.h>
#include <data/forms/playlist.h>
#include <data/pages/htmlPage.h>

namespace data {

    struct PlaylistPage : public HtmlPage {
        std::string id;
        std::string name;
        std::string image;
        std::string description;
        time_t publication;
        bool votesEnabled;
        std::vector<Song> songs;
        PlaylistVote vote;

        PlaylistPage() :id(""), votesEnabled(true) {}
    };

    struct AllPlaylistsPage : public HtmlPage {
        std::vector<PlaylistItem> playlists;
    };

	struct NewPlaylistPage:public HtmlPage {
        PlaylistForm input;
	};

}   // namespace data
#endif  // DATA_PAGE_PLAYLIST_H
