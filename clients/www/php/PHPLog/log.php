<?PHP
require("php/PHPMailer/class.phpmailer.php");

define('ACTION_CLASS_LOG',    100);

define('ACTION_LOGIN',                  ACTION_CLASS_LOG+0);
define('ACTION_LOGOUT',                 ACTION_CLASS_LOG+1);
define('ACTION_LOGGING',                ACTION_CLASS_LOG+2);
define('ACTION_CREATE_ACCOUNT',         ACTION_CLASS_LOG+3);
define('ACTION_CREATING_ACCOUNT',       ACTION_CLASS_LOG+4);
define('ACTION_CHECKING_ACCOUNT',       ACTION_CLASS_LOG+5);
define('ACTION_FINISH_ACCOUNT_CREATE',  ACTION_CLASS_LOG+6);
define('ACTION_LOGFORGET',              ACTION_CLASS_LOG+7);
define('ACTION_LOGFORGET_EMAIL',        ACTION_CLASS_LOG+8);
define('ACTION_ACCOUNT_SETTING',        ACTION_CLASS_LOG+9);
define('ACTION_ACCOUNT_FINISH_SETTING', ACTION_CLASS_LOG+10);

if ( (isset ($_GET['verif'])) && ($action == ACTION_NONE)) $action = ACTION_CHECKING_ACCOUNT;

// Strings
define('ENTER_A_NAME' , 'Choose a username');
define('ENTER_A_PASSWORD' , 'Choose a password');
define('REENTER_A_PASSWORD' , 're-enter your password');
define('EMAIL' , 'email');
define('NAME' , 'Username');
define('PASSWORD' , 'password');
define('FORGET' , 'Forget?');
define('MAIL_SENT_TO','A mail has been sent to ');
define('MAIL_GREETING','Welcome on Humanitas communauty! please click on the following link to valid your account.<br>');

define('ERROR_MAIL','<span class="error">Mail error</span><br>');
define('ERROR_PLZ_REGISTER','<span class="error">Invalid request, please re-register</span><br>');
define('ERROR_UNKNOWM_EMAIL','<span class="error">Unknown email, sorry</span><br>');
define('ERROR_EMAIL_EXIST','<span class="error">Email already exist</span><br>');
define('ERROR_INVALID_EMAIL','<span class="error">Invalid email</span><br>');
define('ERROR_INVALID_NAME','<span class="error">Invalid username</span><br>');
define('ERROR_NAME_EXIST','<span class="error">Username already exist</span><br>');
define('ERROR_INVALID_PASSWORD','<span class="error">Invalid password</span><br>');
define('ERROR_INVALID_PASSWORD_NO_MATCH','<span class="error">Invalid password (not match)</span><br>');
define('ERROR_INVALID_CHARACTER','<span class="error">Invalid character</span><br>');

define('SESSION_TIMEOUT_MINUTES',60);

$USER = NULL;
$EMB = false;



// SESSION CREATION
session_start();

if (isset($_SESSION['clientUpdate']))
{

	// dirty fix: never logout is embedded
	if (!isset($_GET['embedded']))
	{
		
		if ((time()-$_SESSION['clientUpdate'])> 60*SESSION_TIMEOUT_MINUTES)
		{
			if (isset($_SESSION['authkey']))  unset($_SESSION['authkey']);
			if (isset($_SESSION['embedded'])) unset($_SESSION['authkey']);
			if (isset($_SESSION['clientIP'])) unset($_SESSION['clientIP']);
			if (isset($_SESSION['clientUpdate'])) unset($_SESSION['clientUpdate']);
			session_destroy();
			print ('<p>Session timeout</p>');
		}
	}
}

