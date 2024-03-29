--TEST--
connect() conditions methods to return JSON by default
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
require_once 'helpers.php';

$basic = configure();

echo jsonType($basic->databases()) ? 'true' : 'false';
?>
--EXPECT--
true