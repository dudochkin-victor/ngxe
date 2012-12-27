function method(resp, params) {
	var context = params.session.context;
	if (params.uid) {
		var rid = parseInt(params.uid, 10);
		if (!isNaN(rid)) {
			var receiver = getContext(rid);
			if (receiver) {
				if (!receiver.challenge)
					receiver.challenge = {};
				if (!receiver.challenge[params.session.identity.id]) {
					receiver.challenge[params.session.identity.id] = params.session.identity.login;

					if (!context.duels)
						context.duels = {};
					context.duels[rid] = {'user' : params.user};
				}
			} else {
				// unknown receiver
			}
		}
	} else {
		util.puts('invalid params');
	}
	// context.personage.race = params.race;
	// resp.push({
	// "cmd" : "showPersStat",
	// "params" : context.personage
	// });
};