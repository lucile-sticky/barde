$(document).ready(function() {
    var userProfile = {
        button: $('#open-profile'),
        profile: $('#profile'),
        search: $('#profile .proposed-songs .search'),
        init: function() {
            var that = this;

            // Events
            this.button.click(function (e) {
                that.expand();
                e.stopPropagation();
            });
            $(document).click(function (e) {
                if (!that.profile.is(e.target) && that.profile.has(e.target).length == 0) {
                    that.collapse();
                    e.stopPropagation();
                }
            });
        },
        expand: function() {
            if (!$('body').hasClass('pushed')) {
                $('body').addClass('pushed');
            }
        },
        collapse: function() {
            $('body').removeClass('pushed');
        },
    };

    var userProposedSongs = {
        search: $('#profile .proposed-songs .search'),
        table: $('#profile .proposed-songs table'),
        init: function() {
            var that = this;

            // Events
            this.search.keyup(function(e) {
                that.filterTable($(this).val());
            });
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
