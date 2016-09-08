#ifndef DATA_LOGIN_H
#define DATA_LOGIN_H

#include <data/htmlPage.h>
#include <forms/login.h>


namespace data {
    struct PageLogin: public HtmlPage {
        form::LoginForm input;
    };

}   // namespace data
#endif  // DATA_LOGIN_H
