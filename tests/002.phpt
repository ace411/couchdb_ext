--TEST--
couchdb_connect() Basic test connection to CouchDB
--SKIPIF--
<?php
if (!extension_loaded('couchdb_ext')) {
	echo 'skip';
}
?>
--FILE--
<?php
$ret = couchdb_connect('http://localhost:5984');

var_dump($ret);
?>
--EXPECT--
{"couchdb":"Welcome","version":"2.3.0","git_sha":"07ea0c7","uuid":"378fe43e363f5b86eebc94b0dd766a8e","features":["pluggable-storage-engines","scheduler"],"vendor":{"name":"The Apache Software Foundation"}}
