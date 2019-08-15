--TEST--
search throws CouchDbException whenever search array is empty
--FILE--
<?php
require_once 'config.php';

try {
    $res = json_decode($couch->search('testdb', []), true);
} catch (CouchDbException $exp) {
    echo $exp->getMessage();
}
?>
--EXPECT--
Parameters cannot be empty