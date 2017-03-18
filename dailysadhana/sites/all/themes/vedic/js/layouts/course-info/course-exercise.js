var DSS = DSS || {};

(function($) {

	function arrayEqual(arr1, arr2) {
		if(typeof arr1 === 'undefined' || typeof arr2 === 'undefined') {
			return false;
		}
		
		if(arr1.length != arr2.length) {
			return false;
		}
		
		for(var i = 0; i < arr1.length; i++) {
			if(arr1[i] != arr2[i]) {
				return false;
			}
		}
		
		return true;
	}
	
	(function initExerciseMessage() {
		
		$('.slide-holder[data-type="Exercise"]').find('.quiz-question').prepend('\
			<div class="ui icon message exercise-message hidden">\
			<i class="side-icon"></i>\
			<div class="content">\
			<div class="header"></div>\
			<div class="message-body"></div>\
			</div>\
			</div>');   
	}());
	
	function showMessage($msgBox, header, msg, icon) {
		
		if(!msg) {
			msg = '';
		}

		$msgBox.find('.header').text(header);
		$msgBox.find('.message-body').html(msg);
		$msgBox.find('.side-icon').attr('class', icon + ' icon side-icon');
		$msgBox.removeClass('hidden');
		DSS.redrawSlider();
	}
	
	function showHint($msgBox, header, msg) {
		
		showMessage($msgBox, header, msg, 'idea');
	}
	
	function showExplanation($msgBox, header, msg) {
		
		showMessage($msgBox, header, msg, 'edit');
	}
	
	function showWarning($msgBox, header, msg) {
		
		showMessage($msgBox, header, msg, 'warning sign');
	}
	
	function finishMultipleChoiceQuestion($quizQuestion, ans) {
		
		$quizQuestion.find('input').attr('disabled', true);
		$quizQuestion.find('.multiple-choice-submit').hide();
		
		$quizQuestion.find('.option-label').each(function(i, item) {
			
			var $label = $(item);
			var $input = $label.find('input');
			var isCorrect = $.inArray('' + $input.val(), ans) >= 0;
			
			if(isCorrect) {
			
				$label.append('<i class="checkmark icon"></i>');
				$label.addClass('correct');
			
			} else {
				
				// If the option is checked
				if($input.prop('checked') == true) {
					$label.append('<i class="remove icon"></i>');
				} else {
					$label.addClass('blurred');
				}
			}
		});
	}
	
	function incrementAttempts($element) {
		var attempts = parseInt($element.data('attempts')) + 1;
		$element.data('attempts', attempts);
		return attempts;
	}

	$('.multiple-choice-submit').on('click', function(e) {
		
		var $form = $(this).closest('form');
		var exerciseNid = $form.closest('.slide-holder').data('nid');
		var $quizQuestion = $form.closest('.quiz-question');
		
		var questionNid = $quizQuestion.data('nid');
		var $msgBox = $quizQuestion.find('.exercise-message');
		
		var ansData = DSS.exerciseData[exerciseNid][questionNid];
		
		var ans = ansData['ans'];
		ans = ans.sort();
		
		// Create data of user form submission
		var formData = {};
		$form.serializeArray().map(function(x) {
			if(typeof formData[x.name] === "undefined") {
				formData[x.name] = x.value;
			} else {
				if(!$.isArray(formData[x.name])) {
					formData[x.name] = [ formData[x.name] ];
				}
				formData[x.name].push(x.value); 
			}
		});
		
		var userAns = formData['ans'];
		if(typeof userAns == 'undefined') {
			showWarning($msgBox, 'Please select an answer');
			return false;
		}
		
		var hint = ansData['hints'][0];
		var explanation = ansData['explanations'][0];

		var attempts = incrementAttempts($quizQuestion);

		// Correct answer
		if(arrayEqual(userAns, ans)) {
			
			showExplanation($msgBox, 'Correct!', explanation);
			finishMultipleChoiceQuestion($quizQuestion, ans);
			
		// Wrong answer
		} else { 

			// 1st attempt
			if(attempts == 1) {
				
				showHint($msgBox, 'Incorrect! Give it one more try', hint);
				$quizQuestion.find('input').prop('checked', false);
				
			// 2nd attempt
			} else { 
				
				showExplanation($msgBox, 'Incorrect!', explanation);
				finishMultipleChoiceQuestion($quizQuestion, ans);
			}
		}
		
		DSS.modal.scrollToTop();
		return false;
	});
	
	function handleDrop($srcItem, $target) {
		
		function addItemToTarget($srcItem, $target) {
			
			$srcItem.addClass('disabled').draggable('disable');
			$target.addClass('items-added');
			$target.append('<div class="correct-item">' + $srcItem.text() + '</div>')
			DSS.redrawSlider();
		}
		
		if($srcItem.hasClass('disabled')) {
			return false;
		}
		
		var exerciseNid = $srcItem.closest('.slide-holder').data('nid');
		var $quizQuestion = $srcItem.closest('.quiz-question');
		var $msgBox = $quizQuestion.find('.exercise-message');
		
		var attempts = incrementAttempts($srcItem);
		
		var questionNid = $quizQuestion.data('nid');
		var ansData = DSS.exerciseData[exerciseNid][questionNid];
		var ans = ansData['ans'];
		
		var key = $srcItem.data('item-key');
		var correctAns = ans[key];
		var optionValue = $target.data('option-value');
		
		var explanation = ansData['explanations'][key];
		var hint = ansData['hints'][key];
		
		// console.log('correctAns = ' + correctAns);
		// console.log('optionValue = ' + optionValue);
		
		var isCorrect = (correctAns == optionValue);
		
		if(isCorrect) {
			
			addItemToTarget($srcItem, $target);
			showExplanation($msgBox, 'Correct!', explanation);
			$srcItem.append('<i class="checkmark icon"></i>');
			
		} else {
			
			if(attempts == 1) {
				
				showHint($msgBox, 'Incorrect! Give it one more try', hint);
				
			} else {
				
				var $correctTarget = $target.parent().find('.match-option:nth-child(' + correctAns + ')');
				addItemToTarget($srcItem, $correctTarget);
				$srcItem.append('<i class="remove icon"></i>');
				
				showExplanation($msgBox, 'Incorrect!', explanation);
			}
		}
		
		DSS.modal.scrollToTop();
	}
	
	// Handle multiple choice questions - drag and drop
	$('.match-item').draggable({
		cursor: 'move', 
		cursorAt: { top: 20, left: 20 },
		helper: 'clone',
		containment: '#modal-content',
		scroll: true,
		disabled: false,
		start: function(event, ui){
			DSS.slider.removeTouch();
        },
        stop: function(event, ui){
        	DSS.slider.addTouch();
        }
	});
	
	$('.match-option').droppable({
		
		accept: '.match-item',
		activeClass: 'hover',
		hoverClass: 'active',
		greedy: true,
		
		drop: function(event, ui) {
			var $srcItem = ui.draggable;
			var $target = $(this);
			
			handleDrop($srcItem, $target);
		},
		over: function(event, ui){
			DSS.slider.removeTouch();
        },
        out: function(event, ui){
        	DSS.slider.addTouch();
        }
	});

}(jQuery));