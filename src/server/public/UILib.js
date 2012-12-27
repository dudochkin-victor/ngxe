var context = null;
var focused = null;
var buffer = null;
var buffer_ctx = null;

UIObject = function() {
	this.childs = [];
	this.x = 0;
	this.y = 0;
	this.width = 0;
	this.height = 0;
	this.visible = true;
	this.draw = function() {
		this.drawChilds();
	};
	this.drawChilds = function() {

	};
	this.addChild = function(obj) {
		this.childs.push(obj);
	};
};

UIManager = function() {
	this.objects = [];
	this.add = function(obj) {
		this.objects.push(obj);
	};
};

StyleGradient = function(gradient) {
	gradient.addColorStop(0.00, 'rgba(102,102, 102, 255)');
	gradient.addColorStop(0.49, 'rgba(58,  58,  58, 255)');
	gradient.addColorStop(0.50, 'rgba(34,  34,  34, 255)');
	gradient.addColorStop(1.00, 'rgba(0,    0,   0, 255)');
};

InverseGradient = function(gradient) {
	gradient.addColorStop(0.00, 'rgba(0,    0,   0, 255)');
	gradient.addColorStop(0.49, 'rgba(34,  34,  34, 255)');
	gradient.addColorStop(0.50, 'rgba(58,  58,  58, 255)');
	gradient.addColorStop(1.00, 'rgba(102,102, 102, 255)');
};

UIButton = function(text) {
	this.childs = [];
	this.x = 0;
	this.y = 0;
	this.text = text;
	this.width = 0;
	this.height = 0;
	this.arc = 10;
	this.visible = true;
	this.type = 'BUTTON';
	this.parent = null;
	this.buffer = null;
	
	this.onMouseDown = function() {
	};
	
	this.onMouseUp = function() {
		focused = null;
	};

	this.prepare = function(sx, sy) {
		if (this.visible) {
			var ctx = null;
			var dx = 2; 
			var dy = 2; 
			
			if (this.parent)
				this.buffer = this.parent.buffer;
			if (!this.buffer) {
				this.buffer = document.createElement('canvas');
				ctx = this.buffer.getContext('2d');
				ctx.textAlign = 'center';
				ctx.textBaseline = 'middle';
				ctx.font = '400 18px Unknown Font, sans-serif';
				this.width = ctx.measureText(this.text).width + 20;
				this.height = 18 + 20;
				this.buffer.width = this.width + 4;
				this.buffer.height = this.height + 4;
			} else {
				ctx = this.buffer.getContext('2d');
				ctx.textAlign = 'center';
				ctx.textBaseline = 'middle';
				ctx.font = '400 18px Unknown Font, sans-serif';
				this.width = ctx.measureText(this.text).width + 20;
				this.height = 18 + 20;
				if (this.parent)
				{
					dx = this.x;
					dy = this.y;
				} else
				{
					this.buffer.width = this.width + 4;
					this.buffer.height = this.height + 4;
				}
			}
			
			ctx.textAlign = 'center';
			ctx.textBaseline = 'middle';
			ctx.font = '400 18px Unknown Font, sans-serif';
			this.width = ctx.measureText(this.text).width + 20;
			
			ctx.strokeStyle = 'rgba(255,255, 255, 0.5)';
			ctx.lineWidth = 4;
			var gradient = ctx.createLinearGradient(dx, dy, dx, dy
					+ this.height);
			StyleGradient(gradient);
			ctx.fillStyle = gradient;

			ctx.beginPath();
			ctx.moveTo(dx, dy + this.arc);
			ctx.lineTo(dx, dy + this.height - this.arc);
			ctx.bezierCurveTo(dx, dy + this.height - this.arc / 2, dx
					+ this.arc / 2, dy + this.height, dx + this.arc, dy
					+ this.height);
			ctx.lineTo(dx + this.width - this.arc, dy + this.height);
			ctx.bezierCurveTo(dx + this.width - this.arc / 2, dy + this.height,
					dx + this.width, dy + this.height - this.arc / 2, dx
							+ this.width, dy + this.height - this.arc);
			ctx.lineTo(dx + this.width, dy + this.arc);
			ctx.bezierCurveTo(dx + this.width, dy + this.arc / 2, dx
					+ this.width - this.arc / 2, dy,
					dx + this.width - this.arc, dy);
			ctx.lineTo(dx + this.arc, dy);
			ctx.bezierCurveTo(dx + this.arc / 2, dy, dx, dy + this.arc / 2, dx,
					dy + this.arc);
			ctx.stroke();
			ctx.fill();
			ctx.fillStyle = 'rgba(255,255, 255, 1)';
			ctx.fillText(this.text, dx + this.width / 2, dy + this.height / 2);
		}
	};

	this.prepare();

	this.draw = function(sx, sy) {
		if (this.visible) {
			var dx = (sx) ? this.x + sx : this.x;
			var dy = (sy) ? this.y + sy : this.y;
			context.drawImage(this.buffer, dx, dy);
		}
	};
	this.drawChilds = function() {

	};

	this.checkArea = function(x, y) {
		var obj = null;
		if ((x > this.x) && (x < this.x + this.width) && (y > this.y)
				&& (y < this.y + this.height))
			obj = this;
		return obj;
	};

	this.addChild = function(obj) {
		this.childs.push(obj);
	};
};


