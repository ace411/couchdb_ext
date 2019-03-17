--TEST--
Check if extension is installed.
--SKIPIF--
<?php
if (!extension_loaded('couchdb_ext')) {
    echo 'skip';
}
?>
--FILE--
<?php
echo 'Extension loaded';
?>
--EXPECT--
'Extension loaded'