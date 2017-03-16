#ifndef APP_VALIDATOR_SONG_H
#define APP_VALIDATOR_SONG_H

#include "abstractValidator.h"

#include <data/song.h>

namespace app { namespace validator {
    class SongValidator : public AbstractValidator {
        data::Song song_;

    public:
        SongValidator(const data::Song& song);
        bool validate();

    };
}}   // namespace app::validator

#endif  // APP_VALIDATOR_SONG_H
