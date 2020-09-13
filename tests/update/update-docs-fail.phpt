--TEST--
updateDocs throws exception when parameters are empty
--FILE--
<?php
require_once __DIR__ . '/../../couch.php';

try{
  $couch->updateDocs('', []);
} catch (CouchDbException $exp) {
  echo $exp->getMessage();
}
?>
--EXPECT--
Argument(s) cannot be empty