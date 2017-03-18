<?php

/**
 * Implements hook_preprocess_page().
 */
function vedic_preprocess_page(&$variables) {
  // You can use preprocess hooks to modify the variables before they are passed
  // to the theme function or template file.
	$variables['node'] = menu_get_object();
	
	

   if(((arg(0) == 'node')&&(arg(1) == 'add')) || ((arg(0) == 'node')&&(arg(2) == 'edit'))) {
    //$variables['theme_hook_suggestions'][] =  'page__nodedit';
	 $variables['theme_hook_suggestions'][] =  'page__nodedit';
   } 


}
