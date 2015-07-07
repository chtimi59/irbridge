<?php

require("class.phpmailer.php");

$mail = new PHPMailer();

$mail->IsSMTP(); // telling the class to use SMTP
$mail->Host       = "secure.innobec.com"; // SMTP server
$mail->SMTPDebug  = 0;                     // enables SMTP debug information (for testing)
                                           // 1 = errors and messages
                                           // 2 = messages only
										   
$mail->SMTPAuth   = true;                  // enable SMTP authentication
$mail->SMTPSecure = "ssl";                 // sets the prefix to the server
$mail->Host       = "secure.innobec.com";  // sets SMTP server
$mail->Port       = 465;                   // set the SMTP port
$mail->Username   = "jdorgeville"; // username
$mail->Password   = "Maxuelle12";  // password

$mail->SetFrom("jdorgeville@innobec.com", 'Humanitas');
$mail->AddReplyTo("jdorgeville@innobec.com","Humanitas");
$mail->AddAddress("jdorgeville@innobec.com");

$mail->Subject  = "First PHPMailer Message";
$mail->Body     = "Hi! \n\n This is my first e-mail sent through PHPMailer.";
$mail->WordWrap = 50;

if(!$mail->Send()) {
  echo 'Message was not sent.';
  echo 'Mailer error: ' . $mail->ErrorInfo;
} else {
  echo 'Message has been sent.';
}
?>
    