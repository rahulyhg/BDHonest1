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

/**
 * 
 * Implements hook_theme().
 */
function vedic_theme(){
  $hooks = array();
  $hooks['user_register_form'] = array (
     'render element' => 'form',
     'path' => drupal_get_path('theme','vedic'),
     'template' => 'templates/user--register',
     'preprocess functions' => array('vedic_preprocess_user_register_form'),
  );
  return $hooks;
}

/**
 * 
 * Implements theme_preprocess_user_register_form().
 */
function vedic_preprocess_user_register_form(&$vars) {
  $args = func_get_args();
  array_shift($args);
  $form_state['build_info']['args'] = $args;
  $vars['form'] = drupal_build_form('user_register_form', $form_state['build_info']['args']);
}