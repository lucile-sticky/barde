#ifndef APP_SONG_H
#define APP_SONG_H

#include <app/master.h>
#include <data/song.h>


namespace app {
    class Song : public Master {

    public:
        Song(cppcms::service& s);
        void displayEdit(std::string songId);
        void displayProposed();

        void ajaxNew();
        void ajaxSetPlaylist(std::string songId, std::string playlistId);
        void ajaxSetDuration(std::string songId, std::string duration);

    private:
        bool insert(const data::User& author, const data::Song& song);
        bool update(const data::Song& song);
    };
}   // namespace app

#endif  // APP_SONG_H

