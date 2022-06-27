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

/**
 * jsonType
 * checks if a specified value is of the JSON data type
 *
 * @param mixed $data
 * @return boolean
 */
function jsonType($data): bool
{
	return is_string($data) ? preg_match('/([{\}\[\]\d\w:",]+)/', $data) : false;
}

/**
 * getRevsFromDocs
 * extracts individual document revisions from a set of documents
 *
 * @param array|null $data
 * @return array
 */
function getRevsFromDocs(?array $data): array
{
	if (is_null($data)) {
		return [];
	}

	return array_reduce(
		$data,
		function ($acc, $doc) {
			$rev = $doc['value']['rev'] ?? null;

			if (!is_null($rev)) {
				$acc[] = $rev;
			}

			return $acc;
		},
		[], 
	);
}
