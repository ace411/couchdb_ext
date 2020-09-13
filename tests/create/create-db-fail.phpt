--TEST--
createDb throws exception when database name is empty string
--FILE--
<?php
require_once __DIR__ . '/../../couch.php';

try {
  $couch->createDb('');
} catch (CouchDbException $exp) {
  echo $exp->getMessage();
}
?>
--EXPECT--
Argument(s) cannot be empty