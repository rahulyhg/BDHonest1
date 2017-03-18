window.onload = function() {
	setInterval(function() {
		var eles = document.querySelectorAll('.ui-title,.ckf-dialog-contents'); //  also
		for(var i in eles) {
			if(eles[i] && eles[i].style && eles[i].textContent.toLowerCase().indexOf('demo version') >= 0) {
				eles[i].style.display = 'none';
			}
		}
	}, 500);
};