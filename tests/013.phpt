--TEST--
deleteDocuments() deletes multiple documents
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
$docs     = $session->documents('recipes', ['Pilau', 'Katogo']);

$revs     = getRevsFromDocs($docs['rows'] ?? null);

echo is_bool(
  $session->deleteDocuments(
    'recipes',
    [
      [
        '_id'       => 'Pilau',
        '_rev'      => $revs[0] ?? '',
      ],
      [
        '_id'       => 'Katogo',
        '_rev'      => $revs[1] ?? '',
      ],
    ],
  ),
) ? 'true' : 'false';
?>
--EXPECT--
true