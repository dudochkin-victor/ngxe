function handle_request() {
	for (/* k=> */r in request) {
		params = r['params'];
		switch (r['cmd']) {
		case 'GATHER': {
			if (context[0]['turncount'] > 0) {
				// собрать
				var data = null;
				for (idx in context) {
					data = content[idx];
					if (data['type'] == 8)
						break;
				}
				// установлено время созревания и время сбора
				// время созревания прошло врямя сбора еще нет
				if ((data['type'] == 8) && (data['ripeningtime'] != 0)
						&& (data['gathertime'] != 0)
						&& (data['ripeningtime'] <= time())
						&& (data['gathertime'] >= time())) {
					if (profession == 3)
						// купцу 50% с поля
						context[0]['coins'] += parseInt(data['valuecrops'], 10) * 1.5;
					else
						context[0]['coins'] += data['valuecrops'];
					data['ripeningtime'] = 0;
					data['gathertime'] = 0;
					update_my_info = true;
					context[0]['turncount']--;
				}
			} else {
				context[0]['turncount'] = 0;
				response.push({
					'cmd' : "HIDE_FARM",
					'params' : null
				});
				response.push({
					'cmd' : "NOTURN",
					'params' : null
				});

			}
			break;
		}

		case 'SOW': {
			// засеять
			var data = null;
			for (idx in context) {
				data = content[idx];
				if (data['type'] == 8)
					break;
			}
			if (data['type'] == 8) {
				// если всё собрано
				// время созревания прошло и время сбора тоже
				if (((data['ripeningtime'] == 0) && (data['gathertime'] == 0))
						|| (data['ripeningtime'] <= time())
						&& (data['gathertime'] <= time())) {
					data['ripeningtime'] = time() + 120 * 60;
					data['gathertime'] = time() + 180 * 60;
					update_my_info = true;
				}
			}
			break;
		}
			// Использовать
		case 'USE': {
			Type = parseInt(params['id'], 10);
			if (Type > 21) {
				item = null;
				for (idx in context) {
					var data = content[idx];
					if (data['type'] == Type)
						item = data;
				}

				if ((item) && (item['count'] > 0)) {
					switch (Type) {
					case 22: // Удобрение
					{
						var data = null;
						for (idx in context) {
							data = content[idx];
							if (data['type'] == 8)
								break;
						}
						// установлено время созревания и
						// время сбора
						// время созревания не подошло и
						// время сбора
						if ((data['type'] == 8) && (data['ripeningtime'] != 0)
								&& (data['gathertime'] != 0)
								&& (data['ripeningtime'] >= time())
								&& (data['gathertime'] >= time())) {
							data['ripeningtime'] = time();
							// с текущего момента можно
							// собирать урожай
							data['gathertime'] = time() + (60 * 60);
							update_my_info = true;
							item['count']--;
							response.push({
								'cmd' : "SHOW_FARM",
								'params' : null
							});
						} else
							response.push({
								'cmd' : "SHOW_FARM",
								'params' : null
							});
						break;
					}
					case 23: // Мухорморы
					{
						if (parseInt(context[0]['mushrooms'], 10) == 0) {
							// В течение часа увеличивают
							// атаку солдат на 50%.
							context[0]['mushrooms'] = time() + 3600;
							update_my_info = true;
							item['count']--;
							response
									.push({
										'cmd' : "INFO",
										'params' : {
											'message' : 'Вы использовали Мухоморы. В течение часа атака солдат увеличена на 50%',
											'x' : 158,
											'y' : 250
										}
									});
						} else
							response.push({
								'cmd' : "INFO",
								'params' : {
									'message' : 'Вы уже используете Мухоморы.',
									'x' : 158,
									'y' : 250
								}
							});
						break;
					}
					case 24: // Молот тора
					{
						if (parseInt(context[0]['torhammer'], 10) == 0) {
							// В течение 3 часов увеличивает
							// защиту солдат на 50%
							context[0]['torhammer'] = time() + 3 * 3600;
							update_my_info = true;
							item['count']--;
							response
									.push({
										'cmd' : "INFO",
										'params' : {
											'message' : 'Вы использовали Молот Тора. В течение 3 часов защита солдат увеличена на 50%.',
											'x' : 158,
											'y' : 250
										}
									});
						} else
							response
									.push({
										'cmd' : "INFO",
										'params' : {
											'message' : 'Вы уже используете Молот Тора.',
											'x' : 158,
											'y' : 250
										}
									});
						break;
					}
					case 25: // Руна ловкости
					{
						if (parseInt(context[0]['runa'], 10) == 0) {
							// В течение 72 часов повышает
							// шанс удачного воровства у
							// друга до 80%.
							context[0]['runa'] = time() + 72 * 3600;
							update_my_info = true;
							item['count']--;
							response
									.push({
										'cmd' : "INFO",
										'params' : {
											'message' : 'Вы использовали Руну Ловкости. В течение 72 часов шанс удачного воровства у друга увеличен до 80%.',
											'x' : 158,
											'y' : 250
										}
									});
						} else
							response
									.push({
										'cmd' : "INFO",
										'params' : {
											'message' : 'Вы уже используете Руну Ловкости',
											'x' : 158,
											'y' : 250
										}
									});
						break;
					}
					case 26: // Магические силки
					{
						if (parseInt(context[0]['silki'], 10) == 0) {
							// В течение 1 часа при удачных
							// нападениях позволяет брать в
							// плен 10% солдат противника.
							context[0]['silki'] = time() + 3600;
							update_my_info = true;
							item['count']--;
							response
									.push({
										'cmd' : "INFO",
										'params' : {
											'message' : 'Вы использовали Магические Силки. В течение 1 часа при удачных нападениях Вы будете брать в плен 10% солдат противника.',
											'x' : 158,
											'y' : 250
										}
									});
						} else
							response
									.push({
										'cmd' : "INFO",
										'params' : {
											'message' : 'Вы уже используете Магические Силки.',
											'x' : 158,
											'y' : 250
										}
									});
						break;
					}
						// ////////////////
					}
					response.push({
						'cmd' : "HIDE_SHAMAN",
						'params' : null
					});
				}
			}
			break;
		}
		case 'GET_INFO': {
			update_my_info = true;
			break;
		}
		case 'ANSWER': {
			redis.hset("answers:uid:".uid, params['key'],
					parseInt(params['value'], 10));
			break;
		}
		case 'PROFESSION': {
			context[0]['profession'] = parseInt(params['value'], 10);
			update_my_info = true;
			break;
		}
		case 'BUY_ITEMS': {
			type = parseInt(params['id'], 10);
			if (type > 8) {
				item = null;
				for (idx in context) {
					var data = content[idx];
					if (data['type'] == type)
						item = data;
				}

				if (item === null) {
					artefact_id = redis.incr("context:uniq");
					// если нету постройки с этим идентификатором
					// контекста
					if (!context[artefact_id])
						context[artefact_id] = [];

					if (!types)
						types = json_decode(redis.get("context:types"), true); // сэкономили

					context[artefact_id]['level'] = 0;
					context[artefact_id]['count'] = 0;
					context[artefact_id]['type'] = type;
					context[artefact_id]['gid'] = types[type]['gid'];
					if (types[type]['init_params']) {
						for ( /* param => */value in types[type]['init_params'])
							context[artefact_id][param] = value;
					}
					item = context[artefact_id];
				}
				if (!types)
					types = json_decode(redis.get("context:types"), true); // DEBUG
				item['gid'] = types[type]['gid']; // DEBUG

				// итак у нас есть контекст (item) ищем стоимость
				// этого контекста
				artifacts = json_decode(redis.get("artifacts"), true);
				// найходим артефакт данного типа смотрим стоимость,
				// снимем деньги если их достаточно,
				// либо выводим сообщение о недостаточности финансов
				// если недостаточно голосов то выводим окно
				// пополнения баланса
				// также смотрим сколько у чела сейчас голосов в
				// приложении и забираем их
				// засовывая их в характеристики перса в voices
				artifact = null;
				for ( var i = 0; i < artifacts.length; i++)
					if (artifacts[i]['id'] == item['type'])
						artifact = artifacts[i];
				if (artifact) // нашли данные по артефакту
				{
					order_summ = floatval(artifact['buycost'])
							* parseInt(params['count'], 10);
					if (artifact['unit'] == 0) // монеты
					{
						// TODO: Проверить голоса пользователя в
						// приложении
						if (context[0]['coins'] >= order_summ) {
							context[0]['coins'] -= order_summ;
							item['count'] += params['count'];
							update_my_info = true;
						} else {
							// не хватило монет
							response
									.push({
										'cmd' : "INFO",
										'params' : {
											'message' : 'Недостаточно золота для покупки.',
											'x' : 158,
											'y' : 250
										}
									});
						}
					} else if (artifact['unit'] == 1) // голоса
					{
						// TODO: Проверить голоса пользователя в
						// приложении
						if (context[0]['coins'] >= order_summ) // DEBUG
						// RELEASE if (context[0]['voices']>=
						// order_summ)
						{
							context[0]['voices'] -= order_summ;
							item['count'] += params['count'];
							update_my_info = true;
						} else {
							// не хватило голосов
							response
									.push({
										'cmd' : "INFO",
										'params' : {
											'message' : 'Недостаточно голосов для покупки. Если вы пополнили счет, то повторите попытку покупки.',
											'x' : 158,
											'y' : 250
										}
									});
							response.push({
								'cmd' : "ADD_MONEY",
								'params' : ceil(order_summ
										- context[0]['voices'])
							});
						}
					}
				}
			}
			break;
		}
		case 'BUYTURN': {
			if (context[0]['turncount'] < 50) {
				if (context[0]['turncount'] + parseInt(params['count'], 10) > 50)
					params['count'] = 50 - context[0]['turncount'];

				if (parseInt(params['count'], 10) < 11)
					order_summ = 0.2 * parseInt(params['count'], 10);
				else if (parseInt(params['count'], 10) < 31)
					order_summ = 0.17 * parseInt(params['count'], 10);
				else
					order_summ = 0.15 * parseInt(params['count'], 10);
				if (context[0]['coins'] >= order_summ) // DEBUG
				// RELEASE if (context[0]['voices']>= order_summ)
				{
					context[0]['voices'] -= order_summ;
					context[0]['turncount'] += parseInt(params['count'], 10);

					update_my_info = true;
					/*
					 * array_push (response, array( 'cmd' : "INFO", 'params' :
					 * array('message' :'ходов:'.parseInt(params['count'], 10),
					 * 'x':158, 'y':250) ) );
					 */
				} else {
					// не хватило голосов
					response
							.push({
								'cmd' : "INFO",
								'params' : {
									'message' : 'Недостаточно голосов для покупки ходов. Если вы пополнили счет, то повторите попытку покупки.',
									'x' : 158,
									'y' : 250
								}
							});
					response.push({
						'cmd' : "ADD_MONEY",
						'params' : ceil(order_summ - context[0]['voices'])
					});
				}
			} else
				response.push({
					'cmd' : "INFO",
					'params' : {
						'message' : 'Не более 50 ходов.',
						'x' : 158,
						'y' : 250
					}
				});
		}

		case 'SELL_ITEMS': {
			type = parseInt(params['id'], 10);
			if (type > 8) {
				item = null;
				for (idx in context) {
					var data = content[idx];
					if (data['type'] == type)
						item = data;
				}
				if (item === null) {
					artefact_id = redis.incr("context:uniq");
					// если нету постройки с этим идентификатором
					// контекста
					if (!context[artefact_id])
						context[artefact_id] = [];

					if (!types)
						types = json_decode(redis.get("context:types"), true); // сэкономили

					context[artefact_id]['level'] = 0;
					context[artefact_id]['count'] = 0;
					context[artefact_id]['type'] = type;
					context[artefact_id]['gid'] = types[type]['gid'];
					item = context[artefact_id];
				}
				// итак у нас есть контекст (item) ищем стоимость
				// этого контекста
				artifacts = json_decode(redis.get("artifacts"), true);
				// TODO: найти артефакт данного типа посмотреть
				// стоимость вернуть .75 % если это не в голосах
				artifact = null;
				for ( var i = 0; i < artifacts.length; i++)
					if (artifacts[i]['id'] == item['type'])
						artifact = artifacts[i];
				if (artifact) // нашли данные по артефакту
				{
					if ((artifact['unit'] == 0) && (item['count'] > 0)
							&& (item['count'] >= params['count'])) // монеты
					{
						order_summ = floatval(artifact['buycost'])
								* parseInt(params['count'], 10);
						context[0]['coins'] += floor(order_summ * 0.75);
						item['count'] -= params['count'];
						update_my_info = true;
					}
				}
			}
			break;
		}
		case 'CALLHELP': {
			friend_id = parseInt(params['id'], 10);
			if (parseInt(context[0]['friendhelptime'], 10) == 0) {
				if (uid != friend_id) // Не себя
				{
					query = "SELECT friends FROM users WHERE user_id=".uid;
					db.query(query);
					if (db.next_record())
						friends = db.f('friends');

					query = "SELECT * FROM users WHERE user_id=" + friend_id
							+ " and user_id in (" + friends + ")";
					db.query(query);
					friend_name = '';
					friend_nickname = '';
					friend_img = '';
					friend_sex = 0;
					if (db.next_record()) {
						friend_name = db.f('name');
						friend_nickname = db.f('nickname');
						friend_img = db.f('img');
						friend_sex = parseInt(db.f('sex'), 10);
						friend = json_decode(redis.get("uid:".friend_id), true);
						// дебажная тема потом убрать {
						if (friend === NULL) {
							// ессли данных по пользователю нет, то
							// генерируем
							// начальное состояние пользователя
							friend = [];
							friend[0]['gid'] = 0;
							friend[0]['voices'] = 0;

							fake = [];
							triggers(db, friend_id, friend);
							steps(db, friend_id, friend, fake);
						}
						// дебажная тема потом убрать }
						friend_shield = shield(friend);
						if (!context[0]['lose'])
							context[0]['lose'] = 0;
						if (!context[0]['wins'])
							context[0]['wins'] = 0;
						if (!friend[0]['lose'])
							friend[0]['lose'] = 0;
						if (!friend[0]['wins'])
							friend[0]['wins'] = 0;

						friend_messages = json_decode(redis
								.get("messages:".friend_id), true);
						if (friend_messages === NULL)
							friend_messages = [];
						// Враг проиграл
						friend_messages
								.push({
									'type' : 1,
									'message' : 'Ваш друг '
											+ context[0]['name']
											+ ' призвал ваших солдат на защиту своего поселения',
									'is_new' : 1,
								});
						redis.set("messages:".friend_id,
								json_encode(friend_messages));
						response
								.push({
									'cmd' : "INFO",
									'params' : {
										'message' : '<br>Половина армии вашего друга '
												+ friend_name
												+ ' на 1 час вызвалась помочь в обороне вашего поселения!<br><br><font size="14" color="#4ABFE0">Ваш друг не теряет армию.</font>',
										'x' : 158,
										'y' : 137,
										'type' : 2
									}
								});
						population = get_population(friend);
						context[0]['friendhelp'] = parseInt(population / 2);
						context[0]['friendhelptime'] = time() + 3600;
						update_my_info = true;
					}
				} else // себя
				{
					response
							.push({
								'cmd' : "INFO",
								'params' : {
									'message' : 'Сам себе не поможешь, никто не поможет.',
									'x' : 158,
									'y' : 250
								}
							});
				}
			} else {
				response.push({
					'cmd' : "INFO",
					'params' : {
						'message' : 'Вы уже воспользовались помошью друга.'
								+ context[0]['friendhelp'],
						'x' : 158,
						'y' : 250
					}
				});
			}
			break;
		}
		case 'THIEVE': {
			enemy_id = parseInt(params['id'], 10);

			if (uid != enemy_id) // Не себя
			{
				if (context[0]['thievetime'] + 180 * 60 < time()) {
					query = "SELECT friends FROM users WHERE user_id=".uid;
					db.query(query);
					if (db.next_record())
						friends = db.f('friends');

					query = "SELECT * FROM users WHERE user_id=" + enemy_id
							+ " and user_id in (" + friends + ")";
					db.query(query);
					enemy_name = '';
					enemy_nickname = '';
					enemy_img = '';
					enemy_sex = 0;
					if (db.next_record()) {
						enemy_name = db.f('name');
						enemy_nickname = db.f('nickname');
						enemy_img = db.f('img');
						enemy_sex = parseInt(db.f('sex'), 10);

						thieve = rand(0, 100);

						enemy = json_decode(redis.get("uid:".enemy_id), true);
						// дебажная тема потом убрать {
						if (enemy === NULL) {
							// ессли данных по пользователю нет, то
							// генерируем
							// начальное состояние пользователя
							enemy = [];
							enemy[0]['gid'] = 0;
							enemy[0]['voices'] = 0;

							fake = [];
							triggers(db, enemy_id, enemy);
							steps(db, enemy_id, enemy, fake);
						}
						// дебажная тема потом убрать }
						if (!context[0]['lose'])
							context[0]['lose'] = 0;
						if (!context[0]['wins'])
							context[0]['wins'] = 0;
						if (!enemy[0]['lose'])
							enemy[0]['lose'] = 0;
						if (!enemy[0]['wins'])
							enemy[0]['wins'] = 0;

						if (thieve >= 77) // Успешная атака
						{
							enemy_lose_coins = enemy[0]['coins'] * 0.2;
							if (enemy_lose_coins > 0) {
								context[0]['coins'] += parseInt(enemy_lose_coins, 10);
								enemy[0]['coins'] -= parseInt(enemy_lose_coins, 10);
								enemy_messages = json_decode(redis
										.get("messages:".enemy_id), true);
								if (enemy_messages === NULL)
									enemy_messages = [];
								enemy_messages.push({
									// Враг проиграл
									'type' : 1,
									'message' : "Ваш друг "
											+ context[0]['name']
											+ " ограбил вас на "
											+ parseInt(enemy_lose_coins, 10)
											+ " монет!",
									'is_new' : 1
								});
								redis.set("messages:".enemy_id,
										json_encode(enemy_messages));
								response
										.push({
											'cmd' : "INFO",
											'params' : {
												'message' : 'В ходе грабежа вам удалось унести с собой '
														+ parseInt(enemy_lose_coins, 10)
														+ ' монет.',
												'x' : 158,
												'y' : 137,
												'type' : 4
											}
										});
							} else
								response
										.push({
											'cmd' : "INFO",
											'params' : {
												'message' : 'Охрана деревни была слишком внимательной, вам не удалось ничего украсть.',
												'x' : 158,
												'y' : 137,
												'type' : 4
											}
										});
							// TODO: Раздать бонусы, отправить
							// событие в игру и в уведомления
							// приложений
						} else { // Неуспешная попытка
							/*
							 * enemy_messages =
							 * json_decode(redis.get("messages:".enemy_id),
							 * true); array_push (enemy_messages, array( 'type' :
							 * 0, // Враг выиграл 'message' : 'На вас напал
							 * '.context[0]['name'].'. Поздравляю с победой
							 * тебя, o великий вождь, племени викингов.',
							 * 'is_new' => 1, ) );
							 * redis.set("messages:".enemy_id,
							 * json_encode(enemy_messages));
							 */
							response
									.push({
										'cmd' : "INFO",
										'params' : {
											'message' : 'Охрана деревни была слишком внимательной, вам не удалось ничего украсть.',
											'x' : 158,
											'y' : 137,
											'type' : 4
										}
									});
							// TODO: Раздать бонусы, отправить
							// событие в игру и в уведомления
							// приложений
						}
						// Делаем пересчет для себя и сохраняем
						// рейтинг
						query = "UPDATE users SET coins="
								+ parseInt(context[0]['coins'], 10)
								+ ", date_lastvisit=NOW() where user_id=" + uid;
						db.query(query);
						// Делаем пересчет для врага и сохраняем
						// рейтинг
						query = "UPDATE users SET coins="
								+ parseInt(enemy[0]['coins'], 10)
								+ " where user_id=" + enemy_id;
						db.query(query);
						// Сохраняем контекст врага
						redis.set("uid:".enemy_id, json_encode(enemy));
						update_my_info = true;
						context[0]['thievetime'] = time();
					}
				} else
					response
							.push({
								'cmd' : "INFO",
								'params' : {
									'message' : "Грабить друзей можно только раз в 24 часа!",
									'x' : 158,
									'y' : 137
								}
							});
			} else // себя
			{
				response
						.push({
							'cmd' : "INFO",
							'params' : {
								'message' : 'Сам себя обокрал и в ломбард неси, в ломбард:). О как наварился:)',
								'x' : 158,
								'y' : 250
							}
						});
			}
			break;
		}
		case 'ATTACK': {
			enemy_id = parseInt(params['id'], 10);

			if (uid != enemy_id) // Не себя
			{
				if (context[0]['turncount'] > 0) {
					query = "SELECT * FROM users WHERE user_id=".enemy_id;
					db.query(query);
					enemy_name = '';
					enemy_nickname = '';
					enemy_img = '';
					enemy_sex = 0;
					if (db.next_record()) {
						enemy_name = db.f('name');
						enemy_nickname = db.f('nickname');
						enemy_img = db.f('img');
						enemy_sex = parseInt(db.f('sex'), 10);
					}

					query = "SELECT * FROM users WHERE user_id=".uid;
					db.query(query);
					my_nickname = '';
					my_img = '';
					my_sex = 0;
					if (db.next_record()) {
						my_nickname = db.f('nickname');
						my_img = db.f('img');
						my_sex = parseInt(db.f('sex'), 10);
					}

					attack = attack(context);
					enemy = json_decode(redis.get("uid:".enemy_id), true);
					// дебажная тема потом убрать {
					if (enemy === NULL) {
						// ессли данных по пользователю нет, то
						// генерируем
						// начальное состояние пользователя
						enemy = [];
						enemy[0]['gid'] = 0;
						enemy[0]['voices'] = 0;

						fake = [];
						triggers(db, enemy_id, enemy);
						steps(db, enemy_id, enemy, fake);
					}
					// дебажная тема потом убрать }
					enemy_shield = shield(enemy);
					if (!context[0]['lose'])
						context[0]['lose'] = 0;
					if (!context[0]['wins'])
						context[0]['wins'] = 0;
					if (!enemy[0]['lose'])
						enemy[0]['lose'] = 0;
					if (!enemy[0]['wins'])
						enemy[0]['wins'] = 0;

					elder = null;
					for (idx in context) {
						if (context[idx]['type'] == 1)
							elder = context[idx];
					}

					enemyelder = null;
					for (enemydata in enemy)
						if (enemydata['type'] == 1)
							enemyelder = enemydata;

					enemychest = null;
					for (enemydata1 in enemy)
						if (enemydata1['type'] == 6)
							enemychest = enemydata1;

					if (attack >= enemy_shield) // Успешная атака
					{
						// потери могут быть не более 30% у
						// выигравшего от потерь проигравшего
						// потери могут быть не менее 10% и не более
						// 40 % у проигравшего

						enemy_lose = parseInt(rand(0, enemyelder['population'] * 0.3)
								+ enemyelder['population'] * 0.1, 10);
						enemyelder['population'] -= enemy_lose;

						user_lose = parseInt(rand(0, enemy_lose * 0.3), 10);
						elder['population'] -= user_lose;

						context[0]['wins']++;
						enemy[0]['lose']++;

						enemy_lose_coins = enemy[0]['coins']
								* (100 - (enemychest['level'] * 10)) / 100;
						context[0]['coins'] += parseInt(enemy_lose_coins, 10);
						enemy[0]['coins'] -= parseInt(enemy_lose_coins, 10);

						enemy_messages = json_decode(redis
								.get("messages:".enemy_id), true);
						if (enemy_messages === NULL)
							enemy_messages = [];
						enemy_messages
								.push({
									// Враг проиграл
									'type' : 1,
									'message' : 'На вас напал '
											+ context[0]['name']
											+ '. С прискорбием, сообщаю тебе о поражении в битве, вождь. Силы были не равны.',
									'is_new' : 1,
									'report' : {
										'message' : 'С прискорбием, сообщаю тебе о поражении в битве, вождь. Силы были не равны.',
										'x' : 158,
										'y' : 137,
										'enemy_name' : context[0]['name'],
										'enemy_nickname' : my_nickname,
										'enemy_img' : my_img,
										'enemy_sex' : my_sex,
										'user_attack' : parseInt(enemy_shield, 10),
										'enemy_shield' : parseInt(attack, 10),
										'user_lose' : enemy_lose,
										'enemy_lose' : user_lose,
										'lose_coins' : parseInt(enemy_lose_coins, 10)
									}
								});
						redis.set("messages:".enemy_id,
								json_encode(enemy_messages));
						response
								.push({
									'cmd' : "RESULT",
									'params' : {
										'message' : 'Поздравляю с победой тебя, o великий вождь, племени викингов.',
										'x' : 158,
										'y' : 137,
										'enemy_name' : enemy_name,
										'enemy_nickname' : enemy_nickname,
										'enemy_img' : enemy_img,
										'enemy_sex' : enemy_sex,
										'user_attack' : parseInt(attack, 10),
										'enemy_shield' : parseInt(enemy_shield, 10),
										'user_lose' : user_lose,
										'enemy_lose' : enemy_lose,
										'win_coins' : parseInt(enemy_lose_coins, 10)
									}
								});
						// TODO: Раздать бонусы, отправить событие в
						// игру и в уведомления приложений
					} else { // Неуспешная атака
						context[0]['lose']++;
						enemy[0]['wins']++;
						// потери могут быть не более 30% у
						// выигравшего от потерь проигравшего
						// потери могут быть не менее 10% и не более
						// 40 % у проигравшего
						user_lose = parseInt(rand(0, elder['population'] * 0.3, 10)
								+ elder['population'] * 0.1, 10);
						elder['population'] -= user_lose;

						enemy_lose = parseInt(rand(0, enemy_lose * 0.3), 10);
						enemyelder['population'] -= enemy_lose;

						enemy_messages = json_decode(redis
								.get("messages:".enemy_id), true);
						if (enemy_messages === NULL)
							enemy_messages = [];
						enemy_messages
								.push({
									'type' : 0, // Враг выиграл
									'message' : 'На вас напал '
											+ context[0]['name']
											+ '. Поздравляю с победой тебя, o великий вождь, племени викингов.',
									'is_new' : 1,
								});
						redis.set("messages:".enemy_id,
								json_encode(enemy_messages));
						response
								.push({
									'cmd' : "RESULT",
									'params' : {
										'message' : 'С прискорбием, сообщаю тебе о поражении в битве, вождь. Силы были не равны.',
										'x' : 158,
										'y' : 137,
										'enemy_name' : enemy_name,
										'enemy_nickname' : enemy_nickname,
										'enemy_img' : enemy_img,
										'enemy_sex' : enemy_sex,
										'user_attack' : parseInt(attack, 10),
										'enemy_shield' : parseInt(enemy_shield, 10),
										'user_lose' : user_lose,
										'enemy_lose' : enemy_lose,
										'lose_coins' : 0
									}
								});
						// TODO: Раздать бонусы, отправить событие в
						// игру и в уведомления приложений
					}
					// Делаем пересчет для себя и сохраняем рейтинг
					attack = attack(context);
					shield = shield(context);
					population = get_population(context);
					query = "UPDATE users SET coins="
							+ parseInt(context[0]['coins'], 10) + ", population="
							+ population + ", attack=" + attack + ", shield="
							+ shield + ", wins=" + context[0]['wins']
							+ ", lose=" + context[0]['lose'] + ", power="
							+ (attack + shield + population)
							+ ", date_lastvisit=NOW() where user_id=" + uid;
					db.query(query);
					// Делаем пересчет для врага и сохраняем рейтинг
					enemy_attack = attack(enemy);
					enemy_shield = shield(enemy);
					enemy_population = get_population(enemy);
					query = "UPDATE users SET coins="
							+ parseInt(enemy[0]['coins'], 10) + ", population="
							+ enemy_population + ", attack=" + enemy_attack
							+ ", shield=" + enemy_shield + ", wins="
							+ enemy[0]['wins'] + ", lose=" + enemy[0]['lose']
							+ ", power="
							+ (enemy_attack + enemy_shield + enemy_population)
							+ " where user_id=" + enemy_id;
					db.query(query);
					// Сохраняем контекст врага
					redis.set("uid:".enemy_id, json_encode(enemy));
					context[0]['turncount']--;
					update_my_info = true;
				} else {
					context[0]['turncount'] = 0;
					response.push({
						'cmd' : "NOTURN",
						'params' : null
					});
				}
			} else // себя
			{
				response
						.push({
							'cmd' : "INFO",
							'params' : {
								'message' : 'Вождь съел грибов? Суицид в этой игре не приветствуется :).',
								'x' : 158,
								'y' : 250
							}
						});
			}
			break;
		}
		case 'UPGRADE': {
			if (context[params['id']]) {
				data = context[params['id']];
				user = context[0];
				if (data['upgradecurrency'] === NULL)
					data['upgradecurrency'] = 0;
				if (data['upgradecost'] === NULL)
					data['upgradecost'] = 0;

				if (data['upgradecurrency'] == 0) // за монеты
				{
					// проверить не находится ли в апгрэйде
					// проверить хватает ли денег
					if (((!data['upgradetime']) || (data['upgradetime'] == 0))
							&& (user['coins'] >= data['upgradecost'])) {
						// проверяем не будет ли следующий апгрэйд
						// больше допустимого
						query = "SELECT * FROM upgrade_costs WHERE object_type="
								+ data['type']
								+ " AND "
								+ " level="
								+ (data['level'] + 1) + " LIMIT 1";
						db.query(query);
						if (db.next_record()) {
							// выставляем время начала апгрэйда
							if (data['upgradetime'] == 0)
								data['upgradetime'] = (time()/* +(180*60) */);
							// снимаем деньги с пользователя
							user['coins'] -= data['upgradecost'];

							if (parseInt(db.f('currency'), 10) == 0) {
								// за монеты
								data['upgradecost'] = parseInt(db.f('cost'), 10);
								data['upgradecurrency'] = 0;

							} else {
								// за голоса
								data['upgradecost'] = parseInt(db.f('cost'), 10);
								data['upgradecurrency'] = 1;
							}
							update_my_info = true;
						}
					} else { // либо не хватает денег либо уже в апгрэйде
						response.push({
							'cmd' : "INFO",
							'params' : {
								'message' : 'Не хватает золота',
								'x' : 158,
								'y' : 250
							}
						});
					}
				} else // за голоса
				{
					// проверить не находится ли в апгрэйде
					// проверить хватает ли денег
					if (((!data['upgradetime']) || (data['upgradetime'] == 0))
							&& (user['coins'] >= data['upgradecost'])) {
						// проверяем не будет ли следующий апгрэйд
						// больше допустимого
						query = "SELECT * FROM upgrade_costs WHERE object_type="
								+ data['type']
								+ " AND "
								+ " level="
								+ (data['level'] + 1) + " LIMIT 1";
						db.query(query);
						if (db.next_record()) {
							// выставляем время начала апгрэйда
							if (data['upgradetime'] == 0)
								data['upgradetime'] = (time() + (180 * 60));
							// TODO: нужно снимать голоса и
							// проверять их наличие
							// если их не достаточно то нужно
							// пульнуть сообщение пользователю

							// снимаем деньги с пользователя
							user['coins'] -= data['upgradecost'];

							if (parseInt(db.f('currency'), 10) == 0) {
								// за монеты
								data['upgradecost'] = parseInt(db.f('cost'), 10);
								data['upgradecurrency'] = 0;

							} else {
								// за голоса
								data['upgradecost'] = parseInt(db.f('cost'), 10);
								data['upgradecurrency'] = 1;
							}
							update_my_info = true;
						}
					} else { // либо не хватает денег либо уже в апгрэйде
						response.push({
							'cmd' : "INFO",
							'params' : {
								'message' : 'Не хватает голосов',
								'x' : 158,
								'y' : 250
							}
						});
					}
				}
			}
			// query = 'insert into answers valuse
			// ('.uid.',"'.params['dialog'].'",'.parseInt(params['value'], 10).')';
			// // последние 10
			// db.query(query );
			break;
		}
		case 'GET_ARTIFACTS': {
			// TODO : загнать в редиску
			artifacts = json_decode(redis.get("artifacts"), true);
			if (artifacts != NULL) {
				response.push({
					'cmd' : "FILL_ARTIFACTS",
					'params' : artifacts
				});
			}
			break;
		}
		case 'READALL': {
			send_messages = false;
			for (i = 0; i < messages.length; i++)
				if (messages[i]['is_new'] == 1) {
					messages[i]['is_new'] = 0;
					send_messages = true;
				}

			if (send_messages)
				response.push({
					'cmd' : "MESSAGES",
					'params' : messages,
				});
			break;
		}
		case 'DELL_EVENT': {
			new_messages = [];
			for (i = 0; i < messages.length; i++)
				if (i != params['id'])
					new_messages.push(messages[i]);
			messages = new_messages;

			response.push({
				'cmd' : "MESSAGES",
				'params' : messages,
			});
			messages_send = true;
			break;
		}
		case 'DEL_ALL_EVENTS': {
			new_messages = [];
			messages = new_messages;

			response.push({
				'cmd' : "MESSAGES",
				'params' : messages,
			});
			messages_send = true;
			break;
		}
		case 'TOP50': {
			query = "SELECT user_id, name, power as ratio, coins FROM prevepoch ORDER BY ratio DESC LIMIT 50";
			db.query(query);
			ratio = [];
			pos = 1;
			while (db.next_record()) {
				ratio.push({
					'pos' : pos,
					'user_id' : db.f('user_id'),
					'name' : db.f('name'),
					'ratio' : db.f('ratio'),
					'coins' : db.f('coins'),
				});
				pos++;
			}
			response.push({
				'cmd' : "TOP50",
				'params' : ratio
			});
			break;
		}
		case 'PANTEON': {
			query = "SELECT user_id, name, power as ratio, coins FROM panteon ORDER BY ratio DESC LIMIT 50";
			db.query(query);
			ratio = [];
			pos = 1;
			while (db.next_record()) {
				ratio.push({
					'pos' : pos,
					'user_id' : db.f('user_id'),
					'name' : db.f('name'),
					'ratio' : db.f('ratio'),
					'coins' : db.f('coins'),
				});
				pos++;
			}
			response.push({
				'cmd' : "PANTEON",
				'params' : ratio
			});
			break;
		}
		case 'SCORE_RATIO': {
			score = parseInt(context[0]['ratio'], 10);
			// выбираем счетчик
			query = "SELECT count(user_id) from users where date_lastvisit>(date(now()) - integer '7') and score>".score;
			db.query(query);
			pos = 0;
			if (db.next_record())
				pos = parseInt(db.f('count'), 10) - 50;
			pos = (pos < 0) ? 0 : pos;
			query = "SELECT user_id, name, score as ratio, coins from users where date_lastvisit>(date(now()) - integer '7') order by ratio desc limit 100 offset ".pos;
			db.query(query);
			ratio = [];
			while (db.next_record()) {
				ratio.push({
					'pos' : pos,
					'user_id' : db.f('user_id'),
					'name' : db.f('name'),
					'ratio' : db.f('ratio'),
					'coins' : db.f('coins'),
				});
				pos++;
			}
			response.push({
				'cmd' : "SCORE_RATIO",
				'params' : ratio
			});
			break;
		}
		case 'POWER_RATIO': {
			power = parseInt(context[0]['power'], 10);

			pos = 0;
			type = parseInt(params, 10);
			if (type) {
				query = "SELECT friends FROM users WHERE user_id=".uid;
				db.query(query);
				if (db.next_record())
					friends = db.f('friends');
				query = "SELECT user_id, name, power as ratio, coins FROM users WHERE date_lastvisit>(date(now()) - integer '7') and (user_id in ("
						+ friends
						+ ") or user_id="
						+ uid
						+ ") order by ratio desc limit 100";
			} else {
				// выбираем счетчик
				query = "SELECT count(user_id) from users where date_lastvisit>(date(now()) - integer '7') and power>".power;
				db.query(query);
				if (db.next_record())
					pos = parseInt(db.f('count'), 10) - 50;
				pos = (pos < 0) ? 0 : pos;
				query = "SELECT user_id, name, power as ratio, coins from users where date_lastvisit>(date(now()) - integer '7') order by ratio desc limit 100 offset ".pos;
			}
			db.query(query);
			ratio = [];
			while (db.next_record()) {
				pos++;
				ratio.push({
					'pos' : pos,
					'user_id' : db.f('user_id'),
					'name' : db.f('name'),
					'ratio' : db.f('ratio'),
					'coins' : db.f('coins'),
				});
			}
			response.push({
				'cmd' : "POWER_RATIO",
				'params' : ratio
			});
			break;
		}
		case 'POP_RATIO': {
			// выбираем счетчик
			for (idx in context) {
				var data = content[idx];
				if (data['type'] == 1) {
					pos = 0;
					type = parseInt(params, 10);
					if (type) {
						query = "SELECT friends FROM users WHERE user_id=".uid;
						db.query(query);
						if (db.next_record())
							friends = db.f('friends');
						query = "SELECT user_id, name, population as ratio, coins FROM users WHERE date_lastvisit>(date(now()) - integer '7') and (user_id in ("
								+ friends
								+ ") or user_id="
								+ uid
								+ ") order by ratio desc limit 100";
					} else {
						attack = attack(context);
						query = "SELECT count(user_id) from users where date_lastvisit>(date(now()) - integer '7') and population>"
								+ data['population'];
						db.query(query);
						if (db.next_record())
							pos = parseInt(db.f('count'), 10) - 50;
						pos = (pos < 0) ? 0 : pos;
						query = "SELECT user_id, name, population as ratio, coins from users where date_lastvisit>(date(now()) - integer '7') order by ratio desc limit 100 offset ".pos;
					}
					db.query(query);
					ratio = [];
					while (db.next_record()) {
						pos++;
						ratio.push({
							'pos' : pos,
							'user_id' : db.f('user_id'),
							'name' : db.f('name'),
							'ratio' : db.f('ratio'),
							'coins' : db.f('coins'),
						});
					}
					response.push({
						'cmd' : "POP_RATIO",
						'params' : ratio
					});
				}
			}
			break;
		}
		case 'WIN_RATIO': {
			if (!context[0]['wins'])
				context[0]['wins'] = 0;
			wins = parseInt(context[0]['wins'], 10);
			pos = 0;
			type = parseInt(params, 10);
			if (type) {
				query = "SELECT friends FROM users WHERE user_id=".uid;
				db.query(query);
				if (db.next_record())
					friends = db.f('friends');
				query = "SELECT user_id, name, wins as ratio, lose, coins FROM users WHERE date_lastvisit>(date(now()) - integer '7') and (user_id in ("
						+ friends
						+ ") or user_id="
						+ uid
						+ ") order by ratio desc limit 100";
			} else {
				// выбираем счетчик
				query = "SELECT count(user_id) from users where date_lastvisit>(date(now()) - integer '7') and wins>".wins;
				db.query(query);
				if (db.next_record())
					pos = parseInt(db.f('count'), 10) - 50;
				pos = (pos < 0) ? 0 : pos;
				query = "SELECT user_id, name, wins as ratio, lose, coins from users where date_lastvisit>(date(now()) - integer '7') order by ratio desc limit 100 offset ".pos;
			}
			db.query(query);
			ratio = [];
			while (db.next_record()) {
				pos++;
				ratio.push({
					'pos' : pos,
					'user_id' : db.f('user_id'),
					'name' : db.f('name'),
					'ratio' : db.f('ratio') + '/' + db.f('lose'),
					'coins' : db.f('coins'),
				});
			}
			response.push({
				'cmd' : "WIN_RATIO",
				'params' : ratio
			});
			break;
		}
		case 'ATTACK_RATIO': {
			// выбираем счетчик
			attack = attack(context);
			query = "SELECT count(user_id) from users where date_lastvisit>(date(now()) - integer '7') and attack>".attack;
			db.query(query);
			pos = 0;
			if (db.next_record())
				pos = parseInt(db.f('count'), 10) - 50;
			pos = (pos < 0) ? 0 : pos;
			query = "SELECT user_id, name, attack as ratio, coins from users where date_lastvisit>(date(now()) - integer '7') order by ratio desc limit 100 offset ".pos;
			db.query(query);
			ratio = [];
			while (db.next_record()) {
				pos++;
				ratio.push({
					'pos' : pos,
					'user_id' : db.f('user_id'),
					'name' : db.f('name'),
					'ratio' : db.f('ratio'),
					'coins' : db.f('coins'),
				});
			}
			response.push({
				'cmd' : "ATTACK_RATIO",
				'params' : ratio
			});
			break;
		}
		case 'SHIELD_RATIO': {
			// выбираем счетчик
			shield = shield(context);
			query = "SELECT count(user_id) from users where date_lastvisit>(date(now()) - integer '7') and shield>".shield;
			db.query(query);
			pos = 0;
			if (db.next_record())
				pos = parseInt(db.f('count'), 10) - 50;
			pos = (pos < 0) ? 0 : pos;
			query = "SELECT user_id, name, shield as ratio, coins from users where date_lastvisit>(date(now()) - integer '7') order by ratio desc limit 100 offset ".pos;
			db.query(query);
			ratio = [];
			while (db.next_record()) {
				pos++;
				ratio.push({
					'pos' : pos,
					'user_id' : db.f('user_id'),
					'name' : db.f('name'),
					'ratio' : db.f('ratio'),
					'coins' : db.f('coins'),
				});
			}
			response.push({
				'cmd' : "SHIELD_RATIO",
				'params' : ratio
			});
			break;
		}
			break;
		case 'ACHIEVES': {
			id = parseInt(params['id'], 10);
			if (id != uid) {
				enemy = json_decode(redis.get("uid:".id), true);
				// дебажная тема потом убрать {
				if (enemy === NULL) {
					// ессли данных по пользователю нет, то
					// генерируем
					// начальное состояние пользователя
					enemy = [];
					enemy[0]['gid'] = 0;
					enemy[0]['voices'] = 0;

					fake = [];
					triggers(db, enemy_id, enemy);
					steps(db, enemy_id, enemy, fake);
					userData = enemy[0];
				}
			} else
				userData = context[0];

			if (!userData['thieve'])
				userData['thieve'] = 0;
			if (!userData['builder'])
				userData['builder'] = 0;
			if (!userData['rich'])
				userData['rich'] = 0;
			if (!userData['peaceable'])
				userData['peaceable'] = 0;
			if (!userData['berserker'])
				userData['berserker'] = 0;
			if (!userData['conqueror'])
				userData['conqueror'] = 0;

			achives['thieve'] = userData['thieve'];
			achives['builder'] = userData['builder'];
			achives['rich'] = userData['rich'];
			achives['peaceable'] = userData['peaceable'];
			achives['berserker'] = userData['berserker'];
			achives['conqueror'] = userData['conqueror'];

			response.push({
				'cmd' : "ACHIEVES",
				'params' : achives
			});

		}
			break;
		case 'FRIENDS': {
			// выбираем счетчик
			friends = explode(",", params);
			for (idx in friends)
				data = parseInt(friends[idx], 10);

			friendsStr = implode(",", friends);
			query = "UPDATE users SET friends='" + friendsStr
					+ "' WHERE user_id=" + uid;
			db.query(query);
			break;
		}
			break;
		case 'ENTER': {
			wins = 0;
			lose = 0;
			// if ( is_autorized(db, uid , auth_key ) ) {

			query = "SELECT * FROM users WHERE user_id=" + uid + " LIMIT 1";
			db.query(query);
			if (!db.next_record()) {
				// TODO: Вставляем в таблицу рейтингов
				query = "INSERT INTO users (user_id, name, nickname, img, sex) values ("
						+ uid
						+ ",'"
						+ params['name']
						+ "','"
						+ params['nickname']
						+ "', "
						+ "'"
						+ params['img']
						+ "'," + parseInt(params['sex'], 10) + ")";
				db.query(query);
			} else {
				attack = attack(context);
				shield = shield(context);
				population = get_population(context);

				query = "UPDATE users SET name='" + params['name']
						+ "', nickname='" + params['nickname'] + "',img='"
						+ params['img'] + "',sex=".parseInt(params['sex'], 10)
						+ ", coins=" + parseInt(context[0]['coins'], 10)
						+ ", population=" + population + ", attack=" + attack
						+ ", shield=" + shield
						+ ", date_lastvisit=NOW() where user_id=" + uid;
				db.query(query);
			}
			if ((!context[0]['name']) || (!context[0]['nickname'])) {
				context[0]['name'] = params['name'];
				context[0]['nickname'] = params['nickname'];
			}

			if (!context[0]['thieve'])
				context[0]['thieve'] = 0;
			if (!context[0]['builder'])
				context[0]['builder'] = 0;
			if (!context[0]['rich'])
				context[0]['rich'] = 0;
			if (!context[0]['peaceable'])
				context[0]['peaceable'] = 0;
			if (!context[0]['berserker'])
				context[0]['berserker'] = 0;
			if (!context[0]['conqueror'])
				context[0]['conqueror'] = 0;

			/*
			 * query = 'DELETE FROM waiting WHERE date_enter < (date(NOW()) -
			 * integer \'1\') or login=\''.login.'\''; db.query(query );
			 * 
			 * ////////////////// query = 'select users.* from waiting, users
			 * where waiting.user_id=users.user_id ORDER BY waiting.date_enter';
			 * db.query(query ); waiters = []; while (db.next_record()) {
			 * array_push (waiters, array( 'user_id' => db.f('user_id'), 'login' =>
			 * db.f('login'), ) ); } query = 'INSERT INTO waiting (user_id,
			 * date_enter) values ('. uid.', NOW())'; db.query(query );
			 */
			response.push({
				'cmd' : "HIDE_REG",
				'params' : null
			});
			response.push({
				'cmd' : "SHOW_GAMESCREEN",
				'params' : null
			});

			// Время конца эпохи
			context[0]['epoch'] = lastday();

			if (!new_registration) {
				/*
				 * array_push (response, array( 'cmd' : "INFO", 'params' :
				 * array('message' :'Приветcтвую тебя великий вождь племени
				 * викингов', 'x':158, 'y':250, 'type' :2) ) );
				 */
				/*
				 * array_push (messages, array( 'type' : 1, // Враг проиграл
				 * 'message' : 'Teст', 'is_new' => 1, ) );
				 */
				response.push({
					'cmd' : "MESSAGES",
					'params' : messages,
				});
			}
			messages_send = true;
			update_my_info = true;
			// return array( 'waiters' => waiters, 'info' => result,
			// );
			// } else
			// return false;
			break;
		}
		default:
			break;
		}
	}
}

