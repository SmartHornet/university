<?php
require_once('config.php');

define('CONFIGURE_DS2406P_FILE','configure_ds2406p.php');
define('CONTROL_DS2406P_FILE','control_ds2406p.php');

define('CONFIGURE_DS2413P_FILE','configure_ds2413p.php');
define('CONTROL_DS2413P_FILE','control_ds2413p.php');

define('CONFIGURE_DS18B20_FILE','configure_ds18b20.php');

define('CONFIGURE_DS2438_FILE','configure_ds2438.php');


function connectToDB($dbname){
	$mysqli = new mysqli(DB_HOST,DB_NAME,DB_PASSWORD,$dbname);

	if(mysqli_connect_errno()){
		echo 'Ошибка соединения с базой данных. <br> Код ошибки: '.$mysqli->connect_errno.'.<br> Сообщение: '.$mysqli->connect_error;
		exit;
	}
	
	return $mysqli;
}

function ShowSiteHeader($string){
	$str = '';
	if(isset($_SESSION['valid_user'])) $str = $_SESSION['valid_user'];
	
	echo '<style type="text/css">
	a.t {
		background-color: Gainsboro; 
		font: sans-serif;
		color: DarkSlateGray;
	}
	
	</style>';
	
	echo '<table width="70%" align="center" border="0">';
		echo '<tr><td colspan="4" align="right"> Текущий пользователь: <b>'.$str.'</b> [<a href="logout.php">Выход</a>] </td></tr>
		</table>';
		echo '<!--<div style="position: relative;>-->
		<table width="70%" align="center" border="0">
			<tr>
				<td colspan="4" align="center"><div style="position: relative; top: 27px; right: 55px"><img src="images/brainhouse.png" height="70"></div></td>
			</tr>
			<tr>
				
				<td colspan="4" align="center"><span style="font: 50pt sans-serif; text-shadow: 10px; color: rgb(89,162,32); font-weight: bold; margin: 100px;">BrainHouse</span></td>
				
			</tr>
		</table>
		<!--<div style="position: absolute; top: 4px;"><img src="images/brainhouse.png" height="80"></div>-->
		<!--</div>-->';
			echo '<hr>';
		echo '<table width="100%" align="center" border="0">
		<tr>
				<td align="center"><h1><a class="t" href="index.php">Панель контроля</a></h1></td>
				<td align="center"><spacer type="horizontal" width="20"></spacer></td>
				<td align="center"><h1><a class="t" href="objects.php">Объекты</a></h1></td>
				<td align="center"><spacer type="horizontal" width="20"></spacer></td>
				<td align="center"><h1><a class="t" href="searchnewdevices.php">Добавление устройства</a></h1></td>
				<td align="center"><spacer type="horizontal" width="20"></spacer></td>
				<td align="center"><h1><a class="t" href="system_setup.php">Настройка системы</a></h1></td>
			</tr>';
			echo '</table>';
			echo '<hr>';
			echo '<table width="70%" align="center" border="0">';
		echo '<tr><td colspan="4" align="center"><h2>'.$string.'</h2></td></tr>';
		echo '</table>';
	
	
	
}

function error($message){

	$content =
	'<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
"http://www.w3.org/TR/html4/loose.dtd">
<html>
  <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title>Поиск новых устройств</title>
  </head>
        <body>
	<?php
	session_start();
	if(!isset($_SESSION["valid_user"])){
		header("location: login.php");
		exit();
	}
			
	error_reporting(E_ALL | E_STRICT);
	if (ini_get("display_errors") != 1){
	ini_set("display_errors", 1);
	};
			
	require("../Functions.php");
	ShowSiteHeader("Ошибка");
	
	echo "'.$message.'";
	
	?>
	</body>
	</html>';
	
	$f = fopen($_SERVER['DOCUMENT_ROOT'].'/SmartHome/errors/error.php','w');
	fwrite($f,$content,strlen($content));
	fclose($f);
	
	header('location: errors/error.php');
	exit();
}

/*function getMicrochipGroups(){
	$mysqli = connectToDB('SmartHome');
	$result = $mysqli->query('SET NAMES utf8');
	$query = 'select * from microchipGroups';
	$result = $mysqli->query($query);
	$mysqli->close();
	$num_results = $result->num_rows;
	
	for($i = 0; $i < $num_results; $i++){
		$row = $result->fetch_assoc();
		$table[$i] = array('microchipGroup' => $row['microchipGroup'], 'tableName' => $row['tableName'], 
		'microchipName' => $row['microchipName'], 'description' => $row['description']);
	}
	
	$result->free();
	return $table;
}*/

