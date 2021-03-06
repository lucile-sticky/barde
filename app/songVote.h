#ifndef APP_SONG_VOTE_H
#define APP_SONG_VOTE_H

#include <app/master.h>
#include <data/songVote.h>
#include <data/songVoteMapper.h>


namespace app {
    class SongVote : public Master {

    public:
        SongVote(cppcms::service& s);
        void ajaxVote(std::string songId, std::string vote);

    private:
        bool insert(unsigned int userId, unsigned int songId, short vote);
    };
}   // namespace app

#endif  // APP_SONG_VOTE_H