function handle_endaction() {
	if ((context[0]['friendhelptime'] != 0)
			&& (context[0]['friendhelptime'] <= time())) {
		context[0]['friendhelptime'] = 0;
		context[0]['friendhelp'] = 0;
		response.push({
			'cmd' : "INFO",
			'params' : {
				'message' : 'Помошь друга окончилась.',
				'x' : 158,
				'y' : 250
			}
		});
	}

	if ((context[0]['silki'] != 0) && (context[0]['silki'] <= time())) {
		context[0]['silki'] = 0;
		response.push({
			'cmd' : "INFO",
			'params' : {
				'message' : 'Действие артефакта Магические Силки окончилось.',
				'x' : 158,
				'y' : 250
			}
		});
	}

	if ((context[0]['runa'] != 0) && (context[0]['runa'] <= time())) {
		context[0]['runa'] = 0;
		response.push({
			'cmd' : "INFO",
			'params' : {
				'message' : 'Действие Руны Ловкости окончилось.',
				'x' : 158,
				'y' : 250
			}
		});
	}

	if ((context[0]['torhammer'] != 0) && (context[0]['torhammer'] <= time())) {
		context[0]['torhammer'] = 0;
		response.push({
			'cmd' : "INFO",
			'params' : {
				'message' : 'Действие амулета Молот Тора окончилось.',
				'x' : 158,
				'y' : 250
			}
		});
	}

	if ((context[0]['mushrooms'] != 0) && (context[0]['mushrooms'] <= time())) {
		context[0]['mushrooms'] = 0;
		response.push({
			'cmd' : "INFO",
			'params' : {
				'message' : 'Действие Мухоморов окончилось.',
				'x' : 158,
				'y' : 250
			}
		});
	}
}

