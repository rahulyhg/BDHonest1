<?php
/**
 * @file
 * Template overrides as well as (pre-)process and alter hooks for the
 * Vedic theme.
 */
function vedic_follow_link($variables) {
  $link = $variables['link'];
  $title = '';//$variables['title'];
  $classes = array();
  $classes[] = 'follow-link';
  $classes[] = "follow-link-{$link->name}";
  $classes[] = $link->uid ? 'follow-link-user' : 'follow-link-site';
  $attributes = array(
    'class' => $classes,
    'title' => '',
    'target' => '_blank',
  );
  $link->options['attributes'] = $attributes;
  return l($title, $link->path, $link->options) . "\n";
}

function vedic_theme() {
  return array(
    'user_login' => array(
      'template' => 'user-login',
      'variables' => array('form' => NULL), ## you may remove this line in this case
    )
  );
}



function vedic_preprocess_user_login(&$variables) {
	$variables['form'] = drupal_build_form('user_login', user_login(array(),$form_state)); ## I have to build the user login myself.
}