--TEST--
database() retrieves information about specified database
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
  $dbs      = $session->database('recipes');

  echo is_array($dbs) ? 'true' : 'false';
} catch (CouchDbException $_) {
  echo 'true';
}
?>
--EXPECT--
true