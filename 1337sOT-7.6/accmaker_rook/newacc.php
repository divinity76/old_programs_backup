<center>
<?
function skills()
{	require('config.php');
	$str = "";

	for ($id = 0; $id <= 6; $id++)
		$str .= ('<skill skillid="' . $id . '" level="10" tries="0"/>' . "\n");

	return $str;
}

function equip()
{	require('config.php');
	$str = "";

	for ($id = 1; $id <= 10; $id++)
	{
		if ($equip[$id-1] != 0)
		{
			$str .= ('<slot slotid="' . $id . '"><item id="' . $equip[$id-1] . '"');
			if ($id == 3)	// backpack
				$str .= ('><inside>' . $bp . '</inside></item>');
			else
				$str .= ('/>');
			$str .= ('</slot>' . "\n");
		}
	}

	return $str;
}

function newacc()
{
	require('config.php');
	$nick = $_POST['nick'];
	$sex = $_POST['sex'];
	$account = $_POST['account'];
	$pass = $_POST['pass'];

	($pass != $account) or die ('Password is same as account number!');
	($pass != $nick) or die ('Password is same as character name!');
	($pass != 'tibia' && $pass != 'lol') or die ('Password is too simple!');

	($sex == 0 || $sex == 1) or die ('Invalid sex!');
	($account >= 100000 && $account <= 999999) or die ('Invalid account number!');
	preg_match("/^[a-zA-Z][a-zA-Z ]*$/", $nick) or die ('Name contains illegal characters!');
	preg_match("/^[a-zA-Z0-9]+$/", $pass) or die ('Password contains illegal characters!');
	!preg_match("/^gm/i", $nick) or die ('GM is only one!');

	$account_file = "$diraccount/$account.xml";
	$player_file = "$dirplayer/$nick.xml";
	$vip_file = "$dirvip/$account.xml";

	!file_exists($account_file) or die ('There is already an account with this number!');
	!file_exists($player_file) or die ("There is already a player $nick!");

	$file = fopen($account_file, 'w');
	fwrite($file,
'<?xml version="1.0"?>
<account pass="' . $pass . '" type="1" premDays="90">
<characters>
	<character name="' . $nick . '"/>
</characters>
</account>');
	fclose($file);

	$file = fopen($player_file, 'w');
	fwrite($file,
'<?xml version="1.0"?>
<player name="' . $nick . '" account="' . $account . '" sex="' . $sex . '" lookdir="1" exp="0" voc="0" level="1" access="0" cap="300" maglevel="0" lastlogin="0">
<spawn x="' . $spawnx . '" y="' . $spawny . '" z="' . $spawnz . '"/>
<temple x="' . $templex . '" y="' . $templey . '" z="' . $templez . '"/>
<health now="150" max="150" food="0"/>
<mana now="0" max="0" spent="0"/>
<look type="' . $look[$sex] . '" head="20" body="30" legs="40" feet="50"/>
<skills>' . "\n" . skills() . '</skills>
<inventory>' . equip() . '</inventory>
<depots><depot depotid="1"><item id="2590"><inside><item id="2594"/></inside></item></depot></depots>
<storage/>
</player>');
	fclose($file);

	$file = fopen($vip_file, 'w');
	fwrite($file,
'<?xml version="1.0"?>
<vips/>');
	fclose($file);

	print "Account $account was successfully created!";
}

newacc();
?>
</center>