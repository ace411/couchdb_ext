--TEST--
Check if couchdb_ext is loaded
--SKIPIF--
<?php
if (!extension_loaded('couchdb_ext')) {
	echo 'skip';
}
?>
--FILE--
<?php
echo 'The extension "couchdb_ext" is available';
?>
--EXPECT--
The extension "couchdb_ext" is available
