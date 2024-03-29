--TEST--
uuids() generates a specified number of uuids
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
  $uuids    = $session->uuids(5);

  echo is_array($uuids) ? 'true' : 'false';
} catch (CouchDbException $_) {
  echo 'true';
}
?>
--EXPECT--
true