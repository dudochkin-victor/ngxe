function method(data) {
	var challenge = data.params.challenge;
	var html = '';
	var count = 0;
	for (var param in challenge)
	{
		count++;
		html += '<option value="'+param + '">' + challenge[param]  + '</option>';
	}
	if (count > 0)
		$('#newCountChallenge').show().text(count);
	else
		$('#newCountChallenge').hide();
	$('#citems').html(html);
	return false;
};