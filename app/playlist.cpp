#include "playlist.h"

#include <cppcms/http_file.h>
#include <cppcms/url_dispatcher.h>
#include <booster/log.h>

#include <app/util/stringMethods.h>

#include <data/playlistMapper.h>
#include <data/playlistCommentMapper.h>
#include <data/songMapper.h>

#include <sstream>

using namespace cppcms::http;

namespace app {
    const std::string Playlist::TOP_LIST_ID = "top";
    const std::string Playlist::TOP_LIST_NAME = "Meta-Playlist";
    const std::string Playlist::TOP_LIST_IMAGE = "/images/playlists/metabaron.png";

    const std::string Playlist::WORST_LIST_ID = "worst";
    const std::string Playlist::WORST_LIST_NAME = "Anti-Playlist";
    const std::string Playlist::WORST_LIST_IMAGE = "/images/playlists/john_difool.png";

    const std::string Playlist::RANDOM_LIST_ID = "random";
    const std::string Playlist::RANDOM_LIST_NAME = "Random songs";
    const std::string Playlist::RANDOM_LIST_IMAGE = "/images/playlists/dice.png";

    const std::string Playlist::PROPOSED_LIST_NAME = "Proposed songs";

    Playlist::Playlist(cppcms::service& s) :
        app::Master(s)
    {
        dispatcher().assign("", &Playlist::displayCurrent, this);
        mapper().assign("");

        dispatcher().assign("/(\\d+)", &Playlist::display, this, 1);
        mapper().assign("/{1}");

        dispatcher().assign("/all", &Playlist::displayAll, this);
        mapper().assign("");

        dispatcher().assign("/top", &Playlist::displayTop, this);
        mapper().assign("");

        dispatcher().assign("/worst", &Playlist::displayWorst, this);
        mapper().assign("");

        dispatcher().assign("/random", &Playlist::displayRandom, this);
        mapper().assign("");

        dispatcher().assign("/proposed", &Playlist::displayProposed, this);
        mapper().assign("");

        dispatcher().assign("/new", &Playlist::displayNew, this);
        mapper().assign("");
    }

    void Playlist::displayCurrent() {
        BOOSTER_DEBUG("displayCurrent");

        data::PlaylistMapper playlistMapper(connectionString_);
        std::string playlistId = playlistMapper.findCurrentPlaylistId();

        if (playlistId.empty()) {
            playlistId = TOP_LIST_ID;
            BOOSTER_WARNING("displayCurrent") << "No current playlist, falling back on " << playlistId;
        }
        display(playlistId);
    }

    void Playlist::display(std::string playlistId) {
        BOOSTER_DEBUG("display") << "playlist ID: " << playlistId;

        data::PlaylistPage playlist(page_);

        if (! checkAuth(playlist.user)) {
            return;
        }

        std::string key = getCacheKey(playlistId, playlist.user);
        if (cache().fetch_page(key)) {
            BOOSTER_DEBUG("display") << "Fetch cache for key=" << key;
            return;
        }

        data::PlaylistMapper playlistMapper(connectionString_);
        playlistMapper.loadPlaylist(playlist, playlistId, playlist.user);

        data::PlaylistCommentMapper playlistCommentMapper(connectionString_);
        playlistCommentMapper.loadComments(playlist, playlistId);
        playlistCommentMapper.loadUserNbComments(playlist.user);

        doDisplay(playlist);

        cache().add_trigger(std::to_string(playlist.user.id));
        cache().store_page(key, CACHE_TTL_MEDIUM);
        BOOSTER_DEBUG("display") << "Store cache for key=" << key;
    }

