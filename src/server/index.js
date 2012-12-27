// -*- JavaScript -*-

//var PROJECT = 'current.project';
var http = require('http'), util = require('util');
var RPCHandler = require("jsonrpc").RPCHandler;
var mysql = require('mysql-libmysqlclient');
var fs = require('fs');
var url = require('url');
var crypto = require('crypto');
var querystring = require('querystring');
var jqtpl = require('jqtpl');
var sessions = require('./session');
var libxml = require('libxmljs');
var qengine = require('./quest');

var conn, result, row, rows;
var css = null;
var PATH = __dirname + '/';
var version = 'v0.0.1';
var port = 8084;
var superuser = 'blacksmith';
var dbconf = JSON.parse(fs.readFileSync('./database.config', 'utf-8'));
var permissions = JSON.parse(fs.readFileSync('./permissions.config', 'utf-8'));

var arguments = process.argv.splice(2);
var components = {};
var methods = {};
var applogic = null;

// Create connection
conn = mysql.createConnectionSync();

conn.connectSync(dbconf.host, dbconf.user, dbconf.password, dbconf.database);

if (!conn.connectedSync()) {
	util.puts("Connection error " + conn.connectErrno + ": "
			+ conn.connectError);
	process.exit(1);
}

process.on('exit', function() {
	if (conn.connectedSync()) {
		var sess = sessions.sessionRoot;
		for ( var idx in sess)
			sessHadler(sess[idx]);
	}
	if (conn != undefined)
		conn.closeSync();
});

process.on('SIGINT', function() {
	process.exit(1);
});
process.on('SIGTERM', function() {
	process.exit(1);
});

// Entry point
util.puts("PORT: " + port);

this.addComponent = function(component, name) {
	console.log('jsonrpc.addComponent: ' + name);
	methods[name] = component.jsonrpc;
};
that = this;

var users = {};
var contexts = {};
var artifacts = null;

var channels = {
	'0' : {
		'title' : 'Main Channel',
		'users' : 0,
		'userlist' : {},
		'messages' : []
	}
};

function getContext(id) {
	if (contexts[id])
		return contexts[id];
	else {
		var req = {
			headers : {}
		};

		req.session = sessions.lookupOrCreate(req, {
			lifetime : 604800,
			domain : '.'
		});
		var session = req.session.data;
		var checkSql = 'SELECT * FROM auth_identity WHERE id='
				+ parseInt(id, 10) + ' LIMIT 1';
		var res = conn.querySync(checkSql);
		if (res != false) {
			var rows = res.fetchAllSync();
			if (rows.length > 0) {
				session.identity = rows[0];
				if (!contexts[params.session.identity.id]) {
					var checkSql = 'SELECT * FROM personage WHERE user_id='
							+ session.identity.id + ' LIMIT 1';
					var res = conn.querySync(checkSql);
					if (res != false) {
						var rows = res.fetchAllSync();
						if (rows.length > 0) {
							session.context = JSON.parse(rows[0]['body']);
							session.context.personageId = rows[0]['id'];
						} else
							session.context = {};
					}
					contexts[session.identity.id] = session.context;
				} else
					session.context = contexts[session.identity.id];
				return contexts[id];
			} else
				return null;
		} else
			return null;
	}
}

var sessHadler = function(obj) {
	var session = obj.data;
	var query = null;
	if (session.context) {
		if (session.context.personageId)
			query = 'UPDATE personage SET body=\''
					+ conn.escapeSync(JSON.stringify(session.context))
					+ '\' WHERE id=' + session.context.personageId;
		else
			query = 'INSERT personage (user_id, body) VALUES ('
					+ session.identity.id + ', \''
					+ conn.escapeSync(JSON.stringify(session.context)) + '\')';

		var res = conn.querySync(query);
		if (res == false)
			util.puts('NOT STORED ' + query);
	} else
		util.puts('NO CONTEXT ' + util.inspect(session));
	if (session.identity)
		delete contexts[session.identity.id];
};

sessions.setHandler(sessHadler);

var defaultGet = function(table, rpc, page) {
	// var req = rpc.HTTPRequest;
	// var session = req.session.data;
	// Check user logged
	if ((page == undefined) || (page == null)) {
		page = 1;
		mpage = 0;
	} else
		mpage = page - 1;
	// if (session.identity)
	var checkSql = 'SELECT *, (SELECT count(*) from ' + table
			+ ') as count FROM ' + table + ' ORDER BY id ASC LIMIT ' + mpage
			* 25 + ', 25';
	var res = conn.querySync(checkSql);
	if (res != false) {
		var rows = res.fetchAllSync();
		if (rows.length > 0) {
			rpc.response({
				'code' : 0,
				'description' : 'Ok',
				'data' : rows,
				'current' : page
			});
		} else
			rpc.response({
				'code' : -1,
				'description' : 'No Data'
			});
	} else {
		util.puts('no res in ' + checkSql);
	}
};
var defaultUpdate = function(table, rpc, id, title, body) {
	if (body != null) {
		id = parseInt(id, 10);
		// if (session.identity)
		var checkSql = '';
		if (isNaN(id))
			checkSql = 'SELECT id FROM ' + table + ' WHERE title=\''
					+ conn.escapeSync(title) + '\' LIMIT 1';
		else
			checkSql = 'SELECT id FROM ' + table + ' WHERE id=\'' + id
					+ '\' LIMIT 1';
		var res = conn.querySync(checkSql);
		if (res != false) {
			var rows = res.fetchAllSync();
			if (rows.length > 0) {
				var updateSql = 'UPDATE ' + table + ' SET body=\''
						+ conn.escapeSync(body) + '\', title=\''
						+ conn.escapeSync(title) + '\' WHERE id='
						+ rows[0]['id'];
				conn.querySync(updateSql);
			} else {
				var insertSql = 'INSERT INTO ' + table
						+ ' (body, title) VALUES (\'' + conn.escapeSync(body)
						+ '\', \'' + conn.escapeSync(title) + '\')';
				conn.querySync(insertSql);
			}
		} else {
			util.puts('no res in ' + checkSql);
		}

		rpc.response({
			'code' : 0,
			'description' : 'Ok',
			'data' : title
		});
	} else
		rpc.response({
			'code' : -1,
			'description' : 'Body is NULL',
			'data' : title
		});
};

var defaultDelete = function(table, rpc, id) {
	// var req = rpc.HTTPRequest;
	// var session = req.session.data;
	// Check user logged
	id = parseInt(id, 10);
	// if (session.identity)
	if (isNaN(id)) {
		rpc.response({
			'code' : -2,
			'description' : 'Error'
		});
		return;
	}

	var insertSql = 'DELETE ' + table + ' WHERE id=' + id;
	var res = conn.querySync(insertSql);
	if (res != false) {
		rpc.response({
			'code' : 0,
			'description' : 'Ok'
		});
	} else
		rpc.response({
			'code' : -1,
			'description' : 'Error'
		});
};

/**
 * Users
 */
methods['admin.users'] = function(rpc, page) {
	defaultGet('auth_identity', rpc, page);
};

methods['admin.deleteUser'] = function(rpc, id) {
	defaultDelete('auth_identity', rpc, id);
};

/**
 * Users
 */
methods['admin.artifacts'] = function(rpc, page) {
	defaultGet('artifacts', rpc, page);
};

methods['admin.updateArtifact'] = function(rpc, id, title, body) {
	defaultUpdate('artifacts', rpc, id, title, body);
};

methods['admin.deleteArtifact'] = function(rpc, id) {
	defaultDelete('artifacts', rpc, id);
};

/**
 * Users
 */
methods['admin.bag'] = function(rpc, page) {
	defaultGet('bag', rpc, page);
};

