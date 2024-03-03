<?php
	session_start();
	unset($_SESSION['valid_user']);
	session_destroy();
	
	if(!isset($_SESSION['valid_user'])){
		header('location: login.php');
		//exit();
	}
	else{
		header('location: index.php');
	}
?>