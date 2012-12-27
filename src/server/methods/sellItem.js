function method(resp, params) {
	var context = params.session.context;
	var slot = parseInt(params.slot, 10); // не более чем размер инвентаря
	if (!context.inventory)
		context.inventory = {};
	if (!context.inventorysize)
		context.inventorysize = 10;
	if (!isNaN(slot) && ( slot <= context.inventorysize) && (context.inventory[slot]))
	{
		// TODO: calculate && change money
		delete context.inventory[slot];
		resp.push({
			"cmd" : "refreshInventory",
			"params" : context.inventory
		});
	} else {
		util.puts(util.inspect(context.inventory))
		// invalid params
	}
};