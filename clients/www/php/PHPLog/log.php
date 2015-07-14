<?PHP
require("php/PHPMailer/class.phpmailer.php");

/* config */
define('LOG_USER_TABLE', 'scounter_users');
define('LOG_TMP_USER_TABLE', 'scounter_tmp_users');
define('LOG_NO_REPLY_EMAIL', 'no-reply@jdo-dev.org');
define('LOG_TITLE', 'Share-Counter');



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
define('MAIL_GREETING','Welcome! please click on the following link to valid your account.<br>');

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
	if ((time()-$_SESSION['clientUpdate'])> 60*SESSION_TIMEOUT_MINUTES)
	{
		if (isset($_SESSION['authkey']))  unset($_SESSION['authkey']);
		if (isset($_SESSION['clientIP'])) unset($_SESSION['clientIP']);
		if (isset($_SESSION['clientUpdate'])) unset($_SESSION['clientUpdate']);
		session_destroy();
		print ('<p>Session timeout</p>');
	}
}

$_SESSION['clientIP'] = $_SERVER['REMOTE_ADDR'];
$_SESSION['clientUpdate'] = time(); 
	 
	 
// --- Basic Login/Logoff ---
function logIn($key) {
    $authkey = NULL;
	
    if (isset($_SESSION['authkey']))
        $authkey = $_SESSION['authkey'];
    if (isset($_GET['authkey'])) 
        $authkey = $_GET['authkey'];
     if ($key)
        $authkey = $key;
	
    if ($authkey) {		
        $sql = 'SELECT * FROM `'.LOG_USER_TABLE.'` WHERE UUID="'.$authkey.'"';
        $req = mysql_query($sql) or sqldie($sql);
        $data = mysql_fetch_assoc($req);
        if (!$data)
            logOff();
		
		/* update connection date */
		$sql = 'UPDATE `'.LOG_USER_TABLE.'` SET `LAST_CONNECTION`=now() WHERE UUID="'.$authkey.'"';
		$req = mysql_query($sql) or sqldie($sql);
		
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



// --- LOGIN_ACTIONS --- 
/* and feed LOG_HTML accordignly */

logIn(NULL);
$LOG_HTML = '';
switch($action) {
	/* Change data into USER Table*/
    case ACTION_ACCOUNT_FINISH_SETTING:
		if ( (!$USER) || (!isset ($_POST['name'])) || (!isset ($_POST['pw'])) || (!isset ($_POST['pw2'])) || (!isset ($_POST['email'])) ) 
           break;
        if (!($err = check_user_account_setting($_POST,$USER)))
        {
            $sql = "UPDATE `".LOG_USER_TABLE."` SET "
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
          $LOG_HTML .= $err;
        }
        break;
        

	/* Form to change data in USER table */
    case ACTION_ACCOUNT_SETTING:
		if (!$USER)
           break;
		
        $sql = 'SELECT * FROM `'.LOG_USER_TABLE.'` WHERE UUID="'.$USER['UUID'].'"';
        $req = mysql_query($sql) or sqldie($sql);  
        $data = mysql_fetch_assoc($req);
        if (!$data) {
            die("internal error");
            break;
        }
        
        $LOG_HTML .= '<form action="index.php?action='.ACTION_ACCOUNT_FINISH_SETTING.'" method="post">';    
        $LOG_HTML .= ENTER_A_NAME.':<input type="text" name="name" value="'.$data['NAME'].'"/><br>';        
        $LOG_HTML .= ENTER_A_PASSWORD.':<input type="password" name="pw" value="'.$data['PASSWORD'].'"/><br>';        
        $LOG_HTML .= REENTER_A_PASSWORD.':<input type="password" name="pw2" value="'.$data['PASSWORD'].'"/><br>';
        $LOG_HTML .= EMAIL.': <input type="text" name="email" value="'.$data['EMAIL'].'"/><br>';                
        $LOG_HTML .= '<input type="submit" />';
        $LOG_HTML .= '</form>';
        break;
        
	/* Send User password by email */        
    case ACTION_LOGFORGET_EMAIL:
        logOff();
        if (!isset ($_POST['email']))
            break;
            
        $sql = 'SELECT * FROM `'.LOG_USER_TABLE.'` WHERE EMAIL="'.$_POST['email'].'"';
        $req = mysql_query($sql) or sqldie($sql);  
        $data = mysql_fetch_assoc($req);
        if (!$data) {
            $LOG_HTML .= ERROR_UNKNOWM_EMAIL;
            break;
        }
        
        //send mail        
		$message  = "<html><body>";
		$message .= NAME.': '.$data['NAME'].'<br>';
		$message .= PASSWORD.': '.$data['PASSWORD'].'<br>';
		$message .="</body></html>";
		if(sendMail($_POST['email'], LOG_TITLE." account", $message)) {
			$LOG_HTML .= MAIL_SENT_TO.$_POST['email']."<br>";
		} else {
			$LOG_HTML .= ERROR_MAIL;
		}       

        break;
         
	/* Forms to ask for our user password */        
    case ACTION_LOGFORGET:
        logOff();

        $LOG_HTML .= '<form action="index.php?action='.ACTION_LOGFORGET_EMAIL.'" method="post">';    
        $LOG_HTML .= EMAIL.': <input type="text" name="email"/><br>';                
        $LOG_HTML .= '<input type="submit" />';
        $LOG_HTML .= '</form>';        
        break;
        
	/* Create a new user */        
    case ACTION_FINISH_ACCOUNT_CREATE:
        logOff();
		
        if ((!isset ($_POST['name'])) || (!isset ($_POST['pw'])) || (!isset ($_POST['pw2'])) || (!isset ($_POST['email']))) 
            break;
        if (!isset ($_GET['verif']))
			break;
       
		
        if (!($err = check_user_account_setting($_POST,$USER)))
        {
			// add new user
            $uuid = guid();
            $sql = "INSERT INTO `".LOG_USER_TABLE."` (`UUID`, `NAME`, `PASSWORD`, `EMAIL`, `CREATION`) VALUES ("
                    ."'".$uuid."',"
                    ."'".$_POST['name']."',"
                    ."'".$_POST['pw']."',"
                    ."'".$_POST['email']."',"
					."now());";
            
            $req = mysql_query($sql) or sqldie($sql);  

			// remove from anti-chambre
			$sql = 'DELETE FROM `'.LOG_TMP_USER_TABLE.'` WHERE UUID="'.$_GET['verif'].'"';
			$req = mysql_query($sql) or sqldie($sql);  
			
			// auto-login
			login($uuid);
			
			// finish account creation
		    $action=0;
		    print "<script>location.href='index.php'</script>"; 
		    print "\n";
            break;
			
        } else {
          $LOG_HTML .= $err;
        }
        // else NO BREAK! (to allow to redo-config)
        
	/* New User email is validate give him the real form */    
    case ACTION_CHECKING_ACCOUNT:
        logOff();

        // check id
        $sql = 'SELECT * FROM `'.LOG_TMP_USER_TABLE.'` WHERE UUID="'.$_GET['verif'].'"';
        $req = mysql_query($sql) or sqldie($sql);  
        $data = mysql_fetch_assoc($req);
        if (!$data) {
            $LOG_HTML .= ERROR_PLZ_REGISTER;
            break;
        }
        
        $LOG_HTML .= '<form action="index.php?action='.ACTION_FINISH_ACCOUNT_CREATE.'&verif='.$_GET['verif'].'" method="post">';    
        $LOG_HTML .= ENTER_A_NAME.':<input type="text" name="name" value="'.((isset($_POST['name']))?$_POST['name']:'').'"/><br>';        
        $LOG_HTML .= ENTER_A_PASSWORD.':<input type="password" name="pw" /><br>';        
        $LOG_HTML .= REENTER_A_PASSWORD.':<input type="password" name="pw2" /><br>';
        $LOG_HTML .= EMAIL.': <input type="text" name="email" value="'.$data['EMAIL'].'"/><br>';                
        $LOG_HTML .= '<input type="submit" />';
        $LOG_HTML .= '</form>';
        break;
        
	/* Add user in anti-chambre */            
    case ACTION_CREATING_ACCOUNT:
        logOff();
		
		/* Seems the best moment to do it */
		cleanAntiChambre();
		
        if (!isset ($_POST['email']))
            break;

        if (!check_email_address($_POST['email'])) {
            $LOG_HTML .= ERROR_INVALID_EMAIL;
            break;
        }
        
        $sql = 'SELECT * FROM `'.LOG_USER_TABLE.'` WHERE EMAIL="'.$_POST['email'].'"';
        $req = mysql_query($sql) or sqldie($sql);  
        $data = mysql_fetch_assoc($req);
        if ($data) {
            $LOG_HTML .= ERROR_EMAIL_EXIST;
            break;
        }
		
		$sql = 'SELECT * FROM `'.LOG_TMP_USER_TABLE.'` WHERE EMAIL="'.$_POST['email'].'"';
        $req = mysql_query($sql) or sqldie($sql);  
        $data = mysql_fetch_assoc($req);
        if ($data) {
            $LOG_HTML .= ERROR_EMAIL_EXIST;
            break;
        }
		
        // save request
        $uid = guid();

        $sql = "INSERT INTO `".LOG_TMP_USER_TABLE."` (`UUID`, `EMAIL`) VALUES ('".$uid."', '".$_POST['email']."');";
        $req = mysql_query($sql) or sqldie($sql);  
        $verifurl = 'http://'.$_SERVER['HTTP_HOST'].'/'.$_SERVER['PHP_SELF'].'?verif='.$uid;
        
        //send mail        
        if ($GLOBALS['DEVELOPEMENT_MODE']) {
            $LOG_HTML .=('<a href="'.$verifurl.'">'.$verifurl.'</a><br>');
        }
		
		$message  = "<html><body>";
		$message .= MAIL_GREETING;
		$message .='<a href="'.$verifurl.'">'.$verifurl.'</a>';
		$message .="</body></html>";
		if(sendMail($_POST['email'], LOG_TITLE." account register", $message)) {
			$LOG_HTML .= MAIL_SENT_TO.$_POST['email']."<br>";
		} else {
			$LOG_HTML .= ERROR_MAIL;
		}            

        break;
        
	/* Form for registration */
    case ACTION_CREATE_ACCOUNT:
        logOff();

        $LOG_HTML .= '<form action="index.php?action='.ACTION_CREATING_ACCOUNT.'" method="post">';        
        $LOG_HTML .= EMAIL.':<input type="text" name="email" /><br>';
        $LOG_HTML .= '<input type="submit" />';
        $LOG_HTML .= '</form>';
        break;
            
	/* Perform login */
    case ACTION_LOGGING:
        logOff();

        if ((!isset ($_POST['name'])) || (!isset ($_POST['pw'])))
            break;
    
        $sql = 'SELECT * FROM `'.LOG_USER_TABLE.'` WHERE NAME="'.$_POST['name'].'"';
        $req = mysql_query($sql) or sqldie($sql);  
        $data = mysql_fetch_assoc($req);
        if (!$data) {
            $LOG_HTML .= ERROR_INVALID_NAME;
            $LOG_HTML .= '<a href="index.php?action='.ACTION_LOGFORGET.'">'.FORGET.'</a><br>';
            break;
        }
        if ($data['PASSWORD']!=$_POST['pw']) {
            $LOG_HTML .= ERROR_INVALID_PASSWORD;
            $LOG_HTML .= '<a href="index.php?action='.ACTION_LOGFORGET.'">'.FORGET.'</a><br>';
            break;
        }

        logIn($data['UUID']);
		// finish loggin
		$action=0;
		print "<script>location.href='index.php'</script>"; 
		print "\n";
        break;
        
	/* Form for login */
    case ACTION_LOGIN:
        $LOG_HTML .= '<form action="index.php?action='.ACTION_LOGGING.'" method="post">';        
        $LOG_HTML .= NAME.':<input type="text" name="name" value="'.(($USER)?$USER['NAME']:'').'"/><br>';
        $LOG_HTML .= PASSWORD.':<input type="password" name="pw" /><br>';
        $LOG_HTML .= '<input type="submit" />';
        $LOG_HTML .= '</form>';		
        break;    

	/* Do logout */
    case ACTION_LOGOUT:	
        logOff();
		// finish logout
		$action=0;
		print "<script>location.href='index.php'</script>"; 
		print "\n";
        break;
}



// --- LOG HELPER ---

function check_user_account_setting($ARRAY, $ORIGNAL) {

    if (isset($ARRAY['name']))
    {
        if ((strlen($ARRAY['name'])<1) || (strlen($ARRAY['name'])>20)) {
            return ERROR_INVALID_NAME;
        }

		if (strpos($ARRAY['name'],"'")) 
			return ERROR_INVALID_CHARACTER;
		
        $sql = 'SELECT * FROM `'.LOG_USER_TABLE.'` WHERE NAME="'.$ARRAY['name'].'"';
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
        
        $sql = 'SELECT * FROM `'.LOG_USER_TABLE.'` WHERE EMAIL="'.$ARRAY['email'].'"';
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

function cleanAntiChambre() {
	$sql = "DELETE FROM `".LOG_TMP_USER_TABLE."` WHERE (DATE + INTERVAL 30 MINUTE < NOW())";
    mysql_query($sql);
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
		
function sendMail($mailTo, $title, $LOG_HTMLmessge) {
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
	$mail->SetFrom(LOG_NO_REPLY_EMAIL, LOG_TITLE);
	$mail->AddReplyTo(LOG_NO_REPLY_EMAIL, LOG_TITLE);	
	$mail->AddAddress($_POST['email']);
	$mail->Subject  = $title;            
	$mail->Body = $LOG_HTMLmessge;

	return $mail->Send();
}

?>
