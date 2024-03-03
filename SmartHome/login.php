<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
"http://www.w3.org/TR/html4/loose.dtd">
<html>
  <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title>Вход в систему</title>
  </head>
        <body>
			<form action="auth.php" method="post">
				<table align="center" border="0">
					<tr><td colspan="2" align="center"><h2>Вход в систему</h2></td></tr>
					<tr>
						<td align="right">Логин:</td>
						<td align="left"><input type="text" name="login"/></td>
					</tr>
					<tr>
						<td align="right">Пароль:</td>
						<td align="left"><input type="password" name="password"/></td>
					</tr>
					<tr><td colspan="2" align="center"><input type="submit" value="Войти"/></td></tr>
				</table>
			</form>
		</body>
</html>