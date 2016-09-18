#ifndef DATA_PAGE_SONG_H
#define DATA_PAGE_SONG_H

#include <data/forms/song.h>
#include <data/pages/htmlPage.h>

namespace data {

	struct SongPage : public HtmlPage {
        int id;
    };
    struct SongAdminPage : public SongPage {
        std::vector<Song> pendingSongs;
    };
	struct NewSongPage : public SongPage {
        NewSongForm input;
	};
	struct EditSongPage : public SongPage {
        EditSongForm input;
	};

}   // namespace data
#endif  // DATA_PAGE_SONG_H
