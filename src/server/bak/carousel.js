function carousel() {
	if (context)
	{
		if (!window.angle) window.angle = 0;
		window.angle +=0.05;
		context.save();
		// clear the drawing surface
		context.clearRect(0, 0, canvas.width, canvas.height);
		
		var delta = Math.PI/personages.length*2;
		for ( var idx = 0; idx < personages.length; idx++) {
			personages[idx].angle = idx+angle; 
			personages[idx].flag = Math.sin(delta*(idx+angle));
		}
		var newArray = personages.slice();
		function sortfunction(a, b){
		//Compare "a" and "b" in some fashion, and return -1, 0, or 1
			return a.flag - b.flag;
		}
		newArray.sort(sortfunction);
		for ( var idx = 0; idx < newArray.length; idx++) {
			var obj = newArray[idx];
			if (idx == newArray.length-1)
				setShadow(context, "#FFFF88", 0, 0, 15);
				obj.draw(context, Math.cos(delta*(obj.angle))*500+canvas.width/2-100, -Math.sin(delta*(obj.angle))*20+200, obj.scaleFactor*0.4+Math.sin(delta*(obj.angle))/10);
			if (idx == newArray.length -1)
				clearShadow(context);
		}
		context.setTransform(1, 0, 0, 1, 0, 1);
		context.restore();
	}
}