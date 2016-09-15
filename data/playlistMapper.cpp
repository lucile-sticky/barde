#include "playlistMapper.h"

#include <booster/log.h>

using namespace std::chrono;

namespace data {

    std::vector<PlaylistItem> PlaylistMapper::cachedPlaylistItems_ = {};
    time_point<system_clock> PlaylistMapper::cachedPlaylistItemsLastUpdated_ = system_clock::now();

    PlaylistMapper::PlaylistMapper(const std::string& connectionString)
        : DbMapper(connectionString)
    {}

    std::string PlaylistMapper::findCurrentPlaylistId() {
        std::string playlistId = "";

        std::string query = "SELECT id FROM playlist "
            "WHERE enabled = 1 "
            "AND publication <= CURDATE() "
            "ORDER BY publication DESC LIMIT 1 ";

        BOOSTER_DEBUG("findCurrentplaylistId") << query;

        cppdb::result result = sql() << query;

        if (result.next()) {
            result >> playlistId;
        }
        return playlistId;
    }

    bool PlaylistMapper::loadPlaylist(Playlist& dest, const std::string& playlistId, const User& user) {
        bool success = false;

        dest.songs.clear();

        std::string query = "SELECT p.id, p.name AS playlist_name, p.image, "
            "p.description, UNIX_TIMESTAMP(p.publication), "
            "IFNULL(vp.vote, 0) AS vote, "
            "s.id AS song_id, s.title AS song_title, a.name AS artist_name, s.file, s.url, "
            "s.show_video, IFNULL(vs.vote, 0) AS song_vote "
            "FROM playlist p "
            "LEFT JOIN song s ON s.playlist_id = p.id "
            "INNER JOIN artist a ON a.id = s.artist_id "
            "LEFT JOIN playlist_vote vp ON vp.playlist_id = p.id AND vp.user_id = ? "
            "LEFT JOIN song_vote vs ON vs.song_id = s.id AND vs.user_id = ? "
            "WHERE p.id = ? ";

        if (user.level < data::User::ADMINISTRATOR) {
            query += "AND p.enabled = 1 ";
        }

        query += "ORDER BY s.position ";

        BOOSTER_DEBUG("loadPlaylist") << query << ", " << user.id <<
            ", " << user.id << ", " << playlistId;

        cppdb::result result = sql() << query << user.id << user.id << playlistId;

        while (result.next()) {
            data::Song song;
            unsigned short showVideo;
            result >> dest.id >> dest.name >> dest.image >>
                dest.description >> dest.publication >> dest.vote.value >>
                song.id >> song.title >> song.artist >> song.file >> song.url >> showVideo >> song.vote.value;
            song.showVideo = showVideo;
            dest.songs.push_back(song);

            success = true;
        }
        return success;
    }

    bool PlaylistMapper::loadTopPlaylist(Playlist& dest, unsigned short nbSongs, OrderBy orderBy) {
        bool success = false;

        dest.songs.clear();

        std::string query="SELECT song_id, SUM(vote) AS total_votes, "
            "s.title AS song_title, a.name AS artist_name, s.file, s.url "
            "FROM ( "
                "SELECT vs.song_id, s.playlist_id, SUM(vs.vote) AS vote "
                "FROM song_vote vs "
                "LEFT JOIN song s ON s.id = vs.song_id "
                "GROUP BY vs.song_id, s.playlist_id "
            "UNION "
                "SELECT s.id, vp.playlist_id, SUM(vp.vote) / 2 "
                "FROM playlist_vote vp "
                "LEFT JOIN song s ON s.playlist_id = vp.playlist_id "
                "GROUP BY s.id, playlist_id "
            ") VOTES "
            "LEFT JOIN song s ON s.id = song_id "
            "LEFT JOIN artist a ON a.id = s.artist_id "
            "WHERE s.file IS NOT NULL "
            "GROUP BY song_id "
            "HAVING total_votes > 0 ";

        switch(orderBy) {
            case OrderBy::DESC:
                query += "ORDER BY total_votes DESC, song_id DESC ";
                break;
            case OrderBy::ASC:
                query += "ORDER BY total_votes ASC, song_id ASC ";
                break;
            default:
                query += "ORDER BY RAND() ";
                break;
        }

        query += "LIMIT ? ";

        BOOSTER_DEBUG("loadTopPlaylist") << query << nbSongs;

        cppdb::result result = sql() << query << nbSongs;

        while (result.next()) {
            data::Song song;
            result >> song.id >> song.vote.totalValues >> song.title >> song.artist
                >> song.file >> song.url;
            dest.songs.push_back(song);

            success = true;
        }
        return success;
    }

