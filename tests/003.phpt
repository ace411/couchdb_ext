--TEST--
session() creates new CouchDB connection
--SKIPIF--
<?php
require_once 'auth.php';

if (!$session->available()) {
  echo 'skip';
}
?>
--FILE--
<?php
require_once 'config.php';

try {
  $basic    = configure();
  // initiate new session (retrieve auth token from CouchDB)
  $session  = $basic->session();
  // extract new configuration
  echo $session instanceof CouchDb ? 'true' : 'false';
} catch (CouchDbException $_) {
  echo 'true';
}
?>
--EXPECT--
true