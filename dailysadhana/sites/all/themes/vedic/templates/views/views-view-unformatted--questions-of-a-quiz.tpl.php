<?php

/**
 * @file
 * Template to display a list of rows.
 */
?>

<div class="quiz-questions">
	<?php foreach ($rows as $delta => $row): ?>
		<div class="question">
			<form action="">
				<?php print $row; ?>
				<input type="submit" class="submit" value="Submit">
			</form>
		</div>
	<?php endforeach; ?>
</div>

<div class="text-center">
<span id="prev-slide"></span>
&nbsp;
<span id="slide-count"></span>
&nbsp;
<span id="next-slide"></span>
</div>