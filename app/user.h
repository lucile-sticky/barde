#ifndef APP_USER_H
#define APP_USER_H

#include <app/master.h>

namespace app {
    class User : public Master {

    public:
        User(cppcms::service& s);
        void ajaxUpdate();
        void ajaxUpdateAvatar();
    };
}   // namespace app

#endif  // APP_USER_H

