--TEST--
insertDocs throws an exception if parameters are empty
--FILE--
<?php
require __DIR__ . '/../config.php';

try {
  $couch->insertDocs('', []);
} catch (CouchDbException $exp) {
  echo $exp->getMessage();
}
?>
--EXPECT--
Argument(s) cannot be empty