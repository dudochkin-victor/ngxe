function method(data) {
	window.selectRaceId = setInterval(methods['selectRace'], 80);

	function mouseDown(event) {
		event = event || window.event;

		var canvas = document.getElementById('canvas');
		var x = event.pageX - canvas.offsetLeft;
		var y = event.pageY - canvas.offsetTop;
		/*
		 * obj = wnd.checkArea(x, y); if (obj) { if (obj.type == 'WINDOW') {
		 * drag = true; offsetX = x - obj.x; offsetY = y - obj.y; } else
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
		var x = event.pageX - canvas.offsetLeft;
		var y = event.pageY - canvas.offsetTop;
		if ((y > canvas.height / 7 * 2) && (y < canvas.height / 7 * 4)) {
			if (x > canvas.width / 2)
				window.selected_race = 'daemons';
			else
				window.selected_race = 'angels';
		}
		if (window.selectRaceBtn) {
			var btn = window.selectRaceBtn;
			if ((x > btn.x) && (x < btn.x + btn.width) && (y > btn.y) && (y < btn.y + btn.height))
			{	
				resp.push({"cmd" : "selectRace", "race" : window.selected_race});
				jsonRPC({
					"method" : "ad.serve",
					"params" : resp,
					"id" : "id value (optional)"
				}, onData, onError);
				return false;
			}
			
				
		}
		/*
		 * obj = wnd.checkArea(x, y); if (obj) { if (obj.type != 'WINDOW') {
		 * obj.onMouseUp(obj); if (obj.type == 'INPUT') { context.save(); //
		 * clear the drawing surface context.clearRect(0, 0, canvas.width,
		 * canvas.height); context.setTransform(1, 0, 0, 1, 0, 1);
		 * context.restore(); } } }
		 */
	}
	window.onmousemove = mouseMove;
	window.onmouseup = mouseUp;
	window.onmousedown = mouseDown;
};