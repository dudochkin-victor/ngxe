function method(resp, params) {
	var context = params.session.context;
	var channelid = parseInt(params.cid, 10);
	if (!isNaN(channelid)) {
		if (!context.subscription)
			context.subscription = {};

		if (channels[channelid]) {
			var channel = channels[channelid];
			channel.users--;
			delete channel.userlist[params.session.identity.id];
			delete context.subscription[channelid];
			if (channel.users == 0)
				delete channels[channelid];
		} else {
			// channel not exist
		}
	} else {
		// invalid params
	}
	// context.personage.race = params.race;
//	resp.push({
//		"cmd" : "showPersStat",
//		"params" : context.personage
//	});
};