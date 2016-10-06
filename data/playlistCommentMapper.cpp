#include "playlistCommentMapper.h"

#include <booster/log.h>


namespace data {

    PlaylistCommentMapper::PlaylistCommentMapper(const std::string& connectionString)
        : DbMapper(connectionString)
    {}

    bool PlaylistCommentMapper::saveComment(unsigned int authorId, const std::string& playlistId, const PlaylistComment& comment) {
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
