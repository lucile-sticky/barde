#include "mediaMapper.h"

#include <booster/log.h>


namespace data {

    MediaMapper::MediaMapper(const std::string& connectionString)
        : DbMapper(connectionString)
    {}

    bool MediaMapper::get(Media& dest, unsigned int id) {
        dest.clear();

        std::string query = "SELECT id, type, label, file FROM media "
            "WHERE id = ? ";

        BOOSTER_DEBUG(__func__) << query << ", " << id;

        cppdb::statement statement = connection() << query << id;
        cppdb::result result = statement.row();

        if(result.empty()) {
            return false;
        }

        dest.id = result.get<unsigned int>("id");
        dest.type = result.get<std::string>("type");
        dest.label = result.get<std::string>("label");
        dest.file = result.get<std::string>("file");

        return true;
    }

    unsigned int MediaMapper::insert(unsigned int userId, const Media& media) {
        std::string query = "INSERT INTO media "
            "(type, label, file, content_type, length, creator_id, creation, enabled) "
            "VALUES (?, ?, ?, ?, ?, ?, NOW(), 1)";

        BOOSTER_DEBUG(__func__) << query << ", " << media.type << ", " << media.label
            << ", " << media.file << ", " << media.contentType << ", " << media.length
            << ", " << userId;

        cppdb::statement statement = connection() << query << media.type << media.label
            << media.file << media.contentType << media.length << userId
            << cppdb::exec;

        if(statement.affected() >= 1) {
            return statement.last_insert_id();
        }
        return 0;
    }

    bool MediaMapper::disable(const Media& media) {
        std::string query = "UPDATE media SET enabled = 0 WHERE id = ?";

        BOOSTER_DEBUG(__func__) << query << ", " << media.id;

        cppdb::statement statement = connection() << query << media.id
            << cppdb::exec;

        return statement.affected() >= 1;
    }

}   // namespace data
