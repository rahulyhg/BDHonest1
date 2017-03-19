<h1 class="register-sign-up">Sign up</h1>
<?php print render($form['form_id']); ?>
<?php print render($form['form_build_id']); ?>
<?php
print render($form['account']['mail']);
print render($form['account']['pass']);
print render($form['field_full_name']);
print render($form['field_phone_number']);
print render($form['field_nick_name']);
print render($form['field_dob']);
print drupal_render($form['actions']);
?>

