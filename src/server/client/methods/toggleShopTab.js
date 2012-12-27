function method(data) {
	var personage = $('#personage-tab').hide();
	var inventory = $('#inventory-tab').hide();
	var map = $('#map-tab').hide();
	var shop = $('#shop-tab').show();
	var quest = $('#quest-tab').hide();
	var clan = $('#clan-tab').hide();
	var duel = $('#duel-tab').hide();
	var premium = $('#premium-tab').hide();
	
	var mentor = $('#mentor-window');
	
	resp.push({"cmd" : "refreshShopItems"});
	jsonRPC({
		"method" : "ad.serve",
		"params" : resp,
		"id" : "id value (optional)"
	}, onData, onError);

	return false;
};