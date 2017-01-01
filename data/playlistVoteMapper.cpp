#include "playlistVoteMapper.h"

#include <booster/log.h>


namespace data {

    PlaylistVoteMapper::PlaylistVoteMapper(const std::string& connectionString)
        : DbMapper(connectionString)
    {}


    bool PlaylistVoteMapper::insert(unsigned int userId, const std::string& playlistId, short vote) {
        std::string query = "INSERT INTO playlist_vote "
            "(playlist_id, user_id, vote, creation) VALUES (?, ?, ?, NOW()) "
            "ON DUPLICATE KEY UPDATE vote = ?";

        BOOSTER_DEBUG(__func__) << query << ", " << playlistId << ", "
            << userId << ", " << vote << ", " << vote << ", " << cppdb::exec;

        cppdb::statement st = connection() << query << playlistId << userId << vote << vote << cppdb::exec;

        return st.affected() >= 1;
    }


}   // namespace data