methods['admin.updateBag'] = function(rpc, id, title, body) {
	defaultUpdate('bag', rpc, id, title, body);
};

methods['admin.deleteBag'] = function(rpc, id) {
	defaultDelete('bag', rpc, id);
};

/**
 * Users
 */
methods['admin.clans'] = function(rpc, page) {
	defaultGet('clans', rpc, page);
};

methods['admin.updateClan'] = function(rpc, id, title, body) {
	defaultUpdate('clans', rpc, id, title, body);
};

methods['admin.deleteClan'] = function(rpc, id) {
	defaultDelete('clans', rpc, id);
};

/**
 * Users
 */
methods['admin.clanUsers'] = function(rpc, page) {
	defaultGet('clan_users', rpc, page);
};

methods['admin.updateClanUser'] = function(rpc, id, title, body) {
	defaultUpdate('clan_users', rpc, id, title, body);
};

methods['admin.deleteClanUser'] = function(rpc, id) {
	defaultDelete('clan_users', rpc, id);
};
/**
 * Users
 */
methods['admin.mail'] = function(rpc, page) {
	defaultGet('mails', rpc, page);
};

methods['admin.updateMail'] = function(rpc, id, title, body) {
	defaultUpdate('mails', rpc, id, title, body);
};

methods['admin.deleteMail'] = function(rpc, id) {
	defaultDelete('mails', rpc, id);
};

/**
 * Users
 */
methods['admin.news'] = function(rpc, page) {
	defaultGet('news', rpc, page);
};

methods['admin.updateNewsItem'] = function(rpc, id, title, body) {
	defaultUpdate('news', rpc, id, title, body);
};

methods['admin.deleteNewsItem'] = function(rpc, id) {
	defaultDelete('news', rpc, id);
};

/**
 * Users
 */
methods['admin.locationTypes'] = function(rpc, page) {
	defaultGet('location_types', rpc, page);
};

methods['admin.updateLocationType'] = function(rpc, id, title, body) {
	defaultUpdate('location_types', rpc, id, title, body);
};

methods['admin.deleteLocationType'] = function(rpc, id) {
	defaultDelete('location_types', rpc, id);
};
/**
 * Users
 */
methods['admin.artifactTypes'] = function(rpc, page) {
	defaultGet('artifact_types', rpc, page);
};

methods['admin.updateArtifactType'] = function(rpc, id, title, body) {
	defaultUpdate('artifact_types', rpc, id, title, body);
};

methods['admin.deleteArtifactType'] = function(rpc, id) {
	defaultDelete('artifact_types', rpc, id);
};

/**
 * Users
 */
methods['admin.locations'] = function(rpc, page) {
	defaultGet('locations', rpc, page);
};

methods['admin.maps'] = function(rpc, page) {
	var checkSql = 'SELECT locations.* from locations, location_types WHERE type=location_types.id AND location_types.title=\'map\'';
	var res = conn.querySync(checkSql);
	if (res != false) {
		var rows = res.fetchAllSync();
		if (rows.length > 0) {
			rpc.response({
				'code' : 0,
				'description' : 'Ok',
				'data' : rows,
				'current' : page
			});
		} else
			rpc.response({
				'code' : -1,
				'description' : 'No Data'
			});
	} else
		rpc.response({
			'code' : -1,
			'description' : 'No Data'
		});
};

methods['admin.textures'] = function(rpc, page) {
	var checkSql = 'SELECT graphics.*, graphics_types.title as type from graphics, graphics_types WHERE'
			+ ' type=graphics_types.id AND (graphics_types.title=\'autoset\' OR graphics_types.title=\'tileset\')';
	var res = conn.querySync(checkSql);
	if (res != false) {
		var rows = res.fetchAllSync();
		if (rows.length > 0) {
			rpc.response({
				'code' : 0,
				'description' : 'Ok',
				'data' : rows,
				'current' : page
			});
		} else
			rpc.response({
				'code' : -1,
				'description' : 'No Data'
			});
	} else
		rpc.response({
			'code' : -1,
			'description' : 'No Data'
		});
};

methods['admin.updateLocation'] = function(rpc, id, title, body) {
	defaultUpdate('locations', rpc, id, title, body);
};

methods['admin.deleteLoaction'] = function(rpc, id) {
	defaultDelete('locations', rpc, id);
};

/**
 * Users
 */
methods['admin.personageTypes'] = function(rpc, page) {
	defaultGet('personage_types', rpc, page);
};

methods['admin.updatePersonageType'] = function(rpc, id, title, body) {
	defaultUpdate('personage_types', rpc, id, title, body);
};

methods['admin.deletePersonageType'] = function(rpc, id) {
	defaultDelete('personage_types', rpc, id);
};

/**
 * Users
 */
methods['admin.npc'] = function(rpc, page) {
	defaultGet('npc', rpc, page);
};

methods['admin.updateNpc'] = function(rpc, id, title, body) {
	defaultUpdate('npc', rpc, id, title, body);
};

methods['admin.deleteNpc'] = function(rpc, id) {
	defaultDelete('npc', rpc, id);
};

/**
 * Users
 */
methods['admin.guilds'] = function(rpc, page) {
	defaultGet('guilds', rpc, page);
};

methods['admin.updateGuild'] = function(rpc, id, title, body) {
	defaultUpdate('guilds', rpc, id, title, body);
};

methods['admin.deleteGuild'] = function(rpc, id) {
	defaultDelete('guilds', rpc, id);
};

/**
 * Users
 */
methods['admin.questMessages'] = function(rpc, page) {
	defaultGet('quest_messages', rpc, page);
};

methods['admin.updateQuestMessage'] = function(rpc, id, title, body) {
	defaultUpdate('quest_messages', rpc, id, title, body);
};

methods['admin.deleteQuestMessage'] = function(rpc, id) {
	defaultDelete('quest_messages', rpc, id);
};

/**
 * Users
 */
methods['admin.graphics'] = function(rpc, page) {
	defaultGet('graphics', rpc, page);
};

methods['admin.updateGraphics'] = function(rpc, id, title, body) {
	defaultUpdate('graphics', rpc, id, title, body);
};

methods['admin.deleteGraphics'] = function(rpc, id) {
	defaultDelete('graphics', rpc, id);
};
/**
 * Users
 */
methods['admin.sounds'] = function(rpc, page) {
	defaultGet('sounds', rpc, page);
};

methods['admin.updateSound'] = function(rpc, id, title, body) {
	defaultUpdate('sounds', rpc, id, title, body);
};

methods['admin.deleteSound'] = function(rpc, id) {
	defaultDelete('sounts', rpc, id);
};

/**
 * Users
 */
methods['admin.events'] = function(rpc, page) {
	defaultGet('events', rpc, page);
};

methods['admin.updateEvent'] = function(rpc, id, title, body) {
	defaultUpdate('events', rpc, id, title, body);
};

methods['admin.deleteEvent'] = function(rpc, id) {
	defaultDelete('events', rpc, id);
};

/**
 * Users
 */
methods['admin.personages'] = function(rpc, page) {
	defaultGet('personage', rpc, page);
};

methods['admin.updatePersonage'] = function(rpc, id, title, body) {
	defaultUpdate('personage', rpc, id, title, body);
};

methods['admin.deletePersonage'] = function(rpc, id) {
	defaultDelete('personage', rpc, id);
};

/**
 * Users
 */
methods['admin.clientMethods'] = function(rpc, page) {
	defaultGet('client_methods', rpc, page);
};

methods['admin.deleteClientMethod'] = function(rpc, id) {
	defaultDelete('client_methods', rpc, id);
};

/**
 * Users
 */
methods['admin.serverMethods'] = function(rpc, page) {
	defaultGet('server_methods', rpc, page);
};

