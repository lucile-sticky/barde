$(function() {

    var lastParam = window.location.pathname.split('/').pop();
    var menu = $('header>nav>ul');

    if (lastParam == 'playlist') {
        lastParam = 'current';
    }
    menu.find('#' + lastParam).addClass('tab-current');

});

