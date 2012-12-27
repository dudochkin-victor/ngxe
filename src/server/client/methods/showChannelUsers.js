function method(data) {
	var users = data.params.userlist;
	var html = '';
	var count = 0;
	for (var param in users)
	{
		count++;
		html += '<option value="'+param + '">' + users[param]  + '</option>';
	}
	$('#citems').html(html);
	if (count > 0)
		$('#newCountUsers').show().text(count);
	else
		$('#newCountUsers').hide();
	return false;
};