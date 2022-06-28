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

try {
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
} catch (CouchDbException $_) {
  echo 'true';
}
?>
--EXPECT--
true