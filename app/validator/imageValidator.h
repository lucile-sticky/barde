#ifndef APP_IMAGE_VALIDATOR_H
#define APP_IMAGE_VALIDATOR_H

#include "abstractValidator.h"

#include <data/media.h>

#include <set>

namespace app { namespace validator {
    class ImageValidator : public AbstractValidator {
    protected:
        data::Media image_;

        std::set<std::string> allowedContentTypes_;
        unsigned long maxLength_;

    public:
        ImageValidator(const data::Media& image, unsigned long maxLength = 5000000);
        bool validate();

    };
}}   // namespace app::validator

#endif  // APP_IMAGE_VALIDATOR_H
