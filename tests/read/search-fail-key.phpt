--TEST--
search throws an exception when parameters are empty
--FILE--
<?php
require_once __DIR__ . '/../config.php';

try {
  $couch->search('', []);
} catch (CouchDbException $exp) {
  echo $exp->getMessage();
}
?>
--EXPECT--
Argument(s) cannot be empty