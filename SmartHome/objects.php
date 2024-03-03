<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
"http://www.w3.org/TR/html4/loose.dtd">
<html>
  <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title>Объекты</title>
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
			
			if(isset($_GET['deleteID'])){
				$objectID = $_GET['deleteID'];
				$mysqli = connectToDB('SmartHome');
				$mysqli->query('SET NAMES utf8');
				$query = 'delete from objects where objectID = '.$objectID;
				$result = $mysqli->query($query);
				//echo $result;
				//echo $query;
				$mysqli->close();
				header('location: objects.php');
				exit();
			}
			
			if(isset($_POST['add'])){
				$object = strip_tags(trim($_POST['objectName']));
				if(strlen($object) != 0){
					$mysqli = connectToDB('SmartHome');
					$mysqli->query('SET NAMES utf8');
					$query = 'insert into objects (name) values("'.$object.'")';
					$result = $mysqli->query($query);
					//echo $result;
					//echo $query;
					$mysqli->close();
				}
			}
			
			/*if(isset($_POST['change'])){
				$objectID = $_POST['change'];
				$mysqli = connectToDB('SmartHome');
				$query = 'delete from objects where objectID = '.$objectID;
				$result = $mysqli->query($query);
				echo $result;
				echo $query;
				$mysqli->close();
			}*/
		
			ShowSiteHeader('Объекты');
			$mysqli = connectToDB('SmartHome');
			$result = $mysqli->query('SET NAMES utf8');
			$query = 'select * from objects';
			$result = $mysqli->query($query);
			$mysqli->close();
			$num_results = $result->num_rows;
			
			echo '<form  action="objects.php" method="post">';
			echo '<table align="center" border="0">';
			
			for($i = 0; $i < $num_results; $i++){
				$row = $result->fetch_assoc();
				echo '<tr><td align="center">'.$row['name'].'</td>
				<td align="center"><a href="objects.php?deleteID='.$row['objectID'].'">удалить</a></td>
				<td align="center"><a href="changeObject.php?changeID='.$row['objectID'].'&name='.$row['name'].'">изменить</a></td>';
			}
			echo '<tr>
				<td colspan="2" ><input type="text" name="objectName" /></td>
				<td align="center"><input type="submit" value="Добавить" name="add" /></td>
			<tr/>';
			
			echo '</table>';
			echo '</form>';
			$result->free();
		?>
		</body>
</html>