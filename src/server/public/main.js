var _replaceContext = false; // replace the system context menu?
var _mouseOverContext = false; // is the mouse over the context menu?
var _noContext = false; // disable the context menu?
var drag = false;
var obj = null;
var offsetX = 0;
var offsetY = 0;
var mouseDownTime = 0;
var waitId = null;
var DBLCLICKTIME = 200;
var view_mode = 0; // 0 - quest view mode | 1 - trigger/instruction view mode
var spepObj = null;

var elements = {};

var app = {
	selectedItem : null
};
var condition = {
	selectedItem : null
};
var instruction = {
	selectedItem : null
};
var items = [];

function selectCondition(view) {
	if (condition.selectedItem != view) {
		$.observable(condition).setProperty("selectedItem", view);
	}
}
function selectInstruction(view) {
	if (instruction.selectedItem != view) {
		$.observable(instruction).setProperty("selectedItem", view);
	}
}

function select(view) {
	if (app.selectedItem != view) {
		$.observable(app).setProperty("selectedItem", view);
	}
}

// Modify the data
function addQuest() {
	var GUID = 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g,
			function(c) {
				var r = Math.random() * 16 | 0, v = c == 'x' ? r
						: (r & 0x3 | 0x8);
				return v.toString(16);
			});
	$.observable(items).insert(items.length, {
		'GUID' : GUID,
		'text' : 'New Item',
		'type' : 'STATE',
		'triggers' : [],
		'instructions' : []

	});
	// Set selection on the added item
	select($("#questList li:last").view());
}

function removeQuest() {
	$.observable(items).remove(items.length - 1, 1);
}

function storeQuests() {
	var quests = {};
	for ( var qi = 0; qi < items.length; qi++) {
		var item = items[qi];
		quests[item.GUID] = {
			'text' : item.text,
			'type' : item.type
		};

		var triggers = item.triggers;
		var instructions = item.instructions;
		quests[item.GUID].logics = logics = {};
		for ( var idx = 0; idx < triggers.length; idx++) {
			var element = triggers[idx];
			logics[element.GUID] = {
				'type' : element.type,
				'text' : element.text,
				'x' : element.x,
				'y' : element.y,
				'next' : element.next,
				'or' : element.or,
				'conditions' : element.conditions
			};
		}
		for ( var idx = 0; idx < instructions.length; idx++) {
			var element = instructions[idx];
			logics[element.GUID] = {
				'type' : element.type,
				'text' : element.text,
				'x' : element.x,
				'y' : element.y,
				'next' : element.next,
				'conditions' : element.conditions,
				'steps' : element.steps
			};
		}
	}
	jsonRPC({
		"method" : "admin.storeLogic",
		"params" : JSON.stringify(quests),
		"id" : "id value (optional)"
	}, onStoreQuests, onError);
}

function onStoreQuests(data) {
}

TriggerGradient = function(gradient) {
	gradient.addColorStop(0.00, 'rgba(102,152, 102, 255)');
	gradient.addColorStop(0.49, 'rgba(58,  98,  58, 255)');
	gradient.addColorStop(0.50, 'rgba(34,  74,  34, 255)');
	gradient.addColorStop(1.00, 'rgba(0,    20,   0, 255)');
};

InstructionGradient = function(gradient) {
	gradient.addColorStop(0.00, 'rgba(152,102, 102, 255)');
	gradient.addColorStop(0.49, 'rgba(98,  58,  58, 255)');
	gradient.addColorStop(0.50, 'rgba(74,  34,  34, 255)');
	gradient.addColorStop(1.00, 'rgba(20,    0,   0, 255)');
};

var components = {};
function component() {
	var name = 'hideWindow';
	components[name] = this;
	this.exec = function(resp, params) {
		// wnd.visible = false;
		resp.push({
			"cmd" : "login1",
			"login" : iLogin.text,
			"password" : iPassword.text
		});
	};
};
new component();

function onData(data) {
	var resp = [];
	var params = data.result.params;
	for ( var idx = 0; idx < params.length; idx++) {
		if (components[params[idx].cmd])
			components[params[idx].cmd].exec(resp, params[idx]);
	}
	// Next Step
	if (resp.length > 0) {
		jsonRPC({
			"method" : "ad.serve",
			"params" : resp,
			"id" : "id value (optional)"
		}, onData, onError);
	}
}

