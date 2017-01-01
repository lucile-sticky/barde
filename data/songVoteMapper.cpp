#include "songVoteMapper.h"

#include <booster/log.h>


namespace data {

    SongVoteMapper::SongVoteMapper(const std::string& connectionString)
        : DbMapper(connectionString)
    {}


    bool SongVoteMapper::insert(unsigned int userId, const std::string& songId, short vote) {
        std::string query = "INSERT INTO song_vote "
            "(song_id, user_id, vote, creation) VALUES (?, ?, ?, NOW()) "
            "ON DUPLICATE KEY UPDATE vote = ?";

        BOOSTER_DEBUG(__func__) << query << ", " << songId << ", "
            << userId << ", " << vote << ", " << vote << ", " << cppdb::exec;

        cppdb::statement st = connection() << query << songId << userId << vote << vote << cppdb::exec;

        return st.affected() >= 1;
    }


}   // namespace data
