function method(resp, params) {
	// Одеваем шмотку
	var context = params.session.context;
	var slot = parseInt(params.slot, 10); // не более чем размер инвентаря
	if (!context.body)
		context.body = {};
	if (!context.inventory)
		context.inventory = {};
	if (!context.personage.inventorysize)
		context.personage.inventorysize = 10;
	if (!isNaN(slot) && (slot < context.personage.inventorysize))
	{
		// Если в слоте инвентаря чтото есть
		if (context.inventory[slot]) {
			var thing = context.inventory[slot];
			if (thing.slot) {
				var slots = [].concat(thing.slot);
				slots.sort(function() {
					return 0.5 - Math.random();
				});
				var toslot = null;
				// Ищем свободный
				for ( var idx = 0; idx < slots.length; idx++) {
					toslot = slots[idx];
					if (!context.body[toslot]) {
						break;
					}
				}
				
				if (!context.body[toslot]) {
					// Просто ложим
					context.body[toslot] = thing;
					delete context.inventory[slot];
				} else {
					// Меняем местами
					context.inventory[slot] = context.body[toslot];
					context.body[toslot] = thing;
				}
			}
		}
	}
//	resp.push({
//		"cmd" : "showPersStat",
//		"params" : context.personage
//	});
};