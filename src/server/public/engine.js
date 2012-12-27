var methods = {};
var drag = false;
var obj = null;
var offsetX = 0;
var offsetY = 0;

var sex = [ 'male', 'female' ];
var race = [ 'angels', 'daemons' ];
var resp = [];
var angels = [ {
	"sex" : 1,
	"type" : "heruvim"
}, {
	"sex" : 1,
	"type" : "paladin"
}, {
	"sex" : 1,
	"type" : "serafim"
}, {
	"sex" : 0,
	"type" : "heruvim"
}, {
	"sex" : 0,
	"type" : "paladin"
}, {
	"sex" : 0,
	"type" : "serafim"
} ];
var daemons = [ {
	"sex" : 1,
	"type" : "iskazh"
}, {
	"sex" : 1,
	"type" : "padsh"
}, {
	"sex" : 0,
	"type" : "iskazh"
}, {
	"sex" : 0,
	"type" : "padsh"
} ];

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

function prepare() {
	buffer_ctx.save();
	// clear the drawing surface
	buffer_ctx.clearRect(0, 0, canvas.width, canvas.height);
	setShadow(buffer_ctx, "white", 0, 0, 20);

	var dx = 0;
	var allDone = true;
	for ( var idx = 0; idx < personages.length; idx++) {
		if (!personages[idx].draw(buffer_ctx, dx, 0))
			allDone = false;
		dx += personages[idx].width * personages[idx].scaleFactor;
	}

	clearShadow(buffer_ctx);
	buffer_ctx.restore();
}

function mouseDown(event) {
	event = event || window.event;

	var canvas = document.getElementById('canvas'), x = event.pageX
			- canvas.offsetLeft, y = event.pageY - canvas.offsetTop;
	/*
	 * obj = wnd.checkArea(x, y); if (obj) { if (obj.type == 'WINDOW') { drag =
	 * true; offsetX = x - obj.x; offsetY = y - obj.y; } else
	 * obj.onMouseDown(obj); }
	 */
}

// Движение мыши
function mouseMove(event) {
	var mouseY = event.pageY;
	var mouseX = event.pageX;
	if (drag) {
		// Изменение координат фигуры
		obj.x = mouseX - offsetX;
		obj.y = mouseY - offsetY;
		context.save();
		// clear the drawing surface
		context.clearRect(0, 0, canvas.width, canvas.height);
		context.setTransform(1, 0, 0, 1, 0, 1);
		context.restore();
	}
}
// Если отпущена кнопка мыши, то переменная drag принимает ложное значение
function mouseUp(event) {
	drag = false;
	var x = event.pageX - canvas.offsetLeft, y = event.pageY - canvas.offsetTop;
	/*
	 * obj = wnd.checkArea(x, y); if (obj) { if (obj.type != 'WINDOW') {
	 * obj.onMouseUp(obj); if (obj.type == 'INPUT') { context.save(); // clear
	 * the drawing surface context.clearRect(0, 0, canvas.width, canvas.height);
	 * context.setTransform(1, 0, 0, 1, 0, 1); context.restore(); } } }
	 */
}

function onKeyPress(event) {
}

