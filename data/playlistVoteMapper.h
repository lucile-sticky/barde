#ifndef PLAYLIST_VOTE_MAPPER_H
#define PLAYLIST_VOTE_MAPPER_H


#include <data/dbMapper.h>
#include <data/playlistVote.h>


namespace data {

    class PlaylistVoteMapper : public DbMapper {
    public:
        PlaylistVoteMapper(const std::string& connectionString);
        bool saveVote(unsigned int userId, const std::string& playlistId, short vote);
    };


}   // namespace data
#endif  // PLAYLIST_VOTE_MAPPER_H