function onError() {
	console.log("JSONRPC failed");
}

function jsonRPC(obj, onData, onError) {
	$.ajax({
		contentType : 'application/json',
		data : JSON.stringify(obj),
		dataType : 'json',
		success : onData,
		error : onError,
		processData : false,
		type : 'POST',
		url : '/jsonrpc/'
	});
	return false;
}

/**
 * This is high-level function. It must react to delta being more/less than
 * zero.
 */
function handle(delta) {
	if (delta < 0)
		console.log('DELTA: ' + delta);
	else
		console.log('DELTA: ' + delta);
}

/**
 * Event handler for mouse wheel event.
 */
function wheel(event) {
	var delta = 0;
	if (!event) /* For IE. */
		event = window.event;
	if (event.wheelDelta) { /* IE/Opera. */
		delta = event.wheelDelta / 120;
	} else if (event.detail) {
		/** Mozilla case. */
		/**
		 * In Mozilla, sign of delta is different than in IE. Also, delta is
		 * multiple of 3.
		 */
		delta = -event.detail / 3;
	}
	/**
	 * If delta is nonzero, handle it. Basically, delta is now positive if wheel
	 * was scrolled up, and negative, if wheel was scrolled down.
	 */
	if (delta)
		handle(delta);
	/**
	 * Prevent default actions caused by mouse wheel. That might be ugly, but we
	 * handle scrolls somehow anyway, so don't bother here..
	 */
	if (event.preventDefault)
		event.preventDefault();
	event.returnValue = false;
}

// Main program starts here. The above functions are alphabetical order
function main() {
	window.scrollTo(0, 1);
	canvas = document.getElementById('canvas');
	canvas.focus();
	// set the canvas height to the window height and width
	context = canvas.getContext('2d');

	buffer = document.createElement('canvas');
	// set the canvas height to the window height and width
	buffer_ctx = this.buffer.getContext('2d');

	onResize();

	/**
	 * Initialization code. If you use your own event management code, change it
	 * as required.
	 */
	if (window.addEventListener)
		window.addEventListener('DOMMouseScroll', wheel, false);
	window.onmousewheel = document.onmousewheel = wheel;

	// window.onkeyup = onKeyUp;
	window.onkeypress = onKeyPress;
	window.onresize = onResize;
	canvas.onmousemove = mouseMove;
	canvas.onmouseup = mouseUp;
	canvas.onmousedown = mouseDown;

	if (canvas.addEventListener) {
		canvas.addEventListener('contextmenu', function(e) {
			if (_replaceContext)
				ContextShow(e);
			else
				e.preventDefault();
		}, false);
	} else {
		document.attachEvent('oncontextmenu', function() {
			window.event.returnValue = false;
		});
	}
	waitId = setInterval(draw, 20);

	jsonRPC({
		"method" : "admin.loadLogic",
		"params" : null,
		"id" : "id value (optional)"
	}, onLoadQuests, onError);

} // main

function onResize() {
	if (window.innerWidth > 800)
		canvas.width = window.innerWidth-15;
	else
		canvas.width = 800;
	if (window.innerHeight > 800)
		canvas.height = window.innerHeight;
	else
		canvas.height = 800;

	if (buffer) {
		buffer.width = window.innerWidth;
		buffer.height = window.innerHeight;
	}

	draw();
}

function InitContext() {
	_divContext.onmouseover = function() {
		_mouseOverContext = true;
	};
	_divContext.onmouseout = function() {
		_mouseOverContext = false;
	};
	_mouseOverContext = true;
	_replaceContext = true;
	// document.getElementById('aDisable').onclick = DisableContext;
	// document.getElementById('aEnable').onclick = EnableContext;
	EnableContext();
}

// call from the onMouseDown event, passing the event if standards compliant
function ContextMouseDown(event) {
	if (_noContext || _mouseOverContext)
		return;

	// IE is evil and doesn't pass the event object
	if (event == null)
		event = window.event;

}

function CloseContext() {
	_mouseOverContext = false;
	_divContext.style.display = 'none';
}