function onResize() {
	if (window.innerWidth > 800)
		canvas.width = window.innerWidth - 15 - 370;
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
	for ( var idx = 0; idx < personages.length; idx++) {
		personages[idx].rescale();
	}
	prepare();
	context.save();
	// clear the drawing surface
	context.clearRect(0, 0, canvas.width, canvas.height);
	context.setTransform(1, 0, 0, 1, 0, 1);
	context.restore();
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

methods.initMethod = function(data) {
	var method = null;
	eval('method = ' + data.params.body);
	methods[data.params.method] = method;
};

methods.initUI = function(data) {
	ui.create(data.params.body);
};

function onData(data) {
	resp = [];
	var params = data.result.params;
	for ( var idx = 0; idx < params.length; idx++) {
		if (methods[params[idx].cmd])
			methods[params[idx].cmd](params[idx]);
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
	console.log("Device control failed");
}

function UI() {
	var elements = {};

	this.getByName = function(name) {
		return elements[name];
	};

	this.create = function(uimodel, parent) {
		for ( var prop in uimodel) {
			model = uimodel[prop];
			var obj = document.createElement(model.element);
			obj.setAttribute('id', prop);
			if (model.className)
				obj.className = model.className;
			if (model.style) {
				for (styleAttr in model.style)
					obj.style[styleAttr] = model.style[styleAttr];
			}

			if (model.text)
				obj.innerHTML = model.text;
			if (model.src)
				obj.setAttribute('src', model.src);
			if (model.size)
				obj.setAttribute('size', model.size);
			if (model.multiple)
				obj.setAttribute('multiple', model.multiple);
			if (model.onclick) {
				if (model.onclick in methods) {
					{

					}
				} else
					jsonRPC({
						"method" : "ad.serve",
						"params" : [ {
							"cmd" : "getMethod",
							"method" : model.onclick
						} ],
						"id" : "id value (optional)"
					}, onData, onError);
				eval('obj.onclick = function(){return methods[\''
						+ model.onclick + '\'](this);};');
			}
			if (model.onkeypress) {
				if (model.onkeypress in methods) {
					{

					}
				} else
					jsonRPC({
						"method" : "ad.serve",
						"params" : [ {
							"cmd" : "getMethod",
							"method" : model.onkeypress
						} ],
						"id" : "id value (optional)"
					}, onData, onError);
				eval('obj.onkeypress = function(e){return methods[\''
						+ model.onkeypress + '\'](this, e);};');
			}
			if (model.onchange) {
				if (model.onchange in methods) {
					{

					}
				} else
					jsonRPC({
						"method" : "ad.serve",
						"params" : [ {
							"cmd" : "getMethod",
							"method" : model.onchange
						} ],
						"id" : "id value (optional)"
					}, onData, onError);
				eval('obj.onchange = function(e){return methods[\''
						+ model.onchange + '\'](this, e);};');
			}
			if (parent)
				parent.appendChild(obj);
			else
				document.body.appendChild(obj);
			var name = (parent) ? parent + '/' + prop : prop;
			elements[name] = obj;
			if (uimodel[prop].childs)
				this.create(uimodel[prop].childs, obj);
		}
	};
};

function getCookie(name) {
	var matches = document.cookie
			.match(new RegExp("(?:^|; )"
					+ name.replace(/([\.$?*|{}\(\)\[\]\\\/\+^])/g, '\\$1')
					+ "=([^;]*)"));
	return matches ? decodeURIComponent(matches[1]) : undefined;
}

function logout(name, path, domain) {
	if (getCookie('SID')) {
		document.cookie = "SID=; path=/; domain=." + document.domain
				+ "; expires=Thu, 01-Jan-70 00:00:01 GMT";
	}
}

var ui = new UI();

// Main program starts here. The above functions are alphabetical order
function fallen_main() {
	window.scrollTo(0, 1);
	canvas = document.getElementById('canvas');
	canvas.focus();
	// set the canvas height to the window height and width
	window.context = canvas.getContext('2d');

	window.buffer = document.createElement('canvas');
	// set the canvas height to the window height and width
	window.buffer_ctx = this.buffer.getContext('2d');
	window.personages = [];
	for ( var idx = 0; idx < angels.length; idx++) {
		var obj = new Sprite('/img/pers/'
				+ ((angels[idx].sex==1) ? 'w.' : 'm.') + angels[idx].type
				+ '.png');
		personages.push(obj);
	}
	for ( var idx = 0; idx < daemons.length; idx++) {
		var obj = new Sprite('/img/pers/'
				+ ((daemons[idx].sex==1) ? 'w.' : 'm.') + daemons[idx].type
				+ '.png');
		personages.push(obj);
	}
	// window.onkeyup = onKeyUp;
	//window.onkeypress = onKeyPress;
	//window.onresize = onResize;
	window.onmousemove = mouseMove;
	window.onmouseup = mouseUp;
	window.onmousedown = mouseDown;
	resp.push({
		"cmd" : "init"
	});
	jsonRPC({
		"method" : "ad.serve",
		"params" : resp,
		"id" : "id value (optional)"
	}, onData, onError);
	onResize();
} // main
