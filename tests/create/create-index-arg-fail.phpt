--TEST--
createIndex throws an exception when arguments are empty
--FILE--
<?php
require_once __DIR__ . '/../../couch.php';

try {
  $couch->createIndex(DATABASE, []);
} catch (CouchDbException $exp) {
  echo $exp->getMessage();
}
?>
--EXPECT--
Argument(s) cannot be empty