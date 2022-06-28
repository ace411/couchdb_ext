--TEST--
uuids() generates a specified number of uuids
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

$uuids    = $session->uuids(5);

echo is_array($uuids) ? 'true' : 'false';
?>
--EXPECT--
true