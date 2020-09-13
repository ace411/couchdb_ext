--TEST--
insertDocs throws an exception if parameters are empty
--FILE--
<?php
require __DIR__ . '/../../couch.php';

try {
  $couch->insertDocs('', []);
} catch (CouchDbException $exp) {
  echo $exp->getMessage();
}
?>
--EXPECT--
Argument(s) cannot be empty