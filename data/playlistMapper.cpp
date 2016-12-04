#include "playlistMapper.h"

#include <booster/log.h>

using namespace std::chrono;

namespace data {

    const std::string PlaylistMapper::SQL_FROM_UNION_VOTES = "FROM ( "
                "SELECT sv.song_id, s.playlist_id, SUM(sv.vote) AS vote "
                "FROM song_vote sv "
                "LEFT JOIN song s ON s.id = sv.song_id "
                "GROUP BY sv.song_id, s.playlist_id "
            "UNION "
                "SELECT s.id, pv.playlist_id, SUM(pv.vote) / 2 "
                "FROM playlist_vote pv "
                "LEFT JOIN song s ON s.playlist_id = pv.playlist_id "
                "GROUP BY s.id, playlist_id "
            ") votes ";

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

        cppdb::result result = connection() << query;

        if (result.next()) {
            result >> playlistId;
        }
        return playlistId;
    }

    bool PlaylistMapper::loadPlaylist(PlaylistPage& dest, const std::string& playlistId, const User& user) {
        bool success = false;

        dest.songs.clear();

        std::string query = "SELECT p.id, p.name, p.image, "
            "p.description, UNIX_TIMESTAMP(p.publication) AS publication, "
            "vp.vote AS vote, "
            "s.id AS song_id, s.title AS song_title, a.name AS artist_name, s.file, s.url, "
            "s.duration, s.show_video, u.alias AS proposer, vs.vote AS song_vote "
            "FROM playlist p "
            "LEFT JOIN song s ON s.playlist_id = p.id "
            "INNER JOIN artist a ON a.id = s.artist_id "
            "LEFT JOIN user u ON u.id = s.proposer_id AND u.privacy = 'public' "
            "LEFT JOIN playlist_vote vp ON vp.playlist_id = p.id AND vp.user_id = ? "
            "LEFT JOIN song_vote vs ON vs.song_id = s.id AND vs.user_id = ? "
            "WHERE p.id = ? ";

        if (user.level < data::User::ADMINISTRATOR) {
            query += "AND p.enabled = 1 ";
        }

        query += "ORDER BY s.position ";

        BOOSTER_DEBUG("loadPlaylist") << query << ", " << user.id <<
            ", " << user.id << ", " << playlistId;

        cppdb::result result = connection() << query << user.id << user.id << playlistId;

        while (result.next()) {
            dest.id = result.get<std::string>("id");
            dest.name = result.get<std::string>("name");
            dest.image = result.get<std::string>("image", "");
            dest.description = result.get<std::string>("description", "");
            dest.publication = result.get<time_t>("publication");
            dest.vote.value = result.get<short>("vote", 0);

            unsigned int songId = result.get<unsigned int>("song_id", 0);
            if (songId != 0) {
                data::Song song;

                song.id = songId;
                song.title = result.get<std::string>("song_title", "");
                song.artist = result.get<std::string>("artist_name", "");
                song.file = result.get<std::string>("file", "");
                song.url = result.get<std::string>("url", "");
                song.duration = result.get<unsigned int>("duration", 0);
                song.showVideo = result.get<unsigned short>("show_video", 0);
                song.proposer = result.get<std::string>("proposer", User::ANONYMOUS_ALIAS);
                song.vote.value = result.get<short>("song_vote", 0);

                dest.songs.push_back(song);
            }
            success = true;
        }
        return success;
    }

    bool PlaylistMapper::loadTopPlaylist(PlaylistPage& dest, const User& user, unsigned short nbSongs, OrderBy orderBy) {
        bool success = false;

        dest.songs.clear();

        std::string query="SELECT votes.song_id, SUM(votes.vote) AS total_votes, "
            "s.title AS song_title, a.name AS artist_name, s.file, s.url, "
            "s.duration, s.show_video, u.alias AS proposer, sv.vote AS song_vote "
            + SQL_FROM_UNION_VOTES +
            "INNER JOIN song s ON s.id = votes.song_id "
            "INNER JOIN artist a ON a.id = s.artist_id "
            "LEFT JOIN user u ON u.id = s.proposer_id AND u.privacy = 'public' "
            "LEFT JOIN song_vote sv ON sv.song_id = s.id AND sv.user_id = ? "
            "WHERE s.file IS NOT NULL "
            "GROUP BY song_id ";

        switch(orderBy) {
            case OrderBy::DESC:
                query += "ORDER BY total_votes DESC, song_id DESC ";
                break;
            case OrderBy::ASC:
                query += "ORDER BY total_votes ASC, song_id ASC ";
                break;
            default:
                query += "HAVING total_votes > 0 "; // Increase random playlist quality
                query += "ORDER BY RAND() ";
                break;
        }

        query += "LIMIT ? ";

        BOOSTER_DEBUG("loadTopPlaylist") << query << ", " << user.id << ", " << nbSongs;

        cppdb::result result = connection() << query << user.id << nbSongs;

        while (result.next()) {
            data::Song song;
            song.id = result.get<unsigned int>("song_id");
            song.vote.totalValues = result.get<float>("total_votes");
            song.title = result.get<std::string>("song_title", "");
            song.artist = result.get<std::string>("artist_name", "");
            song.file = result.get<std::string>("file", "");
            song.url = result.get<std::string>("url", "");
            song.duration = result.get<unsigned int>("duration", 0);
            song.showVideo = result.get<unsigned short>("show_video", 0);
            song.proposer = result.get<std::string>("proposer", User::ANONYMOUS_ALIAS);
            song.vote.value = result.get<short>("song_vote", 0);
            dest.songs.push_back(song);

            success = true;
        }
        return success;
    }

    bool PlaylistMapper::loadUserTopPlaylist(PlaylistPage& dest, const User& user,  unsigned short nbSongs, OrderBy orderBy) {
        bool success = false;

        dest.songs.clear();

        std::string query="SELECT votes.song_id, SUM(votes.vote) AS total_votes, "
            "s.title AS song_title, a.name AS artist_name, s.file, s.url, "
            "s.duration, s.show_video, u.alias AS proposer, "
            "sv.vote AS song_vote "
            + SQL_FROM_UNION_VOTES +
            "INNER JOIN song s ON s.id = song_id "
            "INNER JOIN artist a ON a.id = s.artist_id "
            "LEFT JOIN user u ON u.id = s.proposer_id AND u.privacy = 'public' "
            "LEFT JOIN song_vote sv ON sv.song_id = s.id AND sv.user_id = ? "
            "WHERE s.file IS NOT NULL "
            "AND (sv.vote IS NULL OR sv.vote >= 0) "
            "GROUP BY song_id ";

        switch(orderBy) {
            case OrderBy::DESC:
                query += "ORDER BY song_vote DESC, total_votes DESC, song_id DESC ";
                break;
            case OrderBy::ASC:
                query += "ORDER BY song_vote, total_votes ASC, song_id ASC ";
                break;
            default:
                query += "HAVING total_votes > 0 "; // Increase random playlist quality
                query += "ORDER BY song_vote DESC, RAND() ";
                break;
        }

        query += "LIMIT ? ";

        BOOSTER_DEBUG("loadUserTopPlaylist") << query << ", " << user.id << ", " << nbSongs;

        cppdb::result result = connection() << query << user.id << nbSongs;

        while (result.next()) {
            data::Song song;
            song.id = result.get<unsigned int>("song_id");
            song.vote.totalValues = result.get<float>("total_votes");
            song.title = result.get<std::string>("song_title", "");
            song.artist = result.get<std::string>("artist_name", "");
            song.file = result.get<std::string>("file", "");
            song.url = result.get<std::string>("url", "");
            song.duration = result.get<unsigned int>("duration", 0);
            song.showVideo = result.get<unsigned short>("show_video", 0);
            song.proposer = result.get<std::string>("proposer", User::ANONYMOUS_ALIAS);
            song.vote.value = result.get<short>("song_vote", 0);
            dest.songs.push_back(song);

            success = true;
        }
        return success;
    }

    bool PlaylistMapper::loadProposedPlaylist(PlaylistPage& dest) {
        bool success = false;

        dest.songs.clear();

        std::string query = "SELECT s.id AS song_id, CONCAT(s.title, ' (', s.id, ')') AS song_title, "
            "a.name AS artist_name, s.file, s.url "
            "FROM song s "
            "LEFT JOIN playlist pl ON pl.id = s.playlist_id "
            "INNER JOIN artist a ON a.id = s.artist_id "
            "WHERE pl.id IS  NULL";

        BOOSTER_DEBUG("loadProposedPlaylist") << query;

        cppdb::result result = connection() << query;

        while (result.next()) {
            data::Song song;
            result >> song.id >> song.title >> song.artist >> song.file >> song.url;
            dest.songs.push_back(song);

            success = true;
        }
        return success;
    }

    bool PlaylistMapper::loadAllPlaylists(AllPlaylistsPage& dest) {
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

            cppdb::result result = connection() << query;

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

        cppdb::result result = connection() << query;

        while (result.next()) {
            HtmlPage::ComingPlaylist playlist;
            result >> playlist.id >> playlist.name >> playlist.nbSongs;
            dest.comingPlaylists.push_back(playlist);

            success = true;
        }

        return success;
    }

    bool PlaylistMapper::insert(const PlaylistItem& playlist) {
        std::string query = "INSERT INTO playlist "
            "(name, image, description, enabled) "
            "VALUES (?, ?, ?, 0) ";

        BOOSTER_DEBUG("insert") << query << ", " << playlist.name << ", " << playlist.image
            << ", " << playlist.description;

        cppdb::statement st = connection() << query <<  playlist.name << playlist.image
            << playlist.description
            << cppdb::exec;

        return st.affected() >= 1;
    }

}   // namespace data
