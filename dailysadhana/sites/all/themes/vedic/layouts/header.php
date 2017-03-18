<div class="ui modal alert-modal">
  <i class="close icon"></i>
  <div class="header">
  </div>
  <div class="content">
  </div>
  <div class="actions">
    <div class="ui positive right labeled icon button">
      Ok
    </div>
  </div>
</div>

<div class="l-page">
  <header class="l-header" role="banner">
    <div class="header-bar">
      <div class="header-menu-left ui large secondary menu">
        <a class="item sidebar-icon"><i class="sidebar icon"></i></a>
        <div  id="mobile-nav" class="mob-nav"><i class="sidebar icon"></i></div>
        <a class="logo-holder" href="<?php print url('<front>'); ?>">
          <img class="logo" src="<?php print '/' . drupal_get_path('theme', 'vedic') . '/images/logo.png'; ?>" alt="">
        </a>
        <div class="header-menu-items ui large left menu">
          <i class="sprite header-flower"></i>
          <a class="item" href="/">Home</a>
          <a class="item" href="/courses">Courses</a>
          <a class="item" href="/faq">FAQ</a>
          <a class="item" href="/blog">Blog</a>
          <a class="item" href="/contact">Contact</a>
          <i class="sprite header-flower"></i>
        </div>
        <!-- <i class="sprite header-feather"></i> -->

        <div class="header-menu-right header-menu-items ui right menu">
        
          <?php
            global $user;
            if ($logged_in) {
              if (!property_exists($user, 'realname')) {
                $logged_user = entity_metadata_wrapper('user', user_load($user->uid));
          ?>
          <div class="ui dropdown item labeled-button">
            <i class="icon large user labeled-icon"></i>
            
            <?php echo strtok($logged_user->field_full_name->raw(), " "); 
             $uid =$user->uid;
             $userpoints = "SELECT sum( points ) total_points FROM userpoints  WHERE uid =".$uid ."  GROUP BY uid ";
             $points = db_query($userpoints)->fetchField();
             if(!empty($points))
                  echo ' (MyPoints : '.$points.')'; ?>
            <i class="icon dropdown"></i>
            <div class="menu">
              <a class="item" href="/user/<?php echo $user->uid; ?>/edit"><i class="icon settings"></i>My Account</a>
              <a class="item hide" href="/mybonus"><i class="icon settings"></i>My Bonus</a>  
              <a class="item" href="/learn/vedic-philosophy"><i class="icon settings"></i>Current Course</a>          
              <a class="item" href="/welcome"><i class="icon settings"></i>Course Dashboard</a>
              <a class="item" href="/user/logout"><i class="icon sign out"></i>Logout</a>

            </div>
          </div>
          <?php
              }
            }
            else {
          ?>
              <a href="/user/login" class="item">Register/Login</a>
          <?php
            }
          ?>
        </div>
      </div>
    </div>
  </header>
</div>
