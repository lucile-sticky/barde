#ifndef SONG_MAPPER_H
#define SONG_MAPPER_H


#include "dbMapper.h"
#include "song.h"
#include <data/pages/song.h>


namespace data {

    class SongMapper : public DbMapper {
    public:
        static const std::string SQL_GLOBAL_VOTES;

        SongMapper(const std::string& connectionString);

        bool loadSong(Song& dest, const std::string& songId);
        bool loadUserProposedSongs(User& dest);
        bool loadPendingSongs(SongAdminPage& dest);

        bool insert(const User& proposer, unsigned int artistId, const Song& song);
        bool update(unsigned int artistId, const Song& song);
        bool updatePlaylistId(unsigned int songId, unsigned int playlistId);
        bool updateDuration(unsigned int songId, unsigned int duration);
    };


}   // namespace data
#endif  // SONG_MAPPER_H
