function showModal(data) {
	$('#modal-header h3:first').text(data.params.header);
	$('#modal-body').html(data.params.body);
	$('#modal').modal({
		'show' : true,
		'keyboard' : true,
		'backdrop' : true
	});
};