function handle_thread() {
	for (idx in context) {
		var data = content[idx];
		// Дом старейшины
		if ((data['type'] == 1)
				&& ((data['earntime'] == 0) || (data['earntime'] <= time()))) {
			// подошло время заработка

			// TODO: пересчитать типа взять earntime отнять текущее. посмотреть
			// не больше ли 7 дней
			// сократить время до текущего или 7ми дней
			// если больше то в цикле прибавлять прибавить за один день и
			// earntime увеличить до следующего дня
			// в конце установить время на текущее+60

			// добавляем 1 минуту так как она уже прошла
			if (data['earntime'])
				diffearntime = time() - data['earntime'] + 60;
			else
				diffearntime = 0;
			if (diffearntime > 3600) // DEBUG
			// RELEASE if (diffearntime>518400)
			{
				// FREEZE
				response
						.push({
							'cmd' : "INFO",
							'params' : {
								'message' : 'Вы не появлялись в игре более 7 дней, поэтому ваш профиль был заморожен. На вас не могли нападать, но и прирост викингов и населения был приостановлен!',
								'x' : 158,
								'y' : 250,
								'type' : 1
							}
						});
				diffearntime = 518400; // установить отличие 1 неделя за
				// минусом 1 дня
			}
			i = 86400; // день
			population = data['population'];

			do {
				if (i > diffearntime)
					i = diffearntime;
				context[0]['coins'] += population * parseInt(i / 60);
				data['earntime'] += i;
				diffearntime -= i;
				population += data['growth'];
			} while (diffearntime > 0);
			data['earntime'] = (time() + 60);
			update_my_info = true;
		}

		// подошло время добавить ход
		if ((data['type'] == 0)
				&& (parseInt(data['turncount'], 10) < 10)
				&& ((parseInt(data['turntime'], 10) == 0) || (parseInt(data['turntime'], 10) <= time()))) {
			// пока время обновления не дощло до текущего времени
			while (parseInt(data['turntime'], 10) <= time()) {
				if (data['turncount'] == 10) {
					// если ходы достигли критической массы то ставим следующую
					// проверку в будущее и выходим
					data['turntime'] = time() + 180 * 60;
					break;
				} else {
					// обновляем время и ходы
					data['turntime'] += 180 * 60;
					data['turncount']++;
					update_my_info = true;
				}
			}
		}

		// подошло время плодится
		if ((data['type'] == 1)
				&& ((data['growthtime'] == 0) || (data['growthtime'] <= time()))) {
			// добавляем 1 день так как он уже прошел
			if (data['growthtime'])
				diffgrowthtime = time() - data['growthtime'] + 86400;
			else
				diffgrowthtime = 0;

			// установить отличие 1 неделя за минусом 1 дня
			if (diffgrowthtime > 518400)
				diffgrowthtime = 518400;
			i = 86400; // день

			do {
				if (i > diffgrowthtime)
					i = diffgrowthtime;
				if (profession == 4)
					// мамаше за каждый уровень дома старейшин +2 викинга
					data['population'] += data['growth'] * parseInt(i / 86400, 10)
							+ parseInt(data['level'], 10) * 2;
				else
					data['population'] += data['growth'] * parseInt(i / 86400, 10);
				data['growthtime'] += i;
				diffgrowthtime -= i;
			} while (diffgrowthtime > 0);
			// на следующий день
			data['growthtime'] = time() + 86400;
		}

		// подошло время апгрэйда
		if ((data['upgradetime']) && (data['upgradetime'] != 0)
				&& (data['upgradetime'] <= time())) {
			data['upgradetime'] = 0;
			data['level'] += 1;
			{
				// в зависимости от типа выставяем дополнительные параметры
				switch (data['type']) {
				case 1: {
					// проставлять рост населения
					data['growth'] += 10;
					break;
				}
				case 4: // атаку
				{
					data['attack'] = parseInt(data['attack'] * 1.25, 10);
					break;
				}
				case 7: // защиту
				{
					data['shield'] = parseInt(data['shield'] * 1.25, 10);
					break;
				}
				case 8: // заработок от урожая
				{
					data['valuecrops'] = data['level'] * 5000;
					break;
				}
				default:
					break;
				}
			}
			update_my_info = true;
		}
	}
}