// call from the onContextMenu event, passing the event
// if this function returns false, the browser's context menu will not show up
function ContextShow(event) {
	return false;
}

function EnableContext() {
	_noContext = false;
	_mouseOverContext = false; // this gets left enabled when "disable menus"
	// is chosen
	// document.getElementById('aEnable').style.display = 'none';
	return false;
}
var positiveGuid = null;
var negativeGuid = null;
function ConnectPositive(GUID) {
	positiveGuid = GUID;
	_divContext.style.display = 'none';
	return false;
}

function ConnectNegative(GUID) {
	negativeGuid = GUID;
	_divContext.style.display = 'none';
	return false;
}
function mouseDown(event) {
	var type = 0; // Single click
	var downTime = new Date().getTime();
	if ((downTime - mouseDownTime) < DBLCLICKTIME)
		type = 1; // double click
	mouseDownTime = downTime;

	event = event || window.event;
	var canvas = document.getElementById('canvas');
	var x = event.pageX - canvas.offsetLeft;
	var y = event.pageY - canvas.offsetTop;

	if (app.selectedItem) {
		if (app.selectedItem.data) {
			var instructions = app.selectedItem.data.instructions;
			var triggers = app.selectedItem.data.triggers;
			for ( var idx = 0; idx < triggers.length; idx++) {
				var element = triggers[idx];
				obj = element.checkArea(x, y);
				if (obj)
					break;
			}
			if (!obj)
				for ( var idx = 0; idx < instructions.length; idx++) {
					var element = instructions[idx];
					obj = element.checkArea(x, y);
					if (obj)
						break;
				}
		}
	}

	if (obj) {
		if (type == 0) {
			offsetX = x - obj.x;
			offsetY = y - obj.y;
			// only show the context menu if the right mouse button is pressed
			// and a hyperlink has been clicked (the code can be made more
			// selective)
			if (event.button == 2) { // context menu
				_replaceContext = true;
				if (_noContext || _mouseOverContext)
					return;

				// IE is evil and doesn't pass the event object
				if (event == null)
					event = window.event;

				// we assume we have a standards compliant browser, but check if
				// we have IE
				// var target = event.target != null ? event.target
				// : event.srcElement;
				if (_replaceContext) {
					// document.body.scrollTop does not work in IE
					var scrollTop = document.body.scrollTop ? document.body.scrollTop
							: document.documentElement.scrollTop;
					var scrollLeft = document.body.scrollLeft ? document.body.scrollLeft
							: document.documentElement.scrollLeft;

					// hide the menu first to avoid an "up-then-over" visual
					// effect
					if (obj.type == 'TRIGGER') {
						_divContext.innerHTML = '<ul class="cmenu">'
								+ '<li><a id="aContextNav" href="" onclick="return ConnectPositive(\''
								+ obj.GUID
								+ '\');">Connect Positive Out</a></li>'
								+ '<li><a id="aAddWebmark" href="" onclick="return ConnectNegative(\''
								+ obj.GUID
								+ '\');">Connect Negative Out</a></li>'
								+ '</ul>';
					} else if (obj.type == 'INSTRUCTION') {
						_divContext.innerHTML = '<ul class="cmenu">'
								+ '<li><a id="aContextNav" href="" onclick="return ConnectPositive(\''
								+ obj.GUID + '\');">Connect Out</a></li>'
								+ '</ul>';
					}
					_divContext.style.display = 'none';
					_divContext.style.left = event.clientX + scrollLeft + 'px';
					_divContext.style.top = event.clientY + scrollTop + 'px';
					_divContext.style.display = 'block';

					_replaceContext = false;
					return false;
				}
			} else {
				// single click
				if (positiveGuid) {
					if (obj.GUID != positiveGuid) {
						elements[positiveGuid].next = obj.GUID;
						draw();
					}
					positiveGuid = null;
				} else if (negativeGuid) {
					if (obj.GUID != negativeGuid) {
						elements[negativeGuid].or = obj.GUID;
						draw();
					}
					negativeGuid = null;
				} else
					drag = true;
				if (!_mouseOverContext)
					_divContext.style.display = 'none';
			}
			obj.onMouseDown(obj);
		} else if (type == 1) {
			// double click - toggle view mode
			if (view_mode == 0) {
				spepObj = obj;
				view_mode = 1;
			} else
				view_mode = 0;
			draw();
		}
	} else {
		if ((view_mode == 1) && (type == 1)) { // double click on empty zone to
			// return to quest mode
			view_mode = 0;
			draw();
		}
	}
	// only show the context menu if the right mouse button is pressed
	// and a hyperlink has been clicked (the code can be made more
	// selective)
	if (!_mouseOverContext)
		_divContext.style.display = 'none';
}

