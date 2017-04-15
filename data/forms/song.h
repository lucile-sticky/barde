#ifndef DATA_FORM_SONG_H
#define DATA_FORM_SONG_H

#include <cppcms/form.h>


namespace data {

	struct SongForm:public cppcms::form {
		cppcms::widgets::text artist;
		cppcms::widgets::text title;
		cppcms::widgets::file newFile;
		cppcms::widgets::hidden file;
		cppcms::widgets::regex_field url;
		cppcms::widgets::checkbox showVideo;
		cppcms::widgets::numeric<unsigned short> position;
		cppcms::widgets::submit submit;
		cppcms::form inputs;
		cppcms::form buttons;

		SongForm() {
            using booster::locale::translate;

            artist.id("artist");
			artist.message(translate("Artist:"));
			artist.limits(1, 100);

			title.id("title");
			title.message(translate("Title:"));
			title.limits(1, 100);

			newFile.id("new_file");
			newFile.message(translate("File:"));
			newFile.limits(1000, 10000000);
            newFile.mime(booster::regex("(audio|video)/.+"));

			file.id("file");

			url.id("url");
			url.message(translate("URL:"));
            url.regex(booster::regex("^https?://.+"));  // fix me for better URLs
            url.value("http://");
			url.limits(8, 200);

            showVideo.id("show_video");
			showVideo.message(translate("Show video:"));

			position.id("position");
			position.message(translate("Position:"));
			position.range(1, 10000);

			submit.value(translate("Submit"));
        }
    };

    struct NewSongForm : public SongForm {
        NewSongForm() : SongForm() {
			inputs.add(artist);
			inputs.add(title);
			inputs.add(url);
			buttons.add(submit);
			add(inputs);
			add(buttons);
        }
    };

    struct EditSongForm : public SongForm {
        EditSongForm() : SongForm() {
			inputs.add(artist);
			inputs.add(title);
			inputs.add(newFile);
			inputs.add(file);
			inputs.add(url);
			inputs.add(showVideo);
			inputs.add(position);
			buttons.add(submit);
			add(inputs);
			add(buttons);
        }
    };

}   // namespace data
#endif  // DATA_FORM_SONG_H
