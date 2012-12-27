function method(data) {
	$('#attackUser').hide();
	if (!window.challenge)
		window.challenge = {};
	$('#labelUsers').removeClass('warning');
	$('#labelChallenge').addClass('warning');
	$('#newCountChallenge').hide();
	window.usersMode = 1;
	resp.push({"cmd" : "getChallenge"});
	jsonRPC({
		"method" : "ad.serve",
		"params" : resp,
		"id" : "id value (optional)"
	}, onData, onError);
	return false;
};