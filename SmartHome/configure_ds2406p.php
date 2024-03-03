<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
"http://www.w3.org/TR/html4/loose.dtd">
<html>
  <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title>Настройка DS2406P</title>
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
				$channel = $_POST['channel'];
				$mode = $_POST['mode'];
				$id = $_POST['id'];
				$name = $_POST['name'];
				$objectID = $_POST['objectID'];
				
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
				
				if($mode == "latch"){
					$resetHeader = $_POST['resetHeader'];
					$query = $query.', 
					resetHeader'.$channel.' = "'.$resetHeader.'"';
				}
				$query = $query.' where id = "'.$id.'"';
				
				$mysqli = connectToDB('SmartHome');
				$mysqli->query('SET NAMES utf8');
				$mysqli->query($query);
				$mysqli->close();
				header('location: system_setup.php');
				exit();
				
			}
			
			

			/*echo 'mode '.$_POST['mode'].'<br>';
			if(isset($_POST['channel'])) echo 'channel '.$_POST['channel'].'<br>';
			if(isset($_POST['id'])) echo 'id '.$_POST['id'].'<br>';
			if(isset($_POST['name'])) echo 'name '.$_POST['name'].'<br>';
			if(isset($_POST['objectID'])) echo 'objectID '.$_POST['objectID'].'<br>';*/
			
			//echo getSetupMenu('12.4E04B2000000');
			
			if(isset($_POST['id'])) $id = $_POST['id'];
			if(isset($_POST['channelA'])) {$showChannel = 'Канал А'; $channel = 'ChannelA';}
			if(isset($_POST['channelB'])) {$showChannel = 'Канал Б'; $channel = 'ChannelB';}
			
			$mysqli = connectToDB('SmartHome');
			$result = $mysqli->query('SET NAMES utf8');
			$query = 'select * from '.getDeviceTableName("12").' where id = "'.$id.'"';
			$result = $mysqli->query($query);
			$row = $result->fetch_assoc();
			
			$isConfigured = $row['isConfigured'];
			$objectID = $row['objectID'.$channel];
			$mode = $row['mode'.$channel];
			$name = $row['name'.$channel];
			$color_1 = $row['color_1_'.$channel];
			$color_0 = $row['color_0_'.$channel];
			$value_1 = $row['value_1_'.$channel];
			$value_0 = $row['value_0_'.$channel];
			$headerBtn_1 = $row['headerBtn_1_'.$channel];
			$headerBtn_0 = $row['headerBtn_0_'.$channel];
			$resetHeader = $row['resetHeader'.$channel];
			
			/*echo $isConfigured.'<br>' ;
			echo $objectID.'<br>' ;
			echo $mode.'<br>';
			echo $name.'<br>';
			echo $color_1.'<br>';*/
			
			
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
				if($isConfigured != 0){
					if(($isConfigured == 5) || (($isConfigured == 1) && ($channel == "ChannelA")) || (($isConfigured == 2) && ($channel == "ChannelB")))
					if($row['objectID'] == $objectID) $selectedObject = $i;
				}
			}
			
			//echo '$selectedObject '.$selectedObject.'<br>';
			
			//текущий режим
			$selectedMode = 0; $form = "pio";
			if($mode == "sensed") {$selectedMode = 1; $form = "sensed";}
			if($mode == "latch") {$selectedMode = 2; $form = "latch";}
	
		echo '<script type="text/javascript">
			function changeMode(value){
				var pio = "none",sensed = "none", latch = "none";
				if(value == "pio") pio = "block";
				if(value == "sensed") sensed = "block";
				if(value == "latch") latch = "block";
				
				document.getElementById("divForm_pio").style.display = pio;
				document.getElementById("divForm_sensed").style.display = sensed;
				document.getElementById("divForm_latch").style.display = latch;
			}
			
			function sendForm(){
				var divpio = document.getElementById("divForm_pio");
				var divsensed = document.getElementById("divForm_sensed");
				var divlatch = document.getElementById("divForm_latch");
				
				name = document.getElementById("name").value;
				selectObject = document.getElementById("selectObject").options[document.getElementById("selectObject").selectedIndex].value;
				
				
				if(divpio.style.display == "block"){
					document.getElementById("name_pio").value = name;
					document.getElementById("objectID_pio").value = selectObject;
					document.forms["form_pio"].submit();
					//alert("pio");
				}
				
				if(divsensed.style.display == "block"){
					document.getElementById("name_sensed").value = name;
					document.getElementById("objectID_sensed").value = selectObject;
					document.forms["form_sensed"].submit();
					//alert("sensed");
				}
				
				if(divlatch.style.display == "block"){
					document.getElementById("name_latch").value = name;
					document.getElementById("objectID_latch").value = selectObject;
					document.forms["form_latch"].submit();
					//alert("latch");
				}
			}';
			
			 echo 'function load(){
				
				document.getElementById("selectMode").selectedIndex = '.$selectedMode.';
				document.getElementById("selectObject").selectedIndex = '.$selectedObject.';
				document.getElementById("name").value = "'.$name.'";
				
				document.getElementById("divForm_'.$form.'").style.display = "block";
				
				var colorSelectLenght = document.getElementById("color_1_0").options.length;
				var indexColor_1 = 0, indexColor_0 = 0;
				for(var i = 0; i < colorSelectLenght; i++){
					if(document.getElementById("color_1_0").options[i].value == "'.$color_1.'") indexColor_1 = i;
					if(document.getElementById("color_1_0").options[i].value == "'.$color_0.'") indexColor_0 = i;
				}
				
				document.getElementById("color_1_0").selectedIndex = indexColor_1;
				document.getElementById("color_1_1").selectedIndex = indexColor_1;
				document.getElementById("color_1_2").selectedIndex = indexColor_1;
				
				document.getElementById("color_0_0").selectedIndex = indexColor_0;
				document.getElementById("color_0_1").selectedIndex = indexColor_0;
				document.getElementById("color_0_2").selectedIndex = indexColor_0;
			}';
			
		echo '</script>
			
  </head>
        <body onload="load()">';
		
			
			ShowSiteHeader('Настройка DS2406P');
				
			echo '<h2 align="left">'.$showChannel.'</h2>';
			
			echo '<p><b>Название: </b>
				<input id="name" value="'.$name.'"></p>';
			
			echo '<p><b>Объект: </b><select id="selectObject">';
			for($i = 0; $i < $countObjects; $i++){
				echo '<option value="'.$objectArray[$i][0].'">'.$objectArray[$i][1].'</option>';
			}
			echo '</select></p>';
				
			
					
			echo '<p><b>Режим работы: </b><select id="selectMode" onChange="changeMode(this.value)">
					<option value="pio">Управление</option>
					<option value="sensed">Сенсор</option>
					<option value="latch">Сенсор-флажок</option>
				</select></p>';
					
			echo '<p><table border="0" align="left">
				<tr>
					<td>';
						echo '<div id="divForm_pio" style="display:none" >
							<form name="form_pio" action="'.CONFIGURE_DS2406P_FILE.'" method="post">
								<input type="hidden" name="mode" value="pio">
								<input type="hidden" name="channel" value="'.$channel.'">
								<input type="hidden" name="id" value="'.$id.'">
								<input id="name_pio" type="hidden" name="name" value="'.$name.'">
								<input id="objectID_pio" type="hidden" name="objectID" value="'.$objectID.'">
								<input type="hidden" name="save" value="save">
								<table border="1" align="left">
								<tr>
									<td>Заголовок для кнопки установки в "1"</td>
									<td><input type="text" value="'.$headerBtn_1.'" name="headerBtn_1"></td>
								</tr>
								<tr>
									<td>Заголовок для кнопки установки в "0"</td>
									<td><input type="text" value="'.$headerBtn_0.'" name="headerBtn_0"></td>
								</tr>
								<tr>
									<td>Значение поля для логической "1"</td>
									<td><input type="text" value="'.$value_1.'" name="value_1"></td>
								</tr>
								<tr>
									<td>Значение поля для логического "0"</td>
									<td><input type="text" value="'.$value_0.'" name="value_0"></td>
								</tr>
								<tr>
									<td>Цвет поля для логической "1"</td>
									<td><select id="color_1_0" name="color_1">
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
									<td>Цвет поля для логического "0"</td>
									<td><select id="color_0_0" name="color_0">
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
						</div>
						';
					
						echo '<div id="divForm_sensed" style="display:none" >
							<form name="form_sensed" action="'.CONFIGURE_DS2406P_FILE.'" method="post">
								<input type="hidden" name="mode" value="sensed">
								 <input type="hidden" name="channel" value="'.$channel.'">
								 <input type="hidden" name="id" value="'.$id.'">
								 <input id="name_sensed" type="hidden" name="name" value="'.$name.'">
								 <input id="objectID_sensed" type="hidden" name="objectID" value="'.$objectID.'">
								 <input type="hidden" name="save" value="save">
								<table border="1" align="left">
								<tr>
									<td>Значение поля для логической "1"</td>
									<td><input type="text" value="'.$value_1.'" name="value_1"></td>
								</tr>
								<tr>
									<td>Значение поля для логического "0"</td>
									<td><input type="text" value="'.$value_0.'" name="value_0"></td>
								</tr>
								<tr>
									<td>Цвет поля для логической "1"</td>
									<td><select id="color_1_1" name="color_1">
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
									<td>Цвет поля для логического "0"</td>
									<td><select id="color_0_1" name="color_0">
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
						</div>
						';
					
						echo '<div id="divForm_latch" style="display:none" >
							<form name="form_latch" action="'.CONFIGURE_DS2406P_FILE.'" method="post">
								<input type="hidden" name="mode" value="latch">
								<input type="hidden" name="channel" value="'.$channel.'">
								<input type="hidden" name="id" value="'.$id.'">
								<input id="name_latch" type="hidden" name="name" value="'.$name.'">
								<input id="objectID_latch" type="hidden" name="objectID" value="'.$objectID.'">
								<input type="hidden" name="save" value="save">
								<table border="1" align="left">
								<tr>
									<td>Значение поля для логической "1"</td>
									<td><input type="text" value="'.$value_1.'" name="value_1"></td>
								</tr>
								<tr>
									<td>Значение поля для логического "0"</td>
									<td><input type="text" value="'.$value_0.'" name="value_0"></td>
								</tr>
								<tr>
									<td>Цвет поля для логической "1"</td>
									<td><select id="color_1_2" name="color_1">
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
									<td>Цвет поля для логического "0"</td>
									<td><select id="color_0_2" name="color_0">
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
									<td>Заголовок для кнопки сброса в "0"</td>
									<td><input type="text" value="'.$resetHeader.'" name="resetHeader"></td>
								</tr>
								</table>
							</form>
						</div>
					</td>	
				</tr>';
					
			echo '<tr>
				<td align="center">
					<p><button style="margin-top: 20px" onClick="sendForm();">Сохранить</button></p>
				</td>
			</tr>
			';
			echo '</table>
			</p>
			';
			
		echo '</body>
</html>';
?>