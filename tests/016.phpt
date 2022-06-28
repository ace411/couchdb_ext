--TEST--
createDesignDocument() creates a CouchDB design document
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

$basic    = configure();
$session  = $basic->session();

echo is_bool(
  $session->createDesignDocument(
    'recipes',
    'recipesDoc',
    [
      'language'  => 'javascript',
      'views'     => [
        'servings-view' => [
          'map' => 'function (doc) { emit(doc._id, doc.servings) }'
        ],
      ],
    ],
  ),
) ? 'true' : 'false';
?>
--EXPECT--
true