// Движение мыши
function mouseMove(event) {
	var mouseY = event.pageY;
	var mouseX = event.pageX;
	if (drag) {
		// Изменение координат фигуры
		obj.x = mouseX - offsetX;
		obj.y = mouseY - offsetY;
		draw();
	}
}
// Если отпущина кнопка мыши, то переменная drag принимает ложное значение
function mouseUp(event) {
	drag = false;
	// var x = event.pageX - canvas.offsetLeft;
	// var y = event.pageY - canvas.offsetTop;
	// obj = wnd.checkArea(x, y);
	if (obj) {
		if (obj.type != 'WINDOW') {
			obj.onMouseUp(obj);
			if (obj.type == 'INPUT')
				draw();
		}
	}
}

function onKeyPress(event) {
	if (focused) {
		switch (event.keyCode) {
		case 8:
			// Backspace
			focused.text = focused.text.substr(0, focused.text.length - 1);
			if (focused.pos > 0)
				focused.pos--;
			focused.prepare();
			draw();
			break;
		case 46:
			// Delete
			// focused.text = focused.text.substr(0, focused.text.length-1);
			break;
		case 45:
			// insert
			break;
		case 35:
			// end
			break;
		case 36:
			// home
			break;
		case 33:
			// pgup
			break;
		case 34:
			// pgdn
			break;
		case 37:
			// left
			break;
		case 38:
			// up
			break;
		case 39:
			// right
			break;
		case 40:
			// down
			break;
		case 13:
			// Enter
			console.log(focused.text);
			break;
		case 144:
			// Enter
			break;
		case 0:
			if (event.charCode != 0) {
				focused.text += String.fromCharCode(event.charCode);
				if (focused.text.length > focused.chars)
					focused.pos++;
				focused.prepare();
				draw();
			}
			break;
		default:
			break;
		}
	} // else
	// console.log(event);
}

function setShadow(context, color, ox, oy, blur) {
	context.shadowColor = color;
	context.shadowOffsetX = ox;
	context.shadowOffsetY = oy;
	context.shadowBlur = blur;
	return [ color, ox, oy, blur ].join(", ");
}

function clearShadow(context) {
	context.shadowColor = 0;
	context.shadowOffsetX = 0;
	context.shadowOffsetY = 0;
	context.shadowBlur = 0;
	return;
}

