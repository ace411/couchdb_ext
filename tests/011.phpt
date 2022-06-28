--TEST--
updateDocuments() modifies multiple documents
--SKIPIF--
<?php
require_once 'config.php';

$basic = configure();
if (!$basic->available()) {
  echo 'skip';
}
?>
--FILE--
<?php
require_once 'config.php';
require_once 'helpers.php';

$basic    = configure(
  [
    'type' => CouchDb::RETURN_ARRAY,
  ],
);
$session  = $basic->session();
$docs     = $session->documents('recipes', ['FishStew', 'LambStew']);

$revs     = getRevsFromDocs($docs['rows'] ?? null);

echo is_bool(
  $session->updateDocuments(
    'recipes',
    [
      [
        '_id'       => 'FishStew',
        '_rev'      => $revs[0] ?? '',
        'servings'  => 4,
      ],
      [
        '_id'       => 'LambStew',
        '_rev'      => $revs[1] ?? '',
        'servings'  => 6,
      ],
    ],
  ),
) ? 'true' : 'false';
?>
--EXPECT--
true