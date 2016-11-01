$(function() {
    var playlistComments = {
        comments: $('.comments-wrapper'),
        init: function() {
            var that = this;

            this.comments.find('button').click(function(e) {
                var eltComment = $(this).siblings('textarea')

                that.sendComment(eltComment);
            });

            this.comments.find('textarea').keypress(function(e) {
                if (e.which == 13) {
                    var eltComment = $(this);

                    that.sendComment(eltComment);
                }
            });
        },
        sendComment: function(eltComment) {
            var comment = eltComment.val().trim();

            if (comment.length == 0) {
                return;
            }

            var playlistId = this.comments.data('id');
            var parentId = eltComment.parent().data('parent');

            if (!parentId) {
                parentId = 0;
            }

            var url = '/playlists/comment-playlist/ajax-comment/' + playlistId + '/' + parentId;

            var that = this;
            $.ajax({
                type: 'POST',
                url: url,
                data: { comment: comment },
                success: function() {
                    eltComment.val('');
                    that.successMessage();
                },
                error: function() {
                    that.failureMessage();
                }
            });
        },
        successMessage: function() {
            this.comments.find('.alert-warning').hide();
            this.comments.find('.alert-success').show();
        },
        failureMessage: function() {
            this.comments.find('.alert-success').hide();
            this.comments.find('.alert-warning').show();
        },
    };

    playlistComments.init();
});
