function method(resp, params) {
	var context = params.session.context;
	if (params.iId) {
		var iId = parseInt(params.iId, 10);
		if (!isNaN(iId)) {
			if (!context.inventory) context.inventory = {};
			if (!context.inventorysize)
				context.inventorysize = 10;
			// Ищем свободную ячейку в инвентаре
			var idx = 0;
			for (idx = 0; idx < context.inventorysize; idx++)
			{
				if (!context.inventory[idx])
					break;
			}
			if (context.inventorysize > idx)
			{
				var slot = idx;
				var item = {};
				for (idx = 0; idx<artifacts.length; idx++)
				{
					if (artifacts[idx].id == iId)
					{
						var artifact = artifacts[idx];
						for (var prop in artifact)
							item[prop] = artifact[prop];
					}
				}
				context.inventory[slot] = item;
				resp.push({
					"cmd" : "refreshInventory",
					"params" : context.inventory
				});
			} else
				resp.push({
					"cmd" : "alert",
					"params" : "No Empty Slot In Inventory"
				});
		}
	} else {
		// invalid params
	}
};