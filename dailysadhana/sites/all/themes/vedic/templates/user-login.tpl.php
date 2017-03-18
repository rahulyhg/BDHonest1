<?php
 /* split the username and password from the submit button
   so we can put in links above */

    print drupal_render($form['name']);
    print drupal_render($form['pass']);
    print drupal_render($form['form_build_id']);
    print drupal_render($form['form_id']);
   // print drupal_render($form['actions']);

?>