function draw() {
	context.save();
	// clear the drawing surface
	context.clearRect(0, 0, canvas.width, canvas.height);
	// context.drawImage(buffer, 0, 0);
	context.setTransform(1, 0, 0, 1, 0, 1);
	if (view_mode == 0) {

		if (app.selectedItem) {
			var counter = 0;
			for (idx in elements) {
				counter++;
				var element = elements[idx];
				if (element.next) {
					// draw out
					var next = elements[element.next];
					var dx = (next.x - element.x) / 2;
					var dy = (element.y - next.y) / 2;
					var controlX1 = element.x
							+ ((element.type == 'TRIGGER') ? element.width / 4 * 3
									: element.width / 2) + ((dy > 0) ? dx : 0);
					var controlY1 = element.y + element.height / 2
							+ Math.abs(dy);
					var controlX2 = next.x + next.width / 2
							- ((dy > 0) ? dx : 0);
					var controlY2 = next.y + next.height / 2 - Math.abs(dy);
					context.beginPath();
					context
							.moveTo(
									element.x
											+ ((element.type == 'TRIGGER') ? element.width / 4 * 3
													: element.width / 2),
									element.y + element.height);
					context.bezierCurveTo(controlX1, controlY1, controlX2,
							controlY2, next.x + next.width / 2, next.y + 8);
					context.lineWidth = 5;
					context.strokeStyle = "green"; // line color
					context.stroke();
				}
				if (element.or && (element.type == 'TRIGGER')) {
					// draw out
					var next = elements[element.or];
					if (next)
					{
						var dx = (next.x - element.x) / 2;
						var dy = (element.y - next.y) / 2;
						var controlX1 = element.x + element.width / 4
								+ ((dy > 0) ? dx : 0);
						var controlY1 = element.y + element.height / 2
								+ Math.abs(dy);
						var controlX2 = next.x + next.width / 2
								- ((dy > 0) ? dx : 0);
						var controlY2 = next.y + next.height / 2 - Math.abs(dy);
						context.beginPath();
						context.moveTo(element.x + element.width / 4, element.y
								+ element.height);
						context.bezierCurveTo(controlX1, controlY1, controlX2,
								controlY2, next.x + next.width / 2, next.y + 8);
						context.lineWidth = 5;
						context.strokeStyle = "brown"; // line color
						context.stroke();
					}
				}
			}

			for (idx in elements) {
				var element = elements[idx];
				element.draw();
			}
		}
	} else if (view_mode = 1) {
		// draw trigger/instruction details
	}
	context.restore();
	var allDone = true;
	if (allDone)
		clearInterval(waitId);
}

function updateElements() {
	var triggers = app.selectedItem.data.triggers;
	var instructions = app.selectedItem.data.instructions;
	elements = {};
	for ( var idx = 0; idx < triggers.length; idx++) {
		var element = triggers[idx];
		elements[element.GUID] = element;
	}
	for ( var idx = 0; idx < instructions.length; idx++) {
		var element = instructions[idx];
		elements[element.GUID] = element;
	}

	draw();
}

function createTrigger(title) {
	var trigger = new UIButton(title);
	trigger.x = canvas.width / 2 + Math.random() * canvas.width / 6
			- canvas.width / 12;
	trigger.y = canvas.height / 2 + Math.random() * canvas.height / 6
			- canvas.height / 12;
	trigger.conditions = [];
	trigger.GUID = 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g,
			function(c) {
				var r = Math.random() * 16 | 0, v = c == 'x' ? r
						: (r & 0x3 | 0x8);
				return v.toString(16);
			});
	trigger.type = 'TRIGGER';

	trigger.prepare = function(sx, sy) {
		if (this.visible) {
			var ctx = null;
			var dx = 2;
			var dy = 8;

			if (this.parent)
				this.buffer = this.parent.buffer;
			if (!this.buffer) {
				this.buffer = document.createElement('canvas');
				ctx = this.buffer.getContext('2d');
				ctx.textAlign = 'center';
				ctx.textBaseline = 'middle';
				ctx.font = '400 18px Unknown Font, sans-serif';
				this.width = ctx.measureText(this.text).width + 20;
				this.height = 18 + 35;
				this.buffer.width = this.width + 4;
				this.buffer.height = this.height + 4;
			} else {
				ctx = this.buffer.getContext('2d');
				ctx.textAlign = 'center';
				ctx.textBaseline = 'middle';
				ctx.font = '400 18px Unknown Font, sans-serif';
				this.width = ctx.measureText(this.text).width + 20;
				this.height = 18 + 35;
				if (this.parent) {
					dx = this.x;
					dy = this.y;
				} else {
					this.buffer.width = this.width + 4;
					this.buffer.height = this.height + 4;
				}
			}

			var ww = this.width;
			var hh = this.height - 12;

			ctx.textAlign = 'center';
			ctx.textBaseline = 'middle';
			ctx.font = '400 18px Unknown Font, sans-serif';
			ww = ctx.measureText(this.text).width + 20;

			ctx.strokeStyle = 'rgba(255,255, 255, 0.5)';
			ctx.lineWidth = 4;
			var gradient = ctx.createLinearGradient(dx, dy, dx, dy + hh);
			TriggerGradient(gradient);
			ctx.fillStyle = gradient;

			ctx.beginPath();
			ctx.moveTo(dx, dy + this.arc);
			ctx.lineTo(dx, dy + hh - this.arc);
			ctx.bezierCurveTo(dx, dy + hh - this.arc / 2, dx + this.arc / 2, dy
					+ hh, dx + this.arc, dy + hh);
			ctx.lineTo(dx + ww - this.arc, dy + hh);
			ctx.bezierCurveTo(dx + ww - this.arc / 2, dy + hh, dx + ww, dy + hh
					- this.arc / 2, dx + ww, dy + hh - this.arc);
			ctx.lineTo(dx + ww, dy + this.arc);
			ctx.bezierCurveTo(dx + ww, dy + this.arc / 2, dx + ww - this.arc
					/ 2, dy, dx + ww - this.arc, dy);
			ctx.lineTo(dx + this.arc, dy);
			ctx.bezierCurveTo(dx + this.arc / 2, dy, dx, dy + this.arc / 2, dx,
					dy + this.arc);
			ctx.stroke();
			ctx.fill();

			ctx.fillStyle = 'gold';
			ctx.beginPath();
			var endAngle = Math.PI * 2; // End point on circle
			ctx.arc(dx + ww / 2, dy, 6, 0, endAngle);

			ctx.stroke();
			ctx.fill();

			ctx.fillStyle = 'brown';
			ctx.beginPath();
			var endAngle = Math.PI * 2; // End point on circle
			ctx.arc(dx + ww / 4, dy + hh, 6, 0, endAngle);

			ctx.stroke();
			ctx.fill();

			ctx.fillStyle = 'green';
			ctx.beginPath();
			var endAngle = Math.PI * 2; // End point on circle
			ctx.arc(dx + ww / 4 * 3, dy + hh, 6, 0, endAngle);

			ctx.stroke();
			ctx.fill();

			ctx.fillStyle = 'white';
			ctx.fillText(this.text, dx + ww / 2, dy + hh / 2);
		}
	};
	trigger.onMouseDown = function() {
		$("#itemDetail").link(this, "#conditionViewTemplate");
	};
	trigger.prepare();
	$(trigger).bind("propertyChange", function(event, args) {
		if (args.path === "text") {
			trigger.prepare();
			draw();
		}
	});
	return trigger;
}

