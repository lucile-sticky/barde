#include "songVoteMapper.h"

#include <booster/log.h>


namespace data {

    SongVoteMapper::SongVoteMapper(const std::string& connectionString)
        : DbMapper(connectionString)
    {}


    bool SongVoteMapper::saveVote(unsigned int userId, const std::string& songId, short vote) {
        std::string query = "INSERT INTO song_vote "
            "(song_id, user_id, vote) VALUES (?, ?, ?) "
            "ON DUPLICATE KEY UPDATE vote = ?";

        BOOSTER_DEBUG("saveVote") << query << ", " << songId << ", "
            << userId << ", " << vote << ", " << vote << ", " << cppdb::exec;

        cppdb::statement st = sql() << query << songId << userId << vote << vote << cppdb::exec;

        return st.affected() >= 1;
    }


}   // namespace data
