--TEST--
extension is capable of producing JSON results as PHP hashtables
--FILE--
<?php
require_once __DIR__ . '/../couch.php';

$conn = new CouchDb(array_merge(CONFIG, [
  'json' => false
]));

echo gettype($conn->uuids(2)) . PHP_EOL;
echo gettype($conn->allDbs());
?>
--EXPECT--
array
array