#ifndef DATA_PAGE_SONG_H
#define DATA_PAGE_SONG_H

#include <data/forms/song.h>
#include <data/pages/htmlPage.h>

namespace data {

	struct SongPage : public HtmlPage {
        int id;

        SongPage(const HtmlPage& other) :
            HtmlPage(other)
       {}
    };

    struct SongAdminPage : public SongPage {
        std::vector<Song> pendingSongs;

        SongAdminPage(const HtmlPage& other) :
            SongPage(other)
       {}
    };

    struct NewSongPage : public SongPage {
        NewSongForm input;

        NewSongPage(const HtmlPage& other) :
            SongPage(other)
       {}
	};

    struct EditSongPage : public SongPage {
        EditSongForm input;
        Song song;

        EditSongPage(const HtmlPage& other) :
            SongPage(other)
       {}
	};

}   // namespace data
#endif  // DATA_PAGE_SONG_H
