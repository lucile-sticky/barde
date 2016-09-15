#ifndef FORM_LOGIN_H
#define FORM_LOGIN_H

#include <cppcms/form.h>


namespace form {
    struct LoginForm: public cppcms::form {
        cppcms::widgets::text username;
        cppcms::widgets::password password;
        cppcms::widgets::submit submit;
        cppcms::form inputs;
        cppcms::form buttons;

        LoginForm() {
            using booster::locale::translate;

            username.id("username");
            username.limits(3,30);
            username.message(translate("Username"));
            username.attributes_string("class='test'");

            password.id("password");
            password.limits(6,30);
            password.message(translate("Password"));

            submit.value(translate("Login"));
            submit.attributes_string("class='validation-button'");

            inputs.add(username);
            inputs.add(password);
            buttons.add(submit);
            add(inputs);
            add(buttons);
        }
    };

}   // namespace form
#endif  // FORM_LOGIN_H
