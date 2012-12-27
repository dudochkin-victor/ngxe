function UI() {
	var components = {};

	this.getByName = function() {
	};

	this.create = function(uimodel) {
		
		function createWindow() {
		}
		;
		function createButton() {
		}
		;
		function createTextField() {
		}
		;

		for ( var prop in uimodel) {			
			model = uimodel[prop];		
			var obj = document.createElement(model.element);
			obj.setAttribute('id',prop);
			for (var styleName in model.sytle)
				obj.style[styleName] = model.style[styleName]; 
			//obj.style = model.style;
//			/*obj.style.height = model.height + "px";
//			obj.style.left = model.x+"px";
//			obj.style.top = model.x+"px";
//			obj.style.position = "absolute";
//			//obj.style.background = model.background;
//			obj.style.border = "4px solid #000";
			obj.className = model.className;
			obj.innerHTML = prop;
			
			document.body.appendChild(obj);
		}
	};
};

var ui = new UI();
ui.create({
	'blue window' : {
		'element' : 'div',
		'style' : {
			'width' : '100px',
			'height' : '100px',
			'top' : '100px',
			'left' : '100px',
			'position' : 'absolute',
			'background': '#55C',
			'border' : '4px solid #000'
		},
		'className' : 'st_subcontent',
		'childs' : {
			'noBtn' : {
				'element' : 'button',
				'style' : {
					'width' : 20,
					'heigth' : 30
				}
			},
			'yesBtn' : {
				'element' : 'button',
				'style' : {
					'width' : 20,
					'height' : 30
				},
				'onclick' : 'send'
			}
		}
	},
	'red window' : {
		'element' : 'div',
		'style' : {
			'width' : '100px',
			'height' : '100px',
			'top' : '100px',
			'left' : '100px',
			'position' : 'absolute',
			'background': '#55C',
			'border' : '4px solid #000',
		},
		'className' : 'st_subcontent',
		'childs' : {
			'noBtn' : {
				'element' : 'button',
				'style' : {
					'width' : 20,
					'heigth' : 30
				}
			},
			'yesBtn' : {
				'element' : 'button',
				'style' : {
					'width' : 20,
					'height' : 30
				},
				'onclick' : 'send'
			}
		}
	}
	
});

function changeIt() {
	if (!document.styleSheets[0]) return;
	var theRules = new Array();
	if (document.styleSheets[0].cssRules)
		theRules = document.styleSheets[0].cssRules
	else if (document.styleSheets[1].rules)
		theRules = document.styleSheets[0].rules
	else return;
	theRules[15].style.backgroundColor = '#EE44F5';
}

function main(){
	changeIt();
	console.log(document.styleSheets.length);

	var theRules = new Array();
	if (document.styleSheets[0]) {
		if (document.styleSheets[0].cssRules)
			theRules = document.styleSheets[0].cssRules
		else if (document.styleSheets[0].rules)
			theRules = document.styleSheets[0].rules

		console.log(theRules);
		//theRules.style.color = '#0000cc';
		//theRules.style.setProperty('color','#00cc00',null);
	}
}