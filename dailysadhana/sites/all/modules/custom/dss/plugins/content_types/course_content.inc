<?php

module_load_include('inc', 'dss', 'dss.helper');

$plugin = array(
	'single' => TRUE,  // Just do this one, it is needed.
	'title' => t('Course Content'),  // Title to show up on the pane screen.
	'description' => t('Course Content page'), // Description to show up on the pane screen.
	'category' => t('DSS'), // A category to put this under.
	'edit form' => 'dss_pane_course_content_pane_edit_form', // A function that will return the settings form for the pane.
	'render callback' => 'dss_pane_course_content_pane_render', // A function that will return the renderable content.
	'admin info' => 'dss_pane_course_content_pane_admin_info', // A function that will return the information displayed on the admin screen (optional).
	'defaults' => array( // Array of defaults for the settings form.
		'section' => 'content',
		),
	'all contexts' => TRUE, // This is NEEDED to be able to use substitution strings in your pane.
	);


/**
 * An edit form for the pane's settings.
 */
function dss_pane_course_content_pane_edit_form($form, &$form_state) {
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
function dss_pane_course_content_pane_edit_form_submit(&$form, &$form_state) {
	foreach (array_keys($form_state['plugin']['defaults']) as $key) {
		if (isset($form_state['values'][$key])) {
			$form_state['conf'][$key] = $form_state['values'][$key];
		}
	}
}

/**
 * 'admin info' callback for panel pane.
 */
function dss_pane_course_content_pane_admin_info($subtype, $conf, $contexts) {
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
function dss_pane_course_content_pane_render($subtype, $conf, $args, $contexts) {


	global $user;

	if(!isset($contexts['argument_entity_id:node_1'])) {

		$block = new stdClass();

	 	// initial content is blank
		$block->title = $conf['override_title'] ? $conf['override_title_text'] : '';
		// $block->content = 'Something wrong with this';

		return $block;
	}

	$associated_course = entity_metadata_wrapper('node', $contexts['argument_entity_id:node_1']->data);
	$course_chapters = $associated_course->field_course_chapters->value();
	$course_progress = dss_get_course_progress_entity($user->uid, $associated_course->getIdentifier(), true);

	switch ($conf['section']) {
		case 'header':
			return dss_pane_course_header_content($conf, $associated_course, $course_chapters, $course_progress);
			break;

		default:
			return dss_pane_course_main_content($conf, $associated_course, $course_chapters, $course_progress);
			break;
	}
}

function dss_pane_course_header_content($conf, $associated_course, $course_chapters, $course_progress) {

	// $cover_img_url = '/sites/all/themes/vedic/images/no-image.png';
	// if($associated_course->field_cover_image->value()['uri']) {
	// 	$cover_img_url = file_create_url($associated_course->field_cover_image->value()['uri']);
	// }

	$content = '<h1 class="ui header progress-header">' .
		'<div class="header-content">' .
		// dss_breadcrumb($breadcrumb_links) .
		'<div class="title course-title welcome-decorative">' . $associated_course->title->value() . dss_get_edit_link($associated_course->getIdentifier()) . '</div>
		<div class="subtitle">by Swami Mukundananda</div>
		</div>
		</h1>
		<div data-percent="0" class="ui bottom attached progress course-progress"><div style="transition-duration: 300ms; width: 0%;" class="bar"></div></div>';

	$block = new stdClass();

	$block->title = $conf['override_title'] ? $conf['override_title_text'] : '';
	$block->content = $content;

	return $block;
}

function dss_pane_course_main_content($conf, $associated_course, $course_chapters, $course_progress) {

	global $user;

	$chapter_label = $associated_course->field_chapter_label->raw();
	$lesson_label = $associated_course->field_lesson_label->raw();

	// dpm($associated_course->value());
	// dpm($course_progress->value());

	$progress = json_decode($course_progress->body->value->value(array('decode' => TRUE)), true);
	$dss_access_check_disabled = dss_is_content_editor() && variable_get('dss_disable_access_check_admin', false);
        $is_content_editor =dss_is_content_editor();
	$content = '';
	// $content = '<h2 class="ui header flute-decorative center">Course Content</h2>';

	if(!empty($associated_course->body->value())) {
		$content .= $associated_course->body->value->value();
		// $content = '<div class="feather-note">
		// 	<div class="sprite feather-pen"></div>
		// 	' . $associated_course->body->value->value() . '
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

	foreach ($course_chapters as $key => $course_chapter_node) {

		if(empty($course_chapter_node)) continue;

		$course_chapter = entity_metadata_wrapper('node', $course_chapter_node);
		$course_chapter_completed = dss_get_percent_completed_course_chapter($progress, $course_chapter);

		$url = url('node/' . $course_chapter->getIdentifier());
		$card_class = 'course-chapter-item ';
		$disabled = false;

		// Check if doesn't have access to the current chapter
		if(!$dss_access_check_disabled && $course_chapter->field_chapter_number->raw() > $course_progress->field_current_chapter_number->raw()) {
			$url = '';
			$card_class .= 'not-available';
			$disabled = true;
		}

		// If this is the current course chapter
		if($course_chapter->getIdentifier() == $course_progress->field_current_chapter->raw()) {
			$card_class .= 'cur ';
		}

		$img_url = '/sites/all/themes/vedic/images/no-image.png';
		if($course_chapter->field_cover_image->value()['uri']) {
			$img_url = file_create_url($course_chapter->field_cover_image->value()['uri']);
		}

		// Card content
		$content .= '
			<div class="ui fluid card stacked segment course-card ' . $card_class . '" data-chapter-nid="' . $course_chapter->getIdentifier() . '">
			<div class="content">
				<div class="meta">
				' . $chapter_label . ' ' . $course_chapter->field_chapter_number->raw() . ($is_content_editor ? ' &nbsp; <a href="/node/' . $course_chapter->getIdentifier() . '/edit?destination=' . drupal_get_path_alias(current_path()) . '/content" class="edit-link"></a>' : '') . '
				</div>
				<div class="header">
			<a href="' . $url . '">' . $course_chapter->title->raw() . '</a>
			</div>
			</div>
			<a href="' . $url . '" class="image">
				<div style="background-image: url(\'' . $img_url . '\')"></div>
			</a>
			<div class="extra content">';

		if ($disabled) {
			$content .= '<i class="lock icon right floated"></i>';
		} else {
			$content .= '<span class="right floated">
				<a href="' . $url . '" class="ui button">View</a>
				</span>';
		}

		$content .= '<span>
			' . ($course_chapter_completed >= 100 ? ' <i class="item-progress checkmark icon"></i>' : '<i class="file text outline icon"></i>') . '
			' . count($course_chapter->field_course_lessons->raw()) . ' lessons &nbsp; <i class="video outline icon"></i> ' . $course_chapter->field_video_minutes->raw() . ' mins
			</span>
			</div>
			</div>';

		// if($key == 1) break;
	}

	$cover_img_url = '/sites/all/themes/vedic/images/swamiji-portrait.jpg';
	if($course_chapter->field_cover_image->value()['uri']) {
		$cover_img_url = file_create_url($associated_course->field_cover_image->value()['uri']);
	}

	$content .= '</div></div>';
	// $content .= '<div class="cover-image paper-cutting white"><div class="paper-cutting-inner white"><img src="' . $cover_img_url . '"></div></div>';

	$block = new stdClass();

 	// initial content is blank
	$block->title = $conf['override_title'] ? $conf['override_title_text'] : '';
	$block->content = $content;

	return $block;
}
