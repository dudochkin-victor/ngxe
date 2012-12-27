function(resp, params) {
	var checkSql = 'SELECT * FROM ratio ORDER BY id DESC LIMIT 10';
	var res = conn.querySync(checkSql);
	if (res != false) {
		var rows = res.fetchAllSync();
		if (rows.length > 0) {
			util.puts('RATIO ' + util.inspect(rows));
			resp.push({
				"cmd" : "refreshRatio",
				"params" : rows
			});
		} else
		{
			//No news
		}
	} else {
		util.puts('no res in news');
	}
};