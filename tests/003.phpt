--TEST--
session() creates new CouchDB connection
--SKIPIF--
<?php
require_once 'config.php';

$basic = configure();
if (!$basic->available()) {
  echo 'skip';
}
?>
--FILE--
<?php
require_once 'config.php';

$basic    = configure();
// initiate new session (retrieve auth token from CouchDB)
$session  = $basic->session();
// extract new configuration
echo $session instanceof CouchDb ? 'true' : 'false';
?>
--EXPECT--
true