--TEST--
deleteDoc deletes a database document
--FILE--
<?php
require __DIR__ . '/../../couch.php';

echo gettype($couch->deleteDoc(DATABASE, 'fishStew', 'foo-bar'));
?>
--EXPECT--
boolean