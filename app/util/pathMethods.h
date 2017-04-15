#ifndef APP_UTIL_PATHMETHODS_H
#define APP_UTIL_PATHMETHODS_H

#include <string>

namespace app { namespace util {
    class PathMethods {
        static const char FILE_SEPARATOR = '/';

    public:
        static std::string combine(const std::string& directoryPath, const std::string& fileName);
        static std::string combine(const std::string& directoryPath, const std::string& directoryName, const std::string& fileName);

        static std::string getExtension(const std::string fileName);
    };
}}

#endif  // APP_UTIL_PATHMETHODS_H
