<?php

/**
 * Database actions
 *
 * @author Lochemem Bruno Michael
 * @license PHP
 * @package couchdb-ext
 */

declare(strict_types=1);

require_once 'config.php';

// alternatively, you can use the sequence !in_array(DATABASE, $couch->allDbs())
$dbCheck = $couch->dbExists(DATABASE);

if ($couch->isAvailable() && !$dbCheck) {
  $couch->createDb(DATABASE);
}

// insert document
$couch->insertDoc(DATABASE, [
  '_id'       => 'Pilau',
  'servings'  => 2,
  'subtitle'  => 'Tastes better with chunks of beef',
]);

// print a list of changes made to the database
print_r($couch->changes(DATABASE, [
  'include_docs'  => true, // include document data 
  'descending'    => true, // return changes in reverse-chronological order
]));
