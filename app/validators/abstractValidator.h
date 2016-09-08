#ifndef APP_VALIDATOR_ABSTRACT_H
#define APP_VALIDATOR_ABSTRACT_H

#include <string>

namespace app { namespace validator {
    class AbstractValidator {
    protected:
        std::string lastMessage_;

    public:
        AbstractValidator() :lastMessage_("")   {}

        virtual bool validate() = 0;

        std::string lastMessage() const {
            return lastMessage_;
        }
    };
}}   // namespace app::validator

#endif  // APP_VALIDATOR_ABSTRACT_H
