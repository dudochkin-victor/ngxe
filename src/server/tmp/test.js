var fs = require('fs');
var util = require('util');
var mysql = require('mysql-libmysqlclient');
var qengine = require('quest');
var dbconf = JSON.parse(fs.readFileSync('../database.config', 'utf-8'));
// Create connection
conn = mysql.createConnectionSync();
conn.connectSync(dbconf.host, dbconf.user, dbconf.password, dbconf.database);

if (!conn.connectedSync()) {
	util.puts("Connection error " + conn.connectErrno + ": "
			+ conn.connectError);
	process.exit(1);
}

process.on('exit', function() {
	if (conn != undefined)
		conn.closeSync();
});

var applogic = null;
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

if (applogic) {
	var response = [];
	var context = null;
	if (!context)
		context = qengine.handle(applogic, null, response);
	else
		qengine.handle(applogic, context, response);

	util.puts('CONTEXT:\n' + util.inspect(context, false, 5));
	util.puts('RESPONSE:\n' + util.inspect(response, false, 5));
}