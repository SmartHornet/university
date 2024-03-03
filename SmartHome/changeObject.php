<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
"http://www.w3.org/TR/html4/loose.dtd">
<html>
  <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title>Объекты</title>
  </head>
        <body>
		<?php
			//echo '<pre>'.var_dump($_POST).'</pre>';
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
			
			if(isset($_POST['save'])){
				$object = strip_tags(trim($_POST['objectName']));
				if(strlen($object) != 0){
					$changeID = $_POST['changeID'];
					$mysqli = connectToDB('SmartHome');
					$mysqli->query('SET NAMES utf8');
					$query = "update objects set name = '".$object."' where objectID = ".$changeID;
					$result = $mysqli->query($query);
					//echo $result;
					//echo $query;
					$mysqli->close();
				}
				header('location: objects.php');
				exit();
			}
			
			if(isset($_GET['changeID'])){
			ShowSiteHeader('Изменение обьекта '.$_GET['name']);
			
			echo '<form action="changeObject.php" method="post">';
			echo '<table align="left" border="0">';
			echo '<tr><td><input type="text" name="objectName" value="'.$_GET['name'].'"/></td>';
			echo '<td align="center"><input type="submit" value="Сохранить" name="save" /></td></tr>';
			echo '</table>';
			echo '<input type="hidden" value="'.$_GET['changeID'].'" name="changeID" />';
			echo '</form>';		
	}
?>
		</body>
</html>