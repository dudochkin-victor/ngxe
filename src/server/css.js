var util = require('util');

var styles = {
	'animation' : {
		'moz' : true,
		'webkit' : true,
		'desc' : ''
	},
	'animation-name' : {
		'moz' : true,
		'webkit' : true,
		'desc' : ''
	},
	'animation-duration' : {
		'moz' : true,
		'webkit' : true,
		'desc' : ''
	},
	'animation-timing-function' : {
		'moz' : true,
		'webkit' : true,
		'desc' : ''
	},
	'animation-delay' : {
		'moz' : true,
		'webkit' : true,
		'desc' : ''
	},
	'animation-iteration-count' : {
		'moz' : true,
		'webkit' : true,
		'desc' : ''
	},
	'animation-direction' : {
		'moz' : true,
		'webkit' : true,
		'desc' : ''
	},
	'animation-play-state' : {
		'moz' : true,
		'webkit' : true,
		'desc' : ''
	},
	'appearance' : {
		'moz' : true,
		'webkit' : true,
		'desc' : ''
	},
	'backface-visibility' : {
		'moz' : false,
		'webkit' : true,
		'desc' : ''
	},
	'background' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'background-attachment' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'background-color' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'background-image' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'background-position' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'background-repeat' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'background-clip' : {
		'moz' : false,
		'webkit' : true,
		'desc' : ''
	},
	'background-origin' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'background-size' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-bottom' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-bottom-color' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-bottom-style' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-bottom-width' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-collapse' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-color' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-left' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-left-color' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-left-style' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-left-width' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-right' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-right-color' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-right-style' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-right-width' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-spacing' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-style' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-top' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-top-color' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-top-style' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-top-width' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-width' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-bottom-left-radius' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-bottom-right-radius' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-image' : {
		'moz' : true,
		'webkit' : true,
		'opera' : true,
		'desc' : ''
	},
	'border-image-outset' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-image-repeat' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-image-slice' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-image-source' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-image-width' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-radius' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-top-left-radius' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'border-top-right-radius' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'bottom' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'box-align' : {
		'moz' : true,
		'webkit' : true,
		'desc' : ''
	},
	'box-direction' : {
		'moz' : true,
		'webkit' : true,
		'desc' : ''
	},
	'box-flex' : {
		'moz' : true,
		'webkit' : true,
		'desc' : ''
	},
	'box-flex-group' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'box-lines' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'box-ordinal-group' : {
		'moz' : true,
		'webkit' : true,
		'desc' : ''
	},
	'box-orient' : {
		'moz' : true,
		'webkit' : true,
		'desc' : ''
	},
	'box-pack' : {
		'moz' : true,
		'webkit' : true,
		'desc' : ''
	},
	'box-sizing' : {
		'moz' : true,
		'webkit' : true,
		'desc' : ''
	},
	'box-shadow' : {
		'moz' : false,
		'webkit' : true,
		'desc' : ''
	},
	'caption-side' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'clear' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'clip' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'color' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'column' : {
		'moz' : true,
		'webkit' : true,
		'desc' : ''
	},
	'column-count' : {
		'moz' : true,
		'webkit' : true,
		'desc' : ''
	},
	'column-fill' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'column-gap' : {
		'moz' : true,
		'webkit' : true,
		'desc' : ''
	},
	'column-rule' : {
		'moz' : true,
		'webkit' : true,
		'desc' : ''
	},
	'column-rule-color' : {
		'moz' : true,
		'webkit' : true,
		'desc' : ''
	},
	'column-rule-style' : {
		'moz' : true,
		'webkit' : true,
		'desc' : ''
	},
	'column-rule-width' : {
		'moz' : true,
		'webkit' : true,
		'desc' : ''
	},
	'column-span' : {
		'moz' : false,
		'webkit' : true,
		'desc' : ''
	},
	'column-width' : {
		'moz' : true,
		'webkit' : true,
		'desc' : ''
	},
	'columns' : {
		'moz' : true,
		'webkit' : true,
		'desc' : ''
	},
	'content' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'counter-increment' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'counter-reset' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'cursor' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'direction' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'display' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'empty-cells' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'float' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'font' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'font-family' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'font-size' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'font-style' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'font-variant' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'font-weight' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'@font-face' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'font-size-adjust' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'font-stretch' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'grid-columns' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'grid-rows' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'hanging-punctuation' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'height' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'icon' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'@keyframes' : {
		'moz' : true,
		'webkit' : true,
		'desc' : ''
	},
	'left' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'letter-spacing' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'line-height' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'list-style' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'list-style-image' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'list-style-position' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'list-style-type' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'margin' : {
		'moz' : true,
		'webkit' : true,
		'desc' : ''
	},
	'margin-bottom' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'margin-left' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'margin-right' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'margin-top' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'max-height' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'max-width' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'min-height' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'min-width' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'nav-down' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'nav-index' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'nav-left' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'nav-right' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'nav-up' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'opacity' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'outline' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'outline-color' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'outline-offset' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'outline-style' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'outline-width' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'overflow' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'overflow-x' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'overflow-y' : {
		'moz' : true,
		'webkit' : true,
		'desc' : ''
	},
	'padding' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'padding-bottom' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'padding-left' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'padding-right' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'padding-top' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'page-break-after' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'page-break-before' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'page-break-inside' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'perspective' : {
		'moz' : false,
		'webkit' : true,
		'desc' : ''
	},
	'perspective-origin' : {
		'moz' : false,
		'webkit' : true,
		'desc' : ''
	},
	'position' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'punctuation-trim' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'quotes' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'resize' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'right' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'rotation' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'rotation-point' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'table-layout' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'target' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'target-name' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'target-new' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'target-position' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'text-align' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'text-decoration' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'text-indent' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'text-justify' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'text-outline' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'text-overflow' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'text-shadow' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'text-transform' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'text-wrap' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'top' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'transform' : {
		'moz' : true,
		'webkit' : true,
		'ms' : true,
		'opera' : true,
		'desc' : ''
	},
	'transform-origin' : {
		'moz' : true,
		'webkit' : true,
		'ms' : true,
		'opera' : true,
		'desc' : ''
	},
	'transform-style' : {
		'moz' : false,
		'webkit' : true,
		'desc' : ''
	},
	'transition' : {
		'moz' : true,
		'webkit' : true,
		'opera' : true,
		'desc' : ''
	},
	'transition-property' : {
		'moz' : true,
		'webkit' : true,
		'opera' : true,
		'desc' : ''
	},
	'transition-duration' : {
		'moz' : true,
		'webkit' : true,
		'opera' : true,
		'desc' : ''
	},
	'transition-timing-function' : {
		'moz' : true,
		'webkit' : true,
		'opera' : true,
		'desc' : ''
	},
	'transition-delay' : {
		'moz' : true,
		'webkit' : true,
		'opera' : true,
		'desc' : ''
	},
	'vertical-align' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'visibility' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'width' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'white-space' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'word-spacing' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'word-break' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'word-wrap' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	},
	'z-index' : {
		'moz' : false,
		'webkit' : false,
		'desc' : ''
	}
};

function ucfirst(str) {
	var f = str.charAt(0).toUpperCase();
	return f + str.substr(1, str.length - 1);
}

for ( var style in styles) {
	var dom = style;
	var parts = dom.split('-');
	if (parts.length > 1) {
		dom = parts[0];
		for ( var pidx = 1; pidx < parts.length; pidx++)
			dom += ucfirst(parts[pidx]);
	}
	styles[style].dom = dom;
	if (styles[style].moz)
		styles[style].moz = '-moz-' + style;
	else
		delete styles[style].moz;
	if (styles[style].webkit)
		styles[style].webkit = '-webkit-' + style;
	else
		delete styles[style].webkit;
	if (styles[style].opera)
		styles[style].opera = '-o-' + style;
	else
		delete styles[style].opera;
	if (styles[style].ms)
		styles[style].ms = '-ms-' + style;
	else
		delete styles[style].ms;
}

util.puts(JSON.stringify(styles));