function copyProp(dst, src)
{
	for (idx in src)
		dst[idx] = src[idx];
}

function onLoadQuests(data) {
	var logic = data.result.data.object;
	if (logic)
		logic = JSON.parse(logic);
	$.observable(items).remove(0, items.length);
	for (guid in logic) {
		var element = logic[guid];
		$.observable(items).insert(items.length, {
			'GUID' : guid,
			'text' : element.text,
			'type' : element.type,
			'triggers' : [],
			'instructions' : []

		});
		// Set selection on the added item
		var view = $('#questList li:last').view();
		select(view);
		for (idx in element.logics) {
			var obj = element.logics[idx];
			if (obj.type == 'TRIGGER') {
				var triggers = view.data.triggers;
				var trigger = createTrigger(obj.text);
				trigger.GUID = idx;
				copyProp(trigger, obj);
				var pos = triggers.length;
				$.observable(triggers).insert(pos, trigger);
				trigger.pos = pos;
			} else if (obj.type == 'INSTRUCTION') {
				var instructions = view.data.instructions;
				var instruction = createInstruction(obj.text);
				instruction.GUID = idx;
				copyProp(instruction, obj);
				var pos = instructions.length;
				$.observable(instructions).insert(pos, instruction);
				instruction.pos = pos;
			} else
				console.log(idx);
		}
	}
}

