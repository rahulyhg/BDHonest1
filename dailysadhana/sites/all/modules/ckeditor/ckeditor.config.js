/*
Copyright (c) 2003-2013, CKSource - Frederico Knabben. All rights reserved.
For licensing, see LICENSE.html or http://ckeditor.com/license
*/

/*
 WARNING: clear browser's cache after you modify this file.
 If you don't do this, you may notice that browser is ignoring all your changes.
 */
CKEDITOR.editorConfig = function(config) {
	config.indentClasses = [ 'rteindent1', 'rteindent2', 'rteindent3', 'rteindent4' ];

	// [ Left, Center, Right, Justified ]
	config.justifyClasses = [ 'rteleft', 'rtecenter', 'rteright', 'rtejustify' ];

	// The minimum editor width, in pixels, when resizing it with the resize handle.
	config.resize_minWidth = 450;

	config.allowedContent = true; // Allows all tags
	// config.extraAllowedContent = 'div(*); h1(*); h2(*); h3(*); iframe(*); dl; dt; dd';

	// Protect PHP code tags (<?...?>) so CKEditor will not break them when
	// switching from Source to WYSIWYG.
	// Uncommenting this line doesn't mean the user will not be able to type PHP
	// code in the source. This kind of prevention must be done in the server
	// side
	// (as does Drupal), so just leave this line as is.
	config.protectedSource.push(/<\?[\s\S]*?\?>/g); // PHP Code

	// [#1762328] Uncomment the line below to protect <code> tags in CKEditor (hide them in wysiwyg mode).
	// config.protectedSource.push(/<code>[\s\S]*?<\/code>/gi);
	config.extraPlugins = '';

	/*
		* Append here extra CSS rules that should be applied into the editing area.
		* Example:
		* config.extraCss = 'body {color:#FF0000;}';
		*/
	config.extraCss = '';

	config.image2_alignClasses = [ 'img-left', 'img-center', 'img-right' ];
	config.image2_captionedClass = 'img-captioned';

	/**
		* Sample extraCss code for the "marinelli" theme.
		*/
	if (Drupal.settings.ckeditor.theme == "marinelli") {
		config.extraCss += "body{background:#FFF;text-align:left;font-size:0.8em;}";
		config.extraCss += "#primary ol, #primary ul{margin:10px 0 10px 25px;}";
	}
	if (Drupal.settings.ckeditor.theme == "newsflash") {
		config.extraCss = "body{min-width:400px}";
	}

	/**
		* CKEditor's editing area body ID & class.
		* See http://drupal.ckeditor.com/tricks
		* This setting can be used if CKEditor does not work well with your theme by default.
		*/
	config.bodyClass = '';
	config.bodyId = '';
	/**
		* Sample bodyClass and BodyId for the "marinelli" theme.
		*/
	if (Drupal.settings.ckeditor.theme == "marinelli") {
		config.bodyClass = 'singlepage';
		config.bodyId = 'primary';
	}

	// Make CKEditor's edit area as high as the textarea would be.
	if (this.element.$.rows > 0) {
		config.height = this.element.$.rows * 20 + 'px';
	}
}

/*
 * Sample toolbars
 */

//Toolbar definition for basic buttons
Drupal.settings.cke_toolbar_DrupalBasic = [ [ 'Format', 'Bold', 'Italic', '-', 'NumberedList','BulletedList', '-', 'Link', 'Unlink', 'Image' ] ];

//Toolbar definition for Advanced buttons
Drupal.settings.cke_toolbar_DrupalAdvanced = [
	['Source'],
	['Cut','Copy','Paste','PasteText','PasteFromWord','-','SpellChecker', 'Scayt'],
	['Undo','Redo','Find','Replace','-','SelectAll'],
	['Image','Flash','Table','HorizontalRule','Smiley','SpecialChar'],
	['Maximize', 'ShowBlocks'],
	'/',
	['Format'],
	['Bold','Italic','Underline','Strike','-','Subscript','Superscript','-','RemoveFormat'],
	['NumberedList','BulletedList','-','Outdent','Indent','Blockquote'],
	['JustifyLeft','JustifyCenter','JustifyRight','JustifyBlock','-','BidiLtr','BidiRtl'],
	['Link','Unlink','Anchor','Linkit','LinkToNode','LinkToMenu']
];

// Toolbar definition for all buttons
Drupal.settings.cke_toolbar_DrupalFull = [
	['Source'],
	['Cut','Copy','Paste','PasteText','PasteFromWord','-','SpellChecker', 'Scayt'],
	['Undo','Redo','Find','Replace','-','SelectAll'],
	['Image','Flash','Table','HorizontalRule','Smiley','SpecialChar','Iframe'],
	'/',
	['Bold','Italic','Underline','Strike','-','Subscript','Superscript','-','RemoveFormat'],
	['NumberedList','BulletedList','-','Outdent','Indent','Blockquote','CreateDiv'],
	['JustifyLeft','JustifyCenter','JustifyRight','JustifyBlock','-','BidiLtr','BidiRtl','-','Language'],
	['Link','Unlink','Anchor','Linkit','LinkToNode', 'LinkToMenu'],
	'/',
	['Format','Font','FontSize'],
	['TextColor','BGColor'],
	['Maximize', 'ShowBlocks'],
	['DrupalBreak', 'DrupalPageBreak']
];

/*
CKEDITOR.on( 'dialogDefinition', function( ev ) {
		// Take the dialog name and its definition from the event data
		var dialogName = ev.data.name;
		var dialogDefinition = ev.data.definition;

				console.log('Inside dialogDefinition.. dialogName = ' + dialogName + ', dialogDefinition = ' + dialogDefinition);
				console.log(dialogDefinition);

		if ( dialogName == 'image' || dialogName == 'enhanced_image' ) {

				// // Set the default value for the URL field.
				// var width = infoTab.get( 'txtWidth' );
				// width[ 'default' ] = '';

				// var height = infoTab.get( 'txtHeight' );
				// height[ 'default' ] = '';
		}
});
*/
	CKEDITOR.on('dialogDefinition', function( ev ) {
		var dialogName = ev.data.name;
		var dialogDefinition = ev.data.definition;
		var infoTab = dialogDefinition.getContents('info');

		if(dialogName === 'table') {
			var width = infoTab.get('txtWidth');
			width['default'] = '100%';
			var cellSpacing = infoTab.get('txtCellSpace');
			cellSpacing['default'] = '0';
			var cellPadding = infoTab.get('txtCellPad');
			cellPadding['default'] = '0';
			var border = infoTab.get('txtBorder');
			border['default'] = '0';
		}

		// // Below code not possible, because image width and height are set after the image is selected
		// if ( dialogName == 'image' || dialogName == 'enhanced_image' ) {

		// 	// Set the default value for the URL field.
		// 	var width = infoTab.get( 'txtWidth' );
		// 	width[ 'default' ] = '';

		// 	var height = infoTab.get( 'txtHeight' );
		// 	height[ 'default' ] = '';
		// }
	});
