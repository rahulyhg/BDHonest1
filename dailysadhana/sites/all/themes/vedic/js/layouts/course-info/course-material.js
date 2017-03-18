var DSS = DSS || {};

(function($) {

	DSS.isAutoplay = function() {
		return $('#lesson-meta').data('autoplay-slides') == '1';
	};

	DSS.handleAutoplay = function() {

		if(DSS.isAutoplay()) {
			DSS.slider.getCurrentSlideElement().find('audio').trigger('play');
		}
	};

	DSS.redrawSlider = function() {
		DSS.slider.redrawSlider();
		DSS.modal.modal('refresh');
	};

	DSS.pauseAllAudiosVideos = function() {

		// Pause the videos
		$('iframe').each(function() {
			this.contentWindow.postMessage('{"event":"command","func":"pauseVideo","args":""}', '*');
			this.contentWindow.postMessage('{"method":"pause","value":""}', '*');
		});

		// $('.course-content-modal').find('audio').trigger('pause');
		// Pause all audios, above method wont update the playpause button
		$('.course-content-modal audio').each(function() {
			$(this).trigger('pause');
		});
	};

	DSS.alertModal = function(header, content) {
		var $alertModal = $('.alert-modal.ui.modal');
		$alertModal.find('.header').text(header);
		$alertModal.find('.content').html(content);
		var settings = {
			duration: 200,
			allowMultiple: true,
			closable: false,
			transition: 'shake'
		};

		if($('.course-content-modal').hasClass('active')) {

			DSS.pauseAllAudiosVideos();
			$('.course-content-modal').removeClass('visible active').hide();
			// DSS.modal.modal('hide');

			settings.onHide = function() {
				$('.alert-modal').removeClass('visible active').hide();
				$('.course-content-modal').addClass('visible active').fadeIn();
				DSS.modal.modal('refresh');
			};
		}

		console.log(settings);
		$alertModal.modal(settings).modal('show');
	};

	DSS.itemOpenCheck = function($item) {

		if($item.hasClass('enforced-waiting')) {

			var $enforced = $('.course-materials-list').find('.open-course-material.enforced');
			var enforcedLink = '<a href="' + $enforced.attr('href') + '">' + $enforced.find('.title').text() + '</a>';

			DSS.alertModal('Not now', 'You cannot view this content unless you complete ' + enforcedLink + '.');
			return false;

		} else if($item.hasClass('is-once')) {

			//DSS.alertModal('No access', 'You cannot access this content now.');
			
			return false;

		}else if($item.hasClass('video-once')) {

			DSS.alertModal('No access', 'This video lecture has already been viewed and cannot be reviewed again.');
			return false;

		}

		return true;
	};

	DSS.lazyLoadVideo = function($slideElement) {


		var $videoWrapper = $slideElement.find('.video-wrapper');
		var videoSrc = $videoWrapper.data('video-src');

		if($videoWrapper.length && videoSrc != '') {

			DSS.modal.showDimmer($slideElement);
			$videoWrapper.find('iframe').on('load', function() {
				DSS.modal.hideDimmer($slideElement);
			}).attr('src', videoSrc);
			$videoWrapper.data('video-src', '');
		}
	};

	DSS.lazyLoadAudio = function($slideElement) {

		var $audioWrapper = $slideElement.find('.audio-wrapper');
		var audioSrc = $audioWrapper.data('audio-src');

		if($audioWrapper.length && audioSrc != '') {

			$audioWrapper.prepend('<audio controls><source src="' + audioSrc + '" type="audio/mpeg"></audio>');
			$audioWrapper.data('audio-src', '');
		}
	};

	DSS.setSliderActiveMenuItem = function() {

		$('.course-material-menu .item').removeClass('active');
		$('.course-material-menu .item[data-type="' + DSS.slider.getCurrentSlideElement().attr('data-type') + '"]').addClass('active');
	};

	DSS.sliderSettings = {
		nextSelector: '#none',
		prevSelector: '#none',
		infiniteLoop: false,
		// allowMultiple: true,
		adaptiveHeight: true,
		adaptiveHeightSpeed: 150,
		// mode: 'fade',
		pager: false,

		onSlideBefore: function(thisSlider, $slideElement, oldIndex, newIndex) {

			if(DSS.itemOpenCheck($('.course-materials-list .item[data-type="' + $slideElement.attr('data-type') + '"]')) == false) {
				return false;
			}

			if(newIndex == 0) {
				$('.prev-nav').addClass('disabled');
			} else {
				$('.prev-nav').removeClass('disabled');
			}

			if(newIndex == DSS.slider.getSlideCount() - 1) {
				$('.next-nav').addClass('disabled');
			} else {
				$('.next-nav').removeClass('disabled');
			}

			// Scroll modals to top
			DSS.modal.scrollToTop();

			// Pause videos and audios
			DSS.pauseAllAudiosVideos();

			// Set active menu item
			DSS.setSliderActiveMenuItem();

			// Mark as done
			var dataType = $slideElement.attr('data-type');
			var nid = $slideElement.attr('data-nid');

			switch(dataType) {
				case 'Video Lecture':
				case 'Extra Material':
					DSS.lazyLoadVideo($slideElement);
					break;

				case 'Exercise':
					if(!$slideElement.hasClass('data-loaded')) {

		        		$.get('/api/exercise/' + nid, function(response) {
		        			DSS.exerciseData = DSS.exerciseData || {};
		        			DSS.exerciseData[nid] = response['data'];
		        		});
		        	}
		        	$slideElement.addClass('data-loaded');

		        	// no break;

		        case 'Lesson Review':
					DSS.lazyLoadAudio($slideElement);
		        	DSS.markCompleted(nid);
			}

			return true; // Move the slide
		},

		onSlideAfter: function(thisSlider, $slideElement, oldIndex, newIndex) {

			DSS.handleAutoplay();

			// setTimeout(DSS.redrawSlider, 400); // To resize modal height, but seems useless
			DSS.modal.setScrollIndicator();
			DSS.modal.adjustVideoDisplay();
			DSS.redrawSlider();
		}
	};

	DSS.modal = $('.ui.modal.course-content-modal');

	DSS.modal.scrollToTop = function() {
		// Use .modals.dimmer for variable height
		$('#modal-content').animate({
			scrollTop: 0
		}, 400);
	};

	DSS.modal.showDimmer = function($slideElement) {
		$slideElement.find('.video-dimmer').addClass('active');
	};

	DSS.modal.hideDimmer = function($slideElement) {
		$slideElement.find('.video-dimmer').removeClass('active');
	};

	// FIXED HEIGHT
	DSS.modal.setScrollIndicator = function() {

		var $modalContent = $('#modal-content');
		var isVideo = DSS.slider && DSS.slider.getCurrentSlideElement().find('iframe').length;

		// Considering 15px as the bottom padding
		if(isVideo || $modalContent.scrollTop() + $modalContent.innerHeight() >= $modalContent[0].scrollHeight - 40) {

			// End of modal-content, hide indicator
            $('#scroll-indicator').stop().animate({
            	visibility: 'hidden',
            	opacity: 0
            	},
            	400
            );
        } else {

        	// Show indicator
        	$('#scroll-indicator').stop().animate({
            	visibility: 'visible',
            	opacity: 1
            	},
            	50
            );
        }
	};

	DSS.modal.adjustVideoDisplay = function() {

		// Is desk
		if($('body').width() < 768) {
			$('.video-bg').css({
				height: 'auto',
				padding: '0'
			});
			return;
		}

		var height = $('#modal-content').height();
		var width = $('.video-bg').parent().width();
		var padding = 0;
		var aspectRatio = 1.7846;

		if(width / height > aspectRatio) {
			// Width is more, set padding left & right
			var actualWidth = height * aspectRatio;
			padding = (width - actualWidth) / 2;

			// console.log('width=' + width + ', actualWidth=' + actualWidth + ',padding=' + padding);
			$('.video-bg').css({
				height: height + 'px',
				padding: '0 ' + padding + 'px'
			});
		} else {
			// Height is more, set padding top & bottom
			aspectRatio = 1.78575;
			var actualHeight = width / aspectRatio;
			padding = (height - actualHeight) / 2;

			// console.log('height=' + height + ', actualHeight=' + actualHeight + ',padding=' + padding);
			$('.video-bg').css({
				height: height + 'px',
				padding: padding + 'px 0'
			});
		}

		DSS.redrawSlider();
	};

	// Open modal
	$(document).on('click', '.open-course-material', function() {

		var $item = $(this);
		if(DSS.itemOpenCheck($item) === false) {
			return false;
		}

		// Opening modal
		DSS.modal
		.modal({
			transition: 'fade up',
            closable: false,
            onShow: function() {
            	// DSS.modal.adjustVideoDisplay();
            },
			onHide: function() {
				DSS.pauseAllAudiosVideos();
			},
			onVisible: function() {
				DSS.handleAutoplay();
				// $(window).resize();  // To reload modal and esp slider
			}
		})
		.modal('show');

        // Initialize the slider if not yet done
		if(DSS.modal.hasClass('init-slider')) {

			// Load modal slider
			DSS.slider = $('.course-material-slides').bxSlider(DSS.sliderSettings);

			DSS.slider.getCurrentSlideElement = function() {
				return $('.course-material-slides .slide-holder:nth-child(' + (parseInt(DSS.slider.getCurrentSlide()) + 1) + ')');
			};

			DSS.slider.initSlider = (function() {

				var currentIndex = DSS.slider.getCurrentSlide();

				if(currentIndex == 0) {
					$('.prev-nav').addClass('disabled');
				}

				DSS.handleAutoplay();
				DSS.setSliderActiveMenuItem();

				var $currentSlideElement = DSS.slider.getCurrentSlideElement();
				DSS.lazyLoadVideo($currentSlideElement);
				DSS.lazyLoadAudio($currentSlideElement);

				DSS.redrawSlider(); // To resize modal height
				setTimeout(DSS.modal.adjustVideoDisplay, 100); // Don't call immediately: causing a bug

				$(window).resize(function() {
					setTimeout(DSS.modal.adjustVideoDisplay, 100);
				});

			}());

			$('.prev-nav').click(function() {
				DSS.slider.goToPrevSlide();
			});

			$('.next-nav').click(function() {
				DSS.slider.goToNextSlide();
			});
		}

		DSS.slider.goToSlide($item.attr('data-slide-num'));

		DSS.modal.removeClass('init-slider');

		DSS.redrawSlider(); // window might be resized
	});

	$(window).resize(function() {
		DSS.modal.modal('refresh');
		DSS.modal.setScrollIndicator();
	});

	function scrollUpModalContent() {
		var $modalContent = $('#modal-content');

		$modalContent.animate({
	        scrollTop: $modalContent.scrollTop() - 200
	    }, 200);
	}

	function scrollDownModalContent() {
		var $modalContent = $('#modal-content');

		$modalContent.animate({
	        scrollTop: $modalContent.scrollTop() + 200
	    }, 200);
	}

	$('#scroll-indicator').click(scrollDownModalContent);

	// Init autoplay
  var $meta = $('#lesson-meta');
  var autoplayState = $meta.data('autoplay-slides');
  $('.autoplay').prop('checked', autoplayState);

	// Autoplay changes
	$('.course-content-modal').on('click', '.autoplay', function() {
		var state = $(this).prop('checked');
		DSS.setAutoplay(state);
	});

	// FIXED HEIGHT
	$('#modal-content').on('scroll', DSS.modal.setScrollIndicator);
	DSS.modal.setScrollIndicator();

	// Initialize audio with audioPlayer
	// $('audio').audioPlayer();

	// Bind arrow key events to move slides
	$(document).on('keydown', 'body', function(e) {

        if (e.shiftKey || e.altKey || e.ctrlKey || e.metaKey) {
            return true;
        }

        var keyCode = e.keyCode || e.which;

        switch (keyCode) {

        	case 27:
        		// Esc key
        		DSS.modal.modal('hide');
        		break;

            case 36:
            case 37:
            case 80:
                DSS.slider.goToPrevSlide();
                break;

            case 35:
            case 39:
            case 78:
                DSS.slider.goToNextSlide();
                break;

            case 33:
            case 38:
            case 77:
            	scrollUpModalContent();
            	break;

            case 36:
            case 40:
            case 79:
            	scrollDownModalContent();
            	break;
        }
    });

}(jQuery));
