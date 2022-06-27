--TEST--
insertDocuments() creates multiple documents
--SKIPIF--
<?php
$basic = CouchDb::connect();
if (!$basic->available()) {
  echo 'skip';
}
?>
--FILE--
<?php
require_once 'config.php';

$basic    = configure();
$session  = $basic->session();
$created  = $session->insertDocuments(
  'recipes',
  [
    [
      '_id'       => 'FishStew',
      'subtitle'  => 'Delicious with freshly baked bread',
    ],
    [
      '_id'       => 'LambStew',
      'subtitle'  => 'Serve with a whole meal scone topping',
    ],
    [
      '_id'       => 'Pilau',
      'subtitle'  => 'Sumptuous with beef chunks',
    ],
    [
      '_id'       => 'Katogo',
      'subtitle'  => 'Tastes especially delightful when served with a side of rice',
    ],
  ],
);

echo is_bool($created) ? 'true' : 'false';
?>
--EXPECT--
true