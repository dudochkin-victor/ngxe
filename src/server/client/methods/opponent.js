function method(data) {
	var opp = $('#citems option:selected').text();
	$('#cmessage').val(opp + ' : ');
	if (window.uid != $('#citems').val()) {
		window.userToAttack = $('#citems').val();
		$('#attackUser').show();
		$('#userToAttack').text(opp);
		if (window.usersMode == 1) {
			$('#challengeDuel').hide();
			$('#acceptChallenge').show();
			$('#rejectChallenge').show();
		} else {
			$('#challengeDuel').show();
			$('#acceptChallenge').hide();
			$('#rejectChallenge').hide();
		}
	}
	return true;
};