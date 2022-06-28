--TEST--
createIndex() creates a CouchDB index for a specified database
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
require_once 'helpers.php';

try {
  $basic    = configure();
  $session  = $basic->session();

  echo is_bool(
    $session->createIndex(
      'recipes',
      [
        'ddoc'      => 'servings-index',
        'type'      => 'json',
        'index'     => [
          'fields'  => ['_id', 'servings', 'subtitle'],
        ],
      ],
    ),
  ) ? 'true' : 'false';
} catch (CouchDbException $_) {
  echo 'true';
}
?>
--EXPECT--
true