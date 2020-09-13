<?php

declare(strict_types=1);

/**
 * @var CONFIG Default CouchDb test database
 */
const DATABASE  = 'testdb';

/**
 * @var CONFIG Default CouchDb configuration
 */
const CONFIG    = [
  'host' => 'localhost',
  'user' => '',
  'pass' => '',
];

$couch = new CouchDb(CONFIG);
