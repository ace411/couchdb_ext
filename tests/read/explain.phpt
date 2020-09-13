--TEST--
explain outputs query index-specific metadata
--FILE--
<?php
require_once __DIR__ . '/../../couch.php';

$data = $couch->explain(DATABASE, [
  'selector'  => [
    '_id' => ['$regex' => '(?i)git']
  ],
  'fields'    => ['_id', 'servings', 'subtitle'],
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