#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>
#include <cppdb/frontend.h>
#include <booster/log.h>

#include <data/htmlPage.h>
#include <app/login.h>
#include <app/playlist.h>
#include <app/song.h>
#include <app/playlistVote.h>
#include <app/songVote.h>

class WepApp  : public cppcms::application {


public:
    WepApp(cppcms::service &s) :
       cppcms::application(s) {

        attach(new app::Login(s), "login", "{1}",
            "(/|/login|/logout)?", 1);

        attach(new app::Playlist(s), "playlist", "/playlist{1}",
            "/playlist(.*)", 1);

        attach(new app::Song(s), "song", "/song{1}",
            "/song(.*)", 1);

        // Votes
        attach(new app::PlaylistVote(s), "vote-playlist", "/vote-playlist{1}",
            "/vote-playlist(.*)", 1);

        attach(new app::SongVote(s), "vote-song", "/vote-song{1}",
            "/vote-song(.*)", 1);

        // Root
        mapper().root("/playlists");
    }
};


int main(int argc,char ** argv)
{
    BOOSTER_INFO("main") << "Running...";
    try {
        cppcms::service srv(argc,argv);
        srv.applications_pool().mount(cppcms::applications_factory<WepApp>());
        srv.run();
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        BOOSTER_ERROR("main") << e.what();
        return 1;
    }
    BOOSTER_INFO("main") << "Done";
    return 0;
}

