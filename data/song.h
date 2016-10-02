#ifndef DATA_SONG_H
#define DATA_SONG_H

#include <cppcms/view.h>

#include <data/playlist.h>
#include <data/songVote.h>

namespace data {

	struct Song : public cppcms::base_content {
        unsigned int id;
		std::string title;
		std::string artist;
		std::string file;
		std::string url;
        unsigned int duration;
        unsigned short position;
        bool showVideo;
		std::string proposer;
        PlaylistItem playlist;
        SongVote vote;

        Song() : showVideo(false), proposer(""), position(0)  {}
	};

}   // namespace data
#endif  // DATA_SONG_H
