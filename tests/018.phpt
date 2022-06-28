--TEST--
changes() retrieves a history of all actions performed on a database
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
  $docs     = $session->changes(
    'recipes',
    [
      'descending'    => true,
      'conflicts'     => false,
      'include_docs'  => true,
    ],
  );

  echo is_array($docs) ? 'true' : 'false';
} catch (CouchDbException $_) {
  echo 'true';
}
?>
--EXPECT--
true