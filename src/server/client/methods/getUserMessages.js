function method(data) {
	$('#labelMainChannel').removeClass('warning');
	$('#labelChannel').removeClass('warning');
	$('#labelMessages').addClass('warning');
	$('#newCountMessages').hide();
	window.chatMode = 2;
	var html = '';
	if (window.userMessages)
	{
		var messages = window.userMessages;
		for ( var idx in messages)
			html += '<li><strong>' + messages[idx].author + ':</strong> '
					+ messages[idx].message + '</li>';
		resp.push({"cmd" : "getChatMessages", "newMess" : true});
	}
	else
		resp.push({"cmd" : "getChatMessages"});
	$('#cmessages').html(html).prop({ scrollTop: $("#cmessages").prop("scrollHeight") });
	jsonRPC({
		"method" : "ad.serve",
		"params" : resp,
		"id" : "id value (optional)"
	}, onData, onError);
	return false;
};