methods['admin.deleteServerMethod'] = function(rpc, id) {
	defaultDelete('server_methods', rpc, id);
};

/**
 * Users
 */
methods['admin.clientForms'] = function(rpc, page) {
	defaultGet('client_forms', rpc, page);
};

methods['admin.deleteClientForm'] = function(rpc, id) {
	defaultDelete('client_forms', rpc, id);
};

/**
 * Users
 */
methods['admin.clientStyle'] = function(rpc, page) {
	defaultGet('client_style', rpc, page);
};

methods['admin.updateClientStyle'] = function(rpc, id, title, body) {
	defaultUpdate('client_style', rpc, id, title, body);
};

methods['admin.deleteClientStyle'] = function(rpc, id) {
	defaultDelete('client_style', rpc, id);
};

methods['admin.updateServerMethod'] = function(rpc, id, title, body) {
	if (body != null) {
		id = parseInt(id, 10);
		// if (session.identity)
		var checkSql = '';
		if (isNaN(id))
			checkSql = 'SELECT id FROM server_methods WHERE title=\''
					+ conn.escapeSync(title) + '\' LIMIT 1';
		else
			checkSql = 'SELECT id FROM server_methods WHERE id=\'' + id
					+ '\' LIMIT 1';
		var res = conn.querySync(checkSql);
		if (res != false) {
			var rows = res.fetchAllSync();
			if (rows.length > 0) {
				var updateSql = 'UPDATE server_methods SET body=\''
						+ conn.escapeSync(body) + '\', title=\''
						+ conn.escapeSync(title) + '\' WHERE id='
						+ rows[0]['id'];
				conn.querySync(updateSql);
			} else {
				var insertSql = 'INSERT INTO server_methods (body, title) VALUES (\''
						+ conn.escapeSync(body)
						+ '\', \''
						+ conn.escapeSync(title) + '\')';
				conn.querySync(insertSql);
			}

			var str = 'components[\'' + title + '\'] = ' + body;
			try {
				eval(str);
			} catch (e) {
				util.puts(e + 'in \n\'' + str + '\'');
			}

		} else {
			util.puts('no res in admin.clientForms');
		}

		rpc.response({
			'code' : 0,
			'description' : 'Ok',
			'data' : title
		});
	}
};

methods['admin.updateClientMethod'] = function(rpc, id, title, body) {
	if (body != null) {
		id = parseInt(id, 10);
		// if (session.identity)
		var checkSql = '';
		if (isNaN(id))
			checkSql = 'SELECT id FROM client_methods WHERE title=\''
					+ conn.escapeSync(title) + '\' LIMIT 1';
		else
			checkSql = 'SELECT id FROM client_methods WHERE id=\'' + id
					+ '\' LIMIT 1';
		var res = conn.querySync(checkSql);
		if (res != false) {
			var rows = res.fetchAllSync();
			if (rows.length > 0) {
				var updateSql = 'UPDATE client_methods SET body=\''
						+ conn.escapeSync(body) + '\', title=\''
						+ conn.escapeSync(title) + '\' WHERE id='
						+ rows[0]['id'];
				conn.querySync(updateSql);
			} else {
				var insertSql = 'INSERT INTO client_methods (body, title) VALUES (\''
						+ conn.escapeSync(body)
						+ '\', \''
						+ conn.escapeSync(title) + '\')';
				conn.querySync(insertSql);
			}
		} else {
			util.puts('no res in admin.clientForms');
		}

		rpc.response({
			'code' : 0,
			'description' : 'Ok',
			'data' : title
		});
	}
};

