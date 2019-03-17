--TEST--
Check if CouchDB is available
--SKIPIF--
<?php
$couch = new CouchDb();
if (!$couch->isAvailable()) {
    echo 'skip';
}
?>
--FILE--
<?php
<<<<<<< HEAD
$ret = couchdb_connect('http://localhost:5984');

var_dump($ret);
?>
--EXPECT--
{"couchdb":"Welcome","version":"2.3.0","git_sha":"07ea0c7","uuid":"378fe43e363f5b86eebc94b0dd766a8e","features":["pluggable-storage-engines","scheduler"],"vendor":{"name":"The Apache Software Foundation"}}
=======
$couch = new CouchDb();
echo $couch->isAvailable();
?>
--EXPECT--
1
>>>>>>> php_cpp
