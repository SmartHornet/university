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
			
	require('Functions.php');
	
	if(isset($_POST['addDevice'])){
		$ID = $_POST['addDevice'];
		$tableName = getDeviceTableName($ID);
		
		$mysqli = connectToDB('SmartHome');
		$result = $mysqli->query('SET NAMES utf8');
		
		$query = 'insert into '.$tableName.' (id) values("'.$ID.'")';
		$result = $mysqli->query($query);
		
		$mysqli->close();
		if(is_object($result)) $result->free();
	}
	
	
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
"http://www.w3.org/TR/html4/loose.dtd">
<html>
  <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title>Поиск новых устройств</title>
  </head>
        <body>
		<?php
			ShowSiteHeader('Поиск новых устройств');
			
			$connectDivecesArray = getConnectedDevices();
			$countDevices = count($connectDivecesArray);
			
			$countNewDevices = 0;
			$newDivecesArray[0] = ' '; 
	
			$mysqli = connectToDB('SmartHome');
			$result = $mysqli->query('SET NAMES utf8');
			for($i = 0; $i < $countDevices; $i++){
				$tableName = getDeviceTableName($connectDivecesArray[$i]);
				
			//добавлено ли уже данное устройство в систему?
				$query = 'select id from '.$tableName.' where id = "'.$connectDivecesArray[$i].'"';
				$result = $mysqli->query($query);
				$num_results = $result->num_rows;
				if($num_results == 0) {$newDivecesArray[$countNewDevices] = $connectDivecesArray[$i]; $countNewDevices++;}
			}
			
			//print_r($newDivecesArray);
			
			if($countNewDevices > 0){
				echo '<form action="searchnewdevices.php" method="post">';
				echo '<table align="center" border="2">';
				echo '<tr><th>Уникальный номер</th><th>Название микросхемы</th><th>Описание</th><th>Добавить в систему</th></tr>';
				
				for($i = 0; $i < $countNewDevices; $i++){
					$group = substr($newDivecesArray[$i],0,2);
					$query = 'select microchipName, description from microchipGroups where microchipGroup = "'.$group.'"';
					$result = $mysqli->query($query);
					$row = $result->fetch_assoc();
					
					echo '<tr>
					<td align="center">'.$newDivecesArray[$i].'</td>
					<td align="center">'.$row['microchipName'].'</td>
					<td align="center">'.$row['description'].'</td>
					<td align="center"><button type="submit" value="'.$newDivecesArray[$i].'" name="addDevice">Добавить</button></td>
					</tr>';
				}
				
				echo '</form>';
				echo '</table>';
			}
			else echo'<p align="center">Новых устройст не обнаружено!</p>';
			
			echo '<p align="center"><button onClick=" document.location.href = document.location.href;">Обновить</button></p>';
			
			$mysqli->close();
			if(is_object($result)) $result->free();
			
		?>
		</body>
</html>