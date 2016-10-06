#ifndef APP_PLAYLIST_COMMENT_H
#define APP_PLAYLIST_COMMENT_H

#include <app/master.h>
#include <data/playlistComment.h>
#include <data/playlistCommentMapper.h>


namespace app {
    class PlaylistComment : public Master {

    public:
        PlaylistComment(cppcms::service& s);
        void ajaxComment(std::string playlistId, std::string parentId);
    };
}   // namespace app

#endif  // APP_PLAYLIST_COMMENT_H

