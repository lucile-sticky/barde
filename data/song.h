#ifndef DATA_SONG_H
#define DATA_SONG_H

#include <data/htmlPage.h>
#include <data/songVote.h>

#include <forms/song.h>

#include <booster/regex.h>

namespace data {

	struct Song : public cppcms::base_content {
        struct Playlist {
            std::string id;
            std::string name;
            bool enabled;

            Playlist() : enabled(0)  {}
        };

        unsigned int id;
		std::string title;
		std::string artist;
		std::string file;
		std::string url;
        unsigned short position;
        bool showVideo;
		std::string proposer;
        Playlist playlist;
        SongVote vote;

        Song() : showVideo(false), proposer(""), position(0)  {}

	};

	struct PageSong : public HtmlPage {
        int id;
    };
    struct AdminPageSong : public PageSong {
        std::vector<Song> pendingSongs;
    };
	struct PageNewSong : public PageSong {
        form::NewSongForm input;
	};
	struct PageEditSong : public PageSong {
        form::EditSongForm input;
	};

}   // namespace data
#endif  // DATA_SONG_H
