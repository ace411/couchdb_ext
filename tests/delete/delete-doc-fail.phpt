--TEST--
deleteDoc throws an exception when empty parameters are detected
--FILE--
<?php
require __DIR__ . '/../config.php';

try {
  $couch->deleteDoc('', '', '');
} catch (CouchDbException $exp) {
  echo $exp->getMessage();
}
?>
--EXPECT--
Argument(s) cannot be empty