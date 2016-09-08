$(function() {
    $('form input#username').focus();

    $('form input.validation-button').click(function(e) {
        $('#login-wrapper').hide();
    });
});

