#ifndef APP_SONG_H
#define APP_SONG_H

#include <app/master.h>
#include <data/artistMapper.h>
#include <data/playlistMapper.h>
#include <data/song.h>
#include <data/songMapper.h>


namespace app {
    class Song : public Master {
        std::unique_ptr<data::SongMapper> dbSongMapper_;
        std::unique_ptr<data::ArtistMapper> dbArtistMapper_;
        std::unique_ptr<data::PlaylistMapper> dbPlaylistMapper_;

    public:
        Song(cppcms::service& s);
        void displayEdit(std::string songId);
        void displayProposed();

        void ajaxNew();
        void ajaxSetPlaylist(std::string songId, std::string playlistId);

    private:
        bool insert(const data::Song& song);
        bool update(const data::Song& song);
    };
}   // namespace app

#endif  // APP_SONG_H

