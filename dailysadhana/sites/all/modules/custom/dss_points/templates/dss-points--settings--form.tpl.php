<?php $template_path = DRUPAL_ROOT .'/sites/all/themes/vedic/layouts/header.php'; include_once($template_path);  ?>
<?php 	global $base_url;
   $css_path = $base_url .'/sites/all/themes/vedic/semantic/dist/semantic.css'; 
   drupal_add_css($css_path, array('group' => CSS_THEME, 'type' => 'file', 'weight' => -125, 'preprocess' => FALSE )); ?>
<div class="ui dimmer pane-page-dimmer"></div>

<div class="l-main">

  <div class="l-sidebar">
      <div class="l-sidebar-section">
        <div class="l-sidebar-upper">
          <div class="l-sidebar-header">
          </div> 
        </div>
        <div class="l-sidebar-links ui vertical menu visible">
        <p><a class="item odd" href="/learn/vedic-philosophy/content"><i class="list layout icon"></i> Go to the Course</a></p>
        </div>
  
      </div>
  </div>
  <div class="l-content" role="main">
    <div class="l-content-inner">
       <?php $messages = theme('status_messages');
       if(!empty($messages)){
          print render($messages);
       }?>
       <?php $tabs = menu_local_tabs();  print '<div class="bonus-tabs" >'. render($tabs) .'</div>';?>
      <div class="l-content-header top-acont-box">

<?php 
$block = module_invoke('block', 'block_view', '6');
print render($block['content']);
print '<a href="/admin/structure/block/manage/block/6/configure" class="edit-link">Edit</a>';
?>

      </div>
      <div class="ui padded segment clearfix">
          <?php print  drupal_render_children($form);?>
      </div>
    </div>
  </div>
</div>