function getDeviceTableName($ID){
	$mysqli = connectToDB('SmartHome');
	$result = $mysqli->query('SET NAMES utf8');

	$group = substr($ID,0,2);
		/*и по группе определяю таблицу где хрянятся дынные об этой устройстве*/
	$query = 'select tableName from microchipGroups where microchipGroup = "'.$group.'"';
	$result = $mysqli->query($query);
	$row = $result->fetch_assoc();
	$tableName = $row['tableName'];
	
	$mysqli->close();
	if(is_object($result)) $result->free();
				
	return $tableName;
}

function myArray_search($value,$array){
	if($array == null) return false;
	else return array_search($value,$array);
}

function getConnectedDevices(){
	if(file_exists(OWNET_PHP)){
		require_once(OWNET_PHP);
	}
	else exit('Ошибка: Нет файла OWNET_PHP: '.OWNET_PHP);
			
	$ownet = new OWNet(OWSERVER);
	@ $connectDiveces = $ownet -> dir('uncached');
	$connectDiveces = $connectDiveces['data_php'];
			
	$connectDiveces = str_replace('/uncached/','',$connectDiveces);
	$token = strtok($connectDiveces,',');
	$i = 0;
	while($token != 'bus.0'){
		$connectDivecesArray[$i] = $token;
		$i++;
		$token = strtok(',');
	}
	unset($ownet);
	if($i == 0) $connectDivecesArray = null;
	return $connectDivecesArray;
}

function getAddedDevices(){
	$mysqli = connectToDB('SmartHome');
	$result = $mysqli->query('SET NAMES utf8');

	$query = 'select tableName from microchipGroups';
	$result = $mysqli->query($query);
	$num_results = $result->num_rows;
	
	$countAddedDevices = 0;
	for($i = 0; $i < $num_results; $i++){
		$row = $result->fetch_assoc();
		$tableName = $row['tableName'];
		
		$query = 'select id from '.$tableName;
		$subresult = $mysqli->query($query);
		$subnum_results = $subresult->num_rows;
		for($j = 0; $j < $subnum_results; $j++){
			$subrow = $subresult->fetch_assoc();
			$addedDevicesArray[$countAddedDevices] = $subrow['id'];
			$countAddedDevices++;
		}
	}
	
	$mysqli->close();
	if($countAddedDevices == 0) $addedDevicesArray = null;
	if(is_object($result)) $result->free();
	if(isset($subresult)) if(is_object($subresult)) $subresult->free();
	return $addedDevicesArray;
}

function getAddedDevicesForName($tableName){
	$mysqli = connectToDB('SmartHome');
	$result = $mysqli->query('SET NAMES utf8');
	
	$countAddedDevices = 0;
	$query = 'select id from '.$tableName;
	//echo '$query '.$query.'<br>';
	$result = $mysqli->query($query);
	$mysqli->close();
	$num_results = $result->num_rows;
	//echo '$num_results '.$num_results.'<br>';
	for($i = 0; $i < $num_results; $i++){
		$row = $result->fetch_assoc();
		$addedDevicesArray[$countAddedDevices] = $row['id'];
		$countAddedDevices++;
	}
	
	if($countAddedDevices == 0) $addedDevicesArray = null;
	if(is_object($result)) $result->free();
	return $addedDevicesArray;
}

function getSetupMenu_ds2406p($deviceID){
	$setupMenu = '
	<form action="'.CONFIGURE_DS2406P_FILE.'" method="post">
	<input type="hidden" name="id" value="'.$deviceID.'">
	<table border="0">
	<tr>
		<td><input type="submit" name="channelA" value="Канал А"></td>
	</tr>
	<tr>
		<td><input type="submit" name="channelB" value="Канал Б"></td>
	</tr>
	<tr>
		<td><input type="submit" name="delete" value="Удалить"></td>
	</tr>
	</table>
	</form>
	';
	return $setupMenu;
}

function getSetupMenu_ds2413p($deviceID){
	$setupMenu = '
	<form action="'.CONFIGURE_DS2413P_FILE.'" method="post">
	<input type="hidden" name="id" value="'.$deviceID.'">
	<table border="0">
	<tr>
		<td><input type="submit" name="channelA" value="Канал А"></td>
	</tr>
	<tr>
		<td><input type="submit" name="channelB" value="Канал Б"></td>
	</tr>
	<tr>
		<td><input type="submit" name="delete" value="Удалить"></td>
	</tr>
	</table>
	</form>
	';
	return $setupMenu;
}

