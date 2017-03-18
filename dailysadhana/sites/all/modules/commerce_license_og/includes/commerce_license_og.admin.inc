<?php

/**
 * @file
 * Contains admin menu callbacks for the Commerce License OG module.
 */

/**
 * Settings form callback.
 */
function commerce_license_og_settings_form($form, &$form_state) {
  $help = t('This form allows you to enable Organic Groups licensing for your product types.') . '<br />';
  $help .= t('Only licensable product types selected on the <em class="placeholder">General</em> tab are available here.');
  $form['help'] = array(
    '#markup' => $help,
  );

  // Create a list of licensable product types and their labels.
  $license_product_types = commerce_license_product_types();
  $product_types = array();
  foreach (commerce_product_type_options_list() as $type => $label) {
    if (in_array($type, $license_product_types)) {
      $product_types[$type] = $label;
    }
  }
  // If no licensable product types were found, stop here.
  if (count($product_types) == 0) {
    return array(
      'error' => array(
        '#markup' => t('No licensable product types found, select at least one on the <em class="placeholder">General</em> tab.'),
      ),
    );
  }

  // Our current settings, for use in #default_values in the form.
  $settings = commerce_license_og_product_types();

  $form['commerce_license_og_product_types'] = array(
    '#title' => t('Product types'),
    '#type' => 'container',
    '#tree' => TRUE,
    '#description' => t('Each product type can be enabled and configured for licensing.'),
  );
  $container = &$form['commerce_license_og_product_types'];

  // OG group reference fields are simply entity references with some extra
  //   behavior handlers and cool widgets attached. Because of this though,
  //   you can only have a single entity type on any given OG reference field.
  //   This means that we need to provide a way to select the targeted entity
  //   type on a product type by product type basis, here.

  foreach ($product_types as $name => $label) {
    $container[$name] = array(
      '#type' => 'fieldset',
      '#title' => $label,
      '#collapsible' => FALSE,
    );
    $container[$name]['enabled'] = array(
      '#type' => 'checkbox',
      '#default_value' => !empty($settings[$name]['enabled']),
      '#title' => t('Licensing Enabled'),
    );
    $container[$name]['entity_type'] = array(
      '#type' => 'select',
      '#title' => t('Group type'),
      '#description' => t('Choose a type of group to license.'),
      '#default_value' => (isset($settings[$name]['entity_type']) ? $settings[$name]['entity_type'] : NULL),
      '#options' => commerce_license_og_entity_types(),
      '#states' => array(
        'visible' => array(
          ':input[name="commerce_license_og_product_types\[' . $name . '\]\[enabled\]"]' => array('checked' => TRUE),
        ),
        'required' => array(
          ':input[name="commerce_license_og_product_types\[' . $name . '\]\[enabled\]"]' => array('checked' => TRUE),
        ),
      ),
    );
  }

  $form = system_settings_form($form);
  $form['#submit'][] = 'commerce_license_og_settings_form_submit';

  return $form;
}

/**
 * Submit callback of the file settings form.
 */
function commerce_license_og_settings_form_submit($form, &$form_state) {
  // This will create the cl_og_[entity_type] field on any newly selected product
  // types, and remove it from any newly deselected product types.
  commerce_license_og_flush_caches();
}
