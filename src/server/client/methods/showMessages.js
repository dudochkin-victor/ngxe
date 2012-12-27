function method(data) {
	var html = '';
	var params = data.params;
	var messages = [];
	if (!window.chatMode)
		window.chatMode = 0;
	if (!window.userMessages)
		window.userMessages = [];
	if (!window.mainMessages)
		window.mainMessages = [];
	if (params.cid != undefined) {
		if (params.cid == 0) {
			if (params.messages.length > 0)
				$('#newCountMainChannel').show().text(params.messages.length);
			else if (window.chatMode == 0)
				$('#newCountMainChannel').hide();
			if (params.newMess)
				window.mainMessages = window.mainMessages
						.concat(params.messages);
			else
				window.mainMessages = params.messages;
			if (window.chatMode == 0)
			{
				messages = window.mainMessages;
				if (messages.length > 0) {
					for ( var idx in messages)
						html += '<li><strong>' + messages[idx].author
								+ ':</strong> ' + messages[idx].message + '</li>';
					
				}
				$('#cmessages').html(html).prop({ scrollTop: $("#cmessages").prop("scrollHeight") });
			}
		} else {
			if (params.messages.length > 0)
				$('#newCountChannel').show().text(params.messages.length);
			else if (window.chatMode == 1)
				$('#newCountChannel').hide();
			if (!window.channels)
				window.channels = {};
			if (!window.channels[params.cid])
				window.channels[params.cid] =[];
			if (params.newMess)
				window.channels[params.cid] = window.channels[params.cid]
						.concat(params.messages);
			else
				window.channels[params.cid] = params.messages;
			if (window.chatMode == 1)
			{
				messages = window.channels[params.cid];
				if (messages.length > 0) {
					for ( var idx in messages)
						html += '<li><strong>' + messages[idx].author
								+ ':</strong> ' + messages[idx].message + '</li>';
					
				}
				$('#cmessages').html(html).prop({ scrollTop: $("#cmessages").prop("scrollHeight") });
			}
		}
	} else if (params.uid) {
		if (params.messages.length > 0)
			$('#newCountMessages').show().text(params.messages.length);
		else if (window.chatMode == 2)
			$('#newCountMessages').hide();
		if (params.newMess)
			window.userMessages = window.userMessages.concat(params.messages);
		else
			window.userMessages = params.messages;
		window.uid = params.uid;
		if (window.chatMode == 2) {
			messages = window.userMessages;
			if (messages.length > 0) {
				for ( var idx in messages)
					html += '<li><strong>' + messages[idx].author
							+ ':</strong> ' + messages[idx].message + '</li>';
				
			}
			$('#cmessages').html(html).prop({ scrollTop: $("#cmessages").prop("scrollHeight") });
		}
	}
	return false;
};