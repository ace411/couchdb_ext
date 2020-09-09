--TEST--
deleteDb throws exception when invalid arguments are detected
--FILE--
<?php
require __DIR__ . '/../config.php';

try {
  $couch->deleteDb('');
} catch (CouchDbException $exp) {
  echo $exp->getMessage();
}
?>
--EXPECT--
Database name cannot be empty