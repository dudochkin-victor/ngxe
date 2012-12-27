function method(data) {
	// ui.getByName('inventory-window').style.display = 'block';
	var str = '';
	for ( var param in data.params) {
		var obj = {};
		if (data.params[param].body != "")
			try {
				obj = JSON.parse(data.params[param].body);
			} catch (e) {
				console.log(e);
			}
		str += '<div id=\"inventoryItem-'
				+ param
				+ '\" class=\"cell rounded shadowed gradient\" onclick=\"methods[\'sellItem\'](this);\" title=\"Продать \''
				+ data.params[param].title
				+ '\'?\">'
				+ ((obj.src != "") ? ('<img src=\"/img/artifacts/' + obj.src + '\">')
						: "") + '</div>';
	}
	$('#inventoryItems').html(str);
	return false;
};