#ifndef DATA_PLAYLIST_COMMENT_H
#define DATA_PLAYLIST_COMMENT_H

#include <cppcms/view.h>


namespace data {

	struct PlaylistComment : public cppcms::base_content {
        unsigned int id;
        unsigned int parentId;
		std::string comment;
        std::string author;
        time_t creation;

        bool isParent() const {
            return parentId == 0;
        }
	};

}   // namespace data
#endif  // DATA_PLAYLIST_COMMENT_H
