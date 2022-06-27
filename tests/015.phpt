--TEST--
find() performs parameterized Mango Query-powered database search
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
$docs     = $session->find(
  'recipes',
  [
    'execution_stats' => false,
    'fields'          => ['_id', 'servings'],
    'selector'        => [
      '_id'           => ['$regex' => '(?i)eef'],
    ],
  ],
);

echo jsonType($docs) ? 'true' : 'false';
?>
--EXPECT--
true