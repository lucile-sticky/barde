#include "pathMethods.h"

using namespace std;

namespace app { namespace util {

    std::string PathMethods::combine(const std::string& directoryPath, const std::string& fileName) {
        string result = "";

        if (!directoryPath.empty()) {
            result += directoryPath;
        }

        if (!fileName.empty()) {
            if (result[result.length() - 1] != FILE_SEPARATOR &&
                    fileName[0] != FILE_SEPARATOR) {
                result += FILE_SEPARATOR;
            }
            result += fileName;
        }

        return result;
    }

    std::string PathMethods::combine(const std::string& directoryPath, const std::string& directoryName, const std::string& fileName) {
        string result = combine(directoryPath, directoryName);
        result = combine(result, fileName);
        return result;
    }


    std::string PathMethods::getExtension(const std::string fileName) {
        string result = "";
        size_t l = fileName.length();
        size_t pos;
        char c;

        for (pos = l - 1; pos > 0; --pos) {
            c = fileName[pos];
            if (c == '.' || c == '/' || c == '\\') {
                break;
            }
        }

        if (c == '.') {
            for (; pos < l; ++pos) {
                c = fileName[pos];
                result += c;
            }
        }
        return result;
    }
}}
