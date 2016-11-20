#include "songVote.h"

#include <cppcms/http_response.h>
#include <cppcms/json.h>
#include <cppcms/url_dispatcher.h>
#include <booster/log.h>

#include <app/playlist.h>


using namespace cppcms::http;

namespace app {
    SongVote::SongVote(cppcms::service& s) :
        app::Master(s)
    {
        dispatcher().assign("/ajax-vote/([0-9a-z]+)/(\\d+)/(.+)", &SongVote::ajaxVote, this, 1, 2, 3);
        mapper().assign("/{1}/{2}/{3}");
    }

    void SongVote::ajaxVote(std::string playlistId, std::string songId, std::string vote) {
        data::User user;

        if (! checkAuth(user, data::User::CITIZEN)) {
            response().make_error_response(response::forbidden);
            BOOSTER_WARNING("ajaxVote") << "Forbid user "
                << user.alias << " to vote for song";
            return;
        }

        data::SongVoteMapper songVoteMapper(connectionString_);
        bool result = songVoteMapper.insert(
            user.id,
            songId,
            data::SongVote::stringToVote(vote)
        );

        std::string key = Playlist::getCacheKey(playlistId, user);
        cache().rise(key);
        BOOSTER_DEBUG("ajaxVote") << "Clean cache for key=" << key;

        cppcms::json::value jsonOutput;
        jsonOutput["success"] = result;
        response().out() << jsonOutput;
    }

}   // namespace app
