// 1 Мышечная масса - FP 
// 2 Здоровьe HP 
// 3 Ловкость DEX 
// 4 IQ IQ 
// 5 Шестое чувство SS

// атакующий
var player1 = {
	'name' : 'БАРСУК',
	'lvl' : 1,
	'pp' : 30,
	'hp' : 100,
	'dex' : 100,
	'iq' : 100,
	'ss' : 100
};

// защищающийся
var player2 = {
	'name' : 'БОБЕР',
	'lvl' : 1,
	'pp' : 30,
	'hp' : 100,
	'dex' : 100,
	'iq' : 100,
	'ss' : 100
};

var krit = {
	'mod' : 0
};
var thing = {
	'mod' : 0
};

var strike = 0;

document.write('<p>');
while ((player1.hp > 0) && (player2.hp > 0)) {
	if (strike % 2 == 0) {
		attacker = player1;
		shielder = player2;
	} else {
		attacker = player2;
		shielder = player1;
	}
	var attak_chance = 50
			* (((attacker.dex + attacker.ss / 2) / Math.pow(attacker.lvl, 2.5)) / (shielder.iq / Math
					.pow(shielder.lvl, 2.5))) + thing.mod;
	var evade_chance = 25
			* ((shielder.ss / Math.pow(shielder.lvl, 2.5)) / (attacker.dex / Math
					.pow(attacker.lvl, 2.5))) + thing.mod;
	if ((attak_chance > Math.random()*100) && (evade_chance < Math.random()*100)) {
		var crit_chance = 15
				* ((attacker.iq / Math.pow(attacker.lvl, 2.5)) / (shielder.dex / Math
						.pow(shielder.lvl, 2.5))) + thing.mod;
		var damage = 0;
		var crit = (crit_chance > Math.random()*100);
		if (crit)
			damage = (2 + krit.mod); // Crit damage
		else
			damage = Math.floor(attacker.pp * Math.pow(1.1, strike / 10));
		shielder.hp -= damage;
		document.write('<b>'+strike + ': ' + attacker.name
				+ ' STRIKE SUCCESS. ' + (crit?'CRIT ':' ')+'DAMAGE: ' + damage + ' '
				+ attak_chance + '/' + evade_chance  + '</b><br/>');
	} else
		document.write(strike + ': ' + attacker.name + ' STRIKE MISS. '
				+ attak_chance + '/' + evade_chance + '<br/>');
	strike++;
};
document.write('</p>');
if (player1.hp > 0)
	document.write('<h2>' + player1.name + ' WIN' + '</h2>');
else
	document.write('<h2>' + player2.name + ' WIN' + '</h2>');