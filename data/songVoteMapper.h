#ifndef SONG_VOTE_MAPPER_H
#define SONG_VOTE_MAPPER_H


#include <data/dbMapper.h>
#include <data/playlistVote.h>


namespace data {

    class SongVoteMapper : public DbMapper {
    public:
        SongVoteMapper(const std::string& connectionString);
        bool insert(unsigned int userId, const std::string& songId, short vote);
    };


}   // namespace data
#endif  // SONG_VOTE_MAPPER_H
