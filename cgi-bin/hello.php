/usr/bin/php

<?php
// header("Content-Type: text/html");
echo "Hello, World!\n";
echo "<li>REQUEST_METHOD: {$_SERVER['REQUEST_METHOD']}</li>\n";
echo "<li>QUERY_STRING: {$_SERVER['QUERY_STRING']}</li>\n";
echo "<li>CONTENT_TYPE: {$_SERVER['CONTENT_TYPE']}</li>\n";
echo "<li>CONTENT_LENGTH: {$_SERVER['CONTENT_LENGTH']}</li>\n";
echo "<li>REMOTE_ADDR: {$_SERVER['REMOTE_ADDR']}</li>\n";
echo "<li>SCRIPT_NAME: {$_SERVER['SCRIPT_NAME']}</li>\n";
echo "<li>SCRIPT_PATH: {$_SERVER['SCRIPT_PATH']}</li>\n";
?>
