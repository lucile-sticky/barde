#include "artistMapper.h"

#include <booster/log.h>


namespace data {

    ArtistMapper::ArtistMapper(const std::string& connectionString)
        : DbMapper(connectionString)
    {}

    bool ArtistMapper::getByName(Artist& dest, const std::string& name) {
        std::string query = "SELECT id, name FROM artist WHERE LCASE(name) = LCASE(?) "
            "ORDER BY id DESC LIMIT 1 ";

        BOOSTER_DEBUG("getByName") << query << ", " << name;

        cppdb::statement statement = connection() << query << name;
        cppdb::result result = statement.row();

        if(result.empty()) {
            return false;
        }

        dest.id = result.get<unsigned int>("id");
        dest.name = result.get<std::string>("name");
        return true;
    }

    unsigned int ArtistMapper::insert(const Artist& artist) {
        std::string query = "INSERT INTO artist (name) VALUES (?)";

        BOOSTER_DEBUG("insert") << query << ", " << artist.name;

        cppdb::statement statement = connection() << query << artist.name << cppdb::exec;

        if(statement.affected() >= 1) {
            return statement.last_insert_id();
        }
        return 0;
    }

    bool ArtistMapper::update(const Artist& artist) {
        std::string query = "UPDATE artist SET name = ? WHERE id = ?";

        BOOSTER_DEBUG("update") << query << ", " << artist.name << ", " << artist.id;

        cppdb::statement statement = connection() << query << artist.name << artist.id << cppdb::exec;

        return statement.affected() >= 1;
    }

}   // namespace data
