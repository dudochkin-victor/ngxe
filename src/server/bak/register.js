function register(data) {
		resp.push({"cmd" : "register", "login" : $('#register_login').val(), "password" : $('#register_password').val()});
		jsonRPC({
			"method" : "ad.serve",
			"params" : resp,
			"id" : "id value (optional)"
		}, onData, onError);
		return false;
	};