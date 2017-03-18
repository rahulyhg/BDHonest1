<?php

require_once(dirname(__FILE__) . '/../../dss.module');

$plugin = array(
	'single' => TRUE,  // Just do this one, it is needed.
	'title' => t('Lesson Content'),  // Title to show up on the pane screen.
	'description' => t('Lesson Content page'), // Description to show up on the pane screen.
	'category' => t('DSS'), // A category to put this under.
	'edit form' => 'dss_pane_lesson_content_pane_edit_form', // A function that will return the settings form for the pane.
	'render callback' => 'dss_pane_lesson_content_pane_render', // A function that will return the renderable content.
	'admin info' => 'dss_pane_lesson_content_pane_admin_info', // A function that will return the information displayed on the admin screen (optional).
	'defaults' => array( // Array of defaults for the settings form.
		'section' => 'content',
		),
	'all contexts' => TRUE, // This is NEEDED to be able to use substitution strings in your pane.
	);


/**
 * An edit form for the pane's settings.
 */
function dss_pane_lesson_content_pane_edit_form($form, &$form_state) {

	$conf = $form_state['conf'];

	$form['section'] = array(
		'#type' => 'radios',
		'#title' => t('Section'),
		'#description' => t('Select the section in Lesson Content Page'),
		'#options' => array(
			'header' => t('Header'),
			'content' => t('Content'),
			),
		'#default_value' => isset($conf['section']) ? $conf['section'] : 'content',
		);

	return $form;
}

/**
 * Submit function, note anything in the formstate[conf] automatically gets saved
 * Notice, the magic that automatically does that for you.
 */
function dss_pane_lesson_content_pane_edit_form_submit(&$form, &$form_state) {

	foreach (array_keys($form_state['plugin']['defaults']) as $key) {
		if (isset($form_state['values'][$key])) {
			$form_state['conf'][$key] = $form_state['values'][$key];
		}
	}
}

/**
 * 'admin info' callback for panel pane.
 */
function dss_pane_lesson_content_pane_admin_info($subtype, $conf, $contexts) {
	if (!empty($conf)) {
		$block = new stdClass;
		$block->title = $conf['override_title'] ? $conf['override_title_text'] : '';
		$block->content = $conf['text'];
		return $block;
	}
}

/**
 * Run-time rendering of the body of the block (content type)
 * See ctools_plugin_examples for more advanced info
 */
function dss_pane_lesson_content_pane_render($subtype, $conf, $args, $contexts) {

	global $user;

	if(!isset($contexts['argument_entity_id:node_1']) || !isset($contexts['relationship_entity_from_field:field_associated_chapter-node-node_1']) || !isset($contexts['relationship_entity_from_field:field_associated_course-node-node_1'])) {

		$block = new stdClass();

	 	// initial content is blank
		$block->title = $conf['override_title'] ? $conf['override_title_text'] : '';
		$block->content = 'This lesson is not associated with a course';

		return $block;
	}

	$associated_lesson = entity_metadata_wrapper('node', $contexts['argument_entity_id:node_1']->data);
	$associated_chapter = entity_metadata_wrapper('node', $contexts['relationship_entity_from_field:field_associated_chapter-node-node_1']->data);
	$associated_course = entity_metadata_wrapper('node', $contexts['relationship_entity_from_field:field_associated_course-node-node_1']->data);
	$course_materials = $associated_lesson->field_course_materials->value();

	switch($conf['section']) {
		case 'header':
			return dss_pane_lesson_header_content($conf, $associated_course, $associated_chapter, $associated_lesson, $course_materials);
		default:
			return dss_pane_lesson_main_content($conf, $associated_course, $associated_chapter, $associated_lesson, $course_materials);
	}
}

