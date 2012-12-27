<?php
session_start();
include 'libs/db_pgsql.php';
include 'config_vikings.php'; 

$db = new pDB_Sql();
$db->Database = Config::$DB_name;
$db->User     = Config::$DB_user;
$db->Password = Config::$DB_pass;
$db->Host     = Config::$DB_host;

//file_put_contents( 'log.txt', file_get_contents( 'log.txt' )."\nRATIO ".$query );
$query = "DELETE FROM prevepoch";
$db->query($query );

$query = "INSERT INTO prevepoch SELECT user_id, name, date_register, date_lastvisit, score, attack, shield, population, coins, nickname, img, sex, wins, lose, power FROM users ORDER BY POWER DESC LIMIT 50";
$db->query($query );

$query = "DELETE FROM tmp_panteon";
$db->query($query );

$query = "INSERT INTO tmp_panteon SELECT user_id, name, date_register, date_lastvisit, score, attack, shield, population, coins, nickname, img, sex, wins, lose, power FROM panteon ORDER BY POWER DESC LIMIT 50";
$db->query($query );
$query = "INSERT INTO tmp_panteon SELECT user_id, name, date_register, date_lastvisit, score, attack, shield, population, coins, nickname, img, sex, wins, lose, power FROM users ORDER BY POWER DESC LIMIT 50";
$db->query($query);
$query = "DELETE FROM PANTEON";
$db->query($query );
$query = "INSERT INTO panteon SELECT DISTINCT ON (user_id) user_id, name, date_register, date_lastvisit, score, attack, 
		shield, population, coins, nickname, img, sex, wins, lose, power  FROM tmp_panteon ORDER BY user_id, power DESC LIMIT 50";
$db->query($query );

$redis = new Redis();
$redis->connect('127.0.0.1', 6379);
$redis->select(Config::$REDIS_DB);
$keys = $redis->getKeys("uid:*");
//echo var_export($keys, true);
foreach ($keys as $key_id => $key)
{
	$context = json_decode($redis->get($key), true);
	//echo var_export($context, true);
	$thieve = $context[0]['thieve'];
	$builder = $context[0]['builder'];
	$rich = $context[0]['rich'];
	$peaceable = $context[0]['peaceable'];
	$berserker = $context[0]['berserker'];
	$conqueror = $context[0]['conqueror'];
	
	if ($key!='uid:counter')
	{
		//TODO : Выяснить ачивы
		foreach ($context as $context_id => &$data)
		{
			switch ($data['type'])
			{
				case 0:
					if ($data['coins'] >= 100000000) $context[0]['rich'] |= 32;
					if ($data['coins'] >= 30000000) $context[0]['rich'] |= 16;
					if ($data['coins'] >= 5000000) $context[0]['rich'] |=8;
					if ($data['coins'] >= 1000000) $context[0]['rich'] |=4;
					if ($data['coins'] >= 500000) $context[0]['rich'] |= 2;
					if ($data['coins'] >= 100000) $context[0]['rich'] |=1;
					
					$data['coins'] = 0;
					break;
				case 1:
					if ($data['level']==20) $context[0]['builder'] |=1;
					$data['level'] = 1;
					$data['upgradecost'] = 12500;
					$data['growth'] = 5;
					$data['population'] = 5;
					break;
				case 2:
					$data['level'] = 1;
					break;
				case 3:
					if ($data['level']==15) $context[0]['builder'] |=2;
					$data['level'] = 1;
					$data['upgradecost'] = 10000;
					break;
				case 4:
					if ($data['level']==15) $context[0]['builder'] |=8;
					$data['level'] = 1;
					$data['upgradecost'] = 5000;
					$data['attack'] = 100;
					break;
				case 5:
					$data['level'] = 1;
					break;
				case 6:
					if ($data['level']==5) $context[0]['builder'] |=32;
					break;
				case 7:
					if ($data['level']==15) $context[0]['builder'] |=4;
					$data['level'] = 1;
					$data['upgradecost'] = 5000;
					$data['shield'] = 100;
					break;
				case 8:
					if ($data['level']==10) $context[0]['builder'] |=16;
					$data['level'] = 1;
					$data['valuecrops'] = 5000;
					$data['upgradecost'] = 20000;
					break;
				case 9:
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
				case 16:
				case 17:
				case 18:
				case 19:
				case 20:
					$data['count'] = 0;
					break;
			}
			//file_put_contents( 'request.txt', file_get_contents( 'request.txt' )."\n".'EARNTIME OUT');
		}
		if (($rich != $context[0]['rich']) && ( $context[0]['rich'] & 63))
		{ 
			// 50 000 000 монет
			$context[0]['coins']+=50000000;
		}
		if (($builder != $context[0]['builder']) && ( $context[0]['builder'] & 63))
		{ 
			//по 1 штуке Удобрения, Мухоморов, Амулета, Магических силков и Руны ловкости + 10 000 000 золота.
			$context[0]['coins']+=10000000;
		}
		if (($thieve != $context[0]['thieve']) && ( $context[0]['thieve'] & 63))
		{
			//5 штук Мухоморов
		}
		if (($peaceable != $context[0]['peaceable']) && ( $context[0]['peaceable'] & 63))
		{
			//10 штук Магических силков
		}
		if (($berserker != $context[0]['berserker']) && ( $context[0]['berserker'] & 63))
		{
			//5 штук Рун ловкости
		}
		if (($conqueror != $context[0]['conqueror']) && ( $context[0]['conqueror'] & 63))
		{
			//50 штук Секир
		}
		
		$redis->set($key, json_encode($context));
		$messages_key = str_replace("uid:", "messages:", $key);
		$messages = json_decode($redis->get($messages_key), true);
		if ($messages === NULL) $messages = array();
		$month = date('m');
		$year = date('Y');
		$day = date('d');
		$epochdate = "{$year}-{$month}-{$day}";
		//$result = strtotime('-1 second', strtotime('+1 month', $result));
		array_push ($messages, 
				array(
					'type' => 0, // Враг выиграл
					'message' => 'Эпоха началась '.$epochdate,
					'is_new' => 1,
				)
			);
		$redis->set($messages_key, json_encode($messages));
	}
}
echo "OK";
?>