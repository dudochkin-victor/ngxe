function method(resp, params) {
	var CHANNEL_LENGTH = 30;
	var context = params.session.context;
	var channelid = parseInt(params.cid, 10);
	if (!isNaN(channelid)) {
		if (channels[channelid]) {
			var channel = channels[channelid];
			var now = new Date().getTime();
			channel.messages.push({
				'authid' : params.session.identity.id,
				'author' : params.session.identity.login,
				'message' : params.message,
				'date' : now
			});

			if (!channel.userlist[params.session.identity.id]) {
				channel.users++;
				channel.userlist[params.session.identity.id] = params.session.identity.login;
				if (!context.subscription)
					context.subscription = {};
				context.subscription[channelid] = channel.title;
			}

			while (channel.messages.length > CHANNEL_LENGTH)
				channel.messages.shift();
			var messages = [];

			for ( var idx = 0; idx < channel.messages.length; idx++) {
				var message = channel.messages[idx];
				if ((message.date) && (message.date > context.lastPing))
					messages.push(message);
			}
			params.newMess = true;
			context.lastPing = now;
			resp.push({
				"cmd" : "showMessages",
				"params" : {
					'cid' : channelid,
					'title' : channel.title,
					'messages' : messages,
					'newMess' : params.newMess
				}
			});
		} else {
			util.puts('channel not exist');
		}
	} else if ((params.user) && (users[params.user])) {
		// TODO: find user by login
		var rid = parseInt(users[params.user], 10);
		if (!isNaN(rid)) {
			var receiver = getContext(rid);
			if (receiver) {
				var now = new Date().getTime();
				if (!receiver.messages)
					receiver.messages = [];
				receiver.messages.push({
					'authid' : params.session.identity.id,
					'author' : params.session.identity.login,
					'message' : params.message,
					'date' : now
				});
				while (receiver.messages.length > CHANNEL_LENGTH)
					receiver.messages.shift();
				if (!context.messages)
					context.messages = [];
				context.messages.push({
					'authid' : params.session.identity.id,
					'author' : params.session.identity.login,
					'message' : params.message,
					'date' : now
				});
				while (context.messages.length > CHANNEL_LENGTH)
					context.messages.shift();
				var messages = [];
				if (!context.lastPing)
					context.lastPing = 0;
				util.puts('NOW: ' + now + ' LAST: ' + context.lastPing + ' LENGTH: ' + context.messages.length)
				for ( var idx = 0; idx < context.messages.length; idx++) {
					var message = context.messages[idx];
					if ((message.date) && (message.date > context.lastPing))
						messages.push(message);
				}
				params.newMess = true;
				context.lastPing = now;
				resp.push({
					"cmd" : "showMessages",
					"params" : {
						'uid' : params.session.identity.id,
						'title' : 'UserMessages',
						'messages' : messages,
						'newMess' : params.newMess
					}
				});
			} else {
				util.puts('UNKNOWN RECEIVER');
			}
		}
	} else {
		util.puts('INVALID PARAMS')
	}
};