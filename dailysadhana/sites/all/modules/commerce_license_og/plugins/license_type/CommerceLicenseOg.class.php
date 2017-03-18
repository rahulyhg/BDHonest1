<?php

/**
 * Organic groups license type.
 */
class CommerceLicenseOg extends CommerceLicenseBase  {

  /**
   * Implements CommerceLicenseInterface::isConfigurable().
   */
  public function isConfigurable() {
    return FALSE;
  }

  /**
   * Implements CommerceLicenseInterface::accessDetails().
   */
  public function accessDetails() {
    // Create a set of links for each licensed OG.
    $settings = commerce_license_og_product_types($this->wrapper->product->getBundle());
    $field_name = commerce_license_og_field_name($settings);

    $links = '';
    foreach ($this->wrapper->product->{$field_name} as $group_wrapper) {
      $uri = entity_uri($group_wrapper->type(), $group_wrapper->value());
      $links .= l($group_wrapper->label(), $uri['path'], $uri['options']) . "<br/>";
    }
    return $links;
  }

  /**
   * Implements CommerceLicenseInterface::checkoutCompletionMessage().
   */
  public function checkoutCompletionMessage() {
    $text = 'Congratulations! You are now a member of the following groups:<br />';
    $text .= $this->accessDetails();

    return $text;
  }

  /**
   * Overrides Entity::save().
   *
   * Maintains OG memberships, adding or removing them from the owner when necessary.
   */
  public function save() {
    if ($this->uid && $this->product_id) {
      $settings = commerce_license_og_product_types($this->wrapper->product->getBundle());
      $field_name = commerce_license_og_field_name($settings);

      $group_wrappers = iterator_to_array($this->wrapper->product->{$field_name});
      $owner = $this->wrapper->owner->value();

      // Assume no change is necessary, yet.
      $save_owner = FALSE;
      if (!empty($this->license_id)) {
        $this->original = entity_load_unchanged('commerce_license', $this->license_id);
        // A plan change occurred. Remove previous group memberships
        if ($this->original->product_id && $this->product_id != $this->original->product_id) {
          $original_settings = commerce_license_og_product_types($this->original->wrapper->product->getBundle());
          $original_field_name = commerce_license_og_field_name($original_settings);
          foreach ($this->original->wrapper->product->{$field_name} as $original_group_wrapper) {
            // Remove each previous group membership.
            // @todo: Only do this if the new plan also lacks this group.
            $updated = $this->removeGroupMembership($original_group_wrapper, $settings['entity_type'], $owner);
            $save_owner = ($save_owner || $updated);
          }
        }
      }

      if ($this->status == COMMERCE_LICENSE_ACTIVE) {
        foreach ($group_wrappers as $group_wrapper) {
          $updated = $this->verifyGroupMembership($group_wrapper, $settings['entity_type'], $owner);
          $save_owner = ($save_owner || $updated);
        }
      }
      elseif ($this->status > COMMERCE_LICENSE_ACTIVE) {
        foreach ($group_wrappers as $group_wrapper) {
          $updated = $this->removeGroupMembership($group_wrapper, $settings['entity_type'], $owner);
          $save_owner = ($save_owner || $updated);
        }
      }

      // If a membership was added or removed, save the owner.
      if ($save_owner) {
        user_save($owner);
      }
    }

    parent::save();
  }

  /**
   * Used to ensure that $account has a membership to $group_wrapper.
   *
   * @return bool Whether or not the membership was newly added.
   */
  public function verifyGroupMembership($group_wrapper, $group_type, $account) {
    $current_membership = og_get_membership($group_type, $group_wrapper->getIdentifier(), 'user', $account->uid);
    if ($current_membership) {
      // Membership exists. Do nothing.
      return FALSE;
    }
    else {
      $values = array(
        'entity_type' => 'user',
        'entity' => $account,
      );
      $membership = og_group($group_type, $group_wrapper->getIdentifier(), $values);
      if ($membership) {
        return TRUE;
      }
    }
  }

  /**
   * Used to ensure that $account has its membership to $group_wrapper removed.
   *
   * @return bool Whether or not an existing membership was removed.
   */
  public function removeGroupMembership($group_wrapper, $group_type, $account) {
    $current_membership = og_get_membership($group_type, $group_wrapper->getIdentifier(), 'user', $account->uid);
    if (!$current_membership) {
      // Membership doesn't exist. Do nothing.
      return FALSE;
    }
    else {
      $account = og_ungroup($group_type, $group_wrapper->getIdentifier(), 'user', $account->uid);
      return TRUE;
    }
  }

}
