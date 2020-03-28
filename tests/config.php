<?php

declare(strict_types=1);

/**
 * @var array CONFIG
 */
const CONFIG    = [
  'user' => '',
  'pass' => '',
  'host' => 'localhost'
];

/**
 * @var string DATABASE
 */
const DATABASE  = 'testdb';

$couch = new CouchDb(CONFIG);
