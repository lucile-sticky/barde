#include "stringMethods.h"

using namespace std;

namespace app { namespace util {

    string StringMethods::toSnakeCase(const string& s) {
        string result = "";
        size_t l = s.length();
        char c;
        for (size_t i = 0; i < l; ++i) {
            c = tolower(s[i]);
            if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '.' || c == '-') {
                result += c;
            } else if (i > 0 && s[i-1] != '_') {
                result += '_';
            }
        }
        return result;
    }

}}
