function(resp, params) {
	// TODO check fields
	var insertSql = 'INSERT INTO auth_identity (login, password, email, first_name, second_name) VALUES (\''
			+ conn.escapeSync(params.login)
			+ '\', \''
			+ crypto.createHash('sha1').update(params.password).digest('hex')
			+ '\',\''
			//+ conn.escapeSync(params.email)
			+ '\',\''
			//+ conn.escapeSync(params.first_name)
			+ '\',\''
			//+ conn.escapeSync(params.second_name) 
			+ '\')';
	var res = conn.querySync(insertSql);
	if (res != false) {
		var checkSql = 'SELECT * FROM auth_identity WHERE login=\''
			+ conn.escapeSync(params.login) + '\' AND password=\''
			+ crypto.createHash('sha1').update(params.password).digest('hex') + '\' LIMIT 1';
		var res = conn.querySync(checkSql);
		if (res != false) {
			var rows = res.fetchAllSync();
				params.session.identity = rows[0];
				resp.push({
					"cmd" : "hideWindow",
					"params" : "register-window"
				});
				resp.push({
					"cmd" : "stopCarousel",
					"params" : ""
				});
				components.getView(resp, {"view": "avatar"});
				components.getView(resp, {"view": "best"});
				components.getView(resp, {"view": "chat-window"});
				components.getView(resp, {"view": "duel"});
				components.getView(resp, {"view": "info"});
				components.getView(resp, {"view": "map"});
				components.getView(resp, {"view": "mentor"});
				components.getView(resp, {"view": "premium"});
				components.getView(resp, {"view": "ratio"});
				components.getView(resp, {"view": "shop"});
				resp.push({
					"cmd" : "clearCanvas",
					"params" : ""
				});
		} else {
			util.puts('no res in login');
		}		
	} else
	{
		// Wrong
		resp.push({
			"cmd" : "showModal",
			"params" : {"header" : "Server error", "body" : "Try another"}
		});
	}
};