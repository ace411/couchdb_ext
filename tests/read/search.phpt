--TEST--
search method performs discretionary search of CouchDB database
--FILE--
<?php
require_once __DIR__ . '/../config.php';

$data = $couch->search(DATABASE, [
  'selector'  => [
    '_id' => ['$regex' => '(?i)git']
  ],
  'skip'      => 0
]);

print_r([
  'fst' => gettype($data),
  'snd' => gettype(json_decode($data, true))
]);
?>
--EXPECT--
Array
(
    [fst] => string
    [snd] => array
)