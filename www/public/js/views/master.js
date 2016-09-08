$(function() {

    var lastParam = window.location.pathname.split('/').pop();
    var menu = $('header>nav>ul');

    if (Number.parseInt(lastParam)) {
        lastParam = 'current';
    }
    menu.find('#' + lastParam).addClass('tab-current');

});