    void Playlist::displayAll() {
        BOOSTER_DEBUG("displayAll");

        data::AllPlaylistsPage allPlaylists(page_);

        if (! checkAuth(allPlaylists.user)) {
            response().make_error_response(response::forbidden);
            BOOSTER_WARNING("displayAll") << "Forbid user "
                << allPlaylists.user.alias << " to access all playlists";
            return;
        }

        std::string key = getCacheKey("ALL", allPlaylists.user);
        if (cache().fetch_page(key)) {
            BOOSTER_DEBUG("displayAll") << "Fetch cache for key=" << key;
            return;
        }

        data::PlaylistMapper playlistMapper(connectionString_);
        playlistMapper.loadAllPlaylists(allPlaylists);

        data::SongMapper songMapper(connectionString_);
        songMapper.loadUserProposedSongs(allPlaylists.user);

        data::PlaylistCommentMapper playlistCommentMapper(connectionString_);
        playlistCommentMapper.loadUserNbComments(allPlaylists.user);

        render("allPlaylists", allPlaylists);

        cache().add_trigger(std::to_string(allPlaylists.user.id));
        cache().store_page(key, CACHE_TTL_LONG);
        BOOSTER_DEBUG("displayAll") << "Store cache for key=" << key;
    }

    void Playlist::displayTop() {
        BOOSTER_DEBUG("displayTop");

        data::PlaylistPage playlist(page_);

        if (! checkAuth(playlist.user)) {
            response().make_error_response(response::forbidden);
            BOOSTER_WARNING("displayTop") << "Forbid user "
                << playlist.user.alias << " to access top playlist";
            return;
        }

        std::string key = getCacheKey(TOP_LIST_ID, playlist.user);
        if (cache().fetch_page(key)) {
            BOOSTER_DEBUG("displayTop") << "Fetch cache for key=" << key;
            return;
        }

        playlist.id = TOP_LIST_ID;
        playlist.name = TOP_LIST_NAME;
        playlist.image = TOP_LIST_IMAGE;
        playlist.autoGenerated = true;

        data::PlaylistMapper playlistMapper(connectionString_);
        playlistMapper.loadTopPlaylist(playlist, playlist.user, TOP_LIST_NB_SONGS, data::PlaylistMapper::OrderBy::DESC);

        data::PlaylistCommentMapper playlistCommentMapper(connectionString_);
        playlistCommentMapper.loadUserNbComments(playlist.user);

        doDisplay(playlist);

        cache().add_trigger(std::to_string(playlist.user.id));
        cache().store_page(key, CACHE_TTL_SHORT);
        BOOSTER_DEBUG("displayTop") << "Store cache for key=" << key;
    }

    void Playlist::displayWorst() {
        BOOSTER_DEBUG("displayWorst");

        data::PlaylistPage playlist(page_);

        if (! checkAuth(playlist.user)) {
            response().make_error_response(response::forbidden);
            BOOSTER_WARNING("displayWorst") << "Forbid user "
                << playlist.user.alias << " to access worst playlist";
            return;
        }

        std::string key = getCacheKey(WORST_LIST_ID, playlist.user);
        if (cache().fetch_page(key)) {
            BOOSTER_DEBUG("displayWorst") << "Fetch cache for key=" << key;
            return;
        }

        playlist.id = WORST_LIST_ID;
        playlist.name = WORST_LIST_NAME;
        playlist.image = WORST_LIST_IMAGE;
        playlist.autoGenerated = true;

        data::PlaylistMapper playlistMapper(connectionString_);
        playlistMapper.loadTopPlaylist(playlist, playlist.user, TOP_LIST_NB_SONGS, data::PlaylistMapper::OrderBy::ASC);

        data::PlaylistCommentMapper playlistCommentMapper(connectionString_);
        playlistCommentMapper.loadUserNbComments(playlist.user);

        doDisplay(playlist);

        cache().add_trigger(std::to_string(playlist.user.id));
        cache().store_page(key, CACHE_TTL_SHORT);
        BOOSTER_DEBUG("displayWorst") << "Store cache for key=" << key;
    }

