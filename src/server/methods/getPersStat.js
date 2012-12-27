function method(resp, params) {
	var context = params.session.context;
	resp.push({
		"cmd" : "showPersStat",
		"params" : context.personage
	});
};