function getSetupMenu_ds18b20($deviceID){
	$setupMenu = '
	<form action="'.CONFIGURE_DS18B20_FILE.'" method="post">
	<input type="hidden" name="id" value="'.$deviceID.'">
	<table border="0">
	<tr>
		<td><input type="submit" name="configure" value="Настроить"></td>
	</tr>
	<tr>
		<td><input type="submit" name="delete" value="Удалить"></td>
	</tr>
	</table>
	</form>
	';
	return $setupMenu;
}

function getSetupMenu_ds2438($deviceID){
	$setupMenu = '
	<form action="'.CONFIGURE_DS2438_FILE.'" method="post">
	<input type="hidden" name="id" value="'.$deviceID.'">
	<table border="0">
	<tr>
		<td><input type="submit" name="configure" value="Настроить"></td>
	</tr>
	<tr>
		<td><input type="submit" name="delete" value="Удалить"></td>
	</tr>
	</table>
	</form>
	';
	return $setupMenu;
}

function getSetupMenu($deviceID){
	$func = 'getSetupMenu_'.getDeviceTableName($deviceID);
	$setupMenu = $func($deviceID);
	return $setupMenu;
	
}

function ownetSet($path,$value){
	if(file_exists(OWNET_PHP)){
		require_once(OWNET_PHP);
	}
	else exit('Ошибка: Нет файла OWNET_PHP: '.OWNET_PHP);
			
	@ $ownet = new OWNet(OWSERVER);
	
	@ $result = ($ownet -> set($path,$value));
	
	unset($ownet);
	return $result;
}

function ownetGet($path){
	if(file_exists(OWNET_PHP)){
		require_once(OWNET_PHP);
	}
	else exit('Ошибка: Нет файла OWNET_PHP: '.OWNET_PHP);
			
	@ $ownet = new OWNet(OWSERVER);
	
	@ $result = $ownet -> get($path);
	
	unset($ownet);
	return $result;
}

function getDeviceTableNames(){
	$mysqli = connectToDB('SmartHome');
	$result = $mysqli->query('SET NAMES utf8');

	$query = 'select tableName from microchipGroups';
	$result = $mysqli->query($query);
	$mysqli->close();
	$num_results = $result->num_rows;
	for($i = 0; $i < $num_results; $i++){
		$row = $result->fetch_assoc();
		$deviceTableNamesArray[$i] = $row['tableName'];
	}
	
	if($num_results == 0) $deviceTableNamesArray = null;
	if(is_object($result)) $result->free();
				
	return $deviceTableNamesArray;
}

//добавляет к массиву $array1 массив $array2 и возвращает результирующий
function mergeArray($array1, $array2){
	$countArray1 = count($array1);
	$countArray2 = count($array2);
	
	for($i = $countArray1, $j = 0; $j < $countArray2; $i++,$j++){
		$array1[$i] = $array2[$j];
	}
	return $array1;
}

function getMainMenu($objectID, $connectDivecesArray){
	$deviceTableNameArray = getDeviceTableNames();
	$countDeviceTableNames = count($deviceTableNameArray);
	
	$mainMenuArrayLenght = 0;
	$mainMenuArray = null;
	for($i = 0; $i < $countDeviceTableNames; $i++){
		$func = 'getMainMenu_'.$deviceTableNameArray[$i];
		$mainMenu = $func($objectID, $connectDivecesArray);
		
		if($mainMenu != null){
			$mainMenuArray = mergeArray($mainMenuArray,$mainMenu);
		}
	}
	
	return $mainMenuArray;
}

function getMainMenu_ds18b20($objectID, $connectDivecesArray){
	$addedDevicesArray = getAddedDevicesForName('ds18b20');
	
	/*echo '$connectDivecesArray ';
	print_r($connectDivecesArray);
	
	echo '$addedDevicesArray ';
	print_r($addedDevicesArray);*/
	
	$countAddedDevices = count($addedDevicesArray);
	
	$mainMenuArrayLenght = 0;
	for($i = 0; $i < $countAddedDevices; $i++){
		if(myArray_search($addedDevicesArray[$i],$connectDivecesArray) !== false){
			//echo 'FIND <br>';
			$mainMenu = getMainMenu_ds18b20_id($addedDevicesArray[$i],$objectID);
			if($mainMenu != null){
				$mainMenuArray[$mainMenuArrayLenght] = $mainMenu;
				$mainMenuArrayLenght++;
			}
		}
	}
	
	if($mainMenuArrayLenght == 0) $mainMenuArray = null;
	return $mainMenuArray;
}

