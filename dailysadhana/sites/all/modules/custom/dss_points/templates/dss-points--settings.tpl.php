<?php $template_path = DRUPAL_ROOT .'/sites/all/themes/vedic/layouts/header.php'; include_once($template_path);  ?>

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
       <?php $tabs = menu_local_tabs();  print '<div class="bonus-tabs" >'. render($tabs) .'</div>';?>
      <div class="l-content-header top-acont-box">
<?php
if ((arg(0) == 'mybonus')&&(arg(1) == '')) {
$block = module_invoke('block', 'block_view', '4');
print render($block['content']);
print '<a href="/admin/structure/block/manage/block/4/configure" class="edit-link">Edit</a>';
}
if (arg(1) == 'mytreasure') {
$block = module_invoke('block', 'block_view', '5');
print render($block['content']);
print '<a href="/admin/structure/block/manage/block/5/configure" class="edit-link">Edit</a>';
}
?>


      </div>
      <div class="ui padded segment clearfix">
          <?php print render($items);?>
      </div>
    </div>
  </div>
</div>