function handle_reglottery() {
	if (new_registration) {
		if (uid_counter % 1000 == 0) {
			context[0]['ratio'] += 10;
			response
					.push({
						'cmd' : "INFO",
						'params' : {
							'message' : 'Поздравляем, вы счастливчик! На ваш счет добавлено 10% рейтинга. Выдача призов происходит раз в неделю. Подробнее об этом можно прочитать под приложением',
							'x' : 158,
							'y' : 250,
							'type' : 3
						}
					});
		} else if (uid_counter % 100 == 0) {
			context[0]['ratio'] += 1;
			response
					.push({
						'cmd' : "INFO",
						'params' : {
							'message' : 'Поздравляем, вы счастливчик! На ваш счет добавлен 1% рейтинга. Выдача призов происходит раз в неделю. Подробнее об этом можно прочитать под приложением.',
							'x' : 158,
							'y' : 250,
							'type' : 3
						}
					});
		} else if (uid_counter % 10 == 0) {
			context[0]['coins'] += 100000;
			response
					.push({
						'cmd' : "INFO",
						'params' : {
							'message' : 'Поздравляем, вы счастливчик! На ваш игровой счет добавлено +100000 золотых монет.',
							'x' : 158,
							'y' : 250,
							'type' : 3
						}
					});
		} else {
			response
					.push({
						'cmd' : "INFO",
						'params' : {
							'message' : 'Увы, но вам не повезло. Приглашайте друзей, быть может, им повезет!',
							'x' : 158,
							'y' : 250,
							'type' : 3
						}
					});
		}
	}
}