$_SESSION['clientIP'] = $_SERVER['REMOTE_ADDR'];
$_SESSION['clientUpdate'] = time(); 
	 
	 
// Login/Logoff
function logIn($key)
{
    $authkey = NULL;

    if (isset($_GET['embedded']))
        $_SESSION['embedded'] = ($_GET['embedded']=='true');
	if (isset($_SESSION['embedded']))
		$GLOBALS['EMB'] = $_SESSION['embedded'];
	
    if (isset($_SESSION['authkey']))
        $authkey = $_SESSION['authkey'];
    if (isset($_GET['authkey'])) 
        $authkey = $_GET['authkey'];
     if ($key)
        $authkey = $key;
	
    if ($authkey) {
		
        $sql = 'SELECT * FROM `users` WHERE UUID="'.$authkey.'"';
        $req = mysql_query($sql) or sqldie($sql);
        $data = mysql_fetch_assoc($req);
        if (!$data)
            logOff();
        $_SESSION['authkey'] = $data['UUID'];
        $GLOBALS['USER'] = $data;
    } else {
        logOff();
    }
}

function logOff() {
    $GLOBALS['USER'] = NULL;
    if (isset($_SESSION['authkey'])) unset($_SESSION['authkey']);
}






logIn(NULL);
$html = '';

