<?php 

function dss_number_to_word($num) {
	$number_to_word = array('zero', 'one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine', 'ten');
	return $number_to_word[$num];
}

function dss_generate_dashed_str($str) {
	return str_replace('/\s+/', '-', strtolower($str));
}

function dss_is_material_acessible_once($type_tid, $associated_course) {
	$once = $associated_course->field_materials_accessible_once->raw();
	if(empty($once)) return false;
	return in_array($type_tid, $once);
}

function dss_is_enforce_completion($type_tid, $associated_course) {
	$enforced = $associated_course->field_enforce_completion_for->raw();
	if(empty($enforced)) return false;
	return in_array($type_tid, $enforced);
}

function dss_get_percent_completed_course_chapter($progress, $course_chapter) {
	global $user;
	$email = $user->mail;
	$user_det = user_load($user->uid);
	$uname = $user_det->field_full_name['und'][0]['value'];
	
	$node_id = arg(1);
	$node_det = node_load($node_id);
    
    $product_id = $node_det->field_product['und'][0]['product_id'];
    $product = commerce_product_load($product_id);
    $p_sku = $product->sku;

	if($p_sku == 'Trial_Vedic_Philosophy') {
	  if(isset($progress['chapters'][$course_chapter->getIdentifier()]['completed'])) {
		$chapter_percent = $progress['chapters'][$course_chapter->getIdentifier()]['completed'];
		 if($chapter_percent >= 100) {
			$variable = $user->uid.'-'.$course_chapter->getIdentifier().'-100-trail';
			$value = variable_get($variable);
			if(empty($value)) {
				$node_load = node_load($course_chapter->getIdentifier());
				$title = $node_load->title;
				//mailing
				$to      = $email;
				$subject = 'Chapter Completion Mail';
				$message = '<div style="background: #8a8a8a;padding: 20px;">
				<div style="width: 80%; margin: 0 auto; background: #f9ae1a; padding: 20px; color: #fff;">
				<img src="http://dsstest.swamimukundananda.org/sites/all/themes/vedic/images/logo.png"/>
				</div>
				<div style="width: 80%; margin: 0 auto; background: #fff; padding: 20px;">
				Hi '.$uname.',<br/>
				You have completed Trail Course Chapter <b>'.$title.'</b>
				</div>
				<div style="width: 80%;margin: 0 auto;background: #052653;padding: 20px; color: #fff;">
				--  '.variable_get('site_name').' team
				</div>
				</div>';
				$headers = "Content-type:text/html;charset=UTF-8" . "\r\n";
				$headers .= 'From: Daily Sadhana <info@mydailysadhana.org>' . "\r\n" . 'X-Mailer: PHP/' . phpversion();
				mail($to, $subject, $message, $headers);
				variable_set($variable,'true');
			}
		}
	 }
	}
	else {

	// Make Changes Here!!! 
	if(isset($progress['chapters'][$course_chapter->getIdentifier()]['completed'])) {
		$chapter_percent = $progress['chapters'][$course_chapter->getIdentifier()]['completed'];
		if($chapter_percent >=50 && $chapter_percent <= 99) { // 50% course completion
			$variable = $user->uid.'-'.$course_chapter->getIdentifier().'-50';
			$value = variable_get($variable);
			if(empty($value)) {
				$node_load = node_load($course_chapter->getIdentifier());
				$title = $node_load->title;
				//mailing
				$to      = $email;
				$subject = 'Congratulations on your progress';
				$message = '<div style="background: #8a8a8a;padding: 20px;">
				<div style="width: 80%; margin: 0 auto; background: #f9ae1a; padding: 20px; color: #fff;">
				<img src="http://dsstest.swamimukundananda.org/sites/all/themes/vedic/images/logo.png"/>
				</div>
				<div style="width: 80%; margin: 0 auto; background: #fff; padding: 20px;">
				Hi '.$uname.',<br/>
				Congratulations on your progress on the Course Chapter <b>'.$title.'</b>
				</div>
				<div style="width: 80%;margin: 0 auto;background: #052653;padding: 20px; color: #fff;">
				--  '.variable_get('site_name').' team
				</div>
				</div>';
				$headers = "Content-type:text/html;charset=UTF-8" . "\r\n";
				$headers .= 'From: Daily Sadhana <info@mydailysadhana.org>' . "\r\n" . 'X-Mailer: PHP/' . phpversion();
				mail($to, $subject, $message, $headers);
				variable_set($variable,'true');
			}
		}
		
		if($chapter_percent >= 100) {  // 100% course completion
			$variable = $user->uid.'-'.$course_chapter->getIdentifier().'-100';
			$value = variable_get($variable);
			if(empty($value)) {
				$node_load = node_load($course_chapter->getIdentifier());
				$title = $node_load->title;
				//mailing
				$to      = $email;
				$subject = 'Chapter Completion Mail';
				$message = '<div style="background: #8a8a8a;padding: 20px;">
				<div style="width: 80%; margin: 0 auto; background: #f9ae1a; padding: 20px; color: #fff;">
				<img src="http://dsstest.swamimukundananda.org/sites/all/themes/vedic/images/logo.png"/>
				</div>
				<div style="width: 80%; margin: 0 auto; background: #fff; padding: 20px;">
				Hi '.$uname.',<br/>
				You have completed Course Chapter <b>'.$title.'</b>
				</div>
				<div style="width: 80%;margin: 0 auto;background: #052653;padding: 20px; color: #fff;">
				--  '.variable_get('site_name').' team
				</div>
				</div>';
				$headers = "Content-type:text/html;charset=UTF-8" . "\r\n";
				$headers .= 'From: Daily Sadhana <info@mydailysadhana.org>' . "\r\n" . 'X-Mailer: PHP/' . phpversion();
				mail($to, $subject, $message, $headers);
				variable_set($variable,'true');
			}
		}
	}
	}
		
	return isset($progress['chapters'][$course_chapter->getIdentifier()]['completed']) ? 
	$progress['chapters'][$course_chapter->getIdentifier()]['completed'] :
	0;
}

