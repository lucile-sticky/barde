#include "imageValidator.h"

#include <booster/log.h>

#include <sstream>

namespace app { namespace validator {

    ImageValidator::ImageValidator(const data::Media& image, unsigned long maxLength) :
        image_(image),
        allowedContentTypes_({"image/png", "image/jpeg", "image/svg"}),
        maxLength_(maxLength)
    {}

    bool ImageValidator::validate() {
        bool result = false;
        std::ostringstream message;

        if (image_.length > maxLength_) {
            message << "The uploaded file is too big";
        } else if (allowedContentTypes_.find(image_.contentType) == allowedContentTypes_.end()) {
            message << "Invalid content-type";
        } else {
            result = true;
        }

        lastMessage_ = message.str();

        if (!result) {
            BOOSTER_WARNING(__func__) << lastMessage_ << " -- " << image_.toString();
        }

        return result;
    }

}}   // namespace app::validator
