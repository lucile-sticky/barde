#ifndef APP_LOGIN_H
#define APP_LOGIN_H

#include <app/master.h>
#include <data/pages/login.h>

namespace app {
    class Login : public Master {

    public:
        Login(cppcms::service& s);
        void display();

    private:
        // Return empty string if missing avatar
        std::string getSerializedAvatar(unsigned int avatarId);

    };
}   // namespace app

#endif  // APP_LOGIN_H