switch($action)
{
    case ACTION_ACCOUNT_FINISH_SETTING:
        if ($EMB) break; // forbidden in embedded mode
		if ( (!$USER) || (!isset ($_POST['name'])) || (!isset ($_POST['pw'])) || (!isset ($_POST['pw2'])) || (!isset ($_POST['email'])) ) 
           break;
        if (!($err = check_user_account_setting($_POST,$USER)))
        {
            $sql = "UPDATE `users` SET "
                        ." `NAME` =  '".$_POST['name']."',"
                        ." `PASSWORD` =  '".$_POST['pw']."',"
                        ." `EMAIL` =  '".$_POST['email']."'"
                    ." WHERE `UUID` =  '".$USER['UUID']."'";
                    
            $req = mysql_query($sql) or sqldie($sql);  
            $USER['NAME']     = $_POST['name'];
            $USER['PASSWORD'] = $_POST['pw'];
            $USER['EMAIL']    = $_POST['email'];
			
			// finish account setting
		    $action=0;
		    print "<script>location.href='index.php'</script>"; 
		    print "\n";
            break;
        } else {
          $html .= $err;
        }
        break;
        

    case ACTION_ACCOUNT_SETTING:
        if ($EMB) break; // forbidden in embedded mode
		if (!$USER)
           break;
		
        $sql = 'SELECT * FROM `users` WHERE UUID="'.$USER['UUID'].'"';
        $req = mysql_query($sql) or sqldie($sql);  
        $data = mysql_fetch_assoc($req);
        if (!$data) {
            die("internal error");
            break;
        }
        
        $html .= '<form action="index.php?action='.ACTION_ACCOUNT_FINISH_SETTING.'" method="post">';    
        $html .= ENTER_A_NAME.':<input type="text" name="name" value="'.$data['NAME'].'"/><br>';        
        $html .= ENTER_A_PASSWORD.':<input type="password" name="pw" value="'.$data['PASSWORD'].'"/><br>';        
        $html .= REENTER_A_PASSWORD.':<input type="password" name="pw2" value="'.$data['PASSWORD'].'"/><br>';
        $html .= EMAIL.': <input type="text" name="email" value="'.$data['EMAIL'].'"/><br>';                
        $html .= '<input type="submit" />';
        $html .= '</form>';
        break;
        
        
    case ACTION_LOGFORGET_EMAIL:
		if ($EMB) break; // forbidden in embedded mode
        logOff();
        if (!isset ($_POST['email']))
            break;
            
        $sql = 'SELECT * FROM `users` WHERE EMAIL="'.$_POST['email'].'"';
        $req = mysql_query($sql) or sqldie($sql);  
        $data = mysql_fetch_assoc($req);
        if (!$data) {
            $html .= ERROR_UNKNOWM_EMAIL;
            break;
        }
        
        //send mail        
		$message  = "<html><body>";
		$message .= NAME.': '.$data['NAME'].'<br>';
		$message .= PASSWORD.': '.$data['PASSWORD'].'<br>';
		$message .="</body></html>";
		if(sendMail($_POST['email'], "Humanitas account", $message)) {
			$html .= MAIL_SENT_TO.$_POST['email']."<br>";
		} else {
			$html .= ERROR_MAIL;
		}       

        break;
         

    case ACTION_LOGFORGET:
		if ($EMB) break; // forbidden in embedded mode
        logOff();

        $html .= '<form action="index.php?action='.ACTION_LOGFORGET_EMAIL.'" method="post">';    
        $html .= EMAIL.': <input type="text" name="email"/><br>';                
        $html .= '<input type="submit" />';
        $html .= '</form>';        
        break;
        

    case ACTION_FINISH_ACCOUNT_CREATE:
		if ($EMB) break; // forbidden in embedded mode
        logOff();
		
        if ((!isset ($_POST['name'])) || (!isset ($_POST['pw'])) || (!isset ($_POST['pw2'])) || (!isset ($_POST['email']))) 
            break;
        
		
        if (!($err = check_user_account_setting($_POST,$USER)))
        {
            $uuid = guid();
            $sql = "INSERT INTO `users` (`UUID`, `NAME`, `PASSWORD`, `EMAIL`) VALUES ("
                    ."'".$uuid."',"
                    ."'".$_POST['name']."',"
                    ."'".$_POST['pw']."',"
                    ."'".$_POST['email']."');";
            
            $req = mysql_query($sql) or sqldie($sql);  
            login($uuid);
			
			// finish account creation
		    $action=0;
		    print "<script>location.href='index.php'</script>"; 
		    print "\n";
            break;
			
        } else {
          $html .= $err;
        }
        // else NO BREAK! (to allow to redo-config)
        
        
    case ACTION_CHECKING_ACCOUNT:
		if ($EMB) break; // forbidden in embedded mode
        logOff();

        // check id
        $sql = 'SELECT * FROM `tmp_users` WHERE UUID="'.$_GET['verif'].'"';
        $req = mysql_query($sql) or sqldie($sql);  
        $data = mysql_fetch_assoc($req);
        if (!$data) {
            $html .= ERROR_PLZ_REGISTER;
            break;
        }
        
        $html .= '<form action="index.php?action='.ACTION_FINISH_ACCOUNT_CREATE.'&verif='.$_GET['verif'].'" method="post">';    
        $html .= ENTER_A_NAME.':<input type="text" name="name" value="'.((isset($_POST['name']))?$_POST['name']:'').'"/><br>';        
        $html .= ENTER_A_PASSWORD.':<input type="password" name="pw" /><br>';        
        $html .= REENTER_A_PASSWORD.':<input type="password" name="pw2" /><br>';
        $html .= EMAIL.': <input type="text" name="email" value="'.$data['EMAIL'].'"/><br>';                
        $html .= '<input type="submit" />';
        $html .= '</form>';
        break;
        
        
    case ACTION_CREATING_ACCOUNT:
		if ($EMB) break; // forbidden in embedded mode
        logOff();

        if (!isset ($_POST['email']))
            break;

        if (!check_email_address($_POST['email'])) {
            $html .= ERROR_INVALID_EMAIL;
            break;
        }
        
        $sql = 'SELECT * FROM `users` WHERE EMAIL="'.$_POST['email'].'"';
        $req = mysql_query($sql) or sqldie($sql);  
        $data = mysql_fetch_assoc($req);
        if ($data) {
            $html .= ERROR_EMAIL_EXIST;
            break;
        }

        // save request
        $uid = guid();

        $sql = "INSERT INTO `tmp_users` (`UUID`, `EMAIL`) VALUES ('".$uid."', '".$_POST['email']."');";
        $req = mysql_query($sql) or sqldie($sql);  
        $verifurl = 'http://'.$_SERVER['HTTP_HOST'].'/'.$_SERVER['PHP_SELF'].'?verif='.$uid;
        
        //send mail        
        if ($GLOBALS['DEVELOPEMENT_MODE']) {
            $html .=('<a href="'.$verifurl.'">'.$verifurl.'</a><br>');
        }
		
		$message  = "<html><body>";
		$message .= MAIL_GREETING;
		$message .='<a href="'.$verifurl.'">'.$verifurl.'</a>';
		$message .="</body></html>";
		if(sendMail($_POST['email'], "HumanITas account register", $message)) {
			$html .= MAIL_SENT_TO.$_POST['email']."<br>";
		} else {
			$html .= ERROR_MAIL;
		}            

        break;
        

    case ACTION_CREATE_ACCOUNT:
		if ($EMB) break; // forbidden in embedded mode
        logOff();

        $html .= '<form action="index.php?action='.ACTION_CREATING_ACCOUNT.'" method="post">';        
        $html .= EMAIL.':<input type="text" name="email" /><br>';
        $html .= '<input type="submit" />';
        $html .= '</form>';
        break;
            

    case ACTION_LOGGING:
		if ($EMB) break; // forbidden in embedded mode
        logOff();

        if ((!isset ($_POST['name'])) || (!isset ($_POST['pw'])))
            break;
                
        $sql = 'SELECT * FROM `users` WHERE NAME="'.$_POST['name'].'"';
        $req = mysql_query($sql) or sqldie($sql);  
        $data = mysql_fetch_assoc($req);
        if (!$data) {
            $html .= ERROR_INVALID_NAME;
            $html .= '<a href="index.php?action='.ACTION_LOGFORGET.'">'.FORGET.'</a><br>';
            break;
        }
        if ($data['PASSWORD']!=$_POST['pw']) {
            $html .= ERROR_INVALID_PASSWORD;
            $html .= '<a href="index.php?action='.ACTION_LOGFORGET.'">'.FORGET.'</a><br>';
            break;
        }

        logIn($data['UUID']);
		// finish loggin
		$action=0;
		print "<script>location.href='index.php'</script>"; 
		print "\n";
        break;
        

    case ACTION_LOGIN:
		if ($EMB) break; // forbidden in embedded mode
        $html .= '<form action="index.php?action='.ACTION_LOGGING.'" method="post">';        
        $html .= NAME.':<input type="text" name="name" value="'.(($USER)?$USER['NAME']:'').'"/><br>';
        $html .= PASSWORD.':<input type="password" name="pw" /><br>';
        $html .= '<input type="submit" />';
        $html .= '</form>';		
        break;    

    case ACTION_LOGOUT:	
		if ($EMB) break; // forbidden in embedded mode
        logOff();
		// finish logout
		$action=0;
		print "<script>location.href='index.php'</script>"; 
		print "\n";
        break;
}


