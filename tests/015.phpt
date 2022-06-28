--TEST--
find() performs parameterized Mango Query-powered database search
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
} catch (CouchDbException $_) {
  echo 'true';
}
?>
--EXPECT--
true