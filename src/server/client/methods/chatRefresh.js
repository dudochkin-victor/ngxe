function method(data) {
	window.chatRefresh = setInterval(function (){
		if ((window.userMessages) && (window.mainMessages))
			resp.push({"cmd" : "getChatMessages", "newMess" : true});
		else
			resp.push({"cmd" : "getChatMessages"});
		jsonRPC({
			"method" : "ad.serve",
			"params" : resp,
			"id" : "id value (optional)"
		}, onData, onError);
	}, 6000);
	return true;
};