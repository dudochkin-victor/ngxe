function method(data) {
	var mentor = $('#mentor-window');
	var shop = $('#shop-window');
	var inventory = $('#inventory-window');
	if (mentor.is(':visible')) {
		mentor.hide();
		if (shop.is(':visible')) {
			if (inventory.is(':visible')) {
				if (inventory.css("right") > shop.css("right")) {
					shop.css("right", 0);
					inventory.css("right", shop.width() + 25);
				} else {
					inventory.css("right", 0);
					shop.css("right", inventory.width() + 25);
				}
			} else
				shop.css("right", 0);
		}
		if (inventory.is(':visible')) {
			if (shop.is(':visible')) {
				if (inventory.css("right") > shop.css("right")) {
					shop.css("right", 0);
					inventory.css("right", shop.width() + 25);
				} else {
					inventory.css("right", 0);
					shop.css("right", inventory.width() + 25);
				}
			} else
				inventory.css("right", 0);
		}

	} else {
		var pos = 0;
		if (inventory.is(':visible'))
			pos += inventory.width() + 25;
		if (shop.is(':visible'))
			pos += shop.width() + 25;

		mentor.show();
		mentor.css("right", pos);
	}
	return false;
};