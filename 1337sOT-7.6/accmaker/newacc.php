<center>
<?
function skills($vocation)
{	require('config.php');
	$str = "";

	for ($id = 0; $id <= 6; $id++)
		$str .= ('<skill skillid="' . $id . '" level="' . $skill[$vocation][$id] . '" tries="0"/>' . "\n");

	return $str;
}

function equip($vocation)
{	require('config.php');
	$str = "";

	for ($id = 1; $id <= 10; $id++)
	{
		if ($equip[$vocation][$id-1] != 0)
		{
			$str .= ('<slot slotid="' . $id . '"><item id="' . $equip[$vocation][$id-1] . '"');
			if ($id == 3)	// backpack
				$str .= ('><inside>' . $bp[$vocation] . '</inside></item>');
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
	$vocation = $_POST['vocation'];
	$account = $_POST['account'];
	$pass = $_POST['pass'];

	($pass != $account) or die ('Password is same as account number!');
	($pass != $nick) or die ('Password is same as character name!');
	($pass != 'tibia' && $pass != 'lol') or die ('Password is too simple!');

	($sex == 0 || $sex == 1) or die ('Invalid sex!');
	($vocation >= 1 && $vocation <= 4) or die ('Invalid vocation!');
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
<player name="' . $nick . '" account="' . $account . '" sex="' . $sex . '" lookdir="1" exp="' . $exp . '" voc="' . $vocation . '" level="' . $lvl . '" access="0" cap="' . $cap . '" maglevel="' . $mlvl[$vocation] . '" lastlogin="0">
<spawn x="' . $spawnx . '" y="' . $spawny . '" z="' . $spawnz . '"/>
<temple x="' . $templex . '" y="' . $templey . '" z="' . $templez . '"/>
<health now="' . $health[$vocation] . '" max="' . $health[$vocation] . '" food="0"/>
<mana now="' . $mana[$vocation] . '" max="' . $mana[$vocation] . '" spent="0"/>
<look type="' . $look[$vocation][$sex] . '" head="20" body="30" legs="40" feet="50"/>
<skills>' . "\n" . skills($vocation) . '</skills>
<inventory>' . equip($vocation) . '</inventory>
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