var styles = {
	"animation" : {
		"moz" : "-moz-animation",
		"webkit" : "-webkit-animation",
		"desc" : "",
		"dom" : "animation"
	},
	"animation-name" : {
		"moz" : "-moz-animation-name",
		"webkit" : "-webkit-animation-name",
		"desc" : "",
		"dom" : "animationName"
	},
	"animation-duration" : {
		"moz" : "-moz-animation-duration",
		"webkit" : "-webkit-animation-duration",
		"desc" : "",
		"dom" : "animationDuration"
	},
	"animation-timing-function" : {
		"moz" : "-moz-animation-timing-function",
		"webkit" : "-webkit-animation-timing-function",
		"desc" : "",
		"dom" : "animationTimingFunction"
	},
	"animation-delay" : {
		"moz" : "-moz-animation-delay",
		"webkit" : "-webkit-animation-delay",
		"desc" : "",
		"dom" : "animationDelay"
	},
	"animation-iteration-count" : {
		"moz" : "-moz-animation-iteration-count",
		"webkit" : "-webkit-animation-iteration-count",
		"desc" : "",
		"dom" : "animationIterationCount"
	},
	"animation-direction" : {
		"moz" : "-moz-animation-direction",
		"webkit" : "-webkit-animation-direction",
		"desc" : "",
		"dom" : "animationDirection"
	},
	"animation-play-state" : {
		"moz" : "-moz-animation-play-state",
		"webkit" : "-webkit-animation-play-state",
		"desc" : "",
		"dom" : "animationPlayState"
	},
	"appearance" : {
		"moz" : "-moz-appearance",
		"webkit" : "-webkit-appearance",
		"desc" : "",
		"dom" : "appearance"
	},
	"backface-visibility" : {
		"webkit" : "-webkit-backface-visibility",
		"desc" : "",
		"dom" : "backfaceVisibility"
	},
	"background" : {
		"desc" : "",
		"dom" : "background"
	},
	"background-attachment" : {
		"desc" : "",
		"dom" : "backgroundAttachment"
	},
	"background-color" : {
		"desc" : "",
		"dom" : "backgroundColor"
	},
	"background-image" : {
		"desc" : "",
		"dom" : "backgroundImage"
	},
	"background-position" : {
		"desc" : "",
		"dom" : "backgroundPosition"
	},
	"background-repeat" : {
		"desc" : "",
		"dom" : "backgroundRepeat"
	},
	"background-clip" : {
		"webkit" : "-webkit-background-clip",
		"desc" : "",
		"dom" : "backgroundClip"
	},
	"background-origin" : {
		"desc" : "",
		"dom" : "backgroundOrigin"
	},
	"background-size" : {
		"desc" : "",
		"dom" : "backgroundSize"
	},
	"border" : {
		"desc" : "",
		"dom" : "border"
	},
	"border-bottom" : {
		"desc" : "",
		"dom" : "borderBottom"
	},
	"border-bottom-color" : {
		"desc" : "",
		"dom" : "borderBottomColor"
	},
	"border-bottom-style" : {
		"desc" : "",
		"dom" : "borderBottomStyle"
	},
	"border-bottom-width" : {
		"desc" : "",
		"dom" : "borderBottomWidth"
	},
	"border-collapse" : {
		"desc" : "",
		"dom" : "borderCollapse"
	},
	"border-color" : {
		"desc" : "",
		"dom" : "borderColor"
	},
	"border-left" : {
		"desc" : "",
		"dom" : "borderLeft"
	},
	"border-left-color" : {
		"desc" : "",
		"dom" : "borderLeftColor"
	},
	"border-left-style" : {
		"desc" : "",
		"dom" : "borderLeftStyle"
	},
	"border-left-width" : {
		"desc" : "",
		"dom" : "borderLeftWidth"
	},
	"border-right" : {
		"desc" : "",
		"dom" : "borderRight"
	},
	"border-right-color" : {
		"desc" : "",
		"dom" : "borderRightColor"
	},
	"border-right-style" : {
		"desc" : "",
		"dom" : "borderRightStyle"
	},
	"border-right-width" : {
		"desc" : "",
		"dom" : "borderRightWidth"
	},
	"border-spacing" : {
		"desc" : "",
		"dom" : "borderSpacing"
	},
	"border-style" : {
		"desc" : "",
		"dom" : "borderStyle"
	},
	"border-top" : {
		"desc" : "",
		"dom" : "borderTop"
	},
	"border-top-color" : {
		"desc" : "",
		"dom" : "borderTopColor"
	},
	"border-top-style" : {
		"desc" : "",
		"dom" : "borderTopStyle"
	},
	"border-top-width" : {
		"desc" : "",
		"dom" : "borderTopWidth"
	},
	"border-width" : {
		"desc" : "",
		"dom" : "borderWidth"
	},
	"border-bottom-left-radius" : {
		"desc" : "",
		"dom" : "borderBottomLeftRadius"
	},
	"border-bottom-right-radius" : {
		"desc" : "",
		"dom" : "borderBottomRightRadius"
	},
	"border-image" : {
		"moz" : "-moz-border-image",
		"webkit" : "-webkit-border-image",
		"opera" : "-o-border-image",
		"desc" : "",
		"dom" : "borderImage"
	},
	"border-image-outset" : {
		"desc" : "",
		"dom" : "borderImageOutset"
	},
	"border-image-repeat" : {
		"desc" : "",
		"dom" : "borderImageRepeat"
	},
	"border-image-slice" : {
		"desc" : "",
		"dom" : "borderImageSlice"
	},
	"border-image-source" : {
		"desc" : "",
		"dom" : "borderImageSource"
	},
	"border-image-width" : {
		"desc" : "",
		"dom" : "borderImageWidth"
	},
	"border-radius" : {
		"desc" : "",
		"dom" : "borderRadius"
	},
	"border-top-left-radius" : {
		"desc" : "",
		"dom" : "borderTopLeftRadius"
	},
	"border-top-right-radius" : {
		"desc" : "",
		"dom" : "borderTopRightRadius"
	},
	"bottom" : {
		"desc" : "",
		"dom" : "bottom"
	},
	"box-align" : {
		"moz" : "-moz-box-align",
		"webkit" : "-webkit-box-align",
		"desc" : "",
		"dom" : "boxAlign"
	},
	"box-direction" : {
		"moz" : "-moz-box-direction",
		"webkit" : "-webkit-box-direction",
		"desc" : "",
		"dom" : "boxDirection"
	},
	"box-flex" : {
		"moz" : "-moz-box-flex",
		"webkit" : "-webkit-box-flex",
		"desc" : "",
		"dom" : "boxFlex"
	},
	"box-flex-group" : {
		"desc" : "",
		"dom" : "boxFlexGroup"
	},
	"box-lines" : {
		"desc" : "",
		"dom" : "boxLines"
	},
	"box-ordinal-group" : {
		"moz" : "-moz-box-ordinal-group",
		"webkit" : "-webkit-box-ordinal-group",
		"desc" : "",
		"dom" : "boxOrdinalGroup"
	},
	"box-orient" : {
		"moz" : "-moz-box-orient",
		"webkit" : "-webkit-box-orient",
		"desc" : "",
		"dom" : "boxOrient"
	},
	"box-pack" : {
		"moz" : "-moz-box-pack",
		"webkit" : "-webkit-box-pack",
		"desc" : "",
		"dom" : "boxPack"
	},
	"box-sizing" : {
		"moz" : "-moz-box-sizing",
		"webkit" : "-webkit-box-sizing",
		"desc" : "",
		"dom" : "boxSizing"
	},
	"box-shadow" : {
		"webkit" : "-webkit-box-shadow",
		"desc" : "",
		"dom" : "boxShadow"
	},
	"caption-side" : {
		"desc" : "",
		"dom" : "captionSide"
	},
	"clear" : {
		"desc" : "",
		"dom" : "clear"
	},
	"clip" : {
		"desc" : "",
		"dom" : "clip"
	},
	"color" : {
		"desc" : "",
		"dom" : "color"
	},
	"column" : {
		"moz" : "-moz-column",
		"webkit" : "-webkit-column",
		"desc" : "",
		"dom" : "column"
	},
	"column-count" : {
		"moz" : "-moz-column-count",
		"webkit" : "-webkit-column-count",
		"desc" : "",
		"dom" : "columnCount"
	},
	"column-fill" : {
		"desc" : "",
		"dom" : "columnFill"
	},
	"column-gap" : {
		"moz" : "-moz-column-gap",
		"webkit" : "-webkit-column-gap",
		"desc" : "",
		"dom" : "columnGap"
	},
	"column-rule" : {
		"moz" : "-moz-column-rule",
		"webkit" : "-webkit-column-rule",
		"desc" : "",
		"dom" : "columnRule"
	},
	"column-rule-color" : {
		"moz" : "-moz-column-rule-color",
		"webkit" : "-webkit-column-rule-color",
		"desc" : "",
		"dom" : "columnRuleColor"
	},
	"column-rule-style" : {
		"moz" : "-moz-column-rule-style",
		"webkit" : "-webkit-column-rule-style",
		"desc" : "",
		"dom" : "columnRuleStyle"
	},
	"column-rule-width" : {
		"moz" : "-moz-column-rule-width",
		"webkit" : "-webkit-column-rule-width",
		"desc" : "",
		"dom" : "columnRuleWidth"
	},
	"column-span" : {
		"webkit" : "-webkit-column-span",
		"desc" : "",
		"dom" : "columnSpan"
	},
	"column-width" : {
		"moz" : "-moz-column-width",
		"webkit" : "-webkit-column-width",
		"desc" : "",
		"dom" : "columnWidth"
	},
	"columns" : {
		"moz" : "-moz-columns",
		"webkit" : "-webkit-columns",
		"desc" : "",
		"dom" : "columns"
	},
	"content" : {
		"desc" : "",
		"dom" : "content"
	},
	"counter-increment" : {
		"desc" : "",
		"dom" : "counterIncrement"
	},
	"counter-reset" : {
		"desc" : "",
		"dom" : "counterReset"
	},
	"cursor" : {
		"desc" : "",
		"dom" : "cursor"
	},
	"direction" : {
		"desc" : "",
		"dom" : "direction"
	},
	"display" : {
		"desc" : "",
		"dom" : "display"
	},
	"empty-cells" : {
		"desc" : "",
		"dom" : "emptyCells"
	},
	"float" : {
		"desc" : "",
		"dom" : "cssFloat"
	},
	"font" : {
		"desc" : "",
		"dom" : "font"
	},
	"font-family" : {
		"desc" : "",
		"dom" : "fontFamily"
	},
	"font-size" : {
		"desc" : "",
		"dom" : "fontSize"
	},
	"font-style" : {
		"desc" : "",
		"dom" : "fontStyle"
	},
	"font-variant" : {
		"desc" : "",
		"dom" : "fontVariant"
	},
	"font-weight" : {
		"desc" : "",
		"dom" : "fontWeight"
	},
	"@font-face" : {
		"desc" : "",
		"dom" : "@fontFace"
	},
	"font-size-adjust" : {
		"desc" : "",
		"dom" : "fontSizeAdjust"
	},
	"font-stretch" : {
		"desc" : "",
		"dom" : "fontStretch"
	},
	"grid-columns" : {
		"desc" : "",
		"dom" : "gridColumns"
	},
	"grid-rows" : {
		"desc" : "",
		"dom" : "gridRows"
	},
	"hanging-punctuation" : {
		"desc" : "",
		"dom" : "hangingPunctuation"
	},
	"height" : {
		"desc" : "",
		"dom" : "height"
	},
	"icon" : {
		"desc" : "",
		"dom" : "icon"
	},
	"@keyframes" : {
		"moz" : "-moz-@keyframes",
		"webkit" : "-webkit-@keyframes",
		"desc" : "",
		"dom" : "@keyframes"
	},
	"left" : {
		"desc" : "",
		"dom" : "left"
	},
	"letter-spacing" : {
		"desc" : "",
		"dom" : "letterSpacing"
	},
	"line-height" : {
		"desc" : "",
		"dom" : "lineHeight"
	},
	"list-style" : {
		"desc" : "",
		"dom" : "listStyle"
	},
	"list-style-image" : {
		"desc" : "",
		"dom" : "listStyleImage"
	},
	"list-style-position" : {
		"desc" : "",
		"dom" : "listStylePosition"
	},
	"list-style-type" : {
		"desc" : "",
		"dom" : "listStyleType"
	},
	"margin" : {
		"moz" : "-moz-margin",
		"webkit" : "-webkit-margin",
		"desc" : "",
		"dom" : "margin"
	},
	"margin-bottom" : {
		"desc" : "",
		"dom" : "marginBottom"
	},
	"margin-left" : {
		"desc" : "",
		"dom" : "marginLeft"
	},
	"margin-right" : {
		"desc" : "",
		"dom" : "marginRight"
	},
	"margin-top" : {
		"desc" : "",
		"dom" : "marginTop"
	},
	"max-height" : {
		"desc" : "",
		"dom" : "maxHeight"
	},
	"max-width" : {
		"desc" : "",
		"dom" : "maxWidth"
	},
	"min-height" : {
		"desc" : "",
		"dom" : "minHeight"
	},
	"min-width" : {
		"desc" : "",
		"dom" : "minWidth"
	},
	"nav-down" : {
		"desc" : "",
		"dom" : "navDown"
	},
	"nav-index" : {
		"desc" : "",
		"dom" : "navIndex"
	},
	"nav-left" : {
		"desc" : "",
		"dom" : "navLeft"
	},
	"nav-right" : {
		"desc" : "",
		"dom" : "navRight"
	},
	"nav-up" : {
		"desc" : "",
		"dom" : "navUp"
	},
	"opacity" : {
		"desc" : "",
		"dom" : "opacity"
	},
	"outline" : {
		"desc" : "",
		"dom" : "outline"
	},
	"outline-color" : {
		"desc" : "",
		"dom" : "outlineColor"
	},
	"outline-offset" : {
		"desc" : "",
		"dom" : "outlineOffset"
	},
	"outline-style" : {
		"desc" : "",
		"dom" : "outlineStyle"
	},
	"outline-width" : {
		"desc" : "",
		"dom" : "outlineWidth"
	},
	"overflow" : {
		"desc" : "",
		"dom" : "overflow"
	},
	"overflow-x" : {
		"desc" : "",
		"dom" : "overflowX"
	},
	"overflow-y" : {
		"moz" : "-moz-overflow-y",
		"webkit" : "-webkit-overflow-y",
		"desc" : "",
		"dom" : "overflowY"
	},
	"padding" : {
		"desc" : "",
		"dom" : "padding"
	},
	"padding-bottom" : {
		"desc" : "",
		"dom" : "paddingBottom"
	},
	"padding-left" : {
		"desc" : "",
		"dom" : "paddingLeft"
	},
	"padding-right" : {
		"desc" : "",
		"dom" : "paddingRight"
	},
	"padding-top" : {
		"desc" : "",
		"dom" : "paddingTop"
	},
	"page-break-after" : {
		"desc" : "",
		"dom" : "pageBreakAfter"
	},
	"page-break-before" : {
		"desc" : "",
		"dom" : "pageBreakBefore"
	},
	"page-break-inside" : {
		"desc" : "",
		"dom" : "pageBreakInside"
	},
	"perspective" : {
		"webkit" : "-webkit-perspective",
		"desc" : "",
		"dom" : "perspective"
	},
	"perspective-origin" : {
		"webkit" : "-webkit-perspective-origin",
		"desc" : "",
		"dom" : "perspectiveOrigin"
	},
	"position" : {
		"desc" : "",
		"dom" : "position"
	},
	"punctuation-trim" : {
		"desc" : "",
		"dom" : "punctuationTrim"
	},
	"quotes" : {
		"desc" : "",
		"dom" : "quotes"
	},
	"resize" : {
		"desc" : "",
		"dom" : "resize"
	},
	"right" : {
		"desc" : "",
		"dom" : "right"
	},
	"rotation" : {
		"desc" : "",
		"dom" : "rotation"
	},
	"rotation-point" : {
		"desc" : "",
		"dom" : "rotationPoint"
	},
	"table-layout" : {
		"desc" : "",
		"dom" : "tableLayout"
	},
	"target" : {
		"desc" : "",
		"dom" : "target"
	},
	"target-name" : {
		"desc" : "",
		"dom" : "targetName"
	},
	"target-new" : {
		"desc" : "",
		"dom" : "targetNew"
	},
	"target-position" : {
		"desc" : "",
		"dom" : "targetPosition"
	},
	"text-align" : {
		"desc" : "",
		"dom" : "textAlign"
	},
	"text-decoration" : {
		"desc" : "",
		"dom" : "textDecoration"
	},
	"text-indent" : {
		"desc" : "",
		"dom" : "textIndent"
	},
	"text-justify" : {
		"desc" : "",
		"dom" : "textJustify"
	},
	"text-outline" : {
		"desc" : "",
		"dom" : "textOutline"
	},
	"text-overflow" : {
		"desc" : "",
		"dom" : "textOverflow"
	},
	"text-shadow" : {
		"desc" : "",
		"dom" : "textShadow"
	},
	"text-transform" : {
		"desc" : "",
		"dom" : "textTransform"
	},
	"text-wrap" : {
		"desc" : "",
		"dom" : "textWrap"
	},
	"top" : {
		"desc" : "",
		"dom" : "top"
	},
	"transform" : {
		"moz" : "-moz-transform",
		"webkit" : "-webkit-transform",
		"ms" : "-ms-transform",
		"opera" : "-o-transform",
		"desc" : "",
		"dom" : "transform"
	},
	"transform-origin" : {
		"moz" : "-moz-transform-origin",
		"webkit" : "-webkit-transform-origin",
		"ms" : "-ms-transform-origin",
		"opera" : "-o-transform-origin",
		"desc" : "",
		"dom" : "transformOrigin"
	},
	"transform-style" : {
		"webkit" : "-webkit-transform-style",
		"desc" : "",
		"dom" : "transformStyle"
	},
	"transition" : {
		"moz" : "-moz-transition",
		"webkit" : "-webkit-transition",
		"opera" : "-o-transition",
		"desc" : "",
		"dom" : "transition"
	},
	"transition-property" : {
		"moz" : "-moz-transition-property",
		"webkit" : "-webkit-transition-property",
		"opera" : "-o-transition-property",
		"desc" : "",
		"dom" : "transitionProperty"
	},
	"transition-duration" : {
		"moz" : "-moz-transition-duration",
		"webkit" : "-webkit-transition-duration",
		"opera" : "-o-transition-duration",
		"desc" : "",
		"dom" : "transitionDuration"
	},
	"transition-timing-function" : {
		"moz" : "-moz-transition-timing-function",
		"webkit" : "-webkit-transition-timing-function",
		"opera" : "-o-transition-timing-function",
		"desc" : "",
		"dom" : "transitionTimingFunction"
	},
	"transition-delay" : {
		"moz" : "-moz-transition-delay",
		"webkit" : "-webkit-transition-delay",
		"opera" : "-o-transition-delay",
		"desc" : "",
		"dom" : "transitionDelay"
	},
	"vertical-align" : {
		"desc" : "",
		"dom" : "verticalAlign"
	},
	"visibility" : {
		"desc" : "",
		"dom" : "visibility"
	},
	"width" : {
		"desc" : "",
		"dom" : "width"
	},
	"white-space" : {
		"desc" : "",
		"dom" : "whiteSpace"
	},
	"word-spacing" : {
		"desc" : "",
		"dom" : "wordSpacing"
	},
	"word-break" : {
		"desc" : "",
		"dom" : "wordBreak"
	},
	"word-wrap" : {
		"desc" : "",
		"dom" : "wordWrap"
	},
	"z-index" : {
		"desc" : "",
		"dom" : "zIndex"
	}
};

