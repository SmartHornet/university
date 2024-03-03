<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
"http://www.w3.org/TR/html4/loose.dtd">
<html>
  <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title>Авторизация</title>
  </head>
        <body>
		<?php
			session_start();
	
			error_reporting(E_ALL | E_STRICT);
			if (ini_get('display_errors') != 1){ini_set('display_errors', 1);};
			
			require('Functions.php');
			
			$login = strip_tags(trim($_POST['login']));
			$password = strip_tags(trim($_POST['password']));
			//echo $login; echo '<br>';
			//echo $password;
			//echo $_POST['login'].'<br>';
			//echo $_POST['password'];
			
			if(strlen($login) > 0 && strlen($password) > 0){
				$mysqli = connectToDB('SmartHome');
				$query = 'select * from users where login="'.$login.'" and password=SHA1("'.$password.'")';
				//echo $query;
				$result = $mysqli->query($query);
				$num_results = $result->num_rows;
				$result->free();
				$mysqli->close();
				if($num_results > 0){
					$_SESSION['valid_user'] = $login;
					header('location: index.php'); exit();
				}
			}
			
			header('location: login.php');
		?>
		</body>
</html>
