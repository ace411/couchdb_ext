--TEST--
createDatabase() creates new database
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
$created  = $basic->createDatabase('recipes');

echo is_bool($created) ? 'true' : 'false';
?>
--EXPECT--
true