UILabel = function(text) {
	this.childs = [];
	this.x = 0;
	this.y = 0;
	this.text = text;
	this.width = 0;
	this.height = 0;
	this.arc = 10;
	this.visible = true;
	this.type = 'LABEL';
	this.parent = null;
	this.buffer = null;
	
	this.onMouseDown = function() {
	};
	
	this.onMouseUp = function() {
		focused = null;
	};

	this.prepare = function(sx, sy) {
		if (this.visible) {
			var ctx = null;
			var dx = 2; 
			var dy = 2; 
			
			if (this.parent)
				this.buffer = this.parent.buffer;
			if (!this.buffer) {
				this.buffer = document.createElement('canvas');
				ctx = this.buffer.getContext('2d');
				ctx.textAlign = 'left';
				ctx.textBaseline = 'top';
				ctx.font = '400 18px Unknown Font, sans-serif';
				this.width = ctx.measureText(this.text).width + 4;
				this.height = 18 + 4;
				this.buffer.width = this.width + 4;
				this.buffer.height = this.height + 4;
			} else {
				ctx = this.buffer.getContext('2d');
				ctx.textAlign = 'left';
				ctx.textBaseline = 'top';
				ctx.font = '400 18px Unknown Font, sans-serif';
				this.width = ctx.measureText(this.text).width + 4;
				this.height = 18 + 4;
				dx = this.x;
				dy = this.y;
			}
			
			ctx.fillStyle = 'rgba(255,255, 255, 1)';
			ctx.fillText(this.text, dx, dy);
		}
	};

	this.prepare();

	this.draw = function(sx, sy) {
		if (this.visible) {
			var dx = (sx) ? this.x + sx : this.x;
			var dy = (sy) ? this.y + sy : this.y;
			context.drawImage(this.buffer, dx, dy);
		}
	};
	this.drawChilds = function() {

	};

	this.checkArea = function(x, y) {
		var obj = null;
		if ((x > this.x) && (x < this.x + this.width) && (y > this.y)
				&& (y < this.y + this.height))
			obj = this;
		return obj;
	};

	this.addChild = function(obj) {
		this.childs.push(obj);
	};
};

UIInput = function(text, width) {
	this.childs = [];
	this.x = 0;
	this.y = 0;
	this.placeholder = text;
	this.text = text;
	this.width = width;
	this.height = 0;
	this.pos = 0;
	this.arc = 4;
	this.visible = true;
	this.type = 'INPUT';
	this.parent = null;
	this.buffer = null;
	this.isPassword = false;
	this.chars = 0;
	this.onMouseDown = function() {
	};
	
	this.onMouseUp = function() {
		focused = this;
		if (this.text == this.placeholder)
		{
			this.text = '';
			this.prepare();
		}
	};

	this.prepare = function(sx, sy) {
		if (this.visible) {
			var ctx = null;
			var dx = 2; 
			var dy = 2; 
			
			if (this.parent)
				this.buffer = this.parent.buffer;
			if (!this.buffer) {
				this.buffer = document.createElement('canvas');
				ctx = this.buffer.getContext('2d');
				ctx.textAlign = 'left';
				ctx.textBaseline = 'top';
				ctx.font = '400 18px Unknown Font, sans-serif';
				if (!this.width)
					this.width = ctx.measureText(this.text).width + 8;
				this.height = 18 + 8;
				this.buffer.width = this.width + 8;
				this.buffer.height = this.height + 8;
			} else {
				ctx = this.buffer.getContext('2d');
				ctx.textAlign = 'left';
				ctx.textBaseline = 'top';
				ctx.font = '400 18px Unknown Font, sans-serif';
				if (!this.width)
					this.width = ctx.measureText(this.text).width + 8;
				this.height = 18 + 8;
				dx = this.x;
				dy = this.y;
			}
			
			ctx.fillStyle = 'rgba(255,255, 255, 1)';
			ctx.beginPath();
			ctx.moveTo(dx, dy + this.arc);
			ctx.lineTo(dx, dy + this.height - this.arc);
			ctx.bezierCurveTo(dx, dy + this.height - this.arc / 2, dx
					+ this.arc / 2, dy + this.height, dx + this.arc, dy
					+ this.height);
			ctx.lineTo(dx + this.width - this.arc, dy + this.height);
			ctx.bezierCurveTo(dx + this.width - this.arc / 2, dy + this.height,
					dx + this.width, dy + this.height - this.arc / 2, dx
							+ this.width, dy + this.height - this.arc);
			ctx.lineTo(dx + this.width, dy + this.arc);
			ctx.bezierCurveTo(dx + this.width, dy + this.arc / 2, dx
					+ this.width - this.arc / 2, dy,
					dx + this.width - this.arc, dy);
			ctx.lineTo(dx + this.arc, dy);
			ctx.bezierCurveTo(dx + this.arc / 2, dy, dx, dy + this.arc / 2, dx,
					dy + this.arc);
			ctx.stroke();
			ctx.fill();
			ctx.fillStyle = 'rgba(0,0, 0, 1)';
			this.chars = Math.floor(this.width/(ctx.measureText(this.text).width / this.text.length));
			if (this.isPassword)
			{
				var str = '';
			    while (str.length < this.text.length) {
			        str += '*';
			    }
			    
				ctx.fillText(str.substr(this.pos, this.chars), dx+2, dy+4);
			}
			else
				ctx.fillText(this.text.substr(this.pos, this.chars), dx+2, dy+4);
		}
	};

	this.prepare();

	this.draw = function(sx, sy) {
		if (this.visible) {
			var dx = (sx) ? this.x + sx : this.x;
			var dy = (sy) ? this.y + sy : this.y;
			context.drawImage(this.buffer, dx, dy);
		}
	};
	this.drawChilds = function() {

	};

	this.checkArea = function(x, y) {
		var obj = null;
		if ((x > this.x) && (x < this.x + this.width) && (y > this.y)
				&& (y < this.y + this.height))
			obj = this;
		return obj;
	};

	this.addChild = function(obj) {
		this.childs.push(obj);
	};
};

