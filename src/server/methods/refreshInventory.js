function method(resp, params) {
	var context = params.session.context;
	if (!context.inventory)
		context.inventory = {};
	resp.push({
		"cmd" : "refreshInventory",
		"params" : context.inventory
	});
};