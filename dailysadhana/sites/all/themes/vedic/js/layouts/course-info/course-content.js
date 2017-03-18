var DSS = DSS || {};

(function($) {

    DSS.setCourseProgress = function(progressPercent) {
        $('.progress.course-progress').progress({
            percent: progressPercent
        });
    };

	function alertModal(header, content) {
		var $modal = $('.alert-modal.ui.modal');
		$modal.find('.header').text(header);
		$modal.find('.content').html(content);
		$modal.modal({
			'duration': 200
		}).modal('show');
	}

	$('.course-chapters-list')
  // .on('click', '.course-chapter-item', function() {
  //
	// 	var $t = $(this);
  //
	// 	// Check if already loaded
	// 	if($t.hasClass('lessons-loaded')) {
	// 		$t.find('.course-lessons-list').toggle();
	// 		return false;
	// 	}
  //
	// 	// Show loader
	// 	if(!$t.find('.title .loader').length) {
	// 		$t.find('.title').append('<div class="ui active mini inline loader"></div>');
	// 	}
  //
	// 	var chapterNid = $t.attr('data-chapter-nid');
	// 	// var type = $t.attr('data-type');
  //
	// 	$.get('/api/chapter/' + chapterNid + '/lessons', function(response) {
	// 		// alert(response['data']);
	// 		if(response.status === 'ok') {
	// 			$t.find('.content').append(response.data);
	// 		} else {
	// 			$t.removeClass('lessons-loaded');
	// 		}
  //
	// 		$t.blur();
	// 		$t.find('.ui.loader').remove(); // Remove loader
	// 	}).fail(function() {
	// 		$t.removeClass('lessons-loaded');
	// 	});
  //
	// 	$t.addClass('lessons-loaded'); // Mark as loaded
	// 	return false;
  //
	// })
  .on('click', '.course-lesson-item', function(e) { // Do not make the above event fire on lesson items

		var $t = $(this);

		if($t.hasClass('not-available')) {
			$t.blur();
			alertModal('Not Available', 'This content is not available to you yet.');
			return false;
		}

		e.stopPropagation();
		return true;
	});

    var progressPercent = $('#course-meta').data('course-completed');
    DSS.setCourseProgress(progressPercent);

	$('.course-chapter-item.cur').click();

} (jQuery));
