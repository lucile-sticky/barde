#include "playlistCommentMapper.h"

#include <booster/log.h>


namespace data {

    PlaylistCommentMapper::PlaylistCommentMapper(const std::string& connectionString)
        : DbMapper(connectionString)
    {}


    bool PlaylistCommentMapper::loadComments(PlaylistPage& dest, const std::string& playlistId) {
        bool success = false;

        dest.comments.clear();

        std::string query = "SELECT pc.id, pc.parent_id, pc.comment, "
            "u.alias AS author, UNIX_TIMESTAMP(pc.creation) AS creation, "
            "IFNULL(pc.parent_id, pc.id) AS position_id "
            "FROM playlist_comment pc "
            "INNER JOIN user u ON u.id = pc.author_id "
            "WHERE pc.playlist_id = ? "
            "ORDER BY position_id DESC, pc.id ASC";


        BOOSTER_DEBUG("loadComments") << query << ", " << playlistId;

        cppdb::result result = connection() << query << playlistId;

        while (result.next()) {
            data::PlaylistComment comment;
            comment.id = result.get<unsigned int>("id");
            comment.parentId = result.get<unsigned int>("parent_id", 0);
            comment.comment = result.get<std::string>("comment");
            comment.author = result.get<std::string>("author");
            comment.creation = result.get<time_t>("creation");

            if (comment.parentId == 0) {
                dest.comments.push_back(comment);
                success = true;
                continue;
            }

            if (dest.comments.empty()) {
                BOOSTER_ERROR("loadComments") << "No comment to add reply " << comment.id;
                continue;
            }

            if (dest.comments.back().id != comment.parentId) {
                BOOSTER_ERROR("loadComments") << "Missing lastest comment " << comment.parentId <<
                    " to add reply " << comment.id;
                continue;
            }

            dest.comments.back().replies.push_back(comment);
        }
        return success;
    }

    bool PlaylistCommentMapper::insert(unsigned int authorId, const std::string& playlistId, const PlaylistComment& comment) {
        std::string query = "INSERT INTO playlist_comment "
            "(playlist_id, parent_id, comment, author_id, status, creation) "
            "VALUES (?, ?, ?, ?, 'published', NOW()) ";

        BOOSTER_DEBUG("insert") << query << ", " << playlistId <<  ", "
            << comment.parentId <<  ", " << comment.comment <<  ", " << authorId;

        cppdb::statement st = connection() << query;
        st.bind(playlistId);
        if (comment.parentId == 0) {
            st.bind_null();
        } else {
            st.bind(comment.parentId);
        }
        st.bind(comment.comment);
        st.bind(authorId);
        st.exec();

        return st.affected() >= 1;
    }


}   // namespace data