function addTriggerHandler() {
	var triggers = $.view(this).data.triggers;
	var trigger = createTrigger('New Trigger');
	var pos = triggers.length;
	$.observable(triggers).insert(pos, trigger);
	trigger.pos = pos;
	updateElements();
	return false;
}
function createInstruction(title) {
	var instruction = new UIButton(title);
	instruction.x = canvas.width / 2 + Math.random() * canvas.width / 6
			- canvas.width / 12;
	instruction.y = canvas.height / 2 + Math.random() * canvas.height / 6
			- canvas.height / 6;
	instruction.conditions = [];
	instruction.steps = [];
	instruction.GUID = 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g,
			function(c) {
				var r = Math.random() * 16 | 0, v = c == 'x' ? r
						: (r & 0x3 | 0x8);
				return v.toString(16);
			});
	instruction.type = 'INSTRUCTION';

	instruction.prepare = function(sx, sy) {
		if (this.visible) {
			var ctx = null;
			var dx = 2;
			var dy = 8;

			if (this.parent)
				this.buffer = this.parent.buffer;
			if (!this.buffer) {
				this.buffer = document.createElement('canvas');
				ctx = this.buffer.getContext('2d');
				ctx.textAlign = 'center';
				ctx.textBaseline = 'middle';
				ctx.font = '400 18px Unknown Font, sans-serif';
				this.width = ctx.measureText(this.text).width + 20;
				this.height = 18 + 35;
				this.buffer.width = this.width + 4;
				this.buffer.height = this.height + 4;
			} else {
				ctx = this.buffer.getContext('2d');
				ctx.textAlign = 'center';
				ctx.textBaseline = 'middle';
				ctx.font = '400 18px Unknown Font, sans-serif';
				this.width = ctx.measureText(this.text).width + 20;
				this.height = 18 + 35;
				if (this.parent) {
					dx = this.x;
					dy = this.y;
				} else {
					this.buffer.width = this.width + 4;
					this.buffer.height = this.height + 4;
				}
			}

			var ww = this.width;
			var hh = this.height - 12;
			ctx.textAlign = 'center';
			ctx.textBaseline = 'middle';
			ctx.font = '400 18px Unknown Font, sans-serif';
			ww = ctx.measureText(this.text).width + 20;

			ctx.strokeStyle = 'rgba(255,255, 255, 0.5)';
			ctx.lineWidth = 4;
			var gradient = ctx.createLinearGradient(dx, dy, dx, dy + hh);
			InstructionGradient(gradient);
			ctx.fillStyle = gradient;

			ctx.beginPath();
			ctx.moveTo(dx, dy + this.arc);
			ctx.lineTo(dx, dy + hh - this.arc);
			ctx.bezierCurveTo(dx, dy + hh - this.arc / 2, dx + this.arc / 2, dy
					+ hh, dx + this.arc, dy + hh);
			ctx.lineTo(dx + ww - this.arc, dy + hh);
			ctx.bezierCurveTo(dx + ww - this.arc / 2, dy + hh, dx + ww, dy + hh
					- this.arc / 2, dx + ww, dy + hh - this.arc);
			ctx.lineTo(dx + ww, dy + this.arc);
			ctx.bezierCurveTo(dx + ww, dy + this.arc / 2, dx + ww - this.arc
					/ 2, dy, dx + ww - this.arc, dy);
			ctx.lineTo(dx + this.arc, dy);
			ctx.bezierCurveTo(dx + this.arc / 2, dy, dx, dy + this.arc / 2, dx,
					dy + this.arc);
			ctx.stroke();
			ctx.fill();

			ctx.fillStyle = 'gold';
			ctx.beginPath();
			var endAngle = Math.PI * 2; // End point on circle
			ctx.arc(dx + ww / 2, dy, 6, 0, endAngle);

			ctx.stroke();
			ctx.fill();

			ctx.fillStyle = 'green';
			ctx.beginPath();
			var endAngle = Math.PI * 2; // End point on circle
			ctx.arc(dx + ww / 2, dy + hh, 6, 0, endAngle);

			ctx.stroke();
			ctx.fill();

			ctx.fillStyle = 'white';
			ctx.fillText(this.text, dx + ww / 2, dy + hh / 2);
		}
	};
	instruction.onMouseDown = function() {
		$("#itemDetail").link(this, "#instructionViewTemplate");
	};
	instruction.prepare();

	$(instruction).bind("propertyChange", function(event, args) {
		if (args.path === "text") {
			instruction.prepare();
			draw();
		}
	});
	return instruction;
}
function addInstructionHandler() {
	var instructions = $.view(this).data.instructions;
	var instruction = createInstruction('New Instruction');
	var pos = instructions.length;
	$.observable(instructions).insert(pos, instruction);
	instruction.pos = pos;
	updateElements();
	return false;
}