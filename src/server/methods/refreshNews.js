function(resp, params) {
	var checkSql = 'SELECT * FROM news ORDER BY id DESC LIMIT 10';
	var res = conn.querySync(checkSql);
	if (res != false) {
		var rows = res.fetchAllSync();
		if (rows.length > 0) {
			resp.push({
				"cmd" : "refreshNews",
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