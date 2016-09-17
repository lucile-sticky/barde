#include "songMapper.h"

#include <booster/log.h>


namespace data {

    SongMapper::SongMapper(const std::string& connectionString)
        : DbMapper(connectionString)
    {}

    bool SongMapper::loadSong(Song& dest, const std::string& songId) {
        bool success = false;

        std::string query = "SELECT s.id, s.title, "
            "a.name AS artist, s.file, s.url, s.show_video, s.position "
            "FROM song s "
            "INNER JOIN artist a ON a.id = s.artist_id "
            "WHERE s.id = ? ";

        BOOSTER_DEBUG("loadSong") << query << ", " << songId;

        cppdb::statement statement = connection() << query << songId;
        cppdb::result result = statement.row();

        if (! result.empty()) {
            dest.id = result.get<unsigned int>("id");
            dest.title = result.get<std::string>("title");
            dest.artist = result.get<std::string>("artist");
            dest.file = result.get<std::string>("file", "");
            dest.url = result.get<std::string>("url", "");
            dest.showVideo = result.get<unsigned short>("show_video", 0);
            dest.position = result.get<unsigned short>("position");

            success = true;
        }
        return success;
    }

    bool SongMapper::loadUserProposedSongs(User& dest) {
        bool success = false;

        dest.proposedSongs.clear();

        std::string query = "SELECT s.id AS song_id, s.title AS song_title, "
            "a.name AS artist_name, s.url, "
            "pl.id AS playlist_id, pl.name AS playlist_name, pl.enabled AS playlist_enabled "
            "FROM song s "
            "LEFT JOIN playlist pl ON pl.id = s.playlist_id "
            "INNER JOIN artist a ON a.id = s.artist_id "
            "WHERE s.proposer_id = ? "
            "ORDER BY s.id DESC";

        BOOSTER_DEBUG("loadUserProposedSongs") << query << ", " << dest.id;

        cppdb::result result = connection() << query << dest.id;

        while (result.next()) {
            User::ProposedSong song;
            unsigned short plEnabled;
            result >> song.id >> song.title >> song.artist >> song.url
                >> song.playlist.id >> song.playlist.name >> plEnabled;
            song.playlist.enabled = plEnabled;
            dest.proposedSongs.push_back(song);

            success = true;
        }
        return success;
    }

    bool SongMapper::loadPendingSongs(AdminPageSong& dest) {
        bool success = false;

        dest.pendingSongs.clear();

        std::string query = "SELECT s.id AS song_id, s.title AS song_title, "
            "a.name AS artist_name, s.file, s.url, u.alias "
            "FROM song s "
            "INNER JOIN artist a ON a.id = s.artist_id "
            "INNER JOIN user u ON u.id = s.proposer_id "
            "WHERE s.playlist_id = 0 "
            "ORDER BY s.id DESC";

        BOOSTER_DEBUG("loadProposedPlaylist") << query;

        cppdb::result result = connection() << query;

        while (result.next()) {
            data::Song song;
            result >> song.id >> song.title >> song.artist >> song.file >> song.url
                >> song.proposer;
            dest.pendingSongs.push_back(song);

            success = true;
        }
        return success;
    }

    bool SongMapper::insert(const User& user, unsigned int artistId, const Song& song) {
        std::string query = "INSERT INTO song "
            "(title, artist_id, url, playlist_id, position, proposer_id) "
            "VALUES (?, ?, ?, 0, 10000, ?) ";

        BOOSTER_DEBUG("insert") << query << ", " << song.title <<  ", "
            << artistId <<  ", " << song.url <<  ", " << user.id;

        cppdb::statement st = connection() << query << song.title << artistId << song.url << user.id << cppdb::exec;

        return st.affected() >= 1;
    }

    bool SongMapper::update(unsigned int artistId, const Song& song) {
        std::string query = "UPDATE song SET title = ?, artist_id = ?, "
            "file = ?, url = ?, show_video = ?, position = ? "
            "WHERE id = ? ";

        BOOSTER_DEBUG("update") << query << ", " << song.title <<  ", " << artistId
            <<  ", " << song.file << ", " << song.url <<  ", " << song.showVideo
            << ", " << song.position << ", " << song.id;

        cppdb::statement st = connection() << query << song.title << artistId << song.file
            << song.url << song.showVideo << song.position << song.id << cppdb::exec;

        return st.affected() >= 1;
    }

    bool SongMapper::setSongPlaylist(unsigned int songId, unsigned int playlistId) {
        std::string query = "UPDATE song SET playlist_id = ? WHERE id = ? ";

        BOOSTER_DEBUG("setSongPlaylist") << query << ", " << playlistId <<  ", "
            << songId;

        cppdb::statement st = connection() << query << playlistId << songId << cppdb::exec;

        return st.affected() >= 1;
    }

}   // namespace data
