$(document).ready(function() {
    var proposeSong = {
        button: $('.propose-song > button'),
        form: $('.propose-song > form'),
        init: function() {
            var that = this;

            // Events
            this.button.click(function (e) {
                if (!that.display()) {
                    that.hide();
                }
                e.stopPropagation();
            });
            $(document).click(function (e) {
                if (!that.form.is(e.target) && that.form.has(e.target).length == 0) {
                    that.hide();
                }
            });
            this.form.find('button').click(function(e) {
                e.preventDefault();

                that.submitNewSong('/playlists/song/ajax-new');
            });

        },
        display: function() {
            if (!this.form.hasClass('displayed')) {
                this.form.removeClass('not-displayed');
                this.form.addClass('displayed');
                return true;
            }
            return false;
        },
        hide: function() {
            if (this.form.hasClass('displayed')) {
                this.form.removeClass('displayed');
                this.form.addClass('not-displayed');
                return true;
            }
            return false;
        },
        displayMessage: function(message) {
            this.form.find('.message').text(message);
        },
        clear: function() {
            this.form.find('input[type=text]').val('');
            this.displayMessage('');
        },
        submitNewSong: function(url) {
            var that = this;

            $.post(url, this.form.serialize()).done(function(response) {
                var json = JSON.parse(response);
                if (json.success) {
                    that.hide();
                    that.clear();
                } else {
                    that.displayMessage(json.message);
                }
            });
        },
    };

    proposeSong.init();
});
