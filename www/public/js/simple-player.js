$(function() {
    var simplePlayer = {
        audio: $('audio'),
        songs: $('#js-songs'),
        current: 0,
        len: 0,
        init: function() {
            var that = this;
            var link;

            this.len = this.songs.find('tbody tr').length;

            // Events
            this.songs.find('tbody>tr>td.selectable').click(function(e) {
                e.preventDefault();

                var link = $(this).parent().find('a.js-song');

                that.current = $(this).parent().index();
                that.run(link);
            });
            this.songs.find('a.js-show-video').click(function(e) {
                that.audio[0].pause();
            });
            this.audio[0].addEventListener('ended',function(e) {
                that.current++;
                if(that.current == that.len){
                    that.current = 0;
                }
                link = that.songs.find('a.js-song')[that.current];
                that.run($(link));
            });
            this.audio[0].addEventListener('loadedmetadata', function(e) {
                that.setCurrentDuration();
            });
            $(document).keypress(function(e) {
                if ((e.which == 32 || e.which == 112) && $(e.target).is('body')) {
                    that.togglePlayPause();
                }
            });

            // Start with first song
            var firstLink = this.songs.find('a.js-song')[this.current];
            this.run($(firstLink));
        },
        run: function(link) {
            this.audio[0].src = link.attr('href');

            var grandParent = link.parents('tr');
            grandParent.addClass('selected').siblings().removeClass('selected')
                    .find('.play span').removeClass('flaticon-play');
            grandParent.find('.play span').addClass('flaticon-play');

            this.audio[0].load()
            this.audio[0].play();
        },
        togglePlayPause: function() {
            if (this.audio[0].paused) {
                this.audio[0].play();
            } else {
                this.audio[0].pause();
            }
        },
        setCurrentDuration: function() {
            var selectedFormattedDuration = this.songs.find('tr.selected .duration').text();
            var measuredDuration = Math.round(this.audio[0].duration);
            var measuredFormattedDuration = this.formatDuration(measuredDuration);

            if (selectedFormattedDuration != measuredFormattedDuration) {
                this.updateSongDuration(measuredDuration);
                this.songs.find('.selected .duration').text(measuredFormattedDuration);
            }
        },
        formatDuration: function(measuredDuration) {
            var duration = moment.duration(measuredDuration, 'seconds');
            return moment({
                    minute: duration.minutes(),
                    second: duration.seconds()
                }).format('mm:ss');
        },
        updateSongDuration: function(duration) {
            var songId = this.songs.find('tr.selected').data('id');
            var url = '/playlists/song/ajax-set-duration/' + songId + '/' + duration;

            $.ajax(url);
        },
    };

    simplePlayer.init();
});
