--TEST--
createIndex throws exception when partial_filter_selector key is absent
--FILE--
<?php
require_once __DIR__ . '/../config.php';

try {
  echo $couch->createIndex(DATABASE, [
    'index' => ['foo'],
    'type'  => 'json'
  ], true);
} catch (CouchDbException $exp) {
  echo $exp->getMessage();
}
?>
--EXPECT--
'partial_filter_selector' key is missing