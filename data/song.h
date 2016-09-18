#ifndef DATA_SONG_H
#define DATA_SONG_H

#include <data/songVote.h>

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

}   // namespace data
#endif  // DATA_SONG_H
