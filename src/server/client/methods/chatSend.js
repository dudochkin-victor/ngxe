function method(data, e) {
	if (e.keyCode == 13) {
		if (!window.currentChannel)
			window.currentChannel = 0; 
		var mess = $('#cmessage').val();
		var parts = mess.split(':');
		if ((parts.length>1))
			resp.push({"cmd" : "sendMessage", "user" : parts[0].replace(/^\s\s*/, '').replace(/\s\s*$/, ''), "message" : parts[1].replace(/^\s\s*/, '').replace(/\s\s*$/, ''), "newMess" : true});
		else
			resp.push({"cmd" : "sendMessage", "cid" : currentChannel, "message" : mess, "newMess" : true});
		
		$('#cmessage').val('');
		jsonRPC({
			"method" : "ad.serve",
			"params" : resp,
			"id" : "id value (optional)"
		}, onData, onError);
		return false;
	}
	return true;
};