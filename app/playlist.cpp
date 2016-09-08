#include "playlist.h"

#include <cppcms/http_file.h>
#include <cppcms/url_dispatcher.h>
#include <booster/log.h>

#include <app/util/stringMethods.h>

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
        app::Master(s),
        dbPlaylistMapper_(new data::PlaylistMapper(connectionString_)),
        dbSongMapper_(new data::SongMapper(connectionString_))
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

        std::string playlistId = dbPlaylistMapper_->findCurrentPlaylistId();
        dbPlaylistMapper_->clear();

        if (playlistId.empty()) {
            playlistId = TOP_LIST_ID;
            BOOSTER_WARNING("displayCurrent") << "No current playlist, falling back on " << playlistId;
        }
        redirectTo("/playlist/" + playlistId);
    }

    void Playlist::display(std::string playlistId) {
        BOOSTER_DEBUG("display") << "playlist ID: " << playlistId;

        if (! checkAuth()) {
            response().make_error_response(response::forbidden);
            BOOSTER_WARNING("display") << "Forbid user "
                << page_.user.alias << " to access playlist ID: "
                << playlistId;
            return;
        }

        std::string key = getCacheKey(playlistId, page_.user);
        if (cache().fetch_page(key)) {
            BOOSTER_DEBUG("display") << "Fetch cache for key=" << key;
            return;
        }

        playlist_.votesEnabled = true;

        dbPlaylistMapper_->loadPlaylist(playlist_, playlistId, page_.user);
        dbPlaylistMapper_->clear();

        dbSongMapper_->loadUserProposedSongs(page_.user);
        dbSongMapper_->clear();

        doDisplay();

        cache().store_page(key, CACHE_TTL_MEDIUM);
        BOOSTER_DEBUG("display") << "Store cache for key=" << key;
    }

    void Playlist::displayAll() {
        BOOSTER_DEBUG("displayAll");

        if (! checkAuth()) {
            response().make_error_response(response::forbidden);
            BOOSTER_WARNING("displayAll") << "Forbid user "
                << page_.user.alias << " to access all playlists";
            return;
        }

        std::string key = "ALL";
        if (cache().fetch_page(key)) {
            BOOSTER_DEBUG("displayAll") << "Fetch cache for key=" << key;
            return;
        }

        data::AllPlaylists allPlaylists;
        allPlaylists.resetFrom(page_);

        dbPlaylistMapper_->loadAllPlaylists(allPlaylists);
        dbPlaylistMapper_->clear();

        render("allPlaylists", allPlaylists);

        cache().store_page(key, CACHE_TTL_MEDIUM);
        BOOSTER_DEBUG("displayAll") << "Store cache for key=" << key;
    }

    void Playlist::displayTop() {
        BOOSTER_DEBUG("displayTop");

        if (! checkAuth()) {
            response().make_error_response(response::forbidden);
            BOOSTER_WARNING("displayTop") << "Forbid user "
                << page_.user.alias << " to access top playlist";
            return;
        }

        playlist_.id = TOP_LIST_ID;
        playlist_.name = TOP_LIST_NAME;
        playlist_.image = TOP_LIST_IMAGE;
        playlist_.player = data::Playlist::TYPE_SIMPLE;
        playlist_.votesEnabled = false;

        std::string key = TOP_LIST_ID;
        if (cache().fetch_page(key)) {
            BOOSTER_DEBUG("displayTop") << "Fetch cache for key=" << key;
            return;
        }

        dbPlaylistMapper_->loadTopPlaylist(playlist_, TOP_LIST_NB_SONGS, data::PlaylistMapper::OrderBy::DESC);
        dbPlaylistMapper_->clear();

        doDisplay();

        cache().store_page(key, CACHE_TTL_SHORT);
        BOOSTER_DEBUG("displayTop") << "Store cache for key=" << key;
    }

    void Playlist::displayWorst() {
        BOOSTER_DEBUG("displayWorst");

        if (! checkAuth()) {
            response().make_error_response(response::forbidden);
            BOOSTER_WARNING("displayWorst") << "Forbid user "
                << page_.user.alias << " to access worst playlist";
            return;
        }

        playlist_.id = WORST_LIST_ID;
        playlist_.name = WORST_LIST_NAME;
        playlist_.image = WORST_LIST_IMAGE;
        playlist_.player = data::Playlist::TYPE_SIMPLE;
        playlist_.votesEnabled = false;

        std::string key = WORST_LIST_ID;
        if (cache().fetch_page(key)) {
            BOOSTER_DEBUG("displayWorst") << "Fetch cache for key=" << key;
            return;
        }

        dbPlaylistMapper_->loadTopPlaylist(playlist_, TOP_LIST_NB_SONGS, data::PlaylistMapper::OrderBy::ASC);
        dbPlaylistMapper_->clear();

        doDisplay();

        cache().store_page(key, CACHE_TTL_SHORT);
        BOOSTER_DEBUG("displayWorst") << "Store cache for key=" << key;
    }

    void Playlist::displayRandom() {
        BOOSTER_DEBUG("displayRandom");

        if (! checkAuth()) {
            response().make_error_response(response::forbidden);
            BOOSTER_WARNING("displayRandom") << "Forbid user "
                << page_.user.alias << " to access random playlist";
            return;
        }

        playlist_.id = RANDOM_LIST_ID;
        playlist_.name = RANDOM_LIST_NAME;
        playlist_.image = RANDOM_LIST_IMAGE;
        playlist_.player = data::Playlist::TYPE_SIMPLE;
        playlist_.votesEnabled = false;

        std::string key = getCacheKey(playlist_.id, page_.user);
        if (cache().fetch_page(key)) {
            BOOSTER_DEBUG("displayRandom") << "Fetch cache for key=" << key;
            return;
        }

        dbPlaylistMapper_->loadTopPlaylist(playlist_, TOP_LIST_NB_SONGS, data::PlaylistMapper::OrderBy::RAND);
        dbPlaylistMapper_->clear();

        doDisplay();

        cache().store_page(key, CACHE_TTL_MEDIUM);
        BOOSTER_DEBUG("displayRandom") << "Store cache for key=" << key;
    }

    void Playlist::displayProposed() {
        BOOSTER_DEBUG("displayProposed");

        if (! checkAuth(data::User::ADMINISTRATOR)) {
            response().make_error_response(response::forbidden);
            BOOSTER_WARNING("displayProposed") << "Forbid user "
                << page_.user.alias << " to access next playlist";
                return;
        }

        dbPlaylistMapper_->loadProposedPlaylist(playlist_);
        dbPlaylistMapper_->clear();

        playlist_.name = PROPOSED_LIST_NAME;
        playlist_.player = data::Playlist::TYPE_JWPLAYER;
        playlist_.votesEnabled = false;

        doDisplay();
    }

    void Playlist::displayNew() {
        BOOSTER_DEBUG("displayNew");

        if (! checkAuth(data::User::ADMINISTRATOR)) {
            response().make_error_response(response::forbidden);
            BOOSTER_WARNING("displayNew") << "Forbid user "
                << page_.user.alias << " to access new playlist";
                return;
        }

        data::PagePlaylist pagePlaylist;
        if (request().request_method() == "POST") {
            pagePlaylist.input.load(context());
            if(! pagePlaylist.input.validate()) {
                pagePlaylist.alerts.errors.push_back("Invalid or missing fields!");
            } else {
                data::Playlist playlist;
                playlist.name = pagePlaylist.input.name.value();

                file* imageFile = pagePlaylist.input.image.value().get();
                std::string uploadFileName = composeUploadDestPath(imageFile);

                playlist.image = composeImagePlaylistPath(imageFile);
                playlist.description = pagePlaylist.input.description.value();

                std::ostringstream msg;
                try {
                    imageFile->save_to(uploadFileName);
                    BOOSTER_INFO("displayNew") << "Uploaded file " << uploadFileName;

                    if (! dbPlaylistMapper_->insert(playlist)) {
                        msg << "Could not create playlist " << playlist_.name;
                        pagePlaylist.alerts.errors.push_back(msg.str());
                        BOOSTER_ERROR("displayNew") << msg.str();
                    } else {
                        msg << "Successfully created playlist " << playlist.name;
                        pagePlaylist.alerts.success.push_back(msg.str());
                        BOOSTER_INFO("displayNew") << msg.str();

                        pagePlaylist.input.clear();
                    }
                } catch(const cppcms::cppcms_error& e) {
                    msg << "Could not upload file " << uploadFileName;
                    pagePlaylist.alerts.errors.push_back(msg.str());
                    BOOSTER_ERROR("displayNew") << msg.str() << " - - " << e.trace();
                }
            }
        }

        pagePlaylist.resetFrom(page_);
        pagePlaylist.pageTitle = translate("New Playlist");

        render("newPlaylist", pagePlaylist);
    }

    std::string Playlist::getCacheKey(const std::string& playlistId, const data::User& user) {
        return Master::getCacheKey(CACHE_PREFIX, playlistId, user);
    }

    void Playlist::doDisplay() {
        playlist_.resetFrom(page_);
        playlist_.pageTitle = playlist_.name;

        if (playlist_.player == data::Playlist::TYPE_JWPLAYER) {
            render("playlistJwp", playlist_);
        } else {
            render("playlistHtml5", playlist_);
        }
    }

}   // namespace app

