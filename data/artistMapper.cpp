#include "artistMapper.h"

#include <booster/log.h>


namespace data {

    ArtistMapper::ArtistMapper(const std::string& connectionString)
        : DbMapper(connectionString)
    {}

    unsigned int ArtistMapper::insert(const std::string& name) {
        std::string query = "INSERT INTO artist (name) VALUES (?)";

        BOOSTER_DEBUG("insert") << query << ", " << name;

        cppdb::statement statement = sql() << query << name << cppdb::exec;

        if(statement.affected() >= 1) {
            return statement.last_insert_id();
        }
        return 0;
    }

    unsigned int ArtistMapper::getByName(const std::string& name) {
        unsigned int artistId = 0;

        std::string query = "SELECT id FROM artist WHERE LCASE(name) = LCASE(?) "
            "ORDER BY id DESC LIMIT 1 ";

        BOOSTER_DEBUG("getByName") << query << ", " << name;

        cppdb::statement statement = sql() << query << name;
        cppdb::result result = statement.row();

        if(! result.empty()) {
            artistId = result.get<unsigned int>(0);
        }
        return artistId;
    }

}   // namespace data
