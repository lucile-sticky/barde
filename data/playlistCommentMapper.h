#ifndef PLAYLIST_COMMENT_MAPPER_H
#define PLAYLIST_COMMENT_MAPPER_H


#include <data/dbMapper.h>

#include <data/playlistComment.h>


namespace data {

    class PlaylistCommentMapper : public DbMapper {
    public:
        PlaylistCommentMapper(const std::string& connectionString);

        bool saveComment(unsigned int authorId, const std::string& playlistId, const PlaylistComment& comment);
    };


}   // namespace data
#endif  // PLAYLIST_COMMENT_MAPPER_H
