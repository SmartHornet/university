<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
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
	
	echo "кутикула";
	
	?>
	</body>
	</html>