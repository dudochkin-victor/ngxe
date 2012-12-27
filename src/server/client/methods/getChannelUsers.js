function method(data) {
	$('#attackUser').hide();
	if (!window.currentChannel)
		window.currentChannel = 0;
	$('#labelChallenge').removeClass('warning');
	$('#labelUsers').addClass('warning');
	$('#newCountUsers').hide();
	window.usersMode = 0;
	resp.push({"cmd" : "getChannelUsers", "cid" : currentChannel});
	jsonRPC({
		"method" : "ad.serve",
		"params" : resp,
		"id" : "id value (optional)"
	}, onData, onError);
	return false;
};