#ifndef DATA_PLAYLIST_H
#define DATA_PLAYLIST_H

#include <data/htmlPage.h>
#include <data/song.h>
#include <data/playlistVote.h>

namespace data {

    struct Playlist: public HtmlPage {
        static constexpr const char* TYPE_SIMPLE = "simple";
        static constexpr const char* TYPE_JWPLAYER = "jwplayer";

        std::string id;
        std::string name;
        std::string image;
        std::string description;
        time_t publication;
        std::string player;
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

	struct PlaylistForm:public cppcms::form {
		cppcms::widgets::text name;
		cppcms::widgets::file image;
		cppcms::widgets::textarea description;
		cppcms::widgets::submit submit;
		cppcms::form inputs;
		cppcms::form buttons;

		PlaylistForm() {
            using booster::locale::translate;

            name.id("name");
			name.message(translate("Name:"));
			name.limits(1, 45);

			image.id("image");
			image.message(translate("Image:"));
			image.limits(1000, 100000);
            image.mime(booster::regex("image/.+"));
            image.non_empty();

            description.id("description");
			description.message(translate("Description:"));
            description.cols(40);
            description.rows(4);
			description.limits(0, 500);

			submit.value(translate("Submit"));

			inputs.add(name);
			inputs.add(image);
			inputs.add(description);
			buttons.add(submit);
			add(inputs);
			add(buttons);
	}};

	struct PagePlaylist:public HtmlPage {
		PlaylistForm input;
	};

}   // namespace data
#endif  // DATA_PLAYLIST_H
