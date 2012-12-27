function(resp, params) {
	var checkSql = 'SELECT * FROM auth_identity WHERE login=\''
			+ conn.escapeSync(params.login) + '\' AND password=\''
			+ crypto.createHash('sha1').update(params.password).digest('hex') + '\' LIMIT 1';
	var res = conn.querySync(checkSql);
	if (res != false) {
		var rows = res.fetchAllSync();
		if (rows.length > 0) {
			params.session.identity = rows[0];
			users[params.session.identity.login] = params.session.identity.id;
			if (!contexts[params.session.identity.id])
			{
				var checkSql = 'SELECT * FROM personage WHERE user_id=' + params.session.identity.id + ' LIMIT 1'; 
				var res = conn.querySync(checkSql);
				if (res != false) {
					var rows = res.fetchAllSync();
					if (rows.length > 0) {
						params.session.context = JSON.parse(rows[0]['body']);
						params.session.context.personageId = rows[0]['id'];
					} else 
						params.session.context = {};
				}
				contexts[params.session.identity.id] = params.session.context;
			} else
				params.session.context = contexts[params.session.identity.id];
			
			var context = params.session.context;
			if (context.personage)
			{
				if (!context.personage.race || (context.personage.race == 0))
				{
					components.getMethod(resp, {"method": "selectRace"});
					components.getMethod(resp, {"method": "startSelectRace"});
					components.getMethod(resp, {"method": "stopSelectRace"});
				} else
				{
					components.mainUI(resp, params);
					resp.push({
						"cmd" : "clearCanvas",
						"params" : ""
					});
					resp.push({
						"cmd" : "getPersStat",
						"params" : ""
					});
				}
			} else
			{
				components.getMethod(resp, {"method": "selectRace"});
				components.getMethod(resp, {"method": "startSelectRace"});
				components.getMethod(resp, {"method": "stopSelectRace"});
			}
			resp.push({
				"cmd" : "hideWindow",
				"params" : "login-window"
			});
//			resp.push({
//				"cmd" : "stopCarousel",
//				"params" : ""
//			});
			resp.push({
				"cmd" : "clearCanvas",
				"params" : ""
			});
		} else
		{
			// Wrong auth
			resp.push({
				"cmd" : "showModal",
				"params" : {"header" : "Wrong login or password", "body" : "Try another"}
			});
		}
	} else {
		util.puts('no res in login');
	}
};