methods['admin.updateForm'] = function(rpc, id, html) {
	var uistack = [];
	var uiobj = {};
	var uiid = null;
	var parseHtml = function(xml, callback) {
		var error = null, method = null, params = [];
		var properties = null;
		var saxParser = new libxml.SaxParser({
			startDocument : function() {
			},
			endDocument : function() {
				// util.puts('endDocument ' + method + ' ' +
				// util.inspect(params))
				callback(error, method, params);
			},
			startElementNS : function(elem, attrs, prefix, uri, namespaces) {
				if (elem.toLowerCase() != 'body') {
					var id = null;
					properties = {};
					properties['element'] = elem;
					for ( var idx = 0; idx < attrs.length; idx++) {
						switch (attrs[idx][0].toLowerCase()) {
						case 'id':
							id = attrs[idx][3];
							break;
						case 'class':
							properties['className'] = attrs[idx][3];
							break;
						case 'style': {
							var style = {};
							var data = attrs[idx][3].replace(' ', '')
									.split(';');
							for ( var sidx = 0; sidx < data.length; sidx++) {
								if (data[sidx] != '') {
									var pairs = data[sidx].split(':');
									var dom = styles[pairs[0].replace(/^\s\s*/,
											'').replace(/\s\s*$/, '')].dom;
									if (dom)
										style[dom] = pairs[1].replace(/^\s\s*/,
												'').replace(/\s\s*$/, '');
								}
							}
							properties['style'] = style;
							break;
						}
						case 'onclick':
							properties[attrs[idx][0]] = attrs[idx][3].replace(
									/\([^)]*\);?|;/, '');
							break;
						default:
							properties[attrs[idx][0]] = attrs[idx][3];
							break;
						}
					}
					if (!id)
						id = elem + Math.floor(Math.random() * 100);

					// в стеке уже есть элемент
					var parent = uistack[uistack.length - 1];
					if (parent) {
						if (!parent['properties']['childs'])
							parent['properties']['childs'] = {};
						parent['properties']['childs'][id] = properties;
					}
					uistack.push({
						'id' : id,
						'properties' : properties
					});
				}
			},
			endElementNS : function(elem, prefix, uri) {
				if (elem.toLowerCase() != 'body') {
					properties = null;
					if (uistack.length == 1) {
						uiobj[uistack[0]['id']] = uistack[0]['properties'];
						uiid = uistack[0]['id'];
					}
					uistack.pop();
				}
			},
			characters : function(chars) {
				if (!chars.match(/^[\s\n\r]+$/)) {
					chars = chars.trim();
					if ((chars != '') && properties) {
						// util.puts('CHARS: ' + chars);
						properties.text = chars;
					}
				}
			},
			comment : function(msg) {
				// util.puts('comment: ' + msg);
			},
			warning : function(msg) {
				// util.puts('warning: ' + msg);
			},
			error : function(msg) {
				util.puts('XML ACHTUNG: ' + msg);
				callback(msg, method, params);
			},
			cdata : function(chars) {
				if (!chars.match(/^[\s\n\r]+$/) && (charsHandler)) {
					charsHandler(chars);
					// util.puts('cdata: ' + chars);
				}
			}
		});
		saxParser.parseString(xml);
	};
	var parser_error = '';
	var body = /<body[\s\S]*>([\s\S]*)<\/body>/img.exec(html);
	util.puts('HERE NORM');
	if (body != null) {
		parseHtml(
				body[0],
				function(error, method, params) {
					util.puts('HERE');
					if (error)
						parser_error += error.replace(/^\s\s*/, '').replace(
								/\s\s*$/, '')
								+ '. ';
					else {
						if (parser_error != '')
							rpc.response({
								'code' : 0,
								'description' : 'Ok',
								'data' : parser_error
							});
						else {
							id = parseInt(id, 10);
							// if (session.identity)
							var checkSql = '';
							if (isNaN(id))
								checkSql = 'SELECT id FROM client_forms WHERE title=\''
										+ conn.escapeSync(uiid) + '\' LIMIT 1';
							else
								checkSql = 'SELECT id FROM client_forms WHERE id=\''
										+ id + '\' LIMIT 1';
							var res = conn.querySync(checkSql);
							if (res != false) {
								var rows = res.fetchAllSync();
								if (rows.length > 0) {
									var updateSql = 'UPDATE client_forms SET body=\''
											+ conn.escapeSync(JSON
													.stringify(uiobj))
											+ '\', title=\''
											+ conn.escapeSync(uiid)
											+ '\' WHERE id=' + rows[0]['id'];
									conn.querySync(updateSql);
								} else {
									var insertSql = 'INSERT INTO client_forms (body, title) VALUES (\''
											+ conn.escapeSync(JSON
													.stringify(uiobj))
											+ '\', \''
											+ conn.escapeSync(uiid)
											+ '\')';
									conn.querySync(insertSql);
								}
							} else {
								util.puts('no res in admin.clientForms');
							}

							rpc.response({
								'code' : 0,
								'description' : 'Ok',
								'data' : uiid
							});
						}
					}
					// util.puts(util.inspect(uiobj, false, 10));
				});
	} else
		rpc.response({
			'code' : -1,
			'description' : 'Invalid HTML'
		});
};