    bool PlaylistMapper::loadProposedPlaylist(Playlist& dest) {
        bool success = false;

        dest.songs.clear();

        std::string query = "SELECT s.id AS song_id, CONCAT(s.title, ' (', s.id, ')') AS song_title, "
            "a.name AS artist_name, s.file, s.url "
            "FROM song s "
            "LEFT JOIN playlist pl ON pl.id = s.playlist_id "
            "INNER JOIN artist a ON a.id = s.artist_id "
            "WHERE pl.id IS  NULL";

        BOOSTER_DEBUG("loadProposedPlaylist") << query;

        cppdb::result result = sql() << query;

        while (result.next()) {
            data::Song song;
            result >> song.id >> song.title >> song.artist >> song.file >> song.url;
            dest.songs.push_back(song);

            success = true;
        }
        return success;
    }

    bool PlaylistMapper::loadAllPlaylists(AllPlaylists& dest) {
        bool success = false;

        duration<double> elapsed_seconds = system_clock::now() - cachedPlaylistItemsLastUpdated_;
        if (cachedPlaylistItems_.empty() || elapsed_seconds.count() > CACHE_EXPIRATION) {

            cachedPlaylistItems_.clear();

            std::string query = "SELECT p.id, p.name, p.image, p.description, "
                "UNIX_TIMESTAMP(p.publication), COUNT(p.id) AS nb_songs "
                "FROM playlist p "
                "LEFT JOIN song s ON s.playlist_id = p.id "
                "WHERE p.enabled = 1 "
                "AND p.publication <= CURDATE() "
                "GROUP BY p.id "
                "ORDER BY p.publication DESC ";

            BOOSTER_DEBUG("loadAllPlaylists") << query;

            cppdb::result result = sql() << query;

            while (result.next()) {
                data::PlaylistItem item;
                result >> item.id >> item.name >> item.image >> item.description
                    >> item.publication >> item.nbSongs;
                cachedPlaylistItems_.push_back(item);

                success = true;
            }
            cachedPlaylistItemsLastUpdated_ = system_clock::now();
        }
        dest.playlists = cachedPlaylistItems_;

        return success;
    }

    bool PlaylistMapper::loadComingPlaylists(HtmlPage& dest) {
        bool success = false;

        dest.comingPlaylists.clear();

        std::string query = "SELECT p.id, p.name, COUNT(p.id) AS nb_songs "
            "FROM playlist p "
            "LEFT JOIN song s ON s.playlist_id = p.id "
            "WHERE p.enabled = 0 "
            "GROUP BY p.id "
            "ORDER BY publication ASC ";

        BOOSTER_DEBUG("loadComingPlaylists") << query;

        cppdb::result result = sql() << query;

        while (result.next()) {
            HtmlPage::ComingPlaylist playlist;
            result >> playlist.id >> playlist.name >> playlist.nbSongs;
            dest.comingPlaylists.push_back(playlist);

            success = true;
        }

        return success;
    }

    bool PlaylistMapper::insert(const Playlist& playlist) {
        std::string query = "INSERT INTO playlist "
            "(name, image, description, enabled) "
            "VALUES (?, ?, ?, 0) ";

        BOOSTER_DEBUG("insert") << query << ", " << playlist.name << ", " << playlist.image
            << ", " << playlist.description;

        cppdb::statement st = sql() << query <<  playlist.name << playlist.image
            << playlist.description
            << cppdb::exec;

        return st.affected() >= 1;
    }

}   // namespace data
