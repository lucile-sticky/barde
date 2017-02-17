#ifndef MEDIA_MAPPER_H
#define MEDIA_MAPPER_H


#include "dbMapper.h"
#include <data/media.h>

namespace data {

    class MediaMapper : public DbMapper {
    public:
        MediaMapper(const std::string& connectionString);

        bool get(Media& dest, unsigned int id);
        unsigned int insert(unsigned int userId, const Media& media);
        bool disable(const Media& media);
    };


}   // namespace data
#endif  // MEDIA_MAPPER_H
