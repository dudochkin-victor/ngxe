function(resp, params) {
	var context = params.session.context;
	components.getMethod(resp, {"method": "alert"});
	if (context)
	{
		components.getMethod(resp, {"method": "clearCanvas"});
		components.getMethod(resp, {"method": "showModal"});
		components.getMethod(resp, {"method": "hideWindow"});
		components.getMethod(resp, {"method": "showWindow"});
		if ((!context.personage.race) || (context.personage.race == 0))
		{
			components.getMethod(resp, {"method": "selectRace"});
			components.getMethod(resp, {"method": "startSelectRace"});
			components.getMethod(resp, {"method": "stopSelectRace"});
		} else
		{
			components.mainUI(resp, params);
			resp.push({
				"cmd" : "clearCanvas",
				"params" : ""
			});
			resp.push({
				"cmd" : "getPersStat",
				"params" : ""
			});
			/*resp.push({
				"cmd" : "alert",
				"params" : "вы уже в игре"
			});*/
		}
	} else {
		components.getView(resp, {"view": "login-window"});
		components.getView(resp, {"view": "register-window"});
		components.getMethod(resp, {"method": "clearCanvas"});
		components.getMethod(resp, {"method": "carousel"});
		components.getMethod(resp, {"method": "showModal"});
		components.getMethod(resp, {"method": "hideWindow"});
		components.getMethod(resp, {"method": "showWindow"});
		components.getMethod(resp, {"method": "stopCarousel"});
		components.getMethod(resp, {"method": "startCarousel"});
//		resp.push({
//			"cmd" : "startCarousel",
//			"params" : ""
//		});
	}
};