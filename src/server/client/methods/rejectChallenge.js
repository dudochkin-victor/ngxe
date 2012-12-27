function method(data) {
	resp.push({"cmd" : "rejectDuel", "uid" : window.userToAttack});
	jsonRPC({
		"method" : "ad.serve",
		"params" : resp,
		"id" : "id value (optional)"
	}, onData, onError);
	return false;
};