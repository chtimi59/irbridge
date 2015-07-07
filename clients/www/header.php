<?PHP
// WARNING: NO TEXT SHOULD BE WRITE HERE (EXCEPT DIE ERRORS)
// BE CAREFULL THAT THEY ALSO NO TEXT (EVEN SPACE) OUTSIDE PHP BALISES
// -> cause this file is used under download.php
include 'conf.php';
define('ADMIN_UUID',     "6A561157-70A6-424C-8963-598E627966A3");

if ($GLOBALS['CONFIG']['sql_isPW']) {
	$db = mysql_connect($GLOBALS['CONFIG']['sql_Host'], $GLOBALS['CONFIG']['sql_login'], $GLOBALS['CONFIG']['sql_pw']); 
} else {
	$db = mysql_connect($GLOBALS['CONFIG']['sql_Host'], $GLOBALS['CONFIG']['sql_login']); 
}
if (!$db)  die('Could not connect: ' . mysql_error());
if(!mysql_select_db('jdodevdb2',$db)) die('Could not connect db: ' . mysql_error());
date_default_timezone_set('America/Montreal');


define('ACTION_CLASS_ROOT',    000);
define('ACTION_NONE',  ACTION_CLASS_ROOT + 0);

$action = ACTION_NONE;
if (isset ($_GET['action'])) $action = $_GET['action'];

function sqldie($sql) {
	if ($GLOBALS['DEVELOPEMENT_MODE']) {
		die('Error SQL !<br>'.$sql.'<br>'.mysql_error());
	} else {
		die('<p>Server internal error</p>Sorry for the inconvenience!<br><a href="index.php">click here</a> to return to main menu'); 
	}
}

function IsAdmin() {

	if ($GLOBALS['USER']==NULL) {
		// not logged at all !
		return false;
	}
	
	if (isset($GLOBALS['EMB'])) {
		// on embedded we do not allow admin priviligeg
		if ($GLOBALS['EMB']) return false; 
	} else {
		// webservices do not set this variable (only index.php use it)
		return false;
	}
	
	// if UUID match
	return ($GLOBALS['USER']['UUID'] == ADMIN_UUID);
}

function CurrentPageURL() {
	$pageURL = ((isset($_SERVER['HTTPS'])) && ($_SERVER['HTTPS']) == 'on') ? 'https://' : 'http://';
	$pageURL .= $_SERVER['SERVER_PORT'] != '80' ? $_SERVER["SERVER_NAME"].":".$_SERVER["SERVER_PORT"].$_SERVER["REQUEST_URI"] : $_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI'];
	return $pageURL;
}

function CurrentPageBaseUrl() {
	$pageURL =CurrentPageURL();
	return preg_replace('/([^\/]*)$/', '', $pageURL);
}
?>