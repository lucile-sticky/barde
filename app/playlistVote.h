#ifndef APP_PLAYLIST_VOTE_H
#define APP_PLAYLIST_VOTE_H

#include <app/master.h>
#include <data/playlistVote.h>
#include <data/playlistVoteMapper.h>


namespace app {
    class PlaylistVote : public Master {

    public:
        PlaylistVote(cppcms::service& s);
        void ajaxVote(std::string playlistId, std::string vote);
    };
}   // namespace app

#endif  // APP_PLAYLIST_VOTE_H

