function method(resp, params) {
	var context = params.session.context;
	context.personage.race = params.race;
	//context.personage.lvl = 1;
	components.mainUI(resp, params);
	resp.push({
		"cmd" : "stopSelectRace",
		"params" : ""
	});
	resp.push({
		"cmd" : "clearCanvas",
		"params" : ""
	});
	resp.push({
		"cmd" : "getPersStat",
		"params" : ""
	});
};