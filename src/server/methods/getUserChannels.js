function method(resp, params) {
	var context = params.session.context;
	resp.push({
		"cmd" : "userChannels",
		"params" : context.subscription
	});
};