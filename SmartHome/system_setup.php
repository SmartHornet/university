<?php
	session_start();
	if(!isset($_SESSION['valid_user'])){
		header('location: login.php');
		exit();
	}
			
	error_reporting(E_ALL | E_STRICT);
	if (ini_get('display_errors') != 1){
	ini_set('display_errors', 1);
	};
			
	require_once('Functions.php');
	
	/*if(isset($_POST['addDevice'])){
		$ID = $_POST['addDevice'];
		$tableName = getDeviceTableName($ID);
		
		$mysqli = connectToDB('SmartHome');
		$result = $mysqli->query('SET NAMES utf8');
		
		$query = 'insert into '.$tableName.' (id) values("'.$ID.'")';
		$result = $mysqli->query($query);
		
		$mysqli->close();
		if(is_object($result)) $result->free();
	}*/
	
	
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
"http://www.w3.org/TR/html4/loose.dtd">
<html>
  <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title>Настройка системы</title>
		<style type="text/css">
		td.red{
			background: red;
		}
		.lime{
			background: lime;
		}
		</style>
  </head>
        <body>
		<?php
			ShowSiteHeader('Настройка системы');
		 
			$addedDevicesArray = getAddedDevices();
			$connectDivecesArray = getConnectedDevices();
			$countAddedDevices = count($addedDevicesArray);
			
			/*echo '$addedDevicesArray ';
			print_r($addedDevicesArray);
			
			echo '<br>$connectDivecesArray ';
			print_r($connectDivecesArray);*/
			
			if($countAddedDevices != 0){
				echo '<table align="center" border="2">';
				echo '<tr><th>Уникальный номер</th><th>Сообщение</th><th>Доступность</th><th>Настройка</th></tr>';
				
				for($i = 0; $i < $countAddedDevices; $i++){
					echo '<tr>
						<td align="center">'.$addedDevicesArray[$i].'</td>
						<td align="center"> </td>';
						if(myArray_search($addedDevicesArray[$i],$connectDivecesArray) === false) echo '<td class="red" align="center">Нет соединения!</td>';
						else echo '<td class="lime" align="center">Подключено</td>';
						echo '<td align="center">'.getSetupMenu($addedDevicesArray[$i]).'</td>
					</tr>';
				}
				echo '</table>';
			}
			else echo'<p align="center">Нет добавленных устройств!</p>';
			
			echo '<p align="center"><button onClick=" document.location.href = document.location.href;">Обновить</button></p>';
			
			//echo '<p align="center"><button onClick=" document.location.href = document.location.href;">Обновить</button></p>';
		?>
		</body>
</html>