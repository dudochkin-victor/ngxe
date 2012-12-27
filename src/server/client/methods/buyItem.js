function method(data, e) {
	var iId = $(data).attr('id').split('-');
	if (iId.length == 2) {
		resp.push({
			"cmd" : "buyItem",
			"iId" : iId[1]
		});
		jsonRPC({
			"method" : "ad.serve",
			"params" : resp,
			"id" : "id value (optional)"
		}, onData, onError);
	}
	return false;
};