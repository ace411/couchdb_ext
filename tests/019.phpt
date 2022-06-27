--TEST--
transaction() executes several CouchDB actions in protothreads
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

try {
  $docs     = $session->transaction(
    [
      fn () => $session->uuids(5),
      fn () => 
        $session->find(
          'recipes',
          [
            'execution_stats' => false,
            'fields'          => ['_id', 'subtitle', 'servings'],
            'selector'        => [
              '_id'           => ['$regex' => '(?i)stew'],
            ],
          ],
        ),
    ],
  );

  echo is_array($docs) ? 'true' : 'false';
} catch (CouchDbException $exp) {
  echo 'true';
}
?>
--EXPECT--
true