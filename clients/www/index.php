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
	<?PHP if (!$USER) { ?>
	<div id="dialogboxDiv">
		<div class="dialog-body">
			<div class="form-logo"></div>
			<div class="form-block">
			<?PHP
			do {
				if ($action == ACTION_NONE || 
				    $action == ACTION_LOGGING ||
					$action == ACTION_LOGFORGET_EMAIL) {
					if ($action == ACTION_LOGGING) {
						if (!$data) {
							print '<label class="form-error">Invalid Username</label>';
						} else {
							if ($data['PASSWORD']!=$_POST['pw'])
								print '<label class="form-error">Invalid Password</label>';
						}
					}
					if ($action == ACTION_LOGFORGET_EMAIL) {
						print '<label class="form-error">'.$LOG_HTML.'</label>';
					}
					
					print '<form action="index.php?action='.ACTION_LOGGING.'" method="post">';        
					print '<label for="name">Username:</label>';
					print '<input type="text" name="name" value="'.(($USER)?$USER['NAME']:'').'"/><br>';
					print '<label for="pw">Password:</label>';
					print '<input type="password" name="pw" /><br>';
					print '<button type="submit">Log in</button>';
					print '<div class="form-extra">';
						print '<a href="index.php?action='.ACTION_CREATE_ACCOUNT.'">Still not a member? click here to register</a>';
						print '<br>';
						print '<a href="index.php?action='.ACTION_LOGFORGET.'">Forgot your password?</a>';
					print '</div>';
					print '</form>';
					break;
				}
			
				if ($action == ACTION_CREATE_ACCOUNT) {
					print '<form action="index.php?action='.ACTION_CREATING_ACCOUNT.'" method="post">';        
					print '<label for="name">Enter your email:</label>';
					print '<input type="text" name="email" /><br>';
					print '<button type="submit">Ask for an account</button>';
					print '</form>';
					break;
				}
				
				if ($action == ACTION_CREATING_ACCOUNT) {
					print $LOG_HTML;        
					break;
				}
				
				if ($action == ACTION_FINISH_ACCOUNT_CREATE) {
					print $LOG_HTML;
					break;
				}
				
				if ($action == ACTION_CHECKING_ACCOUNT || $action == ACTION_FINISH_ACCOUNT_CREATE) {
					print '<form action="index.php?action='.ACTION_FINISH_ACCOUNT_CREATE.'&verif='.$_GET['verif'].'" method="post">';     
					print '<label for="name">Username:</label>';
					print '<input type="text" name="name" value="'.(($USER)?$USER['NAME']:'').'"/><br>';
					print '<label for="pw">Password:</label>';
					print '<input type="password" name="pw" /><br>';
					print '<label for="pw2">Reenter your password:</label>';
					print '<input type="password" name="pw2" /><br>';
					print '<label for="email">Email</label>';
					print '<input type="text" name="email" value="'.$data['EMAIL'].'"/><br>';
					print '<button type="submit">Create Your account</button>';
					print '</form>';
					break;
				}
				
				if ($action == ACTION_LOGFORGET) {
					print '<form action="index.php?action='.ACTION_LOGFORGET_EMAIL.'" method="post">';     
					print '<label for="name">Your Email:</label>';
					print '<input type="text" name="email"/><br>';
					print '<button type="submit">Send me my password</button>';
					print '</form>';
					break;
				}						
								
				print $action;     
				
			} while(0);
			print '</div></div></div></div></body></html>';
			exit();
		?>
			</div>
		</div>
	</div>
	<?PHP } ?>
	<?PHP 
		print '<a href="index.php?action='.ACTION_LOGOUT.'">[logout]</a>';
	?>
</div>
</body>
</html>

