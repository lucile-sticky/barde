#ifndef DATA_MEDIA_H
#define DATA_MEDIA_H

#include <cppcms/view.h>
#include <cppcms/serialization.h>

#include <string>
#include <sstream>


namespace data {

    struct Media : public cppcms::base_content, cppcms::serializable {
        static constexpr const char* TYPE_AVATAR = "avatar";
        static constexpr const char* TYPE_PLAYLIST = "playlist";

        unsigned int id;
        std::string type;
        std::string label;
        std::string file;
        std::string contentType;
        unsigned long length;
        std::string md5sum;
        time_t creation;

        Media() {
            clear();
        }

        void clear() {
            id = 0;
            label = "";
            file = "";
        }

        void serialize(cppcms::archive &a) {
            a & id & type & label & file;
        }

        std::string toString() const {
            std::ostringstream oss;
            oss << "{ id: " << id << ", type: " << type << ", label: " << label
                << ", file: " << file << ", contentType: " << contentType
                << ", length: " << length << " }";
            return oss.str();
        }
    };

}   // namespace data


#endif  // DATA_MEDIA_H;
