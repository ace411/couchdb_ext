--TEST--
Extension throws CouchDbException whenever selector key is missing
--FILE--
<?php
require_once 'config.php';

try {
    $res = json_decode($couch->search('testdb', [
        'sort'  => true
    ]), true);
} catch (CouchDbException $exp) {
    echo $exp->getMessage();
}
?>
--EXPECT--
'selector' key is missing