var editor = null;
function redraw() {
	editor.updateMap();
	editor.drawMap();
}

function saveMap(){
	jsonRPC({
		"method" : "admin.updateLocation",
		"params" : [ $('#editMap').val(), $('#editMap').text().trim(), JSON.stringify(editor.data) ],
		"id" : "id value (optional)"
	}, function (data) {
		console.log(data);
	}, onError);
}

function resizeMap(WW, HH) {
	if (WW > editor.data.map.length) {
		// extract map width
		for ( var dx = editor.data.map.length - 1; dx < WW; dx++) {
			var newrow = [];
			editor.data.map.push(newrow);
			for ( var dy = 0; dy < editor.data.map[0].length; dy++) {
				newrow.push([ null, null, null ]);
			}
		}
	} else if (HH > editor.data.map[0].length) {
		var hh = HH - editor.data.map[0].length;
		for ( var dx = 0; dx < editor.data.map.length; dx++) {
			var row = editor.data.map[dx];
			for ( var dy = 0; dy < hh; dy++) {
				row.push([ null, null, null ]);
			}
		}
	} else if (WW < editor.data.map.length) {
		// contract map width
		var count = editor.data.map.length - WW;
		editor.data.map.splice(WW, count);
	} else if (HH < editor.data.map[0].length) {
		var count = editor.data.map[0].length - HH;
		for ( var dx = 0; dx < editor.data.map.length; dx++) {
			var row = editor.data.map[dx];
			row.splice(HH, count);
		}
	} 
	var mw = editor.data.map.length;
	var mh = editor.data.map[0].length;
	editor.mapCanvas.height = mh * 32 + 1;
	editor.mapCanvas.width = mw * 32 + 1;
	editor.mapCache.height = mh * 32 + 1;
	editor.mapCache.width = mw * 32 + 1;
	redraw();
}
function editor_main() {
	editor = {};
	editor.tileset = new Image();
	editor.auto = new Image();
	editor.tileset.src = '/Graphics/Tilesets/027-Castle03.png';
	editor.auto.src = '/Graphics/Autotiles/053-Wall01.png';
	editor.tileCanvas = document.getElementById('tilesetCanvas');
	editor.autoCanvas = document.getElementById('autoCanvas');
	editor.mapCanvas = document.getElementById('mapCanvas');
	editor.autoCache = document.createElement('canvas');
	editor.mapCache = document.createElement('canvas');
	editor.state = 0;
	editor.map = [];

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
		var tw = parseInt(editor.tileset.width / 32, 10);
		var aw = 8;
		var layers = [ $('#layer0').is(':checked'),
				$('#layer1').is(':checked'), $('#layer2').is(':checked') ];
		for ( var dx = 0; dx < editor.data.map.length; dx++) {
			var row = editor.data.map[dx];
			for ( var dy = 0; dy < row.length; dy++) {
				var layer = row[dy];
				for ( var l = 0; l < layer.length; l++) {
					if (!layers[l])
						continue;
					var id = layer[l];
					if (id != null) {
						if (id >= 384) {
							// TileSet
							id = id - 384;
							var sy = parseInt(id / tw) * 32;
							var sx = parseInt(id % tw) * 32;
							ctx.drawImage(editor.tileset, sx, sy, 32, 32,
									dx * 32, dy * 32, 32, 32);
						} else {
							// AutoSet
							id = id - 48;
							var sy = parseInt(id / aw) * 32;
							var sx = parseInt(id % aw) * 32;
							ctx.drawImage(editor.autoCache, sx, sy, 32, 32,
									dx * 32, dy * 32, 32, 32);
						}
					}
				}
			}
		}
		ctx.restore();
	};
	jsonRPC({
		"method" : "admin.maps",
		"params" : null,
		"id" : "id value (optional)"
	}, onMaps, onError);

	function onMaps(data) {
		if (data.result.data) {
			data = data.result.data;
			for ( var i = 0; i < data.length; i++)
				$('#editMap').append(
						'<option value="' + data[i].id + '">' + data[i].title
								+ '</option>');
		}
	}

	jsonRPC({
		"method" : "admin.textures",
		"params" : null,
		"id" : "id value (optional)"
	}, onTexture, onError);

	function onTexture(data) {
		if (data.result.data) {
			data = data.result.data;
			for ( var i = 0; i < data.length; i++) {
				if (data[i].type == 'tileset')
					$('#tileTexture').append(
							'<option value="' + data[i].id + '">'
									+ data[i].title + '</option>');
				else
					$('#autoTexture').append(
							'<option value="' + data[i].id + '">'
									+ data[i].title + '</option>');
			}
		}
	}
	editor.ready = function() {
		// editor.state++;
		// if (editor.state == 2) {
		$
				.getJSON(
						'Data/Maps/Base.json',
						function(data) {
							editor.data = data;
							editor.state = 0;
							var mw = editor.data.map.length;
							var mh = editor.data.map[0].length;
							$('#mapWidth').val(mw);
							$('#mapHeight').val(mh);
							editor.mapCanvas.height = mh * 32 + 1;
							editor.mapCanvas.width = mw * 32 + 1;

							editor.mapCache.height = mh * 32 + 1;
							editor.mapCache.width = mw * 32 + 1;

							var aw = 8;
							var iter = 0;

							function bitmapAutoTiles(bmp, position, animated) {
								var i = 0;
								var mi_tile = 16;
								// var nb_seq = animated / mi_tile;
								var sx = 0, sy = 0;
								var dy = parseInt(iter / aw), dx = iter % aw;
								var ctx = editor.autoCache.getContext('2d');
								ctx.save();

								for (i = 0; i < 4; i++) {
									// bmp.currentFrame = nb_seq *
									// position[i][1] + position[i][0];
									//						
									// if (animated / mi_tile > 6) {
									// bmp.waitFrame = 5;
									// bmp.arrayFrames = [];
									// for (k=0 ; k < nb_seq / 6 ; k++) {
									// bmp.arrayFrames.push(bmp.currentFrame
									// + (k*6));
									// }
									// }
									//						
									switch (i) {
									case 1:
										sx = mi_tile;
										break;
									case 2:
										sy = mi_tile;
										break;
									case 3:
										sx = 0;
										break;
									}
									var spx = (sx + position[i][0] * 16 - sx);
									var spy = (sy + position[i][1] * 16 - sy);
									ctx.drawImage(editor.auto, spx, spy, 16,
											16, dx * 32 + sx, dy * 32 + sy, 16,
											16);
								}
								ctx.restore();
								iter++;
							}

							function dataAutotile(x, y) {
								var i = 0;
								x = (x - 1) * 2;
								y = (y - 1) * 2;
								var tab = [];
								for (i = 0; i < 4; i++) {
									switch (i) {
									case 1:
										x++;
										break;
									case 2:
										y++;
										break;
									case 3:
										x--;
										break;
									}
									tab.push([ x, y ]);
								}

								return tab;
							}

							function isArray(a) {
								return (typeof (a) === 'object') ? a.constructor
										.toString().match(/array/i) !== null
										|| a.length !== undefined
										: false;
							}

							function valueExist(a, value) {
								var array_find, i, j;
								for (i = 0; i < a.length; i++) {
									if (isArray(value)) {
										array_find = true;
										for (j = 0; j < a[i].length; j++) {
											if (a[i][j] != value[j]) {
												array_find = false;
											}
										}
										if (array_find) {
											return i;
										}
									} else {
										if (a[i] == value) {
											return i;
										}
									}
								}
								return false;
							}

							function clone(srcInstance) {
								if (typeof (srcInstance) != 'object'
										|| srcInstance == null) {
									return srcInstance;
								}
								var newInstance = srcInstance.constructor();
								for ( var i in srcInstance) {
									newInstance[i] = clone(srcInstance[i]);
								}
								return newInstance;
							}

							function constructAutoTiles(seq, bmp, autotile,
									animated) {
								var i, j, k;
								switch (seq) {
								case 0:
									bitmapAutoTiles(bmp, autotile.center,
											animated);
									break;
								case 1:
									var array_corner = [];
									var corner_close = [];
									var split;
									for (i = 1; i <= 4; i++) {
										for (j = 0; j <= array_corner.length; j++) {
											corner_close
													.push((j != 0 ? array_corner[j - 1]
															: '')
															+ i + ";");
										}
										for (j = 0; j < corner_close.length; j++) {
											array_corner.push(corner_close[j]);
											split = corner_close[j].split(';');
											split.pop();
											var tile_corner = [];
											for (k = 1; k <= 4; k++) {
												if (valueExist(split, k) !== false) {
													tile_corner
															.push(autotile.corner[k - 1]);
												} else {
													tile_corner
															.push(autotile.center[k - 1]);
												}
											}

											bitmapAutoTiles(bmp, tile_corner,
													animated);
										}
										corner_close = [];
									}

									break;
								case 2:
									var dir = [ autotile.left, autotile.top,
											autotile.right, autotile.bottom ];
									var new_tile;
									var corner_id = [ 2, 3 ];
									var pos;
									for (i = 0; i < 4; i++) {
										for (j = 0; j < 4; j++) {

											new_tile = clone(dir[i]);

											if (j == 1 || j == 3) {
												pos = corner_id[0] - 1;
												new_tile[pos] = autotile.corner[pos];
											}

											if (j == 2 || j == 3) {

												pos = corner_id[1] - 1;
												new_tile[pos] = autotile.corner[pos];
											}

											bitmapAutoTiles(bmp, new_tile,
													animated);
										}

										corner_id[0]++;
										corner_id[1]++;

										if (corner_id[0] > 4)
											corner_id[0] = 1;
										if (corner_id[1] > 4)
											corner_id[1] = 1;
									}

									break;
								case 3:
									bitmapAutoTiles(bmp,
											[ autotile.left[0],
													autotile.right[1],
													autotile.right[2],
													autotile.left[3] ],
											animated);
									bitmapAutoTiles(bmp, [ autotile.top[0],
											autotile.top[1],
											autotile.bottom[2],
											autotile.bottom[3] ], animated);
									break;
								case 4:
									var dir = [ autotile.top_left,
											autotile.top_right,
											autotile.bottom_right,
											autotile.bottom_left ];
									var new_tile;
									var pos = 3;
									for (i = 0; i < dir.length; i++) {
										for (j = 0; j < 2; j++) {
											new_tile = clone(dir[i]);
											if (j == 1) {
												new_tile[pos - 1] = autotile.corner[pos - 1];
											}
											bitmapAutoTiles(bmp, new_tile,
													animated);
										}
										pos++;
										if (pos > 4)
											pos = 1;
									}
									break;
								case 5:
									var dir = [
											[ autotile.top_left[0],
													autotile.top_right[1],
													autotile.right[2],
													autotile.left[3] ],
											[ autotile.top_left[0],
													autotile.top[1],
													autotile.bottom[2],
													autotile.bottom_left[3] ],
											[ autotile.left[0],
													autotile.right[1],
													autotile.bottom_right[2],
													autotile.bottom_left[3] ],
											[ autotile.top[0],
													autotile.top_right[1],
													autotile.bottom_right[2],
													autotile.bottom[3] ] ];

									for (i = 0; i < dir.length; i++) {
										bitmapAutoTiles(bmp, dir[i], animated);
									}

									break;
								case 6:
									bitmapAutoTiles(bmp, autotile.full,
											animated);
									bitmapAutoTiles(bmp, autotile.full,
											animated);
									break;
								}
							}

							var autotile = {
								center : dataAutotile(2, 3),
								full : dataAutotile(1, 1),
								corner : dataAutotile(3, 1),
								left : dataAutotile(1, 3),
								right : dataAutotile(3, 3),
								top : dataAutotile(2, 2),
								bottom : dataAutotile(2, 4),
								top_left : dataAutotile(1, 2),
								top_right : dataAutotile(3, 2),
								bottom_left : dataAutotile(1, 4),
								bottom_right : dataAutotile(3, 4)
							};

							for ( var j = 0; j < 7; j++)
								constructAutoTiles(j, editor.auto, autotile,
										editor.auto.width);

							var ctx = editor.autoCanvas.getContext('2d');
							ctx.save();
							ctx.clearRect(0, 0, editor.autoCanvas.width,
									editor.autoCanvas.height);
							ctx.drawImage(editor.autoCache, 0, 0,
									editor.autoCache.width,
									editor.autoCache.height);
							ctx.restore();

							editor.updateMap();
							editor.drawMap();
						});
		// }
	};

	editor.tileset.onload = function() {
		editor.tileCanvas.height = editor.tileset.height + 1;
		editor.tileCanvas.width = editor.tileset.width + 1;
		var ctx = editor.tileCanvas.getContext('2d');
		ctx.save();
		ctx.clearRect(0, 0, editor.tileCanvas.width, editor.tileCanvas.height);
		ctx.drawImage(editor.tileset, 0, 0, editor.tileset.width,
				editor.tileset.height);
		ctx.restore();
		editor.ready();
	};

	editor.auto.onload = function() {
		editor.autoCanvas.width = editor.autoCache.width = 8 * 32 + 1;
		editor.autoCanvas.height = editor.autoCache.height = 6 * 32 + 1;
		editor.ready();
	};

	editor.startPoint = null;
	editor.endPoint = null;
	editor.mode = 'tileset';

	editor.tileCanvas.onmousedown = function(e) {
		editor.startPoint = {
			'x' : Math.floor(e.layerX / 32),
			'y' : Math.floor(e.layerY / 32)
		};
		editor.tileCanvas.onmousemove = editor.tileOver;
	};

	editor.tileCanvas.onmouseup = function(e) {
		// editor.startPoint = null;
		editor.endPoint = {
			'x' : Math.round(e.layerX / 32),
			'y' : Math.round(e.layerY / 32)
		};
		editor.tileCanvas.onmousemove = null;
	};

	editor.tileOver = function(e) {
		if (editor.startPoint) {
			var ctx = editor.tileCanvas.getContext('2d');
			ctx.save();
			ctx.clearRect(0, 0, editor.tileCanvas.width,
					editor.tileCanvas.height);
			ctx.drawImage(editor.tileset, 0, 0, editor.tileset.width,
					editor.tileset.height);
			ctx.strokeStyle = "yellow";
			ctx.strokeRect(editor.startPoint.x * 32, editor.startPoint.y * 32,
					(Math.round(e.layerX / 32) - editor.startPoint.x) * 32,
					(Math.round(e.layerY / 32) - editor.startPoint.y) * 32);
			ctx.restore();
		} else {

		}
	};

	editor.tileCanvas.onmouseover = function(e) {
		// editor.tileCanvas.onmousemove = editor.tileOver;
	};

	// ////////////////////
	editor.autoCanvas.onmousedown = function(e) {
		editor.startPoint = {
			'x' : Math.floor(e.layerX / 32),
			'y' : Math.floor(e.layerY / 32)
		};
		editor.autoCanvas.onmousemove = editor.autoOver;
	};

	editor.autoCanvas.onmouseup = function(e) {
		// editor.startPoint = null;
		editor.endPoint = {
			'x' : Math.round(e.layerX / 32),
			'y' : Math.round(e.layerY / 32)
		};
		editor.autoCanvas.onmousemove = null;
	};

	editor.autoOver = function(e) {
		if (editor.startPoint) {
			var ctx = editor.autoCanvas.getContext('2d');
			ctx.save();
			ctx.clearRect(0, 0, editor.autoCanvas.width,
					editor.autoCanvas.height);
			ctx.drawImage(editor.autoCache, 0, 0, editor.autoCache.width,
					editor.autoCache.height);
			ctx.strokeStyle = "yellow";
			ctx.strokeRect(editor.startPoint.x * 32, editor.startPoint.y * 32,
					(Math.round(e.layerX / 32) - editor.startPoint.x) * 32,
					(Math.round(e.layerY / 32) - editor.startPoint.y) * 32);
			ctx.restore();
		} else {

		}
	};

	editor.autoCanvas.onmouseover = function(e) {
		// editor.autoCanvas.onmousemove = editor.tileOver;
	};

	editor.autoCanvas.onmouseout = function(e) {
		editor.autoCanvas.onmousemove = null;
	};
	// ////////////////////
	editor.mapCanvas.onmousedown = function(e) {

		var sx = (editor.endPoint.x > editor.startPoint.x) ? parseInt(editor.startPoint.x)
				: parseInt(editor.endPoint.x);
		var sy = (editor.endPoint.y > editor.startPoint.y) ? parseInt(editor.startPoint.y)
				: parseInt(editor.endPoint.y);
		var sw = Math
				.abs(((editor.endPoint.x > editor.startPoint.x) ? editor.endPoint.x
						: editor.startPoint.x)
						- sx);
		var sh = Math
				.abs(((editor.endPoint.y > editor.startPoint.y) ? editor.endPoint.y
						: editor.startPoint.y)
						- sy);

		var dx = parseInt(e.layerX / 32);
		var dy = Math.floor(e.layerY / 32);
		dy = ((dy > 0) ? dy - 1 : 0);
		var tw = parseInt(editor.tileset.width / 32, 10);
		var aw = 8;
		for ( var iw = 0; iw < sw; iw++) {
			for ( var ih = 0; ih < sh; ih++) {
				var row = editor.data.map[dx + iw];
				if (row && row[dy + ih]) {
					if (editor.mode == 'auto') {
						row[dy + ih][$('#editLayer').val()] = (sy + ih) * aw
								+ sx + iw + 48;
					} else if (editor.mode == 'tileset') {
						row[dy + ih][$('#editLayer').val()] = (sy + ih) * tw
								+ sx + iw + 384;
					}
				}
			}
		}
		redraw();
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
			var sx = (editor.endPoint.x > editor.startPoint.x) ? parseInt(editor.startPoint.x * 32)
					: parseInt(editor.endPoint.x * 32);
			var sy = (editor.endPoint.y > editor.startPoint.y) ? parseInt(editor.startPoint.y * 32)
					: parseInt(editor.endPoint.y * 32);
			var sw = Math
					.abs(((editor.endPoint.x > editor.startPoint.x) ? editor.endPoint.x
							: editor.startPoint.x)
							* 32 - sx);
			var sh = Math
					.abs(((editor.endPoint.y > editor.startPoint.y) ? editor.endPoint.y
							: editor.startPoint.y)
							* 32 - sy);
			var dx = parseInt(e.layerX / 32);
			var dy = Math.floor(e.layerY / 32);
			dy = ((dy > 0) ? dy - 1 : 0);
			if (editor.mode == 'auto') {
				ctx.drawImage(editor.autoCache, sx, sy, sw, sh, dx * 32,
						dy * 32, sw, sh);
			} else if (editor.mode == 'tileset') {
				ctx.drawImage(editor.tileset, sx, sy, sw, sh, dx * 32, dy * 32,
						sw, sh);
			}
			ctx.strokeStyle = "yellow";
			ctx.strokeRect(dx * 32, dy * 32, sw, sh);
			ctx.restore();
		} else {

		}
	};
	// ////////////////////
	editor.switchPanel = function(tab) {
		$('#autoLabel').removeClass('success');
		$('#tilesetLabel').removeClass('success');
		$('#settingsLabel').removeClass('success');
		$('#outputLabel').removeClass('success');

		$('#autoPage').hide();
		$('#tilesetPage').hide();
		$('#settingsPage').hide();
		$('#outputPage').hide();
		$('#' + tab + 'Label').addClass('success');
		$('#' + tab + 'Page').show();
		if ((tab == 'auto') || (tab == 'tileset')) {
			editor.startPoint = editor.endPoint = null;
			editor.mode = tab;
		}

		return false;
	};
}