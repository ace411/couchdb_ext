--TEST--
allDocs prints a list of all documents in a database
--FILE--
<?php
require_once __DIR__ . '/../config.php';

$docs = $couch->allDocs(DATABASE, [
  'include_docs'  => 'true',
  'descending'    => 'true'
]);

print_r([
  'fst' => gettype($docs),
  'snd' => gettype(json_decode($docs, true))
]);
?>
--EXPECT--
Array
(
    [fst] => string
    [snd] => array
)