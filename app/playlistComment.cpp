#include "playlistComment.h"

#include <cppcms/http_response.h>
#include <cppcms/json.h>
#include <cppcms/url_dispatcher.h>
#include <booster/log.h>

#include <app/playlist.h>


using namespace cppcms::http;

namespace app {
    PlaylistComment::PlaylistComment(cppcms::service& s) :
        app::Master(s)
    {
        dispatcher().assign("/ajax-comment/(\\d+)/(\\d+)", &PlaylistComment::ajaxComment, this, 1, 2);
        mapper().assign("/{1}/{2}");
    }

    void PlaylistComment::ajaxComment(std::string playlistId, std::string parentId) {
        data::User user;

        if (! checkAuth(user, data::User::CITIZEN)) {
            response().make_error_response(response::forbidden);
            BOOSTER_WARNING(__func__) << "Forbid user "
                << user.alias << " to comment playlist " << playlistId;
            return;
        }

        data::PlaylistComment playlistComment;
        playlistComment.parentId = std::stoi(parentId);
        playlistComment.comment = request().post("comment");

        data::PlaylistCommentMapper playlistCommentMapper(connectionString_);
        bool success = playlistCommentMapper.insert(
                user.id,
                playlistId,
                playlistComment);

        cache().rise(std::to_string(user.id));
        BOOSTER_DEBUG(__func__) << "Clean caches for user ID " << user.id;

        cppcms::json::value jsonOutput;
        jsonOutput["success"] = success;
        response().out() << jsonOutput;
    }

}   // namespace app
