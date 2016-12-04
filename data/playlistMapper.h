#ifndef PLAYLIST_MAPPER_H
#define PLAYLIST_MAPPER_H


#include "dbMapper.h"
#include <data/pages/playlist.h>
#include <data/user.h>

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
        bool loadPlaylist(PlaylistPage& dest, const std::string& playlistId, const User& user);
        bool loadTopPlaylist(PlaylistPage& dest, const User& user, unsigned short nbSongs, OrderBy orderBy);
        bool loadUserTopPlaylist(PlaylistPage& dest, const User& user, unsigned short nbSongs, OrderBy orderBy);
        bool loadProposedPlaylist(PlaylistPage& dest);
        bool loadAllPlaylists(AllPlaylistsPage& dest);  // Result is cached
        bool loadComingPlaylists(HtmlPage& dest);
        bool insert(const PlaylistItem& playlist);
    };


}   // namespace data
#endif  // PLAYLIST_MAPPER_H
