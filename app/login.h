#ifndef APP_LOGIN_H
#define APP_LOGIN_H

#include <app/master.h>
#include <data/loginMapper.h>
#include <data/pages/login.h>

namespace app {
    class Login : public Master {

        data::LoginPage login_;

    public:
        Login(cppcms::service& s);
        void display();

    };
}   // namespace app

#endif  // APP_LOGIN_H

