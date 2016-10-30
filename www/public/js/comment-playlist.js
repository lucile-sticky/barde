$(function() {
    var playlistComments = {
        comments: $('.comments-wrapper'),
        init: function() {
            var that = this;

            this.comments.find('button').click(function(e) {
                var eltComment = $(this).siblings('textarea')
                var comment = eltComment.val().trim();

                if (comment.length == 0) {
                    return;
                }

                var playlistId = that.comments.data('id');
                var parentId = $(this).parent().data('parent');

                if (!parentId) {
                    parentId = 0;
                }

                that.sendComment(playlistId, parentId, comment, eltComment);
            });
        },
        sendComment: function(playlistId, parentId, comment, eltComment) {
            var success = false;

            var url = '/playlists/comment-playlist/ajax-comment/' + playlistId + '/' + parentId;

            $.ajax({
                type: 'POST',
                url: url,
                data: { comment: comment },
                success: function() {
                    eltComment.val('');
                }
            });
        },
    };

    playlistComments.init();
});
