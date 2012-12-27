function method(data) {
	resp.push({"cmd" : "acceptDuel", "uid" : window.userToAttack});
	jsonRPC({
		"method" : "ad.serve",
		"params" : resp,
		"id" : "id value (optional)"
	}, onData, onError);
	return false;
};