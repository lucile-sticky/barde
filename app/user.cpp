#include "user.h"

#include <cppcms/http_request.h>
#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>
#include <booster/log.h>

#include <app/validator/imageValidator.h>
#include <data/userMapper.h>
#include <data/mediaMapper.h>

#include <sstream>

using namespace cppcms::http;
using namespace app::validator;
using namespace std;

namespace app {
    User::User(cppcms::service& s) :
        app::Master(s)
    {
        dispatcher().assign("/ajax-update", &User::ajaxUpdate, this);
        mapper().assign("");

        dispatcher().assign("/ajax-update-avatar", &User::ajaxUpdateAvatar, this);
        mapper().assign("");
    }

    void User::ajaxUpdate() {
        data::User user;

        if (! checkAuth(user, data::User::CITIZEN)) {
            response().make_error_response(response::forbidden);
            BOOSTER_WARNING(__func__) << "Forbid user "
                << user.alias << " to update her/his data";
            return;
        }

        data::User updateUser;
        updateUser.id = user.id;
        updateUser.privacy = request().post("privacy");

        data::UserMapper userMapper(connectionString_);
        bool success = userMapper.update(updateUser);
        if (success) {
            session()["privacy"] = updateUser.privacy;
        }

        cppcms::json::value jsonOutput;
        jsonOutput["success"] = success;
        response().out() << jsonOutput;
    }

    void User::ajaxUpdateAvatar() {
        data::User user;

        if (! checkAuth(user, data::User::CITIZEN)) {
            response().make_error_response(response::forbidden);
            BOOSTER_WARNING(__func__) << "Forbid user "
                << user.alias << " to update her/his avatar";
            return;
        }

        bool success = false;
        ostringstream message;
        string image = "";

        // Pick first uploaded file if any
        request::files_type uploadedFiles = request().files();
        if (uploadedFiles.size() > 0) {
            file* uploadedFile = uploadedFiles[0].get();

            user.avatar.type = data::Media::TYPE_AVATAR;
            user.avatar.label = "Avatar " + user.alias;
            user.avatar.file = toUploadRelativePath(uploadedFile);
            user.avatar.contentType = uploadedFile->mime();
            user.avatar.length = uploadedFile->size();

            ImageValidator validator(user.avatar);
            if (!validator.validate()) {
                message << validator.lastMessage();
                success = false;
            } else {
                try {
                    std::string avatarFullPath = toFullPath(user.avatar.file);
                    uploadedFile->save_to(avatarFullPath);
                    BOOSTER_INFO(__func__) << "Uploaded file " << avatarFullPath;

                    data::MediaMapper mediaMapper(connectionString_);
                    data::UserMapper userMapper(connectionString_);

                    if (user.hasAvatar()) {
                        mediaMapper.disable(user.avatar);
                    }

                    user.avatar.id = mediaMapper.insert(user.id, user.avatar);
                    if (user.avatar.id != 0) {
                        success = userMapper.updateAvatarId(user.id, user.avatar.id);
                    }

                    if (success) {
                        session()["avatar"] = serializeMedia(user.avatar);

                        image = toPublicUrl(user.avatar.file);  // returned in JSON response
                    }
                } catch(const cppcms::cppcms_error& e) {
                    BOOSTER_ERROR(__func__) << "Could not upload file " << user.avatar.file << " - - " << e.trace();
                }
            }
        }

        if (success) {
            cache().rise(std::to_string(user.id));
            message << "Successfully updated the avatar.";
        } else if (message.str().empty()) {
            message << "Could not upload the avatar, please retry later.";
        }

        cppcms::json::value jsonOutput;
        jsonOutput["success"] = success;
        jsonOutput["message"] = message.str();
        jsonOutput["image"] = image;
        response().out() << jsonOutput;
    }

}   // namespace app

