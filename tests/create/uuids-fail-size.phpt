--TEST--
uuids throws CouchDbException when parameter count is too large
--FILE--
<?php
require_once __DIR__ . '/../../couch.php';

try {
  $couch->uuids(5000);
} catch (CouchDbException $exp) {
  echo $exp->getMessage();
}
?>
--EXPECT--
count parameter too large