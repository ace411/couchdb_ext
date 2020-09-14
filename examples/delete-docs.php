<?php

/**
 * document deletion actions
 *
 * @author Lochemem Bruno Michael
 * @license PHP
 * @package couchdb-ext
 */

declare(strict_types=1);

require_once 'config.php';

// delete all docs whose servings are greater than 5 
echo $couch->deleteDocs(DATABASE, [
  'docs' => $couch->search(DATABASE, [
    'skip'      => 0,
    'fields'    => ['_id', '_rev'],
    'selector'  => [
      'servings' => ['$gt' => 5]
    ],
  ])['docs'],  
]);