function getMainMenu_ds18b20_id($id, $objectID){
	$mysqli = connectToDB('SmartHome');
	$result = $mysqli->query('SET NAMES utf8');
	
	
	$query = 'select highColor, lowColor, high, low, name, normColor
	from ds18b20 where objectID = "'.$objectID.'" and id = "'.$id.'"';

	$result = $mysqli->query($query);
	$num_results = $result->num_rows;
	$mysqli->close();
	
	$mainMenuArray[0] = ' ';
	$mainMenuArray[1] = ' ';
	$mainMenuArray[2] = ' ';
	$mainMenuArray[3] = ' ';
	$mainMenuArray[4] = ' ';
	$mainMenuArray[5] = ' ';
	
	if($num_results != 0){
		$row = $result->fetch_assoc();
		$mainMenuArray[0] = $row['name'];
		
		$path = '/uncached/'.$id.'/temperature';
		
		$temp = ownetGet($path);
		$color = $row['normColor'];
		if($temp >=  $row['high']) $color = $row['highColor'];
		if($temp <=  $row['low']) $color = $row['lowColor'];
		
		$status = '<table border="0">
			<tr>
				<td style="background: '.$color.';">
					<b>'.$temp.' &degC</b>
				</td>
			</tr>
			</table>';
		$mainMenuArray[2] = $status;
		$mainMenuArray[5] = $id;
	}
	else $mainMenuArray = null;
	
	if(is_object($result)) $result->free();
	return $mainMenuArray;
}

function getMainMenu_ds2438($objectID, $connectDivecesArray){
	$addedDevicesArray = getAddedDevicesForName('ds2438');
	
	/*echo '$connectDivecesArray ';
	print_r($connectDivecesArray);
	
	echo '$addedDevicesArray ';
	print_r($addedDevicesArray);*/
	
	$countAddedDevices = count($addedDevicesArray);
	
	$mainMenuArrayLenght = 0;
	for($i = 0; $i < $countAddedDevices; $i++){
		if(myArray_search($addedDevicesArray[$i],$connectDivecesArray) !== false){
			//echo 'FIND <br>';
			$mainMenu = getMainMenu_ds2438_id($addedDevicesArray[$i],$objectID);
			if($mainMenu != null){
				$mainMenuArray[$mainMenuArrayLenght] = $mainMenu;
				$mainMenuArrayLenght++;
			}
		}
	}
	
	if($mainMenuArrayLenght == 0) $mainMenuArray = null;
	return $mainMenuArray;
}

function getMainMenu_ds2438_id($id, $objectID){
	$mysqli = connectToDB('SmartHome');
	$result = $mysqli->query('SET NAMES utf8');
	
	
	$query = 'select highColor, lowColor, high, low, name, normColor
	from ds2438 where objectID = "'.$objectID.'" and id = "'.$id.'"';

	$result = $mysqli->query($query);
	$num_results = $result->num_rows;
	$mysqli->close();
	
	$mainMenuArray[0] = ' ';
	$mainMenuArray[1] = ' ';
	$mainMenuArray[2] = ' ';
	$mainMenuArray[3] = ' ';
	$mainMenuArray[4] = ' ';
	$mainMenuArray[5] = ' ';
	
	if($num_results != 0){
		$row = $result->fetch_assoc();
		$mainMenuArray[0] = $row['name'];
		
		$path = '/uncached/'.$id.'/S3-R1-A/current';
		
		$current = ownetGet($path);
		$color = $row['normColor'];
		if($current >=  $row['high']) $color = $row['highColor'];
		if($current <=  $row['low']) $color = $row['lowColor'];
		
		$status = '<table border="0">
			<tr>
				<td style="background: '.$color.';">
					<b>'.$current.'</b>
				</td>
			</tr>
			</table>';
		$mainMenuArray[2] = $status;
		$mainMenuArray[5] = $id;
	}
	else $mainMenuArray = null;
	
	if(is_object($result)) $result->free();
	return $mainMenuArray;
}