function dss_pane_lesson_header_content($conf, $associated_course, $associated_chapter, $associated_lesson, $course_materials) {

	$breadcrumb_links = array(
		array (
			'title' => $associated_course->title->value(),
			'url' => url('node/' . $associated_course->getIdentifier()) . '/content',
			),
		array(
			'title' => $associated_course->field_chapter_label->value() . ' ' . $associated_chapter->field_chapter_number->value() /* . ': ' . $associated_chapter->title->value() */,
			'url' => url('node/' . $associated_chapter->getIdentifier()),
			),
		array(
			'title' => $associated_course->field_lesson_label->value() . ' ' . $associated_lesson->field_lesson_number->value() /* . ': ' . $associated_lesson->title->value() */,
			// 'url' => url('node/' . $associated_lesson->getIdentifier()),
			),
		);

		// $cover_img_url = '/sites/all/themes/vedic/images/no-image.png';
		// if($associated_lesson->field_cover_image->value()['uri']) {
		// 	$cover_img_url = file_create_url($associated_lesson->field_cover_image->value()['uri']);
		// } else if($associated_chapter->field_cover_image->value()['uri']) {
		// 	$cover_img_url = file_create_url($associated_chapter->field_cover_image->value()['uri']);
		// } else if($associated_course->field_cover_image->value()['uri']) {
		// 	$cover_img_url = file_create_url($associated_course->field_cover_image->value()['uri']);
		// }

		$content = '<h1 class="ui header progress-header has-breadcrumb">' .
			'<div class="header-content">' .
			dss_breadcrumb($breadcrumb_links) .
			'<div class="title welcome-decorative">' . $associated_lesson->title->value() . dss_get_edit_link($associated_lesson->getIdentifier()) . '</div>
			</div>
			</h1>
			<div data-percent="0" class="ui bottom attached progress lesson-progress"><div style="transition-duration: 300ms; width: 0%;" class="bar"></div></div>';

	$block = new stdClass();

 	// initial content is blank
	$block->title = isset($conf['override_title']) && $conf['override_title'] ? $conf['override_title_text'] : '';
	$block->content = $content;

	return $block;
}

