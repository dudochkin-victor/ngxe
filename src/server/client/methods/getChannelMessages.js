function method(data) {
	$('#labelMainChannel').removeClass('warning');
	$('#labelMessages').removeClass('warning');
	$('#labelChannel').addClass('warning');
	$('#newCountChannel').hide();
	window.chatMode = 1;
	if (!window.channels)
		window.channels = {};
	var html = '';
	if (window.channels[1])
	{
		var messages = window.channels[1];
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