methods['admin.setPermissions'] = function(rpc, id, permissions) {
	var req = rpc.HTTPRequest;
	var session = req.session.data;
	// Check user logged
	console.log(id + ' ' + permissions);
	id = parseInt(id, 10);
	permissions = parseInt(permissions, 10);
	// if (session.identity)
	if (isNaN(id) || isNaN(permissions)) {
		rpc.response({
			'code' : -2,
			'description' : 'Error'
		});
		return;
	}

	var insertSql = 'UPDATE auth_identity SET user_rights=' + permissions
			+ ' WHERE id=' + id;
	var res = conn.querySync(insertSql);
	if (res != false) {
		rpc.response({
			'code' : 0,
			'description' : 'Ok'
		});
	} else
		rpc.response({
			'code' : -1,
			'description' : 'Error'
		});
};

methods['admin.updateUser'] = function(rpc, id, login, first_name, second_name,
		email) {
	var req = rpc.HTTPRequest;
	var session = req.session.data;
	// Check user logged
	id = parseInt(id, 10);
	// if (session.identity)
	if (isNaN(id)) {
		rpc.response({
			'code' : -2,
			'description' : 'Error'
		});
		return;
	}

	var updateSql = 'UPDATE auth_identity SET login=\''
			+ conn.escapeSync(login) + '\', first_name=\''
			+ conn.escapeSync(first_name) + '\', second_name=\''
			+ conn.escapeSync(second_name) + '\', email=\''
			+ conn.escapeSync(email) + '\' WHERE id=' + id;
	var res = conn.querySync(updateSql);
	if (res != false) {
		rpc.response({
			'code' : 0,
			'description' : 'Ok'
		});
	} else
		rpc.response({
			'code' : -1,
			'description' : 'Error'
		});
};

