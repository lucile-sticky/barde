#include "playlistVoteMapper.h"

#include <booster/log.h>


namespace data {

    PlaylistVoteMapper::PlaylistVoteMapper(const std::string& connectionString)
        : DbMapper(connectionString)
    {}


    bool PlaylistVoteMapper::saveVote(unsigned int userId, const std::string& playlistId, short vote) {
        std::string query = "INSERT INTO playlist_vote "
            "(playlist_id, user_id, vote) VALUES (?, ?, ?) "
            "ON DUPLICATE KEY UPDATE vote = ?";

        BOOSTER_DEBUG("saveVote") << query << ", " << playlistId << ", "
            << userId << ", " << vote << ", " << vote << ", " << cppdb::exec;

        cppdb::statement st = connection() << query << playlistId << userId << vote << vote << cppdb::exec;

        return st.affected() >= 1;
    }


}   // namespace data