function dss_pane_lesson_main_content($conf, $associated_course, $associated_chapter, $associated_lesson, $course_materials) {

	global $user;
	$chapter_label = $associated_course->field_chapter_label->raw();
	$lesson_label = $associated_course->field_lesson_label->raw();

	$course_progress = dss_get_course_progress_entity($user->uid, $associated_course->getIdentifier(), true);
	$progress = json_decode($course_progress->body->value->value(array('decode' => TRUE)), true);

	$lesson_completed = dss_get_percent_completed_course_lesson($progress, $associated_lesson);
	$is_content_editor = dss_is_content_editor();
	$edit_destination = drupal_get_path_alias(current_path());
	// dpm($associated_lesson->field_cover_image->value());

	$content = '';

	// if(!empty($associated_lesson->body->value())) {
	// 	$content .= $associated_lesson->body->value->value();
	// 	$content = '<div class="feather-note">
	// 		<div class="sprite feather-pen"></div>
	// 		' . $associated_lesson->body->value->value() . '
	// 		</div>';
	// }
	// $content = '<h2 class="ui header flute-decorative center">' . $lesson_label . ' Content</h2>';

	$content .= '<div id="lesson-meta" class="hidden '
		. '" data-lesson-nid="'
		. $associated_lesson->getIdentifier()
		. '" data-course-nid="'
		. $associated_course->getIdentifier()
		. '" data-current-lesson="'
		. ($associated_lesson->getIdentifier() == $course_progress->field_current_lesson->getIdentifier())
		. '" data-autoplay-slides="'
		. dss_get_autoplay($progress)
		. '" data-lesson-completed="'
		. $lesson_completed
		. '"></div>';

	$content .= '
	<div class="clearfix">
	<div class="lesson-content">
	<h2 class="ui header items-list-header title-decorative">Lesson Content</h2>
	<div class="course-materials-list">';

	$no_links_anymore = false; // For enforce completion

	$slide_num = 0;

	$course_materials_main_content = '';
	$nav_content = '';
	$nav_item_count = 1;

	$dss_access_check_disabled = $is_content_editor || variable_get('dss_access_check_disabled', false);

	foreach ($course_materials as $key => $course_material_node) {

		$course_material = entity_metadata_wrapper('node', $course_material_node);
		$type = entity_metadata_wrapper('taxonomy_term', $course_material->field_course_material_type->value());
		$type_icon_class = $type->field_mini_icon_class->raw();
		$course_material_completed = dss_get_percent_completed_course_material($progress, $course_material);
		$li_class = '';

		$video_minutes = $course_material->field_video_minutes->raw();
		$url = url('node/' . $course_material->getIdentifier());

		$is_once = dss_is_material_acessible_once($type->getIdentifier(), $associated_course);

		if($no_links_anymore) {
			// Waiting because previous course material is enforced
			$li_class .= 'material-disabled enforced-waiting ';
			$url = '';
		} else if($is_once && $lesson_completed >= 100 && !dss_is_once_bypass($course_progress, $associated_lesson, $associated_chapter, $associated_course)) {
			// Can be watched only once and the chapter / lesson is beyond the allowed scope
			$li_class .= 'material-disabled is-once ';
			$url = '';
		}
		
		// Updation for hiding video lecture of completed lesson.
		if($lesson_completed >= 100 && $type->name->raw() == 'Video Lecture' ) {
			
			$num_prev_chapters_accessible = $associated_course->field_prev_chapters_accessible->value();
			$num_prev_lessons_accessible = $associated_course->field_prev_lessons_accessible->value();
			$associated_chapter_no = $associated_chapter->field_chapter_number->raw();
			$associated_lesson_no = $associated_lesson->field_lesson_number->raw();
			$current_chapter_no = $course_progress->field_current_chapter_number->raw();
			$current_lesson_no = $course_progress->field_current_lesson_number->raw();
			
			if($associated_chapter_no < $current_chapter_no - $num_prev_chapters_accessible || $associated_lesson_no < $current_lesson_no - $num_prev_lessons_accessible){
				$li_class .= 'material-disabled video-once  ';
				$url = '';
			}
		}

		if($course_material_completed == 100) {
			$li_class .= 'completed ';
		}

		$is_enforced = dss_is_enforce_completion($type->getIdentifier(), $associated_course);

		if(!$dss_access_check_disabled && $is_enforced && $course_material_completed < 100) {
			// Do not allow next course materials to have links
			$li_class .= 'enforced ';
			$no_links_anymore = true;
		}      
      
      if($type->name->raw() =='Exercise'){
      	$excercise_result =  isset($progress['materials'][$course_material->getIdentifier()]['completed']) ? $progress['materials'][$course_material->getIdentifier()]['completed'] :'';
         global $user;
         $uid = $user->uid;
         $identifier_id = $course_material->getIdentifier();
         $userpoints_sql = "select txn_id from userpoints_txn where uid =".$uid. " and entity_id =".$identifier_id." and tid =69";
         $excercise_points = db_query($userpoints_sql)->fetchField();
         if(empty($excercise_points) && $excercise_result >= 100){
           $params = array(
              'uid' => $uid,
              'points' => 50,
              'operation' => 'admin',
              'entity_id' =>$identifier_id ,
              'tid' =>69,
              'entity_type' =>"node" ,
           );
           userpoints_userpointsapi($params);
         }
       }
     
		$content .= '<div class="link course-material-item open-course-material ' . $li_class . '" data-slide-num="' . $slide_num . '" data-type="' . $type->name->raw() . '" data-nid="' . $course_material->getIdentifier() . '">'
		. '<i class="type-icon ' . $type_icon_class . ' large"></i>'
		. '<span class="title">' . $type->name->value()
		. ($is_content_editor ? ' &nbsp; <a href="/node/' . $course_material->getIdentifier() . '/edit?destination=' . $edit_destination . '" class="edit-link"></a>' : '')
		. '</span>'
		. ($video_minutes ? ' <span class="video-minutes">' . $video_minutes . ' mins</span>' : '')
		. ' <i class="item-progress checkmark icon"></i>'
		. '</div>';

		$slides_content = (array) dss_pane_course_material_html($course_material);
		foreach ($slides_content as $slide_content) {
			$course_materials_main_content .= '<div class="slide-holder" data-type="' . $type->name->value() . '" data-nid="' . $course_material->getIdentifier() . '">' . $slide_content . '</div>';
		}

		$type_title = $type->name->value();
		// $type_icon_class = $type->field_mini_icon_class->raw();
		$nav_content .= '<div class="item link open-course-material '
			. $li_class
			. '" data-slide-num="' . $slide_num
			. '" data-type="' . $type_title
			. '" data-nid="' . $course_material->getIdentifier()
			. '"><span class="ui circular label">' . $nav_item_count++
			. '</span><span class="nav-title">' . $type_title
			. ($is_content_editor ? ' &nbsp; <a href="/node/' . $course_material->getIdentifier() . '/edit?destination=' . $edit_destination . '" class="edit-link"></a>' : '')
			. ' <i class="item-progress checkmark icon"></i></span>'
			. '</div>';

		$slide_num += count($slides_content);
	}

	$content .= '
		</div>';

    // Lesson completion message
    $course_materials_main_content .= '<div class="slide-holder" data-type="Lesson Completion" data-nid="-1"><div class="lesson-completion-material text-container editor-content">'
        . '<p class="ui message info">Congratulations! You have completed your lesson for the day.</p>';

    if($associated_lesson->field_message_of_the_day->value() != null) {
            // $course_materials_main_content .= '<h3><strong class="message-of-day" style="">Message of the Day</strong></h3>';
            $course_materials_main_content .= $associated_lesson->field_message_of_the_day->value->value();
    }

    $course_materials_main_content .= '</div></div>';
    $slide_num++; // For the extra "Lesson Completion Message" slide

	// For extra materials
	$extra_materials_slide_num = $slide_num;
	$extra_materials_common = $associated_course->field_extra_materials->value();
	$extra_materials = $associated_lesson->field_extra_materials->value();

	$extra_materials = array_merge($extra_materials_common, $extra_materials);

	$content .= '<br>
	<h2 class="ui header items-list-header title-decorative">' . $associated_course->field_extra_materials_label->raw() . '</h2>
	<div class="course-materials-extra-list">';
   $extra_materials_total =0;
	foreach ($extra_materials as $key => $extra_material_node) {

		$extra_material = entity_metadata_wrapper('node', $extra_material_node);
		$type = entity_metadata_wrapper('taxonomy_term', $extra_material->field_extra_material_type->value());
		$type_icon_class = $type->field_mini_icon_class->raw();
		$url = ''; //url('node/' . $extra_material->getIdentifier());
		$extra_material_completed = dss_get_percent_completed_course_material_extra($progress, $associated_lesson->getIdentifier(), $extra_material->getIdentifier());

		$li_class = '';
		if($extra_material_completed >= 100) {
			$li_class = 'completed ';
		}
		
      $extra_materials_total += $extra_material_completed;
		$content .= '<div class="link course-material-item course-material-extra open-course-material ' . $li_class . '" data-slide-num="' . $slide_num . '" data-type="' . $type->name->raw() . '" data-video-id="' . $extra_material->field_video_url->raw() . '" data-nid="' . $extra_material->getIdentifier() . '">'
		. '<i class="type-icon ' . $type_icon_class . ' large"></i>'
		. '<span class="title">' . $extra_material->title->raw()
		. ($is_content_editor ? ' &nbsp; <a href="/node/' . $extra_material->getIdentifier() . '/edit?destination=' . $edit_destination . '" class="edit-link"></a>' : '')
		. '</span>'
		. ' <i class="item-progress checkmark icon"></i>'
		. '</div>';

		$slides_content = (array) dss_pane_extra_material_html($extra_material);
		foreach ($slides_content as $slide_content) {
			$course_materials_main_content .= '<div class="slide-holder" data-type="Extra Material" data-nid="' . $extra_material->getIdentifier() . '">' . $slide_content . '</div>';
		}
		$slide_num++;   
	}
	if($extra_materials_total >=200){
		foreach ($extra_materials as $exkey => $extra_material_total_node) {
			 $extra_materials_data = entity_metadata_wrapper('node', $extra_material_total_node);
			 $extra_identifier_id = $extra_materials_data->getIdentifier();			 
			 global $user;
          $uid = $user->uid;
	       $extrauserpoints_sql = "select txn_id from userpoints_txn where uid =".$uid. " and entity_id =".$extra_identifier_id." and tid =70";
	       $extra_points = db_query($extrauserpoints_sql)->fetchField();
	       if(empty($extra_points)){
	          $extra_params = array(
              'uid' => $uid,
              'points' => 25,
              'operation' => 'admin',
              'entity_id' =>$extra_identifier_id ,
              'tid' =>70,
              'entity_type' =>"node" ,
           );
           userpoints_userpointsapi($extra_params);
	       }
		}	
	}
   if($lesson_completed == 100){
        global $user;
        $uid = $user->uid;
        $id = arg(1);
        $completepoints_sql = "select txn_id from userpoints_txn where uid =".$uid. " and entity_id =".$id." and tid =68";
	     $complete_points = db_query($completepoints_sql)->fetchField();
	     if(empty($complete_points)){
	     	$complete_params = array(
              'uid' => $uid,
              'points' => 100,
              'operation' => 'admin',
              'entity_id' =>$id ,
              'tid' =>68,
              'entity_type' =>"node" ,
           );
           userpoints_userpointsapi($complete_params);
	     }
   }
	$content .= '</div>';

  	// Navigation
	// $number_to_word = array(
	// 	1 => 'one',
	// 	2 => 'two',
	// 	3 => 'three',
	// 	4 => 'four',
	// 	5 => 'five',
	// 	6 => 'six',
	// 	7 => 'seven',
	// 	8 => 'eight',
	// 	9 => 'nine',
	// 	10 => 'ten',
	// 	);
	// $nav_size = $number_to_word[count($nav_content) + 1];

	$modal_nav_content = '<div class="ui menu course-material-menu">';
	$modal_nav_content .= $nav_content;
	// dpm($associated_course->value());
	$modal_nav_content .= '<div class="item link open-course-material" data-slide-num="' . $extra_materials_slide_num
		. '" data-type="Extra Material"><span class="ui circular label">' . $nav_item_count
		. '</span><span class="nav-title">' . $associated_course->field_extra_materials_label->raw()
		. ($is_content_editor ? ' &nbsp; <a href="/node/' . $associated_lesson->getIdentifier() . '/edit?destination=' . $edit_destination . '" class="edit-link"></a>' : '')
		. '<i class="item-progress checkmark icon"></i></span></div>';
	$modal_nav_content .= '</div>';

	$prev_link = 1; $next_link = 1;

	$nav_btns = '<a class="prev-nav ' . (!$prev_link ? 'disabled' : '') . '"><i class="icon long arrow left"></i> Previous</a>'
		. '<a class="next-nav ' . (!$next_link ? 'disabled' : '') . '">Next <i class="icon long arrow right"></i></a>';

	// Review Status
	$review_html = '';
	if($is_content_editor) {

		$review_status = entity_metadata_wrapper('taxonomy_term', $associated_lesson->field_review_status->value());
		$author = user_load($associated_lesson->value()->uid);

		$review_html .= '<br><div class="ui message info"><h3 class="ui heading">Review Status</h3>';

		if($review_status != null && isset($review_status->field_label_class)) {

			$review_html .= '<div><span class="ui tag label ' . $review_status->field_label_class->raw()
				. '">' . $review_status->name->value()
				. '</span></div>';
		}

		$review_html .= '<p><b>Author:</b> ' . $author->name . '</p>';
		$review_html .= '<p>' . (!empty($associated_lesson->field_review_comments->value()) ? $associated_lesson->field_review_comments->value->value() : '') . '</p>
			</div>';
	}

	// Display modal main content - hidden
	$content .= '<div class="ui large modal course-content-modal init-slider">
  		<i class="close icon"></i>
  		<div class="header"><div class="modal-page-width"><span class="title">' . $associated_lesson->title->raw() . '</span>' . $modal_nav_content . '</div></div>
  		<div id="modal-content" class="content"><div class="modal-page-width"><div class="slides course-material-slides">' . $course_materials_main_content . '</div></div></div>
			<div class="actions clearfix"><div class="modal-page-width">
  			<i class="icon angle down scroll-indicator" id="scroll-indicator"></i>'
		. $nav_btns
		. '</div></div>
		</div>';

	$cover_img_url = '/sites/all/themes/vedic/images/no-image.png';
	if($associated_lesson->field_cover_image->value()['uri']) {
		$cover_img_url = file_create_url($associated_lesson->field_cover_image->value()['uri']);
	} else if($associated_chapter->field_cover_image->value()['uri']) {
		$cover_img_url = file_create_url($associated_chapter->field_cover_image->value()['uri']);
	} else if($associated_course->field_cover_image->value()['uri']) {
		$cover_img_url = file_create_url($associated_course->field_cover_image->value()['uri']);
	}

	$content .= $review_html . '
			</div>
			<div class="lesson-right-content">
				<img src="' . $cover_img_url . '" alt="" />
				<div class="lesson-description">' . (!empty($associated_lesson->body->value()) ? $associated_lesson->body->value->value() : '') . '</div>
			</div>
		</div>';

	$block = new stdClass();

 	// initial content is blank
	$block->title = $conf['override_title'] ? $conf['override_title_text'] : '';
	$block->content = $content;

	return $block;
}

