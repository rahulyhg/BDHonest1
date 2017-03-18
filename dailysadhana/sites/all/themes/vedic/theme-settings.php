<?php

/**
 * @file
 * Theme settings file for the Vedic theme.
 */

require_once dirname(__FILE__) . '/template.php';

/**
 * Implements hook_form_FORM_alter().
 */
function vedic_form_system_theme_settings_alter(&$form, $form_state) {
  // You can use this hook to append your own theme settings to the theme
  // settings form for your subtheme. You should also take a look at the
  // 'extensions' concept in the Omega base theme.
}

// function vedic_preprocess_html(&$vars) {
//   $node = menu_get_object();

//   die('hook');

//   if ($node && $node->nid) {
//     //$vars['theme_hook_suggestions'][] = 'html__' . $node->type;
//   }
// }