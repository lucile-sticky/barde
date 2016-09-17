#ifndef ARTIST_MAPPER_H
#define ARTIST_MAPPER_H


#include "dbMapper.h"


namespace data {

    class ArtistMapper : public DbMapper {
    public:
        ArtistMapper(const std::string& connectionString);
        unsigned int insert(const std::string& name);
        unsigned int getByName(const std::string& name);
    };


}   // namespace data
#endif  // ARTIST_MAPPER_H