function dss_get_percent_completed_course_lesson($progress, $course_lesson) {
	
	return isset($progress['lessons'][$course_lesson->getIdentifier()]['completed']) ?
	$progress['lessons'][$course_lesson->getIdentifier()]['completed'] :
	0;
}

function dss_get_percent_completed_course_material($progress, $course_material) {
	
	//drupal_set_message('<pre>'. print_r($progress, true) .'</pre>');

	return isset($progress['materials'][$course_material->getIdentifier()]['completed']) ?
	$progress['materials'][$course_material->getIdentifier()]['completed'] :
	0;
}

function dss_get_percent_completed_course_material_extra($progress, $associated_lesson_id, $course_material_extra_id) {
	
	return isset($progress['materials_extra']['lessons'][$associated_lesson_id][$course_material_extra_id]['completed']) ?
	$progress['materials_extra']['lessons'][$associated_lesson_id][$course_material_extra_id]['completed'] :
	0;
}

function dss_get_autoplay($progress) {

	if(!isset($progress['settings']['autoplay'])) {
		$progress['settings']['autoplay'] = true;
	}

	return $progress['settings']['autoplay'];
}

function dss_set_autoplay(&$progress, $val) {
	$progress['settings']['autoplay'] = (bool) $val;
}

function dss_is_once_bypass($course_progress, $associated_lesson, $associated_chapter, $associated_course) {

	$num_prev_chapters_accessible = $associated_course->field_prev_chapters_accessible->value();
	$num_prev_lessons_accessible = $associated_course->field_prev_lessons_accessible->value();

	if($associated_chapter->field_chapter_number->raw() >= $course_progress->field_current_chapter_number->raw() - $num_prev_chapters_accessible || $associated_chapter->field_chapter_number->raw() == $course_progress->field_current_chapter_number->raw() - $num_prev_chapters_accessible && $associated_lesson->field_lesson_number->raw() >= $course_progress->field_current_lesson_number->raw() - $num_prev_lessons_accessible) {
		return true;
	}

	$num_prev_lessons_accessible -= $course_progress->field_current_lesson_number->value() - 1;

	if($num_prev_lessons_accessible <= 0) {
		return false;
	}
	
	$chapter = entity_metadata_wrapper('node', $course_progress->field_current_chapter->value());

	while(!empty($chapter->field_prev_chapter->raw())) {

		$chapter = entity_metadata_wrapper('node', $chapter->field_prev_chapter->value());
		$lessons = array_reverse($chapter->field_course_lessons->raw());

		foreach ($lessons as $lesson_id) {
			if($lesson_id == $associated_lesson->getIdentifier()) {
				return true;
			}
			if(--$num_prev_lessons_accessible == 0) break;
		}
	}

	return false;
}

function dss_user_has_role($role_ids) {

	foreach ($role_ids as $role_id) {
		if(user_has_role($role_id)) {
			return true;
		}
	}

	return false;
}

function dss_is_content_editor() {
	return dss_user_has_role(array(3, 4, 5));
}
