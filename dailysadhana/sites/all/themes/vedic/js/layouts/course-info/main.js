var DSS = DSS || {};

(function ($) {

	DSS.closeMobileMenu = function(e) {

		var $body = $('body');

		if($body.hasClass('mobile-opened')) {
			
			// Close mobile menu
			$body.removeClass('mobile-opened');
			$('.pane-page-dimmer').removeClass('active');

			if(e) {
				e.stopPropagation();
				e.preventDefault();
			}
			return false;
		}
	};

	$('.sidebar-icon').on('click', function(e) {

		if($('body').hasClass('mobile-opened')) {

			// Close mobile menu
			DSS.closeMobileMenu(e);

		} else {

			$('body').addClass('mobile-opened');
			$('.pane-page-dimmer').addClass('active');
			e.stopPropagation();
			e.preventDefault();
		}
		return false;
	});

	$('.pane-page-content').on('click', DSS.closeMobileMenu);

	$('.l-sidebar-links a').on('click', function(e) {
		DSS.closeMobileMenu();
		e.stopPropagation();
		return true;
	});

	// Bind arrow key events to move slides 
	$(document).on('keydown', 'body', function(e) {

        if (e.shiftKey || e.altKey || e.ctrlKey || e.metaKey) {
            return true;
        }

        var keyCode = e.keyCode || e.which;
        switch (keyCode) {
        	case 27:
        		// Esc key
        		DSS.closeMobileMenu(e);
        		break;
        }
    });

} (jQuery));