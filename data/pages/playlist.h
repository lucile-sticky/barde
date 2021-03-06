#ifndef DATA_PAGE_PLAYLIST_H
#define DATA_PAGE_PLAYLIST_H

#include <data/playlist.h>
#include <data/playlistVote.h>
#include <data/playlistComment.h>
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
        bool autoGenerated;

        std::vector<Song> songs;

        PlaylistVote vote;
        std::vector<data::PlaylistComment> comments;

        PlaylistPage(const HtmlPage& other) :
            HtmlPage(other),
            id(""),
            autoGenerated(false)
       {}
    };

    struct AllPlaylistsPage : public HtmlPage {
        std::vector<PlaylistItem> playlists;

        AllPlaylistsPage(const HtmlPage& other) :
            HtmlPage(other)
       {}
    };

	struct NewPlaylistPage : public HtmlPage {
        PlaylistForm input;

        NewPlaylistPage(const HtmlPage& other) :
            HtmlPage(other)
       {}
	};

}   // namespace data
#endif  // DATA_PAGE_PLAYLIST_H
