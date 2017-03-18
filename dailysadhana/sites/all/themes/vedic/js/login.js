jQuery(function($) {
  $('#edit-field-full-name-und-0-value').on('blur', function() {
    // ucwords
    $(this).val($(this).val().toLowerCase().replace(/\b[a-z]/g, function(letter) {
      return letter.toUpperCase();
    }));
  });
} (jQuery))
