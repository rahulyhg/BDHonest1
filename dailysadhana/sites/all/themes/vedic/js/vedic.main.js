String.prototype.endsWith = function(suffix) {
  return suffix && this.indexOf(suffix, this.length - suffix.length) !== -1;
};

window.$ = window.jQuery;

var DSS = DSS || {}

// On load
$(function() {

  // Mark current menu items as active
  $('.ui.menu').find('a.item').each(function() {
    var $t = $(this);
    var href = $t.get(0).pathname.substring(1);
    var path = window.location.pathname.substring(1);
    // console.log('href = ' + href + ', path = ' + path);
    if(path === href || $t.hasClass('tree') && path.startsWith(href)) {
      $t.addClass('active');
    }
  });

  $('.ui.dropdown').dropdown({
    // on: 'hover'
  });

  $('ul.tabs').addClass('ui tabular menu').find('li').addClass('item');
  $('.messages').addClass('ui message').prepend('<i class="close icon"></i>');
  $('.messages--warning').addClass('warning');
  $('.messages--status').addClass('info');
  $('.messages--error').addClass('error');

  $('.message .close').on('click', function(e) {
    $(this).closest('.message').transition('fade');
    e.stopPropagation();
  });

  $('.items-list').on('click', '.edit-link', function(e) {

    var href = $(this).attr('href');

    if(typeof href !== 'undefined') {
      window.location.href = href;
      e.stopPropagation();
      return false;
    }

  });

  if($.fn.bxSlider) {
    $('.view-home-page-slider .view-content').bxSlider({
      auto: true,
      useCSS: false,
      pause: 6000,
      autoHover: true,
      onSliderLoad: function() {
        $('.view-home-page-slider .bx-wrapper').append('<img class="slider-overlay" src="https://d4mg7m5aeu7n2.cloudfront.net/userfiles/files/page-images/slider-overlay.png"">');
        $('.view-home-page-slider').fadeTo(400, 1, $.easing.easeInQuad);
      }
    });
  }

  $('.not-available').on('click', function(e) {
    e.preventDefault();
    return false;
  });

  // $('.bg-slider').bxSlider({
  //   auto: true,
  //   useCSS: false,
  //   pause: 4000,
  //   mode: 'fade',
  //   autoHover: false,
  //   onSliderLoad: function() {
  //
  //   }
  // })

  /*
  var zIndexBase = -1000;
  $('.bg-slider .bg-slide').css('z-index', zIndexBase + 2).addClass('cur');

  var bgSlides = JSON.parse($('.view-background-slider .view-content').html());
  var curIndex = -1;

  var nextBgSlide = DSS.nextBgSlide = function(duration, callback) {
    var curSlide = $('.bg-slider .bg-slide.cur'); // In first call this wont exist
    var nextIndex = (curIndex + 1) % bgSlides.nodes.length;
    var imgUrl = bgSlides.nodes[nextIndex].field_image;

    $('<img>').attr('src', imgUrl).load(function() {

      $(this).remove(); // prevent memory leaks

      curIndex = nextIndex;
      var nextSlide = $('.bg-slider .bg-slide.slide-' + curIndex);
      if(nextSlide.length == 0) {
        nextSlide = $('<div class="bg-slide slide-' + curIndex + '">');
        $('.bg-slider').append(nextSlide);
      }
      nextSlide.css({
        'background-image': 'url("' + imgUrl + '")',
        'z-index': zIndexBase + 1
      }).hide(); // Just behind cur slide

      curSlide.removeClass('cur').fadeOut(duration, function() {
        $(this).css('z-index', zIndexBase).fadeIn(duration);
      });

      nextSlide.addClass('cur').fadeIn(duration, function() {
        $(this).css('z-index', zIndexBase + 2); // Position of cur slide
        if(typeof callback === 'function') {
          callback();
        }
      });

    });
  };

  // Initially, load first slide
  nextBgSlide(0, function() {
    $('.container').removeClass('eclipse');
  });

  // setInterval(function() {
  //   nextBgSlide(2500);
  // }, 60000);
  */
});



(function($) {
    $(document).ready(function() {
        $('.views-field-field-question').click(function(e) {
            e.preventDefault();
            var $this = $(this);
            if ($(this).next(".views-field-body").hasClass('show')) {
                $(this).next(".views-field-body").removeClass('show');
            } else {
                $(this).next(".views-field-body").addClass('show');
            }
            // $(this).next(".pane-content").addClass('show');
        });
		
		
		
        $(".mob-nav").on('click', function() {
            if ($('body').hasClass('showmenu')) {
                $('body').removeClass("showmenu");
            } else {
                $('body').addClass("showmenu");
            }
        });
		
		
		
		
    });
})(jQuery);