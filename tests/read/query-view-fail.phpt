--TEST--
queryView throws exception when arguments are empty
--FILE--
<?php
require __DIR__ . '/../../couch.php';

try {
  $couch->queryView('', '', '', []);
} catch (CouchDbException $exp) {
  echo $exp->getMessage();
}
?>
--EXPECT--
Argument(s) cannot be empty