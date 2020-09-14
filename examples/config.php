<?php

/**
 * Sample configuration file
 * 
 * @author Lochemem Bruno Michael
 * @license PHP
 * @package couchdb-ext
 */

declare(strict_types=1);

const DATABASE = 'food';

$config = new CouchDb([
  'host'    => '127.0.0.1', // alternatively specify arbitrary host
  'user'    => '',          // discretionary username
  'pass'    => '',          // discretionary password
  'timeout' => 60,          // request timeout value (defaults to 60)
  'port'    => 5984,        // CouchDB port (defaults to 5984)
  'json'    => false,       // determines whether or not to return JSON (defaults to true)
  'http'    => true,        // determines whether to use HTTP or HTTPS (defaults to true)
]);
