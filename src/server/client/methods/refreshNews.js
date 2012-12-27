function method(data) {
	//ui.getByName('avatar-window').style.display = 'block';
	var str = '';
	for (var param in data.params)
		str += '<div id=\"' + param+ '\" class=\"widecell rounded shadowed gradient\">'+ data.params[param].title + '<br/>' + data.params[param].body + '</div>';
	$('#newsItems').html(str);
	return false;
};