print '<div id="log_header">';
if (!$EMB)
{
	if ($USER) {
		print '<a href="index.php?action='.ACTION_LOGOUT.'">['.$USER['NAME'].' logout]</a>';
		print '<a href="index.php?action='.ACTION_ACCOUNT_SETTING.'">[setting]</a>';
	} else {
		print '<a href="index.php?action='.ACTION_LOGIN.'">[login]</a> ';
		print '<a href="index.php?action='.ACTION_CREATE_ACCOUNT.'">[register]</a>';
	}
} else {
	if ($USER) print 'Welcome '.$USER['NAME'].' !';
}
print '</div>';

print '<div style="clear:both;"></div>';
print $html;
print "\n";


// --- LOG HELPER ---

function check_user_account_setting($ARRAY, $ORIGNAL) {

    if (isset($ARRAY['name']))
    {
        if ((strlen($ARRAY['name'])<1) || (strlen($ARRAY['name'])>20)) {
            return ERROR_INVALID_NAME;
        }

		if (strpos($ARRAY['name'],"'")) 
			return ERROR_INVALID_CHARACTER;
		
        $sql = 'SELECT * FROM `users` WHERE NAME="'.$ARRAY['name'].'"';
        $req = mysql_query($sql) or sqldie($sql);  
        $data = mysql_fetch_assoc($req);
        if ($data) {
            if (isset($ORIGNAL)) 
            {
                if ($ORIGNAL['NAME']!=$data['NAME']) {
                    return ERROR_NAME_EXIST;
                }
            } else {
                return ERROR_NAME_EXIST;
            }
        }
    }
    
    if (isset($ARRAY['email']))
    {        
		if (strpos($ARRAY['email'],"'")) 
			return ERROR_INVALID_CHARACTER;
			
        if (!check_email_address($ARRAY['email'])) {
            return ERROR_INVALID_EMAIL;
        }
        
        $sql = 'SELECT * FROM `users` WHERE EMAIL="'.$ARRAY['email'].'"';
        $req = mysql_query($sql) or sqldie($sql);  
        $data = mysql_fetch_assoc($req);
        if ($data) {
            if (isset($ORIGNAL)) 
            {
                if ($ORIGNAL['EMAIL']!=$data['EMAIL']) {
                    return ERROR_EMAIL_EXIST;
                }
            } else {
                return ERROR_EMAIL_EXIST;
            }
        }
    }
    
    if (isset($ARRAY['pw']))
    {       
		if (strpos($ARRAY['pw'],"'")) 
			return ERROR_INVALID_CHARACTER;
			
        if ((strlen($ARRAY['pw'])<1) || (strlen($ARRAY['pw'])>32)) {
            return ERROR_INVALID_PASSWORD;
        }
        
        if ($ARRAY['pw']!=$ARRAY['pw2']) {
            return ERROR_INVALID_PASSWORD_NO_MATCH;
        }
    }

    return NULL;
}

