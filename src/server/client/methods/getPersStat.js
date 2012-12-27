function method(data) {
	resp.push({"cmd" : "getPersStat"});
	jsonRPC({
		"method" : "ad.serve",
		"params" : resp,
		"id" : "id value (optional)"
	}, onData, onError);
	return false;
};