function method(resp, params) {
	var context = params.session.context;
	if (params.uid) {
		var rid = parseInt(params.uid, 10);
		if (!isNaN(rid)) {
			var receiver = getContext(rid);
			if (receiver) {
				if (!receiver.duels)
					receiver.duels = {};
				if (receiver.duels[params.session.identity.id]) {
					receiver.duels[params.session.identity.id].accept = true;
					if (!context.duels)
						context.duels = {};
					context.duels[rid] = {
						'user' : context.challenge[rid]
					};
					// TODO : fight
				} else {
					// there is no challenge
				}
			} else {
				// unknown receiver
			}
		}
	} else {
		// invalid params
	}
	util.puts(util.inspect(params, true, 10));
	// context.personage.race = params.race;
	// resp.push({
	// "cmd" : "showPersStat",
	// "params" : context.personage
	// });
};