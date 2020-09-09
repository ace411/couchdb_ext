--TEST--
constructor throws exception if mandatory configuration option is empty
--FILE--
<?php
try {
  $couch = new CouchDb([
    'user'    => '',
    'port'    => 5984,
    'timeout' => 60
  ]);
} catch (Throwable $err) {
  echo $err->getMessage();
}
?>
--EXPECT--
'host', 'user', and 'pass' are mandatory config options