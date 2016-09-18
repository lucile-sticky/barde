#ifndef DATA_PAGE_LOGIN_H
#define DATA_PAGE_LOGIN_H

#include <data/pages/htmlPage.h>
#include <data/forms/login.h>


namespace data {
    struct LoginPage: public HtmlPage {
        LoginForm input;
    };

}   // namespace data
#endif  // DATA_PAGE_LOGIN_H
