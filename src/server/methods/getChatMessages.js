function method(resp, params) {
	var context = params.session.context;
	var messages = [];
	var now = new Date().getTime();
	if (!context.lastPing)
		context.lastPing = 0;
	if (!context.messages)
		context.messages = [];
	if (params.newMess) {
		for ( var idx = 0; idx < context.messages.length; idx++) {
			var message = context.messages[idx];
			if ((message.date) && (message.date > context.lastPing))
				messages.push(message);
		}
	} else {
		messages = context.messages;
		params.newMess = false;
	}

	resp.push({
		"cmd" : "showMessages",
		"params" : {
			'uid' : params.session.identity.id,
			'title' : 'UserMessages',
			'messages' : messages,
			'newMess' : params.newMess
		}
	});

	var getChannelMessages = function(channelid) {
		if (!isNaN(channelid)) {
			if (channels[channelid]) {
				var channel = channels[channelid];
				var messages = [];
				if (params.newMess) {
					for ( var idx = 0; idx < channel.messages.length; idx++) {
						var message = channel.messages[idx];
						if ((message.date) && (message.date > context.lastPing))
							messages.push(message);
					}
					params.newMess = true;
				} else {
					messages = channel.messages;
					params.newMess = false;
				}
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
		} else {
			resp.push({
				"cmd" : "showMessages",
				"params" : {
					'cid' : channelid,
					'title' : 'Not Exist',
					'messages' : [],
					'newMess' : params.newMess
				}
			});
		}	
	};
	var channelid = parseInt(params.cid, 10);
	getChannelMessages(0);
	getChannelMessages(channelid);
	context.lastPing = now;
};