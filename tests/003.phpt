--TEST--
session() creates new CouchDB connection
--SKIPIF--
<?php
$basic = CouchDb::connect();
if (!$basic->available()) {
  echo 'skip';
}
?>
--FILE--
<?php
$basic    = CouchDb::connect(
  [
    'user' => 'root',
    'pass' => 'admin',
  ],
);
// initiate new session (retrieve auth token from CouchDB)
$session  = $basic->session();
// extract new configuration
echo $session instanceof CouchDb ? 'true' : 'false';
?>
--EXPECT--
true