function dss_dummy_video() {
	return '<div class="dummy-video"></div>';
}

function dss_pane_course_material_html($course_material) {

	// Writing title
	$content = '';
	$type = entity_metadata_wrapper('taxonomy_term', $course_material->field_course_material_type->value());
	// print_r($type); die();
	$type_name = $type->field_term_machine_name->value();


	// $content .= '<div class="course-material-container ' . $type_name . '" data-material-type="' . $type_name . '"><div class="navigation-container"><div class="course-material">';

	if(in_array($type_name, array('video_vimeo'))) {

		if(variable_get('offline_dev_mode', false)) {
			$content .= dss_dummy_video();
		} else {

			$video_url = '//player.vimeo.com/video/' . $course_material->field_video_url->value();

			$content .= '<div class="video-bg"><div class="video-wrapper" data-video-src="' . $video_url . '">
			<div class="ui dimmer video-dimmer">
               <div class="ui indeterminate text loader">Loading...</div>
            </div>
            <iframe src="about:blank" width="720" height="480" frameborder="0" allowfullscreen="true" webkitallowfullscreen="true" mozallowfullscreen="true"></iframe>
			</div></div>';

			if(isset($text_material[0]['value'])) {
				$content .= '<div class="text-container editor-content video-text"><div class="text-material">';
				foreach ($text_material as $key => $material) {
					$content .= '<div class="slide slide-' . $key . '">' . $material['value'] . '</div>';
				}
				$content .= '</div></div>';
			}
		}

	} else if(in_array($type_name, array('lesson_review'))) {

		$text_material = $course_material->field_text_material->value();
		$text_material_audio = $course_material->field_text_material_audio->value();

		$content = array();
		foreach ($text_material as $key => $material) {

			$audio_html = '';
			if(isset($text_material_audio[$key])) {
				$uri = $text_material_audio[$key]['uri'];
				$url = file_create_url($uri);
				$audio_html = '
				<div class="audio-wrapper clearfix" data-audio-src="' . $url . '">
					<div class="ui toggle checkbox" data-content="Autoplay audio slides">
						<input name="autoplay" class="autoplay" type="checkbox">
						<label>Autoplay</label>
					</div>
				</div>';
			}

			$text_material_slide = '<div class="text-material text-container editor-content">'
				. $audio_html
				. $material['value']
				. '</div>';
			$content []= $text_material_slide;
		}

	} else if($type_name == 'exercise') {

		$quiz_questions = $course_material->field_quiz_questions->value();

		$content = array();
		foreach ($quiz_questions as $key => $quiz_question_node) {

			$question_slide = '<div class="quiz-question clearfix text-container editor-content" data-nid="' . $quiz_question_node->nid . '" data-attempts="0">
					' . dss_quiz_question_html($quiz_question_node)
                    . '</div>';
			$content []= $question_slide;
		}

	}

	// $content .= '</div></div></div>';

	return $content;
}

