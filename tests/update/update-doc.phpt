--TEST--
queryView throws exception when arguments are empty
--FILE--
<?php
require __DIR__ . '/../config.php';

echo gettype(
  $couch->updateDoc(
    DATABASE, 
    'fishStew', 
    '1-41669894c7d25a634f5de4fef75fb982', 
    [
      'servings'  => 4,
      'subtitle'  => 'Delicious with freshly baked bread'
    ]
  )
);
?>
--EXPECT--
boolean