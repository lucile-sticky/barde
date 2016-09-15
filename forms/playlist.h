#ifndef FORM_PLAYLIST_H
#define FORM_PLAYLIST_H

#include <cppcms/form.h>


namespace form {

	struct PlaylistForm:public cppcms::form {
		cppcms::widgets::text name;
		cppcms::widgets::file image;
		cppcms::widgets::textarea description;
		cppcms::widgets::submit submit;
		cppcms::form inputs;
		cppcms::form buttons;

		PlaylistForm() {
            using booster::locale::translate;

            name.id("name");
			name.message(translate("Name:"));
			name.limits(1, 45);

			image.id("image");
			image.message(translate("Image:"));
			image.limits(1000, 100000);
            image.mime(booster::regex("image/.+"));
            image.non_empty();

            description.id("description");
			description.message(translate("Description:"));
            description.cols(40);
            description.rows(4);
			description.limits(0, 500);

			submit.value(translate("Submit"));

			inputs.add(name);
			inputs.add(image);
			inputs.add(description);
			buttons.add(submit);
			add(inputs);
			add(buttons);
	}};

}   // namespace form
#endif  // FORM_PLAYLIST_H
