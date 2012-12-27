function method(data) {
	var personage = $('#personage-tab').hide();
	var inventory = $('#inventory-tab').hide();
	var map = $('#map-tab').show();
	var shop = $('#shop-tab').hide();
	var quest = $('#quest-tab').hide();
	var clan = $('#clan-tab').hide();
	var duel = $('#duel-tab').hide();
	var premium = $('#premium-tab').hide();
	
	/*var mentor = $('#mentor-window');
	if (inventory.is(':visible')) {
		inventory.hide();
		if (shop.is(':visible')) {
			if (mentor.is(':visible')) {
				if (mentor.css("right") > shop.css("right")) {
					shop.css("right", 0);
					mentor.css("right", shop.width() + 25);
				} else {
					mentor.css("right", 0);
					shop.css("right", mentor.width() + 25);
				}
			} else
				shop.css("right", 0);
		}
		if (mentor.is(':visible')) {
			if (shop.is(':visible')) {
				if (mentor.css("right") > shop.css("right")) {
					shop.css("right", 0);
					mentor.css("right", shop.width() + 25);
				} else {
					mentor.css("right", 0);
					shop.css("right", mentor.width() + 25);
				}
			} else
				mentor.css("right", 0);
		}
	} else {
		var pos = 0;
		if (shop.is(':visible'))
			pos += shop.width() + 25;
		if (mentor.is(':visible'))
			pos += mentor.width() + 25;

		inventory.show();
		inventory.css("right", pos);
		resp.push({
			"cmd" : "refreshInventory"
		});
		jsonRPC({
			"method" : "ad.serve",
			"params" : resp,
			"id" : "id value (optional)"
		}, onData, onError);
	}*/
	return false;
};