methods['admin.setPassword'] = function(rpc, id, password) {
	var req = rpc.HTTPRequest;
	var session = req.session.data;
	// Check user logged
	console.log(id + ' ' + permissions);
	id = parseInt(id, 10);
	// if (session.identity)
	if (isNaN(id)) {
		rpc.response({
			'code' : -2,
			'description' : 'Error'
		});
		return;
	}

	var insertSql = 'UPDATE auth_identity SET password=\''
			+ crypto.createHash('sha1').update(password).digest('hex')
			+ '\' WHERE id=' + id;
	var res = conn.querySync(insertSql);
	if (res != false) {
		rpc.response({
			'code' : 0,
			'description' : 'Ok'
		});
	} else
		rpc.response({
			'code' : -1,
			'description' : 'Error'
		});
};
methods['admin.interface'] = function(rpc, type) {
	var req = rpc.HTTPRequest;
	var session = req.session.data;
	// Check user logged
	// if (session.identity)
	var content = fs.readFileSync('./views/' + type + '.html', 'utf-8');
	if (content) {
		rpc.response({
			'code' : 0,
			'description' : 'Ok',
			'data' : content
		});
	} else
		rpc.response({
			'code' : -1,
			'description' : 'No Logic'
		});
};

methods['admin.storeLogic'] = function(rpc, object) {
	var req = rpc.HTTPRequest;
	var session = req.session.data;
	// Check user logged
	// if (session.identity)

	// GC
	var logicObj = JSON.parse(object);
	for (qid in logicObj) {
		logics = logicObj[qid].logics;
		for (lid in logics) {
			element = logics[lid];
			// if 'next' or 'or' then add to inned
			if (element.next && !logics[element.next])
				delete element.next;
			if (element.or && !logics[element.or])
				delete element.or;
		}
	}
	applogic = logicObj;
	var insertSql = 'INSERT INTO logic (object, modified_by) VALUES (\''
			+ conn.escapeSync(JSON.stringify(applogic)) + '\',' + 0 + ')';
	var res = conn.querySync(insertSql);
	if (res != false) {
		rpc.response({
			'code' : 0,
			'description' : 'Ok'
		});
	} else
		rpc.response({
			'code' : -1,
			'description' : 'Error'
		});
};

methods['admin.login'] = function(rpc, login, password) {
	var req = rpc.HTTPRequest;
	var session = req.session.data;
	// Reconnect
	var res = conn.pingSync();
	if (!res) {
		conn = mysql.createConnectionSync();
		conn.connectSync(dbconf.host, dbconf.user, dbconf.password,
				dbconf.database);
		if (!conn.connectedSync()) {
			util.puts("Connection error " + conn.connectErrno + ": "
					+ conn.connectError);
			process.exit(1);
		}
	}
	// console.log(crypto.createHash('sha1').update(password).digest('hex'));
	var checkSql = 'SELECT * FROM auth_identity WHERE login=\''
			+ conn.escapeSync(login) + '\' AND password=\''
			+ crypto.createHash('sha1').update(password).digest('hex')
			+ '\' LIMIT 1';
	var res = conn.querySync(checkSql);
	if (res != false) {
		var rows = res.fetchAllSync();
		if (rows.length > 0) {
			session.identity = rows[0];
			rpc.response({
				'code' : 0,
				'description' : 'Ok'
			});
		} else
			rpc.response({
				'code' : -1,
				'description' : 'Invalid auth'
			});
	} else {
		util.puts('no res in login ' + conn.connectErrno + ": "
				+ conn.connectError);
	}
};

methods['admin.loadLogic'] = function(rpc) {
	var req = rpc.HTTPRequest;
	var session = req.session.data;
	// Check user logged
	// if (session.identity)
	var checkSql = 'SELECT * FROM logic ORDER BY modified DESC LIMIT 1';
	var res = conn.querySync(checkSql);
	if (res != false) {
		var rows = res.fetchAllSync();
		if (rows.length > 0) {
			rpc.response({
				'code' : 0,
				'description' : 'Ok',
				'data' : rows[0]
			});
		} else
			rpc.response({
				'code' : -1,
				'description' : 'No Logic'
			});
	} else {
		util.puts('no res in loadLogic');
	}
};

/*
 * components.init = function(resp, params) { components.getView(resp, {"view":
 * "login"}); components.getView(resp, {"view": "register"});
 * components.getMethod(resp, {"method": "clearCanvas"});
 * components.getMethod(resp, {"method": "carousel"});
 * components.getMethod(resp, {"method": "showModal"});
 * components.getMethod(resp, {"method": "hideWindow"});
 * components.getMethod(resp, {"method": "showWindow"});
 * components.getMethod(resp, {"method": "stopCarousel"});
 * components.getMethod(resp, {"method": "startCarousel"}); resp.push({ "cmd" :
 * "startCarousel", "params" : "" }); };
 */

components.getMethod = function(resp, params) {
	try {
		var body = fs.readFileSync('./client/methods/' + params.method + '.js',
				'utf-8');
		resp.push({
			"cmd" : "initMethod",
			"params" : {
				"method" : params.method,
				"body" : body
			}
		});
	} catch (e) {
		var checkSql = 'SELECT * FROM client_methods WHERE title=\''
				+ conn.escapeSync(params.method) + '\' LIMIT 1';
		var res = conn.querySync(checkSql);
		if (res != false) {
			var rows = res.fetchAllSync();
			if (rows.length > 0) {
				resp.push({
					"cmd" : "initMethod",
					"params" : {
						"method" : params.method,
						"body" : rows[0]['body']
					}
				});
			}
		}
	}
};

components.getView = function(resp, params) {
	try {
		var body = JSON.parse(fs.readFileSync('./client/views/' + params.view
				+ '.json', 'utf-8'));
		resp.push({
			"cmd" : "initUI",
			"params" : {
				"view" : params.view,
				"body" : body
			}
		});
	} catch (e) {
		var checkSql = 'SELECT * FROM client_forms WHERE title=\''
				+ conn.escapeSync(params.view) + '\' LIMIT 1';
		var res = conn.querySync(checkSql);
		if (res != false) {
			var rows = res.fetchAllSync();
			if (rows.length > 0) {
				resp.push({
					"cmd" : "initUI",
					"params" : {
						"view" : params.view,
						"body" : JSON.parse(rows[0]['body'])
					}
				});
			}
		}
	}
	// try {
	// var body = JSON.parse(fs.readFileSync('./client/views/' + params.view
	// + '.json', 'utf-8'));
	// resp.push({
	// "cmd" : "initUI",
	// "params" : {
	// "view" : params.view,
	// "body" : body
	// }
	// });
	// } catch (e) {
	// util.puts(e);
	// }
};

methods['ad.serve'] = function(rpc) {
	var resp = [];
	var args = arguments[0];
	// Reconnect
	var res = conn.pingSync();
	if (!res) {
		conn = mysql.createConnectionSync();
		conn.connectSync(dbconf.host, dbconf.user, dbconf.password,
				dbconf.database);
		if (!conn.connectedSync()) {
			util.puts("Connection error " + conn.connectErrno + ": "
					+ conn.connectError);
			process.exit(1);
		}
	}

	if (args.json && args.json.params) {
		var params = args.json.params;
		// util.puts('SERVE: ' + util.inspect(params));
		for ( var idx = 0; idx < params.length; idx++) {
			if (components[params[idx].cmd]) {
				params[idx]['session'] = rpc.HTTPRequest.session.data;
				components[params[idx].cmd](resp, params[idx]);
			} else {
				util.puts('\nUNINPLEMENTED SERVER METHOD: \n'
						+ util.inspect(params[idx]));
			}
		}
	}

	var session = rpc.HTTPRequest.session.data;
	// Handle applogic
	if (session.identity) {
		loopback = [];
		if (!session.context)
			session.context = qengine.handle(applogic, null, resp, loopback);
		else
			qengine.handle(applogic, session.context, resp, loopback);

		// Handle server methods from applogics
		for ( var idx = 0; idx < loopback.length; idx++) {
			if (components[loopback[idx].cmd]) {
				loopback[idx]['session'] = session;
				components[loopback[idx].cmd](resp, loopback[idx]);
			} else {
				util.puts('\nUNINPLEMENTED SERVER METHOD: \n'
						+ util.inspect(loopback[idx]));
			}
		}
		// util.puts('\nCONTEXT:\n' + util.inspect(session.context, false, 5));
		// util.puts('RESPONSE:\n' + util.inspect(resp, false, 5));
	}

	rpc.response({
		'params' : resp,
		'code' : -1,
		'description' : 'Invalid auth'
	});
};

