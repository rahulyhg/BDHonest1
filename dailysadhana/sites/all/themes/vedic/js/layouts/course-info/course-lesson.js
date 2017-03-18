var DSS = DSS || {};

(function($) {

    DSS.markCompleted = function(nid) {

        var $item1 = $('.course-materials-list .open-course-material[data-nid="' + nid + '"]');
        var $item2 = $('.course-material-menu .open-course-material[data-nid="' + nid + '"]');

        if($item2.hasClass('completed')) {
            return;
        }

        function updateItem($item) {

            $item.addClass('completed');

            if($item.hasClass('enforced')) {
                $item.removeClass('enforced');
                $item = $item.next();
                console.log($item);

                while($item.length > 0) {

                    if(!$item.hasClass('is-once')) {
                        $item.removeClass('material-disabled enforced-waiting');
                    }

                    if($item.hasClass('enforced')) {
                        break;
                    }

                    $item = $item.next();
                }
            }
        }

        updateItem($item1);
        updateItem($item2);

        // Update the db
        $.get('/api/material_viewed/' + nid, function(data) {
            // console.log('Marked ' + nid + ' as completed');
            // console.log(data);
            var lessonNid = $('#lesson-meta').data('lesson-nid');
            var progressPercent = data['lessons'][lessonNid]['completed'];
            DSS.setLessonProgress(progressPercent);
        });
    };

    DSS.checkExtraMaterialsCompleted = function() {

        var completed = true;
        $('.course-materials-extra-list .open-course-material').each(function(i, item) {

            if(!$(item).hasClass('completed')) {
                completed = false;
                return false;
            }
        });

        if(completed) {
            $('.course-material-menu .open-course-material[data-type="Extra Material"]').addClass('completed');
        }
    };

    // Call once after page load
    DSS.checkExtraMaterialsCompleted();

    DSS.markExtraCompleted = function(courseNid, lessonNid, materialNid) {

        $('.course-materials-extra-list .open-course-material[data-nid="' + materialNid + '"]').addClass('completed');

        DSS.checkExtraMaterialsCompleted();

        // Update the db
        $.get('/api/material_extra_viewed/' + courseNid + '/' + lessonNid + '/' + materialNid, function(data) {
            // console.log('Marked extra material ' + courseNid + ' as completed');
            // console.log(data);
        });
    };

    DSS.setAutoplay = function(state) {

        var $meta = $('#lesson-meta');
        var courseNid = $meta.attr('data-course-nid');

        var val = state ? '1' : '0';
        $meta.data('autoplay-slides', val);
        $('.autoplay').prop('checked', state);

        if(val) {
            $('.course-content-modal').attr('data-autoplay-slides', 'true');
        } else {
            $('.course-content-modal').attr('data-autoplay-slides', 'false');
        }

        var url = '/api/autoplay/' + courseNid + '/' + val;
        // console.log('Calling ' + url);
         // Update the db
        $.get(url, function(data) {
            // console.log('Saved autoplay for courseNid = ' + courseNid + ' as ' + val);
            // console.log(data);
        });
    };

    DSS.setLessonProgress = function(progressPercent) {
        $('.progress.lesson-progress').progress({
            percent: progressPercent
        });
    };

    var playerOrigin = '*';
    // var status = $('.status');

    // Listen for messages from the player
    if (window.addEventListener) {
        window.addEventListener('message', onMessageReceived, false);
    }
    else {
        window.attachEvent('onmessage', onMessageReceived, false);
    }

    // Handle messages received from the player
    function onMessageReceived(event) {
        // Handle messages from the vimeo player only
        if (!(/^https?:\/\/player.vimeo.com/).test(event.origin)) {
            return false;
        }
        
        if (playerOrigin === '*') {
            playerOrigin = event.origin;
        }
        
        var data = JSON.parse(event.data);
        
        switch (data.event) {
            case 'ready':
            onReady();
            break;

            case 'playProgress':
            onPlayProgress(data.data);
            break;

            case 'pause':
            onPause();
            break;

            case 'finish':
            onFinish();
            break;
        }
    }

    // Call the API when a button is pressed
    // $('button').on('click', function() {
    //     post($(this).text().toLowerCase());
    // });

    // Helper function for sending a message to the player
    function post(action, value) {
        var data = {
            method: action
        };

        if (value) {
            data.value = value;
        }

        var message = JSON.stringify(data);
        $('iframe').each(function(i, player) {
            player.contentWindow.postMessage(message, playerOrigin);
        });
    }

    function onReady() {
        // status.text('ready');
        var start = new Date().getTime();
console.log(start);
        post('addEventListener', 'pause');
        post('addEventListener', 'finish');
        post('addEventListener', 'playProgress');
    }

    function onPause() {
        // status.text('paused');
        var pause= new Date().getTime();
console.log(pause + 'Paused ');
    }

    function onFinish() {
        // status.text('finished');
        var $visibleSlide = $('.slide-holder:visible');

        var dataType = $visibleSlide.attr('data-type');
        var nid = $visibleSlide.attr('data-nid');

        console.log('dataType = ' + dataType);
        if(dataType == 'Extra Material') {

            var $meta = $('#lesson-meta');

            // console.log('Marking extra material for nid = ' + nid);
            DSS.markExtraCompleted($meta.attr('data-course-nid'), $meta.attr('data-lesson-nid'), nid);
        } else {
            // console.log('Marking material for nid = ' + nid);
            DSS.markCompleted(nid);
        }
    }

    function onPlayProgress(data) {
        // status.text(data.seconds + 's played');
        var $visibleSlide = $('.slide-holder:visible');
        var nid = $visibleSlide.attr('data-nid');
        var v_length = $('.video-minutes').text().split(" ");
        var vlen = v_length[0];
        var video_length = Math.floor((vlen*90)/100);
        var minute = Math.floor(data.seconds/60);
        var vflag = false;
        if ($.isNumeric(video_length) == false) {
        	 video_length = 15;
        }
        if (minute == video_length && vflag == false) {
        	DSS.markCompleted(nid);
        	vflag = true;
        	console.log(video_length +' mins reached');
        }
        //console.log(minute + 'mins ' + data.seconds + 's played');
        
    }

    // Check if this is current lesson
    if($('#lesson-meta').data('current-lesson') == '1') {
    	$('#current-lesson-menu-item').addClass('active');
    }

    var progressPercent = $('#lesson-meta').data('lesson-completed');
    DSS.setLessonProgress(progressPercent);

    // Fullscreen fix on chrome
    $(document).on("webkitfullscreenchange", function(event){

        // Entering fullscreen
        if(event.eventPhase == 3) {

            $('.ui.modals').removeClass('dimmer');
            $('.course-content-modal').removeClass('modal');

        // Exiting fullscreen
        } else if(event.eventPhase == 2) {

            $('.ui.modals').addClass('dimmer');
            $('.course-content-modal').addClass('modal');
        }
    });

}(jQuery));