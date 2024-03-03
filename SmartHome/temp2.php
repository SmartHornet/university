<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
"http://www.w3.org/TR/html4/loose.dtd">
<html>
  <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title>Объектыg</title>
		
  </head>
        <body> 
	
		<?php
		error_reporting(E_ALL | E_STRICT);
		if (ini_get('display_errors') != 1){
			ini_set('display_errors', 1);
		};
			
			require('Functions.php'); 
			
			
			//var_dump(getAddedDevicesForName('ds2438'));
			var_dump(getConnectedDevices());
			
		?>
		</body>
</html>