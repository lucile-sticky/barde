#include "song.h"

#include <cppcms/url_dispatcher.h>
#include <cppcms/json.h>
#include <booster/log.h>

#include <app/playlist.h>
#include <app/validator/songValidator.h>
#include <app/util/stringMethods.h>

#include <data/artistMapper.h>
#include <data/playlistMapper.h>
#include <data/songMapper.h>

#include <sstream>


using namespace cppcms::http;

namespace app {
    Song::Song(cppcms::service& s) :
        app::Master(s)
    {
        dispatcher().assign("/edit/(\\d+)", &Song::displayEdit, this, 1);
        mapper().assign("/{1}");

        dispatcher().assign("/proposed", &Song::displayProposed, this);
        mapper().assign("");

        dispatcher().assign("/ajax-new", &Song::ajaxNew, this);
        mapper().assign("");

        dispatcher().assign("/ajax-set-playlist/(\\d+)/(\\d+)", &Song::ajaxSetPlaylist, this, 1, 2);
        mapper().assign("/{1}/{2}");

        dispatcher().assign("/ajax-set-duration/(\\d+)/(\\d+)", &Song::ajaxSetDuration, this, 1, 2);
        mapper().assign("/{1}/{2}");
    }

    void Song::displayProposed() {
        BOOSTER_DEBUG(__func__);

        data::SongAdminPage pageSong(page_);

        if (! checkAuth(pageSong.user, data::User::ADMINISTRATOR)) {
            forbidAccess(pageSong.user);
            return;
        }

        pageSong.pageTitle = "Proposed songs";

        data::SongMapper songMapper(connectionString_);
        data::PlaylistMapper playlistMapper(connectionString_);

        songMapper.loadPendingSongs(pageSong);
        playlistMapper.loadComingPlaylists(pageSong);

        render("proposedSongs", pageSong);
    }

    void Song::displayEdit(std::string songId) {
        BOOSTER_DEBUG(__func__);

        data::EditSongPage pageSong(page_);

        if (! checkAuth(pageSong.user, data::User::ADMINISTRATOR)) {
            forbidAccess(pageSong.user);
            return;
        }

        data::SongMapper songMapper(connectionString_);
        data::Song song;

    if (request().request_method() == "POST") {
            pageSong.input.load(context());
            if(! pageSong.input.validate()) {
                pageSong.alerts.errors.push_back("Invalid or missing fields!");
            } else {
                song.id = std::stoi(songId);
                song.title = pageSong.input.title.value();
                song.artist.name = pageSong.input.artist.value();

                file* songFile = 0;
                if (pageSong.input.newFile.set()) {
                    songFile = pageSong.input.newFile.value().get();
                    song.file = toUploadRelativePath(songFile, song.artist.name + '-' + song.title);
                } else {
                    song.file = pageSong.input.file.value();
                }

                song.url = pageSong.input.url.value();
                song.showVideo = pageSong.input.showVideo.value();
                song.position = pageSong.input.position.value();

                std::ostringstream msg;
                try {
                    if (songFile) {
                        std::string songFullPath = toFullPath(song.file);
                        songFile->save_to(songFullPath);
                        BOOSTER_INFO(__func__) << "Uploaded file " << songFullPath;
                    }

                    if (! update(song)) {
                        msg << "Could not modifiy \"" << song.title << "\".";
                        pageSong.alerts.errors.push_back(msg.str());
                        BOOSTER_ERROR(__func__) << msg.str();
                    } else {
                        msg << "Successfully modified \"" << song.title << "\".";
                        pageSong.alerts.success.push_back(msg.str());
                        BOOSTER_INFO(__func__) << msg.str();
                    }
                } catch(const cppcms::cppcms_error& e) {
                    msg << "Could not upload file " << song.file;
                    pageSong.alerts.errors.push_back(msg.str());
                    BOOSTER_ERROR(__func__) << msg.str() << " - - " << e.trace();
                }
            }
        } else if (!songId.empty() && songMapper.loadSong(song, songId)) {
            BOOSTER_DEBUG(__func__) << "Loading fields for song " << songId;
            pageSong.input.artist.value(song.artist.name);
            pageSong.input.title.value(song.title);
            pageSong.input.file.value(song.file);
            pageSong.input.url.value(song.url);
            pageSong.input.showVideo.value(song.showVideo);
            pageSong.input.position.value(song.position);
        }

        pageSong.pageTitle = "Modify a song";
        pageSong.song = song;

        render("editSong", pageSong);
    }

