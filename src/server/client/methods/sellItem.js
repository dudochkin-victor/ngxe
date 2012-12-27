function method(data, e) {
	var slot = $(data).attr('id').split('-');
	if (slot.length == 2) {
		resp.push({
			"cmd" : "sellItem",
			"slot" : slot[1]
		});
		jsonRPC({
			"method" : "ad.serve",
			"params" : resp,
			"id" : "id value (optional)"
		}, onData, onError);
	}
	return false;
};