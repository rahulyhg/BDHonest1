<?php include_once(dirname(__FILE__) . '/../header.php'); ?>

<div class="ui dimmer pane-page-dimmer"></div>


<div class="l-main">
  <div class="l-sidebar">
      <div class="l-sidebar-section">
        <div class="l-sidebar-upper">
          <div class="l-sidebar-header">
            <?php print render($page['sidebar_header']); ?>
          </div>

        </div>
        <div class="l-sidebar-links ui vertical menu visible">
          <?php print render($page['sidebar_links']); ?>
        </div>
        <?php if($page['admin_blocks']): ?>
          <div class="ui basic padded segment">
            <?php print render($page['admin_blocks']); ?>
          </div>
        <?php endif; ?>
      </div>
  </div>
  <div class="l-content" role="main">
    <div class="l-content-inner">
      <div class="l-content-header">
        <?php print render($page['content_header']); ?>
      </div>
      <div class="ui padded segment clearfix">
        <?php print render($page['content']); ?>
      </div>
    </div>
  </div>
</div>
</div>
