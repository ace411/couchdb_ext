--TEST--
deleteDb throws exception when invalid arguments are detected
--FILE--
<?php
require __DIR__ . '/../../couch.php';

try {
  $couch->deleteDb('');
} catch (CouchDbException $exp) {
  echo $exp->getMessage();
}
?>
--EXPECT--
Argument(s) cannot be empty