    void Song::ajaxNew() {
        data::User user;

        if (! checkAuth(user, data::User::CITIZEN)) {
            response().make_error_response(response::forbidden);
            BOOSTER_WARNING(__func__) << "Forbid user "
                << user.alias << " to add new song";
            return;
        }

        BOOSTER_DEBUG(__func__);

        data::Song song;
        song.title = request().post("title");
        song.artist.name = request().post("artist");
        song.url = request().post("url");

        validator::SongValidator validator(song);
        bool success = validator.validate();
        std::string message = validator.lastMessage();

        if (success) {
            success = insert(user, song);
            std::ostringstream oss;
            if (success) {
                oss << "Successfully proposed \"" << song.title << "\". Thanks for your contribution to the playlist ^_^";
            } else {
                oss << "Could not submit the song, please retry later!";
            }
            message = oss.str();
        }

        if (success) {
            BOOSTER_INFO(__func__) << message;
        } else {
            BOOSTER_ERROR(__func__) << message;
        }

        cppcms::json::value jsonOutput;
        jsonOutput["success"] = success;
        jsonOutput["message"] = message;
        response().out() << jsonOutput;
    }

    void Song::ajaxSetPlaylist(std::string songId, std::string playlistId) {
        data::User user;

        if (! checkAuth(user, data::User::ADMINISTRATOR)) {
            response().make_error_response(response::forbidden);
            BOOSTER_WARNING(__func__) << "Forbid user "
                << user.alias << " to set song playlist";
            return;
        }

        BOOSTER_DEBUG(__func__);

        data::SongMapper songMapper(connectionString_);

        bool result = songMapper.updatePlaylistId(
            std::stoi(songId),
            std::stoi(playlistId)
        );

        cache().rise(std::to_string(user.id));
        BOOSTER_DEBUG(__func__) << "Clean caches for user ID " << user.id;

        cppcms::json::value jsonOutput;
        jsonOutput["success"] = result;
        response().out() << jsonOutput;
    }

    void Song::ajaxSetDuration(std::string songId, std::string duration) {
        data::User user;

        if (! checkAuth(user, data::User::GUEST)) {
            response().make_error_response(response::forbidden);
            BOOSTER_WARNING(__func__) << "Forbid user "
                << user.alias << " to set song playlist";
            return;
        }

        BOOSTER_DEBUG(__func__);

        data::SongMapper songMapper(connectionString_);

        bool result = songMapper.updateDuration(
            std::stoi(songId),
            std::stoi(duration)
        );

        cppcms::json::value jsonOutput;
        jsonOutput["success"] = result;
        response().out() << jsonOutput;
    }

    bool Song::insert(const data::User& proposer, const data::Song& song) {
        bool success = false;

        data::ArtistMapper artistMapper(connectionString_);
        data::SongMapper songMapper(connectionString_);

        data::Song song2 = song;    // modifiable Song
        if (!artistMapper.getByName(song2.artist, song.artist.name)) {
            song2.artist.id = artistMapper.insert(song2.artist);
        }

        if (song2.artist.empty()) {
            BOOSTER_DEBUG(__func__) << "Could not create artist " << song.artist.name;
        } else {
            success = songMapper.insert(proposer, song2);
        }

        return success;
    }

    bool Song::update(const data::Song& song) {
        data::ArtistMapper artistMapper(connectionString_);
        data::SongMapper songMapper(connectionString_);

        data::Song song2 = song;    // modifiable Song
        song2.artist.id = loadSongArtistId(song.id);
        if (song2.artist.empty()) {
            BOOSTER_DEBUG(__func__) << "Missing artist for song " << song.id;
        } else {
            artistMapper.update(song2.artist);
            songMapper.update(song2);
            return true;
        }
        return false;
    }

    unsigned int Song::loadSongArtistId(unsigned int songId) {
        data::Song song;
        data::SongMapper songMapper(connectionString_);

        if (songMapper.loadSong(song, std::to_string(songId))) {
            return song.artist.id;
        }
        return 0;
    }

}   // namespace app

