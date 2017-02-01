$(document).ready(function() {
    var userProfile = {
        button: $('#open-profile'),
        profile: $('#profile'),
        search: $('#profile .proposed-songs .search'),
        init: function() {
            var that = this;

            // Events
            this.button.click(function (e) {
                that.toggle();
                e.stopPropagation();
            });
            $(document).click(function (e) {
                if (!that.profile.is(e.target) && that.profile.has(e.target).length == 0) {
                    that.collapse();
                    e.stopPropagation();
                }
            });
        },
        toggle: function() {
            if ($('body').hasClass('pushed')) {
                collapse();
            } else {
                $('body').addClass('pushed');
            }
        },
        collapse: function() {
            $('body').removeClass('pushed');
        },
    };

    var userProposedSongs = {
        editForm: $('#profile > .edit > form'),
        search: $('#profile > .proposed-songs .search'),
        table: $('#profile > .proposed-songs table'),
        init: function() {
            var that = this;

            // Events
            this.editForm.find('input').change(function(e) {
                that.updateUser();
            });
            this.search.keyup(function(e) {
                that.filterTable($(this).val());
            });
        },
        updateUser: function() {
            var url = '/playlists/user/ajax-update';
            $.post(url, this.editForm.serialize());
        },
        filterTable: function(filter) {
            var that = this;

            filter = this.simplifyFilter(filter);

            this.table.find('tr').each(function(index) {
                if (that.simplifyFilter($(this).text()).indexOf(filter) !== -1) {
                    $(this).show();
                } else {
                    $(this).hide();
                }
            });
        },
        simplifyFilter: function(filter) {
            return filter.trim().toLowerCase();
        },
    };

    userProfile.init();
    userProposedSongs.init();
});
