#ifndef DATA_PLAYLIST_COMMENT_H
#define DATA_PLAYLIST_COMMENT_H

#include <cppcms/view.h>

#include <data/user.h>

#include <vector>

namespace data {

	struct PlaylistComment : public cppcms::base_content {
        unsigned int id;
        unsigned int parentId;
		std::string comment;
        User author;
        time_t creation;

        std::vector<PlaylistComment> replies;
	};

}   // namespace data
#endif  // DATA_PLAYLIST_COMMENT_H
