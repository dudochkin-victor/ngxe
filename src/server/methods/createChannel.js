function method(resp, params) {
	// find free channell
	var chids = [];
	for (var channelid in channels)
		chids.push(channelid);
	chids.sort();
	
	var channelid = arr[idx]+1;
	for ( var idx = 1; idx < chids.length; idx++) {
		if (channelid < chids[idx])
			break;
		channelid = chids[idx]+1;
	}
	
	var channel = channels[channelid] = {};
	channel.messages = [];
	channel.title = params.title;
	channel.description = params.description;
	channel.authid = params.session.identity.id;
	channel.author = params.session.identity.login;
	channel.users = 0;
	channel.userlist = {};
	channel.userlist[params.session.identity.id] = params.session.identity.login;
	channel.users++;
	context.subscription[channelid] = channel.title;
	// context.personage.race = params.race;
	// resp.push({
	// "cmd" : "showPersStat",
	// "params" : context.personage
	// });
};