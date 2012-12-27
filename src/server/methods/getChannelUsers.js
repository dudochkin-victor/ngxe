function method(resp, params) {
	var context = params.session.context;
	var channelid = parseInt(params.cid, 10);
	if (!isNaN(channelid)) {
		if (channels[channelid]) {
			var channel = channels[channelid];
			resp.push({
				"cmd" : "showChannelUsers",
				"params" : {
					'cid' : channelid,
					'title' : channel.title,
					'users' : channel.users,
					'userlist' : channel.userlist
				}
			});
		} else {
			util.puts('channel not exist');
		}
	}
};