<?php

module_load_include('inc', 'dss', 'dss.helper');

$plugin = array(
	'single' => TRUE,  // Just do this one, it is needed.
	'title' => t('Chapter Content'),  // Title to show up on the pane screen.
	'description' => t('Chapter Content page'), // Description to show up on the pane screen.
	'category' => t('DSS'), // A category to put this under.
	'edit form' => 'dss_pane_chapter_content_pane_edit_form', // A function that will return the settings form for the pane.
	'render callback' => 'dss_pane_chapter_content_pane_render', // A function that will return the renderable content.
	'admin info' => 'dss_pane_chapter_content_pane_admin_info', // A function that will return the information displayed on the admin screen (optional).
	'defaults' => array( // Array of defaults for the settings form.
		'section' => 'content',
		),
	'all contexts' => TRUE, // This is NEEDED to be able to use substitution strings in your pane.
	);


/**
 * An edit form for the pane's settings.
 */
function dss_pane_chapter_content_pane_edit_form($form, &$form_state) {
	$conf = $form_state['conf'];

	$form['section'] = array(
		'#type' => 'radios',
		'#title' => t('Section'),
		'#description' => t('Select the section in Chapter Content Page'),
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
function dss_pane_chapter_content_pane_edit_form_submit(&$form, &$form_state) {
	foreach (array_keys($form_state['plugin']['defaults']) as $key) {
		if (isset($form_state['values'][$key])) {
			$form_state['conf'][$key] = $form_state['values'][$key];
		}
	}
}

/**
 * 'admin info' callback for panel pane.
 */
function dss_pane_chapter_content_pane_admin_info($subtype, $conf, $contexts) {
	// if (!empty($conf)) {
	// 	$block = new stdClass;
	// 	$block->title = $conf['override_title'] ? $conf['override_title_text'] : '';
	// 	$block->content = $conf['text'];
	// 	return $block;
	// }
}

/**
 * Run-time rendering of the body of the block (content type)
 * See ctools_plugin_examples for more advanced info
 */
function dss_pane_chapter_content_pane_render($subtype, $conf, $args, $contexts) {

	global $user;

	if(!isset($contexts['argument_entity_id:node_1'])) {

		$block = new stdClass();

	 	// initial content is blank
		$block->title = $conf['override_title'] ? $conf['override_title_text'] : '';
		// $block->content = 'Something wrong with this';

		return $block;
	}

	$associated_chapter = entity_metadata_wrapper('node', $contexts['argument_entity_id:node_1']->data);
	$course_lessons = $associated_chapter->field_course_lessons->value();
	$associated_course = entity_metadata_wrapper('node', $contexts['relationship_entity_from_field:field_associated_course-node-node_1']->data);
	$course_progress = dss_get_course_progress_entity($user->uid, $associated_course->getIdentifier(), true);

	switch ($conf['section']) {
		case 'header':
			return dss_pane_chapter_header_content($conf, $associated_course, $associated_chapter, $course_lessons, $course_progress);
			break;

		default:
			return dss_pane_chapter_main_content($conf, $associated_course, $associated_chapter, $course_lessons, $course_progress);
			break;
	}
}

function dss_pane_chapter_header_content($conf, $associated_course, $associated_chapter, $course_lessons, $course_progress) {

	$breadcrumb_links = array(
		array (
			'title' => $associated_course->title->value(),
			'url' => url('node/' . $associated_course->getIdentifier()) . '/content',
			),
		array(
			'title' => $associated_course->field_chapter_label->value() . ' ' . $associated_chapter->field_chapter_number->value() /* . ': ' . $associated_chapter->title->value() */,
			// 'url' => url('node/' . $associated_course->getIdentifier()) . '/content',
			),
		);

	// $cover_img_url = '/sites/all/themes/vedic/images/no-image.png';
	// if($associated_chapter->field_cover_image->value()['uri']) {
	// 	$cover_img_url = file_create_url($associated_chapter->field_cover_image->value()['uri']);
	// } else if($associated_course->field_cover_image->value()['uri']) {
	// 	$cover_img_url = file_create_url($associated_course->field_cover_image->value()['uri']);
	// }

	$content = '<h1 class="ui header progress-header has-breadcrumb">' .
		'<div class="header-content">' .
		dss_breadcrumb($breadcrumb_links) .
		'<div class="title welcome-decorative">' . $associated_chapter->title->value() . dss_get_edit_link($associated_chapter->getIdentifier()) . '</div>
		</div>
		</h1>
		<div data-percent="0" class="ui bottom attached progress lesson-progress"><div style="transition-duration: 300ms; width: 0%;" class="bar"></div></div>';

	$block = new stdClass();

	$block->title = $conf['override_title'] ? $conf['override_title_text'] : '';
	$block->content = $content;

	return $block;
}

function dss_pane_chapter_main_content($conf, $associated_course, $associated_chapter, $course_lessons, $course_progress) {

	global $user;

	$chapter_label = $associated_course->field_chapter_label->raw();
	$lesson_label = $associated_course->field_lesson_label->raw();

	$cur_chapter = $course_progress->field_current_chapter_number->raw();
	$cur_lesson = $course_progress->field_current_lesson_number->raw();
	// dpm($associated_course->value());
	// dpm($course_progress->value());

	$progress = json_decode($course_progress->body->value->value(array('decode' => TRUE)), true);
	$dss_access_check_disabled = dss_is_content_editor() && variable_get('dss_disable_access_check_admin', false);

	$content = '';
	// $content = '<h2 class="ui header flute-decorative center">' . $chapter_label . ' Content</h2>';

	if(!empty($associated_chapter->body->value())) {
		$content .= $associated_chapter->body->value->value();
		// $content = '<div class="feather-note">
		// 	<div class="sprite feather-pen"></div>
		// 	' . $associated_chapter->body->value->value() . '
		// 	</div>';
	}

	$content .= '<div id="course-meta" class="hidden '
		. '" data-course-nid="'
		. $associated_course->getIdentifier()
		. '" data-current-lesson-nid="'
		. $course_progress->field_current_lesson->getIdentifier()
		. '" data-autoplay-slides="'
		. dss_get_autoplay($progress)
		. '" data-course-completed="'
		. $course_progress->field_percent_completed->raw()
		. '"></div>';

	// $content .= '<div class="course-item-description">' . (!empty($associated_course->field_course_description->value()) ? $associated_course->field_course_description->value->value(array('decode' => false)) : '') . '</div>';

	$content .= '<div class="cards-content"><div class="ui cards">';
	$is_content_editor = dss_is_content_editor();

	foreach ($course_lessons as $key => $course_lesson_node) {

		if(empty($course_lesson_node)) continue;

		$course_lesson = entity_metadata_wrapper('node', $course_lesson_node);
		$course_lesson_completed = dss_get_percent_completed_course_lesson($progress, $course_lesson);

		$url = url('node/' . $course_lesson->getIdentifier());
		$card_class = 'course-lesson-item ';
		$disabled = false;

		// Check if doesn't have access to the current lesson
		if(!$dss_access_check_disabled && ($associated_chapter->field_chapter_number->raw() > $course_progress->field_current_chapter_number->raw() || $associated_chapter->field_chapter_number->raw() == $course_progress->field_current_chapter_number->raw() && $course_lesson->field_lesson_number->raw() > $course_progress->field_current_lesson_number->raw())) {
			$url = '';
			$card_class .= 'not-available';
			$disabled = true;
		}

		$cur = false;
		// If this is the current course lesson
		if($course_lesson->getIdentifier() == $course_progress->field_current_lesson->raw()) {
			$user_timezone = drupal_get_user_timezone();
			$next_avail_date = new DateTime();
			$next_avail_date->setTimezone(new DateTimeZone($user_timezone));
			$next_avail_date->setTimestamp(intval($course_progress->field_date_next_lesson_avail->value()));
			if ($course_lesson_completed == 100 && $next_avail_date > new DateTime()) {
				drupal_set_message('Your next lesson will be available on ' . $next_avail_date->format('M d \\a\\t H:i:s T'));
			} else if ($course_lesson_completed == 0) { // Lesson not started
				drupal_set_message('Your next lesson is now available!');
			} else if ($course_lesson_completed < 100) { // Lesson not completed
				drupal_set_message("You are currently in 'Lesson " . $course_lesson->field_lesson_number->raw() . ": " . $course_lesson->title->raw() . "'. You need to complete your current lesson to move to the next lesson.");
			}
			$card_class .= 'cur ';
			$cur = true;
		}

		$review_status_label = '';
		if($is_content_editor) {
			$review_status = entity_metadata_wrapper('taxonomy_term', $course_lesson->field_review_status->value());

			if($review_status != null && isset($review_status->field_label_class)) {
				$review_status_label .= '<span class="ui tag label ' . $review_status->field_label_class->raw()
					. '">' . $review_status->name->value()
					. '</span>';
			}
		}

		$img_url = '/sites/all/themes/vedic/images/no-image.png';
		if($course_lesson->field_cover_image->value()['uri']) {
			$img_url = file_create_url($course_lesson->field_cover_image->value()['uri']);
		}

		// dpm($course_lesson->value());

		// Card content
		$content .= '
			<div class="ui fluid card course-card ' . $card_class . '" data-chapter-nid="' . $course_lesson->getIdentifier() . '">
			<div class="content">
				<div class="meta">
				' . $lesson_label . ' ' . $course_lesson->field_lesson_number->raw() . dss_get_edit_link($course_lesson->getIdentifier()) . '
				</div>
				<div class="header">
			<a href="' . $url . '">' . $course_lesson->title->raw() . '</a>
			</div>
				' . $review_status_label . '
			</div>
			<a href="' . $url . '" class="image">
				<div style="background-image: url(\'' . $img_url . '\')"></div>
			</a>
			<div class="extra content">';

		if ($disabled) {
			$content .= '<i class="lock icon right floated"></i>';
		} else {
			$btn_text = 'View';
			$btn_class = 'ui button';
			if ($cur && $course_lesson_completed == 0) { // Lesson not started yet
				$btn_text = 'Start';
				$btn_class .= ' active';
			} else if ($cur && $course_lesson_completed < 100) { // Lesson in progress and not completed
				$btn_text = 'Resume';
				$btn_class .= ' active';
			}
			$content .= '<span class="right floated">
				<a href="' . $url . '" class="' . $btn_class . '">' . $btn_text . '</a>
				</span>';
		}

		$content .= '<span>
			' . ($course_lesson_completed >= 100 ? ' <i class="item-progress checkmark icon"></i>' : '<i class=" video outline icon"></i>') . $course_lesson->field_video_minutes->raw() . ' mins
			</span>
			</div>
			</div>';

		// if($key == 1) break;
	}

	$content .= '</div></div>';
	// $content .= '<div class="cover-image paper-cutting white"><div class="paper-cutting-inner white"><img src="' . $cover_img_url . '"></div></div>';

	$block = new stdClass();

 	// initial content is blank
	$block->title = $conf['override_title'] ? $conf['override_title_text'] : '';
	$block->content = $content;

	return $block;
}
