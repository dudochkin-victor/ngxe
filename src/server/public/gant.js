var editor = null;
var project = null;
var elements = null;

function redraw() {
	editor.updateMap();
	editor.drawMap();
}
function editor_main() {
	editor = {};
	editor.mapDiv = document.getElementById('mapDiv');
	editor.mapCanvas = document.getElementById('mapCanvas');
	editor.autoCache = document.createElement('canvas');
	editor.mapCache = document.createElement('canvas');
	editor.state = 0;
	editor.resources = [];

	editor.drawMap = function() {
		var ctx = editor.mapCanvas.getContext('2d');
		ctx.save();
		ctx.clearRect(0, 0, editor.mapCanvas.width, editor.mapCanvas.height);
		ctx.drawImage(editor.mapCache, 0, 0, editor.mapCache.width,
				editor.mapCache.height);
		ctx.restore();
	};

	editor.updateMap = function() {
		var ctx = editor.mapCache.getContext('2d');
		ctx.save();
		ctx.clearRect(0, 0, editor.mapCanvas.width, editor.mapCanvas.height);
		ctx.restore();
	};

	function updateElements() {
		console.log(project)
		var triggers = project.triggers;
		var instructions = project.instructions;
		elements = {};
		for ( var idx = 0; idx < triggers.length; idx++) {
			var element = triggers[idx];
			elements[element.GUID] = element;
		}
		for ( var idx = 0; idx < instructions.length; idx++) {
			var element = instructions[idx];
			elements[element.GUID] = element;
		}

		//draw();
	}
	
	editor.ready = function() {
		editor.state++;
		if (editor.state == 2) {
			$
					.getJSON(
							'analis.json',
							function(data) {
								for (var key in data)
								{
									project = data[key].logics;
//									for (var t in project)
//										console.log(project[t])
								}
								editor.data = data;
								editor.state = 0;
								
								editor.mapCache.height = editor.mapCanvas.height;
								editor.mapCache.width = editor.mapCanvas.width;

								

//								for ( var j = 0; j < 7; j++)
//									constructAutoTiles(j, editor.auto,
//											autotile, editor.auto.width);
//
								var context = editor.mapCanvas.getContext('2d');
								context.save();
								context.clearRect(0, 0, editor.mapCanvas.width,
										editor.mapCanvas.height);
								context.setTransform(1, 0, 0, 1, 0, 1);
								updateElements();
								//////
//								if (app.selectedItem) {
//									var counter = 0;
//									for (idx in elements) {
//										counter++;
//										var element = elements[idx];
//										if (element.next) {
//											// draw out
//											var next = elements[element.next];
//											var dx = (next.x - element.x) / 2;
//											var dy = (element.y - next.y) / 2;
//											var controlX1 = element.x
//													+ ((element.type == 'TRIGGER') ? element.width / 4 * 3
//															: element.width / 2) + ((dy > 0) ? dx : 0);
//											var controlY1 = element.y + element.height / 2
//													+ Math.abs(dy);
//											var controlX2 = next.x + next.width / 2
//													- ((dy > 0) ? dx : 0);
//											var controlY2 = next.y + next.height / 2 - Math.abs(dy);
//											context.beginPath();
//											context
//													.moveTo(
//															element.x
//																	+ ((element.type == 'TRIGGER') ? element.width / 4 * 3
//																			: element.width / 2),
//															element.y + element.height);
//											context.bezierCurveTo(controlX1, controlY1, controlX2,
//													controlY2, next.x + next.width / 2, next.y + 8);
//											context.lineWidth = 5;
//											context.strokeStyle = "green"; // line color
//											context.stroke();
//										}
//										if (element.or && (element.type == 'TRIGGER')) {
//											// draw out
//											var next = elements[element.or];
//											if (next) {
//												var dx = (next.x - element.x) / 2;
//												var dy = (element.y - next.y) / 2;
//												var controlX1 = element.x + element.width / 4
//														+ ((dy > 0) ? dx : 0);
//												var controlY1 = element.y + element.height / 2
//														+ Math.abs(dy);
//												var controlX2 = next.x + next.width / 2
//														- ((dy > 0) ? dx : 0);
//												var controlY2 = next.y + next.height / 2 - Math.abs(dy);
//												context.beginPath();
//												context.moveTo(element.x + element.width / 4, element.y
//														+ element.height);
//												context.bezierCurveTo(controlX1, controlY1, controlX2,
//														controlY2, next.x + next.width / 2, next.y + 8);
//												context.lineWidth = 5;
//												context.strokeStyle = "brown"; // line color
//												context.stroke();
//											}
//										}
//									}
//
//									for (idx in elements) {
//										var element = elements[idx];
//										element.draw();
//									}
//								}
								/////
								ctx.restore();
							});
		}
	};

	editor.startPoint = null;
	editor.endPoint = null;
	editor.mode = 'tileset';
	editor.ready();
	editor.ready();
	
	TriggerGradient = function(gradient) {
		gradient.addColorStop(0.00, 'rgba(102,152, 102, 255)');
		gradient.addColorStop(0.49, 'rgba(58,  98,  58, 255)');
		gradient.addColorStop(0.50, 'rgba(34,  74,  34, 255)');
		gradient.addColorStop(1.00, 'rgba(0,    20,   0, 255)');
	};
	
	// ////////////////////
	editor.mapCanvas.onmousedown = function(e) {
		// editor.startPoint = {
		// 'x' : e.layerX,
		// 'y' : e.layerY
		// };
		// editor.mapCanvas.onmousemove = editor.tileOver;
	};

	editor.mapCanvas.onmouseup = function(e) {
		// editor.startPoint = null;
		// editor.endPoint = {
		// 'x' : e.layerX,
		// 'y' : e.layerY
		// };
		// editor.mapCanvas.onmousemove = null;
	};

	editor.mapCanvas.onmousemove = function(e) {
		if (editor.startPoint) {
			var ctx = editor.mapCanvas.getContext('2d');
			ctx.save();
			ctx
					.clearRect(0, 0, editor.mapCanvas.width,
							editor.mapCanvas.height);
			ctx.drawImage(editor.mapCache, 0, 0, editor.mapCache.width,
					editor.mapCache.height);
			var sx = (editor.endPoint.x>editor.startPoint.x)?parseInt(editor.startPoint.x * 32):parseInt(editor.endPoint.x * 32);
			var sy = (editor.endPoint.y>editor.startPoint.y)?parseInt(editor.startPoint.y * 32):parseInt(editor.endPoint.y * 32);
			var sw = Math.abs(((editor.endPoint.x>editor.startPoint.x)?editor.endPoint.x:editor.startPoint.x) * 32 - sx);
			var sh = Math.abs(((editor.endPoint.y>editor.startPoint.y)?editor.endPoint.y:editor.startPoint.y) * 32 - sy);
			var dx = parseInt(e.layerX / 32);
			var dy = Math.floor(e.layerY / 32);
			dy=((dy>0)?dy-1:0);

			ctx.strokeStyle = "yellow";
			ctx.strokeRect(dx * 32, dy * 32, sw, sh);
			ctx.restore();
		} else {

		}
	};
	// ////////////////////
	editor.switchPanel = function(tab) {
		$('#projectsLabel').removeClass('warning');
		$('#resourcesLabel').removeClass('warning');
		$('#tasksLabel').removeClass('warning');
		$('#usageLabel').removeClass('warning');

		$('#projectsPage').hide();
		$('#tasksPage').hide();
		$('#resourcesPage').hide();
		$('#usagePage').hide();
		$('#' + tab + 'Label').addClass('warning');
		$('#' + tab + 'Page').show();

		return false;
	};
}