function clearCanvas(data) {
	context.save();
	// clear the drawing surface
	context.clearRect(0, 0, canvas.width, canvas.height);
	context.setTransform(1, 0, 0, 1, 0, 1);
	context.restore();
};