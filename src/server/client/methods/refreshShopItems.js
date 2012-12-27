function method(data) {
	// ui.getByName('avatar-window').style.display = 'block';
	var str = '';
	for ( var param in data.params) {
		var obj = {};
		try {
			obj = JSON.parse(data.params[param].body);
		} catch (e) {
			console.log(e);
		}
		str += '<div id=\"shopItem-'
				+ data.params[param].id
				+ '\" class=\"cell rounded shadowed gradient\" onclick=\"methods[\'buyItem\'](this);\" title=\"Купить \''
				+ data.params[param].title
				+ '\'?\">'
				+ ((obj.src != "") ? ('<img src=\"/img/artifacts/' + obj.src + '\">')
						: "") + '</div>';
	}
	$('#shopItems').html(str);
	return false;
};