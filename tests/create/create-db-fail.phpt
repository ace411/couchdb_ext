--TEST--
createDb throws exception when database name is empty string
--FILE--
<?php
require_once __DIR__ . '/../config.php';

try {
  $couch->createDb('');
} catch (CouchDbException $exp) {
  echo $exp->getMessage();
}
?>
--EXPECT--
Database name cannot be empty