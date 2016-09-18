#ifndef PLAYLIST_MAPPER_H
#define PLAYLIST_MAPPER_H


#include "dbMapper.h"
#include "htmlPage.h"
#include "playlist.h"
#include "user.h"

#include <chrono>
#include <vector>

namespace data {

    class PlaylistMapper : public DbMapper {
        static const unsigned int CACHE_EXPIRATION = 60;

        static std::vector<PlaylistItem> cachedPlaylistItems_;
        static std::chrono::time_point<std::chrono::system_clock> cachedPlaylistItemsLastUpdated_;

    public:
        enum class OrderBy { DESC, ASC, RAND };

        PlaylistMapper(const std::string& connectionString);

        std::string findCurrentPlaylistId();
        bool loadPlaylist(Playlist& dest, const std::string& playlistId, const User& user);
        bool loadTopPlaylist(Playlist& dest, unsigned short nbSongs, OrderBy orderBy);
        bool loadUserTopPlaylist(Playlist& dest, const User& user, unsigned short nbSongs, OrderBy orderBy);
        bool loadProposedPlaylist(Playlist& dest);
        bool loadAllPlaylists(AllPlaylists& dest);  // Result is cached
        bool loadComingPlaylists(data::HtmlPage& dest);
        bool insert(const Playlist& playlist);
    };


}   // namespace data
#endif  // PLAYLIST_MAPPER_H
