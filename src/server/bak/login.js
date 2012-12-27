function login(data) {
		resp.push({"cmd" : "login", "login" : $('#user_login').val(), "password" : $('#user_password').val()});
		jsonRPC({
			"method" : "ad.serve",
			"params" : resp,
			"id" : "id value (optional)"
		}, onData, onError);
		return false;
	};