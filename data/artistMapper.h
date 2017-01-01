#ifndef ARTIST_MAPPER_H
#define ARTIST_MAPPER_H


#include "dbMapper.h"
#include <data/artist.h>

namespace data {

    class ArtistMapper : public DbMapper {
    public:
        ArtistMapper(const std::string& connectionString);
        bool getByName(Artist& dest, const std::string& name);
        unsigned int insert(const Artist& artist);
        bool update(const Artist& artist);
    };


}   // namespace data
#endif  // ARTIST_MAPPER_H
