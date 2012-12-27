function method(data) {
	var text = 'Select Race';
	var measured = false;
	var width = 0;
	var height = 0;
	var arc = 10;
	var TriggerGradient = function(gradient) {
		gradient.addColorStop(0.00, 'rgba(102,152, 102, 255)');
		gradient.addColorStop(0.49, 'rgba(58,  98,  58, 255)');
		gradient.addColorStop(0.50, 'rgba(34,  74,  34, 255)');
		gradient.addColorStop(1.00, 'rgba(0,    20,   0, 255)');
	};

	var InstructionGradient = function(gradient) {
		gradient.addColorStop(0.00, 'rgba(152,102, 102, 255)');
		gradient.addColorStop(0.49, 'rgba(98,  58,  58, 255)');
		gradient.addColorStop(0.50, 'rgba(74,  34,  34, 255)');
		gradient.addColorStop(1.00, 'rgba(20,    0,   0, 255)');
	};

	if (context) {
		context.save();
		// clear the drawing surface
		context.clearRect(0, 0, canvas.width, canvas.height);
		var sw = 90;
		var sx = angels.length * sw;
		var rx = -50;
		for ( var idx = 0; idx < angels.length * 2; idx++) {
			if (idx == angels.length)
				rx = 50;
			var obj = personages[idx];
			obj.draw(context, canvas.width / 2 - sx + idx * sw + rx,
					canvas.height / 2 - 200, obj.scaleFactor * 0.3);
		}

		context.textAlign = 'center';
		context.textBaseline = 'middle';
		context.font = '800 18px Unknown Font, sans-serif';
		width = context.measureText(text).width + 20;
		height = 24 + 35;

		context.fillStyle = 'white';
		context.fillText(text, canvas.width / 2, canvas.height / 7);

		var dx = canvas.width / 2;
		var dy = canvas.height / 7 * 6;

		var ww = width;
		var hh = height - 12;
		var gradient = context.createLinearGradient(dx, dy, dx, dy + hh);

		if (window.selected_race) {
			context.fillText(window.selected_race, canvas.width / 2,
					canvas.height / 7 * 5);
			TriggerGradient(gradient);
		} else {
			InstructionGradient(gradient);
		}

		ww = context.measureText('Continue').width + 20;
		dx = canvas.width / 2 - ww / 2;
		window.selectRaceBtn = {
			'x' : dx,
			'y' : dy,
			'width' : ww,
			'height' : hh
		};
		context.strokeStyle = 'rgba(255,255, 255, 0.5)';
		context.lineWidth = 4;

		context.fillStyle = gradient;

		context.beginPath();
		context.moveTo(dx, dy + arc);
		context.lineTo(dx, dy + hh - arc);
		context.bezierCurveTo(dx, dy + hh - arc / 2, dx + arc / 2, dy + hh, dx
				+ arc, dy + hh);
		context.lineTo(dx + ww - arc, dy + hh);
		context.bezierCurveTo(dx + ww - arc / 2, dy + hh, dx + ww, dy + hh
				- arc / 2, dx + ww, dy + hh - arc);
		context.lineTo(dx + ww, dy + arc);
		context.bezierCurveTo(dx + ww, dy + arc / 2, dx + ww - arc / 2, dy, dx
				+ ww - arc, dy);
		context.lineTo(dx + arc, dy);
		context.bezierCurveTo(dx + arc / 2, dy, dx, dy + arc / 2, dx, dy + arc);
		context.stroke();
		context.fill();

		context.fillStyle = 'white';
		context.fillText('Continue', dx + ww / 2, dy + hh / 2);

		context.setTransform(1, 0, 0, 1, 0, 1);
		context.restore();
	}
};