function get_population(context) {
	population = 0;
	for (idx in context) {
		var data = content[idx];
		// Дом старейшины
		if (data['type'] == 1)
			population = parseInt(data['population'], 10);
	}
	return population;
};

function attack(context) {
	retval = 0;
	profession = parseInt(context[0]['profession'], 10); // 1 для атаки
	population = get_population(context);

	for (object in context) {
		if (object['attack'])
			retval += object['attack']
					* ((object['count'] === NULL) ? 1 : object['count']);
		if (object['friendhelp'])
			retval += object['friendhelp'];
	}
	if (profession == 1)
		retval = retval * 1.1; // 10% Кенту
	else
		retval += population;
	return retval;
};

function shield(context) {
	retval = 0;
	profession = parseInt(context[0]['profession'], 10); // 1 для атаки
	population = get_population(context);
	for (object in context) {
		if (object['shield'])
			retval += object['shield']
					* ((object['count'] === NULL) ? 1 : object['count']);
		if (object['friendhelp'])
			retval += object['friendhelp'];
	}
	if (profession == 2)
		retval += population * 1.1; // 10% крепышу
	else
		retval += population;
	return retval;
};

function handle_update_myinfo() {
	// запросить клиента получить информацию об апгрэйде
	if (update_my_info) {
		if (context[0]['power']) {
			attack = attack(context);
			shield = shield(context);
			population = get_population(context);
			context[0]['power'] = parseInt(attack + shield + population, 10);
		}
		my_info = [];
		// ужимаем контест и организуем его для флеша. :(
		for (idx in context) {
			var data = context[idx];
			data['id'] = idx;
			my_info.push(data);
		}
		response.push({
			'cmd' : "SET_MYINFO",
			'params' : my_info
		});
	}
}

function lastday() {
	month = date('m');
	year = date('Y');
	result = strtotime("{year}-{month}-01");
	result = strtotime('-1 second', strtotime('+1 month', result));
	return result;
};

function error(code) {
	switch (code) {
	case '0':
		return {
			'error' : 'Cannot connect to API server'
		};
	case '1':
		return {
			'error' : 'Cannot autorize'
		};
	case '2':
		return {
			'error' : 'You need add application'
		};
	default:
		return {
			'error' : 'Undefined error'
		};
	}
};