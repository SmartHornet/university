<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
"http://www.w3.org/TR/html4/loose.dtd">
<html>
  <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title>Панель контроля</title>
  </head>
        <body>
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
			
			ShowSiteHeader('Панель контроля');
			
			$connectDivecesArray = getConnectedDevices();
			
			$mysqli = connectToDB('SmartHome');
			$mysqli->query('SET NAMES utf8');
			$query = 'select * from objects';
			$result = $mysqli->query($query);
			$num_results = $result->num_rows;
			$mysqli->close();
			
			$lenght = 0;
			
			echo '<table align="center" border="1">';
				echo '<tr style="background-color: Aquamarine;"><th>Объект</th><th>Управление</th><th>Состояние</th><th>Сообщение</th><th>Режим</th><th>Устройство</th></tr>';
			
			for($i = 0; $i < $num_results; $i++){
				$row = $result->fetch_assoc();
				$objectID = $row['objectID'];
				$name = $row['name'];
				
				echo '<tr style="background-color: Khaki;"><td colspan="6" align="left"><b>'.$name.'</b></td></tr>';
					
				$mainMenuArray = getMainMenu($objectID, $connectDivecesArray);
				
				$mainMenuArrayLenght = count($mainMenuArray);
				$lenght = $lenght + $mainMenuArrayLenght;
				for($j = 0; $j < $mainMenuArrayLenght; $j++){
					echo '<tr style="background-color: Gainsboro;">
							<td align="center">'.$mainMenuArray[$j][0].'</td>
							<td align="center">'.$mainMenuArray[$j][1].'</td>
							<td align="center">'.$mainMenuArray[$j][2].'</td>
							<td align="center">'.$mainMenuArray[$j][3].'</td>
							<td align="center">'.$mainMenuArray[$j][4].'</td>
							<td align="center">'.$mainMenuArray[$j][5].'</td>
						</tr>';
				}
				
			}
			
			echo '</table>';
			
			if($lenght == 0) echo '<p><h2 style="color: red" align="center">Нет устройств!</h2></p>';
			//ShowSiteHeader('Кутикула');
	
			/*$mysqli = connectToDB('SmartHome');
			
			$query = 'select * from users';
			$result = $mysqli->query($query);
			$num_results = $result->num_rows;
			
			for($i = 0; $i < $num_results; $i++){
				$row = $result->fetch_assoc();
				echo 'userID: '.htmlspecialchars(stripslashes($row['userID'])).'<br>'; 
				echo 'name: '.htmlspecialchars(stripslashes($row['name'])).'<br>';
				echo 'login: '.htmlspecialchars(stripslashes($row['login'])).'<br>';
				echo 'password: '.htmlspecialchars(stripslashes($row['password']));
			}
			
			$result->free();
			$mysqli->close();
			
			error("кутикула");*/
		?>
	</body>
</html>