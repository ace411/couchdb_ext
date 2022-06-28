<?php

/*
  +----------------------------------------------------------------------+
	 | Copyright (c) The PHP Group                                          |
	 +----------------------------------------------------------------------+
	 | This source file is subject to version 3.01 of the PHP license,      |
	 | that is bundled with this package in the file LICENSE, and is        |
	 | available through the world-wide-web at the following url:           |
	 | https://www.php.net/license/3_01.txt                                 |
	 | If you did not receive a copy of the PHP license and are unable to   |
	 | obtain it through the world-wide-web, please send a note to          |
	 | license@php.net so we can mail you a copy immediately.               |
	 +----------------------------------------------------------------------+
	 | Author: Lochemem Bruno Michael                                       |
	 +----------------------------------------------------------------------+
*/

define('COUCHDB_USER', 'root');
define('COUCHDB_PASS', 'admin');
define('COUCHDB_HOST', 'http://localhost');

/**
 * configure
 * create custom CouchDB configurations
 * 
 * @param array $config
 * @return CouchDb
 */
function configure(array $config = []): CouchDb
{
  return CouchDb::connect(
    array_merge(
			[
				'user' => COUCHDB_USER,
				'pass' => COUCHDB_PASS,
				'host' => COUCHDB_HOST,
			],
			$config,
		),
  );
}
