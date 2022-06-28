<?php

if (!extension_loaded('couchdb')) {
  exit();
}

$basic = CouchDb::connect();

try {
  var_dump($basic->available());
} catch (CouchDbException $exp) {
  echo $exp->getMessage() . PHP_EOL;
}
