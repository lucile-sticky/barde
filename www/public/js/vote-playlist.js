$(function() {
    var playlistVotes = {
        player: $('#js-playlist'),
        init: function() {
            var that = this;

            $('#js-pl-like').click(function(e) {
                var playlistId = that.player.data('id');
                var url = '/playlists/vote-playlist/ajax-vote/' + playlistId;
                that.toggleLikeDislike($(this), url, 'like');
            });

            $('#js-pl-dislike').click(function(e) {
                var playlistId = that.player.data('id');
                var url = '/playlists/vote-playlist/ajax-vote/' + playlistId;
                that.toggleLikeDislike($(this), url, 'dislike');
            });

            $('.js-song-like').click(function(e) {
                var playlistId = that.player.data('id');
                var songId = $(this).parent().parent().data('id');
                var url = '/playlists/vote-song/ajax-vote/' + playlistId + '/' + songId;
                that.toggleLikeDislike($(this), url, 'like');
            });

            $('.js-song-dislike').click(function(e) {
                var playlistId = that.player.data('id');
                var songId = $(this).parent().parent().data('id');
                var url = '/playlists/vote-song/ajax-vote/' + playlistId + '/' + songId;
                that.toggleLikeDislike($(this), url, 'dislike');
            });
        },
        toggleLikeDislike: function(element, url, action) {
            if (element.hasClass('selected')) {
                action = 'reset';
            }

            var that = this;
            var fullUrl = url + '/' + action;
            $.ajax(fullUrl).done(function(response) {
                if (element.hasClass('selected')) {
                    that.removeSelected(element);
                } else {
                    that.removeSelected(element).addClass('selected')
                }
            });
        },
        removeSelected: function(element) {
            element.parent().find('.selected').removeClass('selected'); // playlist
            element.parent().siblings().find('.selected').removeClass('selected');  // songs
            return element;
        },
    };

    playlistVotes.init();
});
