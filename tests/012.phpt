--TEST--
documents() retrieves multiple documents
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

$basic    = configure();
$session  = $basic->session();
$docs     = $session->documents('recipes', ['FishStew', 'LambStew']);

echo jsonType($docs) ? 'true' : 'false';
?>
--EXPECT--
true