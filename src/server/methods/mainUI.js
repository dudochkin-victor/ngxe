function method(resp, params) {
	components.getView(resp, {
		"view" : "avatar-window"
	});
	components.getView(resp, {
		"view" : "best-window"
	});
	components.getView(resp, {
		"view" : "chat-window"
	});
	components.getView(resp, {
		"view" : "duel-launcher"
	});
	components.getView(resp, {
		"view" : "news-window"
	});
	components.getView(resp, {
		"view" : "map-window"
	});
	components.getView(resp, {
		"view" : "mentor-launcher"
	});
	components.getView(resp, {
		"view" : "premium-window"
	});
	components.getView(resp, {
		"view" : "ratio-window"
	});
	components.getView(resp, {
		"view" : "shop-launcher"
	});
	components.getView(resp, {
		"view" : "mentor-window"
	});
	components.getView(resp, {
		"view" : "shop-window"
	});
	components.getView(resp, {
		"view" : "inventory-window"
	});
	components.getMethod(resp, {
		"method" : "togglePersonageTab"
	});
	components.getMethod(resp, {
		"method" : "toggleInventoryTab"
	});
	components.getMethod(resp, {
		"method" : "toggleMapTab"
	});
	components.getMethod(resp, {
		"method" : "toggleShopTab"
	});
	components.getMethod(resp, {
		"method" : "toggleQuestTab"
	});
	components.getMethod(resp, {
		"method" : "toggleClanTab"
	});
	components.getMethod(resp, {
		"method" : "toggleDuelTab"
	});
	components.getMethod(resp, {
		"method" : "togglePremiumTab"
	});
	components.getMethod(resp, {
		"method" : "getPersStat"
	});
	components.getMethod(resp, {
		"method" : "showPersStat"
	});
	components.getMethod(resp, {
		"method" : "getChannelMessages"
	});
	components.getMethod(resp, {
		"method" : "getChannelUsers"
	});
	components.getMethod(resp, {
		"method" : "getChallenge"
	});
	components.getMethod(resp, {
		"method" : "getMainMessages"
	});
	components.getMethod(resp, {
		"method" : "getUserChannels"
	});
	components.getMethod(resp, {
		"method" : "getUserMessages"
	});
	components.getMethod(resp, {
		"method" : "chatSend"
	});
	components.getMethod(resp, {
		"method" : "opponent"
	});
	components.getMethod(resp, {
		"method" : "showMessages"
	});
	components.getMethod(resp, {
		"method" : "showChannelUsers"
	});
	components.getMethod(resp, {
		"method" : "showChallenge"
	});
	components.getMethod(resp, {
		"method" : "challenge"
	});
	components.getMethod(resp, {
		"method" : "acceptDuel"
	});
	components.getMethod(resp, {
		"method" : "rejectDuel"
	});
	components.getMethod(resp, {
		"method" : "refreshShopItems"
	});
	components.getMethod(resp, {
		"method" : "refreshInventory"
	});
	components.getMethod(resp, {
		"method" : "refreshRatio"
	});
	components.getMethod(resp, {
		"method" : "refreshNews"
	});
	components.getMethod(resp, {
		"method" : "buyItem"
	});
	components.getMethod(resp, {
		"method" : "sellItem"
	});
	components.getMethod(resp, {
		"method" : "chatRefresh"
	});
	resp.push({
		"cmd" : "getUserMessages"
	});
	resp.push({
		"cmd" : "getChannelUsers"
	});
	resp.push({
		"cmd" : "chatRefresh"
	});
	
	resp.push({
		"cmd" : "hideWindow",
		"params" : "background"
	});
	
	resp.push({
		"cmd" : "showWindow",
		"params" : "canvas"
	});
	var channel = channels[0];
	var context = params.session.context;
	if (!channel.userlist[params.session.identity.id]) {
		channel.users++;
		channel.userlist[params.session.identity.id] = params.session.identity.login;
		if (!context.subscription)
			context.subscription = {};
		context.subscription[0] = channel.title;
	}
	resp.push({
		"cmd" : "showChannelUsers",
		"params" : {
			'cid' : 0,
			'title' : channel.title,
			'users' : channel.users,
			'userlist' : channel.userlist
		}
	});
	components.refreshNews(resp, params);
	components.refreshRatio(resp, params);
};