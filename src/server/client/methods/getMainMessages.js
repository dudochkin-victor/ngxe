function method(data) {
	$('#labelMessages').removeClass('warning');
	$('#labelChannel').removeClass('warning');
	$('#labelMainChannel').addClass('warning');
	$('#newCountMainChannel').hide();
	window.chatMode = 0;
	var html = '';
	if (window.mainMessages)
	{
		var messages = window.mainMessages;
		for ( var idx in messages)
			html += '<li><strong>' + messages[idx].author + ':</strong> '
					+ messages[idx].message + '</li>';
		resp.push({"cmd" : "getChatMessages", "cid" : 0, "newMess" : true});
	}
	else
		resp.push({"cmd" : "getChatMessages", "cid" : 0});
	$('#cmessages').html(html).prop({ scrollTop: $("#cmessages").prop("scrollHeight") });
	
	jsonRPC({
		"method" : "ad.serve",
		"params" : resp,
		"id" : "id value (optional)"
	}, onData, onError);
	return false;
};