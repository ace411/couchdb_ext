<?php

/**
 * search actions
 *
 * @author Lochemem Bruno Michael
 * @license PHP
 * @package couchdb-ext
 */

declare(strict_types=1);

require_once 'config.php';

// apply the search index upon creating it
if (
  $couch->createIndex(DATABASE, [
    'index' => [
      'fields' => ['servings', '_id', 'subtitle']
    ],
    'name'  => 'meal-index',
    'ddoc'  => 'meal-info',
    'type'  => 'json'
  ])
) {
  print_r($couch->search(DATABASE, [
    'skip'      => 0,
    'limit'     => 1,
    'use-index' => 'meal-info',
    'selector'  => [
      '_id' => ['$eq' => 'Pilau'], 
    ],
  ]));
}
