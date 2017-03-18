<?php

module_load_include('inc', 'dss', 'dss.helper');

$plugin = array(
	'single' => TRUE,  // Just do this one, it is needed.
	'title' => t('Discussion Content'),  // Title to show up on the pane screen.
	'description' => t('Discussion Content page'), // Description to show up on the pane screen.
	'category' => t('DSS'), // A category to put this under.
	'edit form' => 'dss_pane_discussion_content_pane_edit_form', // A function that will return the settings form for the pane.
	'render callback' => 'dss_pane_discussion_content_pane_render', // A function that will return the renderable content.
	'admin info' => 'dss_pane_discussion_content_pane_admin_info', // A function that will return the information displayed on the admin screen (optional).
	'defaults' => array( // Array of defaults for the settings form.
		'section' => 'topics',
		),
	'all contexts' => TRUE, // This is NEEDED to be able to use substitution strings in your pane.
	);


/**
 * An edit form for the pane's settings.
 */
function dss_pane_discussion_content_pane_edit_form($form, &$form_state) {
	
	$conf = $form_state['conf'];

	$form['section'] = array(
		'#type' => 'radios',
		'#title' => t('Section'),
		'#description' => t('Select the section in Discussion Content Page'),
		'#options' => array(
			'forums-list' => t('Forums List'),
			'topics' => t('Forum Topics'),
			),
		'#default_value' => isset($conf['section']) ? $conf['section'] : 'topics',
		);

	return $form;
}

/**
 * Submit function, note anything in the formstate[conf] automatically gets saved
 * Notice, the magic that automatically does that for you.
 */
function dss_pane_discussion_content_pane_edit_form_submit(&$form, &$form_state) {

	foreach (array_keys($form_state['plugin']['defaults']) as $key) {
		if (isset($form_state['values'][$key])) {
			$form_state['conf'][$key] = $form_state['values'][$key];
		}
	}
}

/**
 * 'admin info' callback for panel pane.
 */
function dss_pane_discussion_content_pane_admin_info($subtype, $conf, $contexts) {
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
function dss_pane_discussion_content_pane_render($subtype, $conf, $args, $contexts) {

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

}
