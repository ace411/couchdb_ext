--TEST--
getDoc returns empty JSON object when non-existent document is requested
--FILE--
<?php
require_once __DIR__ . '/../../couch.php';

echo $couch->getDoc(DATABASE, 'foo');
?>
--EXPECT--
{}