methods['game.registration'] = function(rpc, login, first_name, second_name,
		email, password) {
	// TODO check fields
	var insertSql = 'INSERT INTO auth_identity (login, password, email, first_name, second_name) VALUES (\''
			+ conn.escapeSync(login)
			+ '\', \''
			+ crypto.createHash('sha1').update(password).digest('hex')
			+ '\',\''
			+ conn.escapeSync(email)
			+ '\',\''
			+ conn.escapeSync(first_name)
			+ '\',\''
			+ conn.escapeSync(second_name) + '\')';
	var res = conn.querySync(insertSql);
	if (res != false) {
		rpc.response({
			'code' : 0,
			'description' : 'Ok'
		});
	} else
		rpc.response({
			'code' : -1,
			'description' : 'Error'
		});
};

jqtpl.template('admin', fs.readFileSync('./views/admin.html', 'utf-8'));
jqtpl.template('toolbar_item', fs.readFileSync('./views/toolbar_item.html',
		'utf-8'));
jqtpl.template('login', fs.readFileSync('./views/login.html', 'utf-8'));
jqtpl.template('index', fs.readFileSync('./views/index.html', 'utf-8'));
jqtpl.template('registration', fs.readFileSync('./views/registration.html',
		'utf-8'));

var checkSql = 'SELECT object from logic ORDER BY id DESC LIMIT 1';
var res = conn.querySync(checkSql);
if (res != false) {
	var rows = res.fetchAllSync();
	if (rows.length > 0) {
		applogic = JSON.parse(rows[0]['object']);
	} else
		util.puts('no logic');
} else {
	util.puts('no res in ' + checkSql);
}

function checkPermission(user_rights, name) {
	return true;
};

function updateCss() {
	var checkSql = 'SELECT * FROM client_style ORDER BY id DESC';
	var res = conn.querySync(checkSql);
	css = null;
	if (res != false) {
		var rows = res.fetchAllSync();
		if (rows.length > 0) {
			for ( var idx = 0; idx < rows.length; idx++)
				css += rows[idx]['title'] + ' ' + rows[idx]['body'];
		} else
			css = 'No Styles';
	} else {
		util.puts('no res in css');
		css = 'OK';
	}
};

var checkSql = 'SELECT * FROM server_methods';
var res = conn.querySync(checkSql);
if (res != false) {
	var rows = res.fetchAllSync();
	if (rows.length > 0) {
		for ( var idx = 0; idx < rows.length; idx++) {
			var row = rows[idx];
			if ((row['title'] != '') && (row['body'] != '')) {
				var str = 'components[\'' + row['title'] + '\'] = '
						+ row['body'];
				try {
					eval(str);
				} catch (e) {
					util.puts(e + 'in \n\'' + str + '\'');
				}
			} else
				util.puts('empty server method');
		}
	}
} else {
	util.puts('no res in server methods');
}

var filenames = fs.readdirSync("./methods");
for ( var i = 0; i < filenames.length; i++) {
	var filename = filenames[i];
	try {
		fname = filename.split('.')[0];
		var body = fs.readFileSync('./methods/' + filename, 'utf-8');
		var str = 'components[\'' + fname + '\'] = ' + body;
		eval(str);
	} catch (e) {
		util.puts(e)
	}
}

http
		.createServer(
				function(req, resp) {
					/**
					 * Default fill basic blocks
					 */
					req.tpl = {};
					req.prepared = {};
					// req.conn = conn;

					// parse URL
					var url_parts = url.parse(req.url);
					req.pathname = url_parts.pathname;

					// TODO switch on production
					req.basedomain = req.headers.host; // req.host_parts[hp_len-2]+'.'+req.host_parts[hp_len-1];
					// TODO: Need to set base domain in session cookie (second
					// level)
					req.session = sessions.lookupOrCreate(req, {
						lifetime : 604800,
						domain : '.' + req.basedomain
					});
					// implement a basic history for each session, be mindful of
					// this as it
					// could eat up memory with high usage
					// if (!req.session.data.history) {
					// req.session.data.history = [];
					// }
					// req.session.data.history.push(req.url);
					var session = req.session.data;

					// as a convention, we will set all non-indentified users to
					// "Guest"
					if (typeof req.session.data.user == 'undefined'
							|| req.session.data.user == '') {
						req.session.data.user = "Guest";
					}
					resp.setHeader('Set-Cookie', req.session
							.getSetCookieHeaderValue());

					// parse query
					req.params = querystring.parse(url_parts.query);
					switch (req.method.toLowerCase()) {
					case 'post':
						// console.log('RPC');
						new RPCHandler(req, resp, methods, true);
						break;
					case 'get':
						var pathParts = req.pathname.split('/');
						console.log('GET: ' + pathParts[1].toLowerCase());
						switch (pathParts[1].toLowerCase()) {
						case 'admin':
							resp.statusCode = 200;
							resp.writeHead(200, {
								'content-type' : 'application/xhtml+xml'
							});
							var render = {};
							if (session.identity == undefined)
								resp.end(jqtpl.tmpl('login'));
							else {
								var toolbar = {};
								var user_rights = session.identity.user_rights;
								for ( var idx = 0; idx < permissions.length; idx++) {
									var element = permissions[idx];
									var mask = 1 << element.id; // mask to check
									// permissions
									if ((req.session.data.identity.login == superuser)
											|| ((user_rights & mask) != 0)) {
										if (toolbar[element['type']] == undefined)
											toolbar[element['type']] = [];
										toolbar[element['type']].push(element);
									}
								}

								for ( var element in toolbar)
									render[element] = jqtpl.tmpl(
											'toolbar_item', toolbar[element]);
								resp.end(jqtpl.tmpl('admin', render));
							}
							break;
						case 'css':
							resp.statusCode = 200;
							resp.writeHead(200, {
								'content-type' : 'text/css'
							});
							updateCss();
							resp.end(css);
							break;
						case 'data': {
							if (pathParts[3] && (pathParts[2].toLowerCase() == 'maps')) {
								var mapTitle = pathParts[3].split('.')[0];
								console.log(mapTitle)
								var checkSql = 'SELECT * FROM locations WHERE title=\''
										+ conn.escapeSync(mapTitle) + '\' LIMIT 1';
								var res = conn.querySync(checkSql);
								if (res != false) {
									var rows = res.fetchAllSync();
									if (rows.length > 0) {
										resp.end(rows[0].body);
									} else
										resp.end('{"map":[], "properties": []}');
								} else {
									util.puts('no res in css');
									resp.end('{"map":[], "properties": []}');
								}
							}
						}
							break;
						case '':
							resp.statusCode = 200;
							resp.writeHead(200, {
								'content-type' : 'text/html'
							});
							resp.end(jqtpl.tmpl('index', {
								a : 1
							}));
							break;
						default:
							break;
						}
						break;
					}
				}).listen(port);
