function(resp, params) {
	if (!artifacts)
	{
		var checkSql = 'SELECT * FROM artifacts ORDER BY id DESC LIMIT 10';
		var res = conn.querySync(checkSql);
		if (res != false) {
			var rows = res.fetchAllSync();
			if (rows.length > 0) {
				artifacts = rows;
			} else
			{
				//No news
			}
		} else {
			util.puts('no res in news');
		}
	}
	resp.push({
		"cmd" : "refreshShopItems",
		"params" : artifacts
	});
};