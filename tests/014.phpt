--TEST--
createIndex() creates a CouchDB index for a specified database
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
require_once 'helpers.php';

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
?>
--EXPECT--
true