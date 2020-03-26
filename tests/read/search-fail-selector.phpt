--TEST--
search throws exception when selector key is absent
--FILE--
<?php
require_once __DIR__ . '/../config.php';

try {
  $couch->search(DATABASE, [
    'limit' => 0,
    'skip'  => 0
  ]);
} catch (CouchDbException $exp) {
  echo $exp->getMessage();
}
?>
--EXPECT--
'selector' key is missing