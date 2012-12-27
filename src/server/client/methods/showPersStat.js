function method(data) {
	//ui.getByName('avatar-window').style.display = 'block';
	var stats = '';
	for (var param in data.params)
	{
		switch(param)
		{
		case "lvl":
			$('#LVL').text(data.params[param]);
			break;
		case "hp":
			$('#HP').css("background-size", data.params[param]+'% 100%');
			$('#HPIMG').attr("title", data.params[param]+'% of HP');
			break;
		case "sp":
			$('#SP').css("background-size", data.params[param]+'% 100%');
			$('#SPIMG').attr("title", data.params[param]+'% of SP');
			break;
		case "mp":
			$('#MP').css("background-size", data.params[param]+'% 100%');
			$('#MPIMG').attr("title", data.params[param]+'% of MP');
			break;

		default:
			break;
		}
	}
	return false;
};