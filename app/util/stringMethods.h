#ifndef APP_UTIL_STRINGMETHODS_H
#define APP_UTIL_STRINGMETHODS_H

#include <string>

namespace app { namespace util {
    class StringMethods {
    public:

        // Works with ASCII chars only
        static std::string toSnakeCase(const std::string& s);

    };
}}

#endif  // APP_UTIL_STRINGMETHODS_H
