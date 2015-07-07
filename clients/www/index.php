<?PHP include 'header.php'; ?>
<?PHP include 'php/PHPLog/log.php'; ?>
<html>
<head>
    <title>Share-Counter</title>

    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0", user-scalable=no" />
    <meta name="apple-mobile-web-app-capable" content="yes" />
    <meta charset="utf-8">
	
    <script type="text/javascript" src="script/jquery-1.10.2.min.js"></script>
    <script src= "http://ajax.googleapis.com/ajax/libs/angularjs/1.3.14/angular.min.js"></script>
    <link rel="stylesheet" type="text/css" href="css/style.css" />    
	<link rel="stylesheet" type="text/css" href="css/font-awesome-4.3.0/css/font-awesome.min.css" />    
	
</head>

<body>
<div ng-app="" ng-init="">

	<a href="index.php?action='.ACTION_CREATE_ACCOUNT.'">[register]</a>;
	<?PHP if (!$USER) { ?>
	<div id="dialogboxDiv">
		<div class="dialog-body">
			<?PHP
				print '<form action="index.php?action='.ACTION_LOGGING.'" method="post">';        
				print NAME.':<input type="text" name="name" value="'.(($USER)?$USER['NAME']:'').'"/><br>';
				print PASSWORD.':<input type="password" name="pw" /><br>';
				print '<input type="submit" value="Login"/>';
				print '<button>Register</button>';
				print '</form>';
				?>
			<a href="index.php?action='.ACTION_CREATE_ACCOUNT.'">[register]</a>;
			<div class="dialog-buttons-list">
		</div>
	</div>
	<?PHP } ?>
</div>


</body>
</html>

