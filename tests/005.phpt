--TEST--
CouchDb object subsumes configuration information
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

var_dump($basic);
?>
--EXPECT--
object(CouchDb)#1 (1) {
  [0]=>
  array(7) {
    ["user"]=>
    string(4) "root"
    ["password"]=>
    string(5) "admin"
    ["token"]=>
    string(0) ""
    ["host"]=>
    string(16) "http://127.0.0.1"
    ["timeout"]=>
    int(60)
    ["port"]=>
    int(5879)
    ["type"]=>
    string(4) "JSON"
  }
}