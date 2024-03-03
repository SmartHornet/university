<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
"http://www.w3.org/TR/html4/loose.dtd">
<html>
  <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title>Настройка DS18B20</title>
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
			
			if(isset($_POST['delete'])){
				$id = $_POST['id'];
				
				$query = 'delete from '.getDeviceTableName($id).' where id = "'.$id.'"';
				$mysqli = connectToDB('SmartHome');
				$mysqli->query('SET NAMES utf8');
				$mysqli->query($query);
				$mysqli->close();
				header('location: system_setup.php');
				exit();
			}
			
			if(isset($_POST['save'])){
				
				$id = $_POST['id'];
				$name = $_POST['name'];
				$objectID = $_POST['objectID'];
				$highColor = $_POST['highColor'];
				$lowColor = $_POST['lowColor'];
				$normColor = $_POST['normColor'];
				$high = $_POST['high'];
				$low = $_POST['low'];
				
				/*echo '$id '.$id.'<br>';
				echo '$name '.$name.'<br>';
				echo '$objectID '.$objectID.'<br>';
				echo '$highColor '.$highColor.'<br>';
				echo '$lowColor '.$lowColor.'<br>';
				echo '$normColor '.$normColor.'<br>';
				echo '$high '.$high.'<br>';
				echo '$low '.$low.'<br>';*/
				
				$query = 'update '.getDeviceTableName($id).' 
				set name = "'.$name.'", 
				objectID = "'.$objectID.'", 
				highColor = "'.$highColor.'", 
				lowColor = "'.$lowColor.'", 
				normColor = "'.$normColor.'", 
				high = "'.$high.'", 
				low = "'.$low.'"
				where id = "'.$id.'"'; 
				
				$mysqli = connectToDB('SmartHome');
				$mysqli->query('SET NAMES utf8');
				$mysqli->query($query);
				$mysqli->close();
				
				header('location: system_setup.php');
				exit();
			
				/*$channel = $_POST['channel'];
				$mode = $_POST['mode'];
				
				
				
				
				$value_1 = $_POST['value_1'];
				$value_0 = $_POST['value_0'];
				$color_1 = $_POST['color_1'];
				$color_0 = $_POST['color_0'];
				
				$mysqli = connectToDB('SmartHome');
				$mysqli->query('SET NAMES utf8');
				$result = $mysqli->query('select isConfigured from '.getDeviceTableName($id).' where id = "'.$id.'"');
				$row = $result->fetch_assoc();
				$isConfigured = $row['isConfigured'];
				$mysqli->close();
				
				if($isConfigured == 0){
					if($channel == 'ChannelA') $isConfigured = 1;
					else $isConfigured = 2;
				}
				else{
					if($isConfigured == 1){
						if($channel == 'ChannelA') $isConfigured = 1;
						else $isConfigured = 5;
					}
					
					if($isConfigured == 2){
						if($channel == 'ChannelB') $isConfigured = 2;
						else $isConfigured = 5;
					}
				}
				
				$query = 'update '.getDeviceTableName($id).' 
				set isConfigured = '.$isConfigured.', 
				objectID'.$channel.' = '.$objectID.', 
				mode'.$channel.' = "'.$mode.'", 
				name'.$channel.' = "'.$name.'", 
				color_1_'.$channel.' = "'.$color_1.'", 
				color_0_'.$channel.' = "'.$color_0.'", 
				value_1_'.$channel.' = "'.$value_1.'", 
				value_0_'.$channel.' = "'.$value_0.'"';
				
				if($mode == "pio"){
					$headerBtn_1 = $_POST['headerBtn_1'];
					$headerBtn_0 = $_POST['headerBtn_0'];
					$query = $query.', 
					headerBtn_1_'.$channel.' = "'.$headerBtn_1.'", 
					headerBtn_0_'.$channel.' = "'.$headerBtn_0.'"';
				}
				
				$query = $query.' where id = "'.$id.'"';
				
				$mysqli = connectToDB('SmartHome');
				$mysqli->query('SET NAMES utf8');
				$mysqli->query($query);
				$mysqli->close();
				header('location: system_setup.php');
				exit();*/
				
			}
			
			

			/*echo 'mode '.$_POST['mode'].'<br>';
			if(isset($_POST['channel'])) echo 'channel '.$_POST['channel'].'<br>';
			if(isset($_POST['id'])) echo 'id '.$_POST['id'].'<br>';
			if(isset($_POST['name'])) echo 'name '.$_POST['name'].'<br>';
			if(isset($_POST['objectID'])) echo 'objectID '.$_POST['objectID'].'<br>';*/
			
			//echo getSetupMenu('12.4E04B2000000');
			
			if(isset($_POST['id'])) $id = $_POST['id'];
			
			$mysqli = connectToDB('SmartHome');
			$result = $mysqli->query('SET NAMES utf8');
			$query = 'select * from '.getDeviceTableName($id).' where id = "'.$id.'"';
			$result = $mysqli->query($query);
			$row = $result->fetch_assoc();
			
			$objectID = $row['objectID'];
			$name = $row['name'];
			$highColor = $row['highColor'];
			$lowColor = $row['lowColor'];
			$normColor = $row['normColor'];
			$high = $row['high'];
			$low = $row['low'];
			
			//Информация для списка обьектов
			$query = 'select * from objects';
			$result = $mysqli->query($query);
			$num_results = $result->num_rows;
			$mysqli->close();
			
			$objectArray = null;
			$countObjects = $num_results;
			$selectedObject = 0;
			
			for($i = 0; $i < $countObjects; $i++){
				$row = $result->fetch_assoc();
				$objectArray[$i][0]=$row['objectID'];
				$objectArray[$i][1]=$row['name'];
				if($row['objectID'] == $objectID) $selectedObject = $i;
			}
	
		echo '<script type="text/javascript">
			function sendForm(){
				document.forms["form"].submit();
			}
			
			function load(){
				document.getElementById("selectObject").selectedIndex = '.$selectedObject.';
				document.getElementById("name").value = "'.$name.'";
				
				var colorSelectLenght = document.getElementById("highColor").options.length;
				var indexHighColor = 0, indexLowColor = 0, indexNormColor = 0;
				for(var i = 0; i < colorSelectLenght; i++){
					if(document.getElementById("highColor").options[i].value == "'.$highColor.'") indexHighColor = i;
					if(document.getElementById("highColor").options[i].value == "'.$lowColor.'") indexLowColor = i;
					if(document.getElementById("highColor").options[i].value == "'.$normColor.'") indexNormColor = i;
				}
				
				document.getElementById("highColor").selectedIndex = indexHighColor;
				document.getElementById("lowColor").selectedIndex = indexLowColor;
				document.getElementById("normColor").selectedIndex = indexNormColor;
			}
			
		</script>
			
  </head>
        <body onload="load()">';
		
			
			ShowSiteHeader('Настройка DS18B20');
					
			echo '<p><table border="0" align="left">
				<tr>
					<td>
						<form name="form" action="'.CONFIGURE_DS18B20_FILE.'" method="post">
								<input type="hidden" name="id" value="'.$id.'">
								<input type="hidden" name="save" value="save">
								
								<p><b>Название: </b> <input name="name" id="name" value="'.$name.'"></p>
							
								<p><b>Объект: </b><select name="objectID" id="selectObject">';
								for($i = 0; $i < $countObjects; $i++){
										echo '<option value="'.$objectArray[$i][0].'">'.$objectArray[$i][1].'</option>';
								};
								echo '</select></p>
								
								<table border="1" align="left">
									<tr>
										<td>Верхний порог температуры</td>
										<td><input type="number" max="100" value="'.$high.'" name="high"> &degC</td>
									</tr>
									<tr>
										<td>Нижний порог температуры</td>
										<td><input type="number" value="'.$low.'" name="low"> &degC</td>
									</tr>
									<tr>
										<td>Цвет при завышении верхней границы</td>
										<td><select id="highColor" name="highColor">
												<option value="#00ffff">морская волна</option>
												<option value="#000080">тёмно-синий</option>
												<option value="#008000">зелёный</option>
												<option value="#008080">сине-зелёный</option>
												<option value="#0000ff">синий</option>
												<option value="#00ff00">лайм</option>
												<option value="#800080">пурпурный</option>
												<option value="#ffffff">белый</option>
												<option value="#ff00ff">фуксия</option>
												<option value="#ff0000">красный</option>
												<option value="#ffff00">жёлтый</option>
											</select>
										</td>
									</tr>
									<tr>
										<td>Цвет при занижении нижней границы</td>
										<td><select id="lowColor" name="lowColor">
												<option value="#00ffff">морская волна</option>
												<option value="#000080">тёмно-синий</option>
												<option value="#008000">зелёный</option>
												<option value="#008080">сине-зелёный</option>
												<option value="#0000ff">синий</option>
												<option value="#00ff00">лайм</option>
												<option value="#800080">пурпурный</option>
												<option value="#ffffff">белый</option>
												<option value="#ff00ff">фуксия</option>
												<option value="#ff0000">красный</option>
												<option value="#ffff00">жёлтый</option>
											</select>
										</td>
									</tr>
									<tr>
										<td>Цвет при норме</td>
										<td><select id="normColor" name="normColor">
												<option value="#00ffff">морская волна</option>
												<option value="#000080">тёмно-синий</option>
												<option value="#008000">зелёный</option>
												<option value="#008080">сине-зелёный</option>
												<option value="#0000ff">синий</option>
												<option value="#00ff00">лайм</option>
												<option value="#800080">пурпурный</option>
												<option value="#ffffff">белый</option>
												<option value="#ff00ff">фуксия</option>
												<option value="#ff0000">красный</option>
												<option value="#ffff00">жёлтый</option>
											</select>
										</td>
									</tr>
								</table>
						</form>
					</td>	
				</tr>
					
				<tr>
					<td align="center">
						<p><button style="margin-top: 20px" onClick="sendForm();">Сохранить</button></p>
					</td>
				</tr>
			
			</table>
			</p>
		</body>
</html>';
?>