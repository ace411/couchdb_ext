--TEST--
view() queries a view and retrieves all the records it is configured to project
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
  $basic    = configure(
    [
      'type' => CouchDb::RETURN_ARRAY,
    ],
  );
  $session  = $basic->session();
  $docs     = $session->view(
    'recipes',
    'recipesDoc',
    'servings-view',
    [
      'descending'  => true,
      'conflicts'   => false,
      'update'      => true,
    ],
  );

  echo is_array($docs) ? 'true' : 'false';
} catch (CouchDbException $_) {
  echo 'true';
}
?>
--EXPECT--
true