UIWindow = function(text, width, height) {
	this.childs = [];
	this.x = 0;
	this.y = 0;
	this.text = text;
	this.width = width;
	this.height = height;
	this.parent = null;
	this.buffer = null;
	this.arc = 10;
	this.visible = true;
	this.type = 'WINDOW';

	this.onMouseDown = function() {
	};
	
	this.onMouseUp = function() {
		focused = null;
	};
	
	this.prepare = function(sx, sy) {
		if (this.visible) {
			if (!this.buffer) {
				this.buffer = document.createElement('canvas');
				this.buffer.width = this.width + 4;
				this.buffer.height = this.height + 4;
			}

			var ctx = this.buffer.getContext('2d');
			var dx = 2; 
			var dy = 2; 

			ctx.textAlign = 'center';
			ctx.textBaseline = 'middle';
			ctx.font = '400 18px Unknown Font, sans-serif';
			ctx.strokeStyle = 'rgba(255,255, 255, 0.5)';
			ctx.lineWidth = 4;
			var gradient = ctx.createLinearGradient(dx, dy, dx, dy + 18 + 10);
			StyleGradient(gradient);
			ctx.fillStyle = gradient;

			ctx.beginPath();
			ctx.moveTo(dx, dy + this.arc);
			ctx.lineTo(dx, dy + this.height - this.arc);
			ctx.bezierCurveTo(dx, dy + this.height - this.arc / 2, dx
					+ this.arc / 2, dy + this.height, dx + this.arc, dy
					+ this.height);
			ctx.lineTo(dx + this.width - this.arc, dy + this.height);
			ctx.bezierCurveTo(dx + this.width - this.arc / 2, dy + this.height,
					dx + this.width, dy + this.height - this.arc / 2, dx
							+ this.width, dy + this.height - this.arc);
			ctx.lineTo(dx + this.width, dy + this.arc);
			ctx.bezierCurveTo(dx + this.width, dy + this.arc / 2, dx
					+ this.width - this.arc / 2, dy,
					dx + this.width - this.arc, dy);
			ctx.lineTo(dx + this.arc, dy);
			ctx.bezierCurveTo(dx + this.arc / 2, dy, dx, dy + this.arc / 2, dx,
					dy + this.arc);
			ctx.stroke();
			ctx.fill();
			ctx.fillStyle = 'rgba(255,255, 255, 1)';
			ctx.fillText(this.text, dx + this.width / 2, dy + (18 + 10) / 2,
					this.width);
			this.prepareChilds();
		}
	};

	this.draw = function(sx, sy) {
		if (this.visible) {
			if (!this.buffer)
				this.prepare();
			var dx = (sx) ? this.x + sx : this.x;
			var dy = (sy) ? this.y + sy : this.y;
			context.drawImage(this.buffer, dx, dy);
			//this.drawChilds();
		}
	};
	
	this.prepareChilds = function() {
		for ( var idx = 0; idx < this.childs.length; idx++)
			this.childs[idx].prepare(this.x, this.y);
	};

	this.checkArea = function(x, y) {
		var obj = null;
		if ((x > this.x) && (x < this.x + this.width) && (y > this.y)
				&& (y < this.y + this.height)) {
			obj = this;
			// gothca
			for ( var idx = 0; idx < this.childs.length; idx++) {
				var chobj = this.childs[idx].checkArea(x - this.x, y - this.y);
				if (chobj)
					return chobj;
			}
		}
		return obj;
	};

	this.addChild = function(obj) {
		this.childs.push(obj);
		obj.parent = this;
	};
};