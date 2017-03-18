<?php

/**
 * @file
 * Template to display a list of rows.
 */
?>
<a class="item">
	<i class="list layout icon"></i>
	Course Content
</a>
<div class="course-content-items ui inverted vertical menu visible">
	<?php foreach ($rows as $delta => $row): ?>
		<?php print $row; ?>
	<?php endforeach; ?>
</div>