    void Playlist::displayRandom() {
        BOOSTER_DEBUG("displayRandom");

        data::PlaylistPage playlist(page_);

        if (! checkAuth(playlist.user)) {
            response().make_error_response(response::forbidden);
            BOOSTER_WARNING("displayRandom") << "Forbid user "
                << playlist.user.alias << " to access random playlist";
            return;
        }

        std::string key = getCacheKey(RANDOM_LIST_ID, playlist.user);
        if (cache().fetch_page(key)) {
            BOOSTER_DEBUG("displayRandom") << "Fetch cache for key=" << key;
            return;
        }

        playlist.id = RANDOM_LIST_ID;
        playlist.name = RANDOM_LIST_NAME;
        playlist.image = RANDOM_LIST_IMAGE;
        playlist.autoGenerated = true;

        data::PlaylistMapper playlistMapper(connectionString_);
        playlistMapper.loadUserTopPlaylist(playlist, playlist.user, TOP_LIST_NB_SONGS, data::PlaylistMapper::OrderBy::RAND);

        data::PlaylistCommentMapper playlistCommentMapper(connectionString_);
        playlistCommentMapper.loadUserNbComments(playlist.user);

        doDisplay(playlist);

        cache().add_trigger(std::to_string(playlist.user.id));
        cache().store_page(key, CACHE_TTL_MEDIUM);
        BOOSTER_DEBUG("displayRandom") << "Store cache for key=" << key;
    }

    void Playlist::displayProposed() {
        BOOSTER_DEBUG("displayProposed");

        data::PlaylistPage playlist(page_);

        if (! checkAuth(playlist.user, data::User::ADMINISTRATOR)) {
            response().make_error_response(response::forbidden);
            BOOSTER_WARNING("displayProposed") << "Forbid user "
                << playlist.user.alias << " to access next playlist";
                return;
        }

        playlist.name = PROPOSED_LIST_NAME;
        playlist.autoGenerated = false;

        data::PlaylistMapper playlistMapper(connectionString_);
        playlistMapper.loadProposedPlaylist(playlist);

        doDisplay(playlist);
    }

    void Playlist::displayNew() {
        BOOSTER_DEBUG("displayNew");

        data::NewPlaylistPage newPlaylist(page_);

        if (! checkAuth(newPlaylist.user, data::User::ADMINISTRATOR)) {
            response().make_error_response(response::forbidden);
            BOOSTER_WARNING("displayNew") << "Forbid user "
                << newPlaylist.user.alias << " to access new playlist";
                return;
        }

        if (request().request_method() == "POST") {
            newPlaylist.input.load(context());
            if(! newPlaylist.input.validate()) {
                newPlaylist.alerts.errors.push_back("Invalid or missing fields!");
            } else {
                data::PlaylistItem playlist;
                playlist.name = newPlaylist.input.name.value();

                file* imageFile = newPlaylist.input.image.value().get();
                std::string uploadFileName = composeUploadDestPath(imageFile);

                playlist.image = composeImagePlaylistPath(imageFile);
                playlist.description = newPlaylist.input.description.value();

                data::PlaylistMapper playlistMapper(connectionString_);
                std::ostringstream msg;
                try {
                    imageFile->save_to(uploadFileName);
                    BOOSTER_INFO("displayNew") << "Uploaded file " << uploadFileName;

                    if (! playlistMapper.insert(playlist)) {
                        msg << "Could not create playlist " << playlist.name;
                        newPlaylist.alerts.errors.push_back(msg.str());
                        BOOSTER_ERROR("displayNew") << msg.str();
                    } else {
                        msg << "Successfully created playlist " << playlist.name;
                        newPlaylist.alerts.success.push_back(msg.str());
                        BOOSTER_INFO("displayNew") << msg.str();

                        newPlaylist.input.clear();
                    }
                } catch(const cppcms::cppcms_error& e) {
                    msg << "Could not upload file " << uploadFileName;
                    newPlaylist.alerts.errors.push_back(msg.str());
                    BOOSTER_ERROR("displayNew") << msg.str() << " - - " << e.trace();
                }
            }
        }

        newPlaylist.pageTitle = translate("New Playlist");

        render("newPlaylist", newPlaylist);
    }

    std::string Playlist::getCacheKey(const std::string& playlistId, const data::User& user) {
        return Master::getCacheKey(CACHE_PREFIX, playlistId, user);
    }

    void Playlist::doDisplay(data::PlaylistPage& playlist) {
        playlist.pageTitle = playlist.name;

        data::SongMapper songMapper(connectionString_);
        songMapper.loadUserProposedSongs(playlist.user);

        render("playlist", playlist);
    }

}   // namespace app

