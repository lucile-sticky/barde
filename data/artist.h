#ifndef DATA_ARTIST_H
#define DATA_ARTIST_H

#include <cppcms/view.h>
#include <string>

namespace data {

    struct Artist : public cppcms::base_content {
        unsigned int id;
        std::string name;

        Artist() : id(0) {}

        bool empty() const {
            return id == 0;
        }
    };

}   // namespace data


#endif  // DATA_ARTIST_H;
