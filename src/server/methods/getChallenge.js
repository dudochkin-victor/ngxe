function method(resp, params) {
	var context = params.session.context;
	if (!context.challenge)
		context.challenge = {};
	resp.push({
		"cmd" : "showChallenge",
		"params" : {
			'challenge' : context.challenge
		}
	});
};