--TEST--
database() retrieves information about specified database
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

$basic    = configure(
  [
    'type' => CouchDb::RETURN_ARRAY,
  ],
);
$session  = $basic->session();

$uuids    = $session->database('recipes');

echo is_array($uuids) ? 'true' : 'false';
?>
--EXPECT--
true