function dss_pane_extra_material_html($extra_material) {

	if(variable_get('offline_dev_mode', false)) {
		return dss_dummy_video();
	}

	return '<div class="video-bg"><div class="video-wrapper" data-video-src="//player.vimeo.com/video/' . $extra_material->field_video_url->value() . '">
		<div class="ui dimmer video-dimmer">
           <div class="ui indeterminate text loader">Loading...</div>
        </div>
        <iframe src="about:blank" width="720" height="480" frameborder="0" allowfullscreen="true" webkitallowfullscreen="true" mozallowfullscreen="true"></iframe>
        </div></div>';
}

function dss_quiz_question_html($question_node) {

	$question = entity_metadata_wrapper('node', $question_node);
	// dpm($question->value());

	$type = entity_metadata_wrapper('taxonomy_term', $question->field_question_type->value());
	$question_type_name = strtolower($type->field_term_machine_name->value());

    $question_content = '';
    $question_content .= ($question->body->value() ? '<p class="question-body">' . ($question->body->value() ? $question->body->value->value() : '') . '</p>' : '');

	switch ($question_type_name) {
		case 'multiple-choice':

			$question_content .= '<p class="options">';
            $type = count($question->field_correct_answers->raw()) > 1 ? 'checkbox' : 'radio';
			$options = $question->field_question_options->value();

			foreach ($options as $key => $option) {
				$question_content .= '<label class="option-label"><input type="' . $type . '" name="ans" value="' . ($key + 1) . '"> ' . $option . '</label>';
			}
			$question_content .= '</p><input type="button" class="ui primary button btn-2 multiple-choice-submit" value="Submit">';

			break;

		case 'match':

            $question_content .= '<p class="match-hint">Drag an item on the left to the correct matching option on the right</p>';
            $question_content .= '<div class="match-items">';

			$items = $question->field_question_items->value();
            foreach ($items as $key => $item) {
				$question_content .= '<div class="match-item" data-item-key="' . $key . '" data-attempts="0"> ' . $item . '</div>';
			}

            $question_content .= '</div>';

            $question_content .= '<div class="match-options">';

			$options = $question->field_question_options->value();
            foreach ($options as $key => $option) {
				$question_content .= '<div class="match-option" data-option-value="' . ($key + 1) . '"><div class="option-text">' . $option . '</div></div>';
			}

            $question_content .= '</div>';
			# code...
			break;

		default:
			# code...
			break;
	}

    $content = '';
    $content .= '<form class="' . $question_type_name . '" action="">';

    $content .= '<div class="question-content">' . $question_content . '</div></form>';

	return $content;
}
