<?php 
echo $argv[1]."\n"; // ключ и имя файла
if ($argv[1])
{
	static $REDIS_DB = 2;
	
	$json_data = file_get_contents($argv[1]);
	$context = json_decode($json_data, true);
	if ($context === NULL)
	{
		echo "File is corrupted\n";
	}
	else
	{
		$redis = new Redis();
		$redis->connect('127.0.0.1', 6379);
		$redis->select($REDIS_DB);
		$redis->set($argv[1], json_encode($context));
		$json_data = $redis->get($argv[1]);
		$context = json_decode($json_data, true);
		echo var_export($context , true)."\nData inserted to Redis :".$argv[1]."\n";	
	}
}
?>