function getMainMenu_ds2413p($objectID, $connectDivecesArray){
	$addedDevicesArray = getAddedDevicesForName('ds2413p');
	
	/*echo '$connectDivecesArray ';
	print_r($connectDivecesArray);
	
	echo '$addedDevicesArray ';
	print_r($addedDevicesArray);*/
	
	$countAddedDevices = count($addedDevicesArray);
	
	$mainMenuArrayLenght = 0;
	for($i = 0; $i < $countAddedDevices; $i++){
		if(myArray_search($addedDevicesArray[$i],$connectDivecesArray) !== false){
			//echo 'FIND <br>';
			$mainMenu = getMainMenu_ds2413p_id($addedDevicesArray[$i],$objectID,'ChannelA');
			if($mainMenu != null){
				$mainMenuArray[$mainMenuArrayLenght] = $mainMenu;
				$mainMenuArrayLenght++;
			}
			
			$mainMenu = getMainMenu_ds2413p_id($addedDevicesArray[$i],$objectID,'ChannelB');
			if($mainMenu != null){
				$mainMenuArray[$mainMenuArrayLenght] = $mainMenu;
				$mainMenuArrayLenght++;
			}
		}
	}
	
	if($mainMenuArrayLenght == 0) $mainMenuArray = null;
	return $mainMenuArray;
}

function getMainMenu_ds2413p_id($id, $objectID, $channel){
	$mysqli = connectToDB('SmartHome');
	$result = $mysqli->query('SET NAMES utf8');
	
	
	if($channel == 'ChannelA'){
		$pio = 'PIO.A';
		$sensed = 'sensed.A';
		$isConfigured = 1;
	}
	else{
		$isConfigured = 2;
		$pio = 'PIO.B';
		$sensed = 'sensed.B';
	}
	
	$query = 'select mode'.$channel.', name'.$channel.', color_1_'.$channel.', color_0_'.$channel.', 
	value_1_'.$channel.', value_0_'.$channel.', headerBtn_1_'.$channel.', headerBtn_0_'.$channel.' from ds2413p
	where objectID'.$channel.' = "'.$objectID.'" and (isConfigured = 5 or isConfigured = '.$isConfigured.') and id = "'.$id.'"';

	$result = $mysqli->query($query);
	$num_results = $result->num_rows;
	$mysqli->close();
	
	if($num_results != 0){
		$row = $result->fetch_assoc();
		$mainMenuArray[0] = $row['name'.$channel];
		
		if($row['mode'.$channel] == 'pio'){
			$control = '
			<form name="'.$id.'_'.$channel.'" action="'.CONTROL_DS2413P_FILE.'" method="post">
				<input type="hidden" name="id" value="'.$id.'">
				<input type="hidden" name="channel" value="'.$channel.'">
				<table border="0">
					<tr>
						<td><button name="pio" value="1" onClick="document.forms["'.$id.'_'.$channel.'"].submit()">'.$row['headerBtn_1_'.$channel].'</button></td>
						<td><button name="pio" value="0" onClick="document.forms["'.$id.'_'.$channel.'"].submit()">'.$row['headerBtn_0_'.$channel].'</button></td>
					</tr>
				</table>
			</form>';
			
			$path = '/uncached/'.$id.'/'.$pio;
			$mode = 'Управление';
		}
		
		if($row['mode'.$channel] == 'sensed'){
			$control = ' ';
			$path = '/uncached/'.$id.'/'.$sensed;
			$mode = 'Сенсор';
		}
		
		$mainMenuArray[1] = $control;
		
		if(ownetGet($path)) 
		{
			$status = '<table border="0">
			<tr>
				<td style="background: '.$row['color_1_'.$channel].';">
					'.$row['value_1_'.$channel].'
				</td>
			</tr>
			</table>';
		}
		else{
			$status = '<table border="0">
			<tr>
				<td style="background: '.$row['color_0_'.$channel].';">
					'.$row['value_0_'.$channel].'
				</td>
			</tr>
			</table>';
		}
		
		$mainMenuArray[2] = $status;
		
		$mainMenuArray[3] = ' ';
		$mainMenuArray[4] = $mode;
		$mainMenuArray[5] = $id.'_'.$channel;
	}
	else $mainMenuArray = null;
	
	if(is_object($result)) $result->free();
	return $mainMenuArray;
}