function check_email_address($email) {
    $regex = '/^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})$/'; 
    return (preg_match($regex, $email));
}

function guid(){
    if (function_exists('com_create_guid')){
        return trim(com_create_guid(), '{}');
    }else{
        mt_srand((double)microtime()*10000);//optional for php 4.2.0 and up.
        $charid = strtoupper(md5(uniqid(rand(), true)));
        $hyphen = chr(45);// "-"
         $uuid = substr($charid, 0, 8).$hyphen
                .substr($charid, 8, 4).$hyphen
                .substr($charid,12, 4).$hyphen
                .substr($charid,16, 4).$hyphen
                .substr($charid,20,12);
        return $uuid;
    }
}
		
function sendMail($mailTo, $title, $htmlmessge) {
	$mail = new PHPMailer();
	$mail->IsSMTP(); // telling the class to use SMTP
	$mail->SMTPDebug  = 0;                     // enables SMTP debug information (for testing)
											   // 1 = errors and messages
											   // 2 = messages only		
											   
	$mail->Host       = $GLOBALS['CONFIG']['smtp_Host'];	
	$mail->Port       = $GLOBALS['CONFIG']['smtp_port'];
	$mail->Username   = $GLOBALS['CONFIG']['smtp_login'];	
	$mail->Password   = $GLOBALS['CONFIG']['smtp_pw'];											   
	$mail->SMTPAuth   = $GLOBALS['CONFIG']['smtp_auth'];
	$mail->SMTPSecure = $GLOBALS['CONFIG']['smtp_secure'];
	
	$mail->IsHTML(true);
	$mail->SetFrom("no-reply@humanitas.com", 'HumanITas');
	$mail->AddReplyTo("no-reply@humanitas.com","HumanITas");	
	$mail->AddAddress($_POST['email']);
	$mail->Subject  = $title;            
	$mail->Body = $htmlmessge;

	return $mail->Send();
}

?>
