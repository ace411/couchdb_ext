--TEST--
connect() establishes CouchDB configuration
--SKIPIF--
<?php
if (!extension_loaded('couchdb')) {
  echo 'skip';
}
?>
--FILE--
<?php
require_once 'config.php';

$basic = configure();

echo $basic instanceof CouchDb ? 'true' : 'false';
?>
--EXPECT--
true