function getMainMenu_ds2406p($objectID, $connectDivecesArray){
	$addedDevicesArray = getAddedDevicesForName('ds2406p');
	
	/*echo '$connectDivecesArray ';
	print_r($connectDivecesArray);
	
	echo '$addedDevicesArray ';
	print_r($addedDevicesArray);*/
	
	$countAddedDevices = count($addedDevicesArray);
	
	$mainMenuArrayLenght = 0;
	for($i = 0; $i < $countAddedDevices; $i++){
		if(myArray_search($addedDevicesArray[$i],$connectDivecesArray) !== false){
			//echo 'FIND <br>';
			$mainMenu = getMainMenu_ds2406p_id($addedDevicesArray[$i],$objectID,'ChannelA');
			if($mainMenu != null){
				$mainMenuArray[$mainMenuArrayLenght] = $mainMenu;
				$mainMenuArrayLenght++;
			}
			
			$mainMenu = getMainMenu_ds2406p_id($addedDevicesArray[$i],$objectID,'ChannelB');
			if($mainMenu != null){
				$mainMenuArray[$mainMenuArrayLenght] = $mainMenu;
				$mainMenuArrayLenght++;
			}
		}
	}
	
	if($mainMenuArrayLenght == 0) $mainMenuArray = null;
	return $mainMenuArray;
}

function getMainMenu_ds2406p_id($id, $objectID, $channel){
	$mysqli = connectToDB('SmartHome');
	$result = $mysqli->query('SET NAMES utf8');
	
	
	if($channel == 'ChannelA'){
		$pio = 'PIO.A';
		$sensed = 'sensed.A';
		$latch = 'latch.A';
		$isConfigured = 1;
	}
	else{
		$isConfigured = 2;
		$pio = 'PIO.B';
		$sensed = 'sensed.B';
		$latch = 'latch.B';
	}
	
	$query = 'select mode'.$channel.', name'.$channel.', color_1_'.$channel.', color_0_'.$channel.', 
	value_1_'.$channel.', value_0_'.$channel.', headerBtn_1_'.$channel.', headerBtn_0_'.$channel.', resetHeader'.$channel.'  from ds2406p
	where objectID'.$channel.' = "'.$objectID.'" and (isConfigured = 5 or isConfigured = '.$isConfigured.') and id = "'.$id.'"';

	$result = $mysqli->query($query);
	$num_results = $result->num_rows;
	$mysqli->close();
	
	if($num_results != 0){
		$row = $result->fetch_assoc();
		$mainMenuArray[0] = $row['name'.$channel];
		
		if($row['mode'.$channel] == 'pio'){
			$control = '
			<form name="'.$id.'_'.$channel.'" action="'.CONTROL_DS2406P_FILE.'" method="post">
				<input type="hidden" name="id" value="'.$id.'">
				<input type="hidden" name="channel" value="'.$channel.'">
				<table border="0">
					<tr>
						<td><button name="pio" value="1" onClick="document.forms["'.$id.'_'.$channel.'"].submit()">'.$row['headerBtn_1_'.$channel].'</button></td>
						<td><button name="pio" value="0" onClick="document.forms["'.$id.'_'.$channel.'"].submit()">'.$row['headerBtn_0_'.$channel].'</button></td>
					</tr>
				</table>
			</form>';
			
			$path = '/uncached/'.$id.'/'.$pio;
			$mode = 'Управление';
		}
		
		if($row['mode'.$channel] == 'latch'){
			$control = '
			<form name="'.$id.'_'.$channel.'" action="'.CONTROL_DS2406P_FILE.'" method="post">
				<input type="hidden" name="id" value="'.$id.'">
				<input type="hidden" name="channel" value="'.$channel.'">
				<table border="0">
					<tr>
						<td><button name="latchReset" value="1" onClick="document.forms["'.$id.'_'.$channel.'"].submit()">'.$row['resetHeader'.$channel].'</button></td>
					</tr>
				</table>
			</form>';
			
			$path = '/uncached/'.$id.'/'.$latch;
			$mode = 'Сенсор-флажок';
		}
		
		if($row['mode'.$channel] == 'sensed'){
			$control = ' ';
			$path = '/uncached/'.$id.'/'.$sensed;
			$mode = 'Сенсор';
		}
		
		$mainMenuArray[1] = $control;
		
		if(ownetGet($path)) 
		{
			$status = '<table border="0">
			<tr>
				<td style="background: '.$row['color_1_'.$channel].';">
					'.$row['value_1_'.$channel].'
				</td>
			</tr>
			</table>';
		}
		else{
			$status = '<table border="0">
			<tr>
				<td style="background: '.$row['color_0_'.$channel].';">
					'.$row['value_0_'.$channel].'
				</td>
			</tr>
			</table>';
		}
		
		$mainMenuArray[2] = $status;
		
		$mainMenuArray[3] = ' ';
		$mainMenuArray[4] = $mode;
		$mainMenuArray[5] = $id.'_'.$channel;
	}
	else $mainMenuArray = null;
	
	if(is_object($result)) $result->free();
	return $mainMenuArray;
}

?>