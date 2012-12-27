var canvas = null;

Sprite = function(filename) {
	var self = this;
	this.img = new Image();
	
	this.img.onload = function() {
		self.rescale();
		self.loaded = true;
	};
	
	this.draw = function(ctx, sx, sy, sf) {
		if (this.loaded) {
			if (sf == undefined)
				sf = this.scaleFactor;
			ctx.drawImage(this.img, sx, sy, this.img.width * sf, this.img.height * sf);
			return true;
		} else {
			return false;
		}
	};
	
	this.scaleFactor = 1;
	this.img.src = filename;
	this.loaded = false;
	
	this.rescale = function() {
		if (canvas)
		this.scaleFactor = (canvas.height < canvas.width) ? canvas.height
				/ this.img.height
				: canvas.width / this.img.width;
	};
};
