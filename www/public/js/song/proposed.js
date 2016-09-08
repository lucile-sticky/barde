$(document).ready(function() {

    var proposedSongs = {
        init: function() {
            var that = this;

            var eltDataTable = $('#proposed-songs table');
            var eltChoosePlaylist = eltDataTable.find('.choose-playlist');

            eltDataTable.DataTable({
                'pageLength': 10,
                'order': [[0, 'desc']],
                'drawCallback': function(settings) {
                    eltChoosePlaylist.prop('selectedIndex', -1);
                },
            });

            eltChoosePlaylist.change(function(e) {
                var songId = $(this).data('song-id');
                var playlistId = $(this).val();
                that.assignPlaylist(songId, playlistId);
            });
        },
        assignPlaylist: function(songId, playlistId) {
            var url = '/playlists/song/ajax-set-playlist/' + songId + '/' + playlistId;
            $.ajax(url);
        },
    };

    proposedSongs.init();
});
