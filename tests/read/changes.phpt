--TEST--
changes logs CouchDB database changes
--FILE--
<?php
require_once __DIR__ . '/../../couch.php';

print_r([
  'fst' => gettype($couch->changes(DATABASE)),
  'snd' => gettype($couch->changes(DATABASE, [
    'include-docs'  => 'true',
    'descending'    => 'true',
    'conflicts'     => 'true',
    'style'         => 'all_docs'
  ]))
]);
?>
--EXPECT--
Array
(
    [fst] => string
    [snd] => string
)