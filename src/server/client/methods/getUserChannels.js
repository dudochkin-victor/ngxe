function method(data) {
	$('#attackUser').hide();
	resp.push({"cmd" : "getUserChannels"});
	jsonRPC({
		"method" : "ad.serve",
		"params" : resp,
		"id" : "id value (optional)"
	}, onData, onError);
	return false;
};