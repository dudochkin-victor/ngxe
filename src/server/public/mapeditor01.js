var editor = null;
function editor_main() {
	editor = {};
	editor.tileset = new Image();
	editor.auto = new Image();
	editor.tileset.src = '/Graphics/Tilesets/027-Castle03.png';
	editor.auto.src = '/Graphics/Autotiles/053-Wall01.png';
	editor.tileCanvas = document.getElementById('tilesetCanvas');
	editor.autoCanvas = document.getElementById('autoCanvas');
	editor.mapCanvas = document.getElementById('mapCanvas');

	editor.tileset.onload = function() {
		editor.tileCanvas.height = editor.tileset.height;
		editor.tileCanvas.width = editor.tileset.width;
		var ctx = editor.tileCanvas.getContext('2d');
		ctx.save();
		ctx.clearRect(0, 0, editor.tileCanvas.width, editor.tileCanvas.height);
		ctx.setTransform(1, 0, 0, 1, 0, 1);
		ctx.drawImage(editor.tileset, 0, 0, editor.tileset.width,
				editor.tileset.height);
		ctx.restore();
	};

	editor.auto.onload = function() {
		editor.autoCanvas.height = editor.auto.height;
		editor.autoCanvas.width = editor.auto.width;
		var ctx = editor.autoCanvas.getContext('2d');
		ctx.save();
		ctx.clearRect(0, 0, editor.autoCanvas.width, editor.autoCanvas.height);
		ctx.setTransform(1, 0, 0, 1, 0, 1);
		ctx.drawImage(editor.auto, 0, 0, editor.auto.width, editor.auto.height);
		ctx.restore();
	};
	editor.tileCanvas.onmousedown = function(e) {
		console.log(e);
	};
	
	editor.tileCanvas.onmouseup = function(e) {
		console.log(e);	
	};
	
	editor.tileCanvas.onmouseover = function(e) {
		editor.tileCanvas.onmousemove = function(e) {
			//console.log(e);
		};
	};
	editor.tileCanvas.onmouseout = function(e) {
		editor.tileCanvas.onmousemove = null;
	};
}