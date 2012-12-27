function method(resp, params) {
	// Снимаем шмотку
	var context = params.session.context;
	var slot = params.slot;
	if (!context.body) context.body = {};
	if (!context.inventory) context.inventory = {};
	if (!context.personage.inventorysize)
		context.personage.inventorysize = 10;
	// Ищем свободную ячейку в инвентаре
	var idx = 0;
	for (idx = 0; idx < context.personage.inventorysize; idx++)
	{
		if (!context.inventory[idx])
			break;
	}
	if (!isNaN(slot) && (idx < context.personage.inventorysize) && (!context.inventory[idx]))
	{
		//Нашли
		context.inventory[idx] = context.body[slot];
		delete context.body[slot];
	} else
	{
		// Нет свободного места
	}
//	resp.push({
//		"cmd" : "showPersStat",
//		"params" : context.personage
//	});
};