--TEST--
insertDocuments() creates multiple documents
--SKIPIF--
<?php
require_once 'auth.php';

if (!$session->available()) {
  echo 'skip';
}
?>
--FILE--
<?php
require_once 'config.php';

try {
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
} catch (CouchDbException $_) {
  echo 'true';
}
?>
--EXPECT--
true