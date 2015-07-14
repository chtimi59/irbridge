<?PHP	
	$systemRoot = $_SERVER['DOCUMENT_ROOT'];
	if(strncmp(PHP_OS, 'WIN', 3) === 0) {
		$GLOBALS['CONF_FILE'] = 'c:/dev/share-counter.conf';
	} else {
		/* $GLOBALS['CONF_FILE'] = '/var/share-counter.conf'; */
		$GLOBALS['CONF_FILE'] = '/homez.767/jdodev/var/share-counter.conf';
	}

	if (($_SERVER['SERVER_ADDR']=='localhost' ) || $_SERVER["SERVER_ADDR"]=="127.0.0.1" || ($_SERVER["SERVER_ADDR"]=="::1")) {
		$GLOBALS['DEVELOPEMENT_MODE']   = true;
		$GLOBALS['DEBUG'] = 1;
	} else {
		$GLOBALS['DEVELOPEMENT_MODE']   = false;
		$GLOBALS['DEBUG'] = 0;
	}
	
	$GLOBALS['CONFIG'] = readconf();
	
	function readconf() {
		$encryptedMessage = file_get_contents ($GLOBALS['CONF_FILE']);	
		$iv = substr($encryptedMessage, 0, 16);
		$encryptedMessage = substr($encryptedMessage, 16);
		$secretHash = '1289rtpf%7+9';
		$encryptionMethod = 'aes128';
		$string = openssl_decrypt($encryptedMessage, $encryptionMethod, $secretHash, 0, $iv);
		return json_decode($string, true);
	}
	
?>