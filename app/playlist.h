#ifndef APP_PLAYLIST_H
#define APP_PLAYLIST_H

#include <app/master.h>
#include <data/pages/playlist.h>


namespace app {
    class Playlist : public Master {
    public:
        static constexpr const char* CACHE_PREFIX = "playlist";

    private:
        static const unsigned short TOP_LIST_NB_SONGS = 20;
        static const std::string TOP_LIST_ID;
        static const std::string TOP_LIST_NAME;
        static const std::string TOP_LIST_IMAGE;

        static const std::string WORST_LIST_ID;
        static const std::string WORST_LIST_NAME;
        static const std::string WORST_LIST_IMAGE;

        static const std::string RANDOM_LIST_ID;
        static const std::string RANDOM_LIST_NAME;
        static const std::string RANDOM_LIST_IMAGE;

        static const std::string PROPOSED_LIST_NAME;

        data::PlaylistPage playlist_;

    public:
        Playlist(cppcms::service& s);
        void displayCurrent();
        void display(std::string playlistId);
        void displayAll();
        void displayTop();
        void displayWorst();
        void displayRandom();
        void displayProposed();
        void displayNew();

        static std::string getCacheKey(const std::string& playlistId, const data::User& user);

    private:
        void doDisplay();


    };
}   // namespace app

#endif  // APP_PLAYLIST_H

