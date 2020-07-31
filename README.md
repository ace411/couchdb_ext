# couchdb_ext

[![Build Status](https://travis-ci.org/ace411/couchdb_ext.svg?branch=master)](https://travis-ci.org/ace411/couchdb_ext)

A simple PHP extension for CouchDB.

## Requirements

- [libcurl](https://https://curl.haxx.se/libcurl/)

- PHP 7.2 or greater

## Installation

Because `couchdb_ext` is an extension built on top of libcurl, installation of said utility is a mandatory prerequisite for using this tool.

Installing libcurl can be done by typing the following in a console of your choosing:

```shell
sudo apt-get install libcurl4-openssl-dev
```

Upon successful installation of libcurl, type the following - also in a console - to install `couchdb_ext`:

```shell
phpize
./configure --enable-couchdb_ext --with-curl=/path/to/curl
make && sudo make install
```

> Do not forget to enable the extension by adding `extension=couchdb_ext` to your `php.ini` file.

If you intend to run the tests in the tests directory, run the following command:

```shell
make test
```

## Docker

There exists a Docker image for this extension. You can find it in the [docker_couchdb_ext](https://github.com/peter279k/docker_couchdb_ext) repository.

## Rationale

I published the [fauxton API client](https://github.com/php-api-clients/fauxton), an asynchronous - non-blocking IO solution which is relatively more performant than the [synchronous version](https://github.com/ace411/fauxton-client) I wrote earlier.

Also, there exists an [article](https://medium.com/@agiroLoki/a-potential-php-extension-for-couchdb-9604cda48f27) whose premise is farther justifying this undertaking.

## Basic Usage

The snippet below is code that can be quite useful in not only configuring CouchDB, but also in determining if the service is available so as to subsequently generate five Universally Unique Identifiers (UUIDs).

```php
if (!extension_loaded("couchdb_ext")) {
    echo "Extension not loaded.";
}

$couch = new CouchDb([
    'user'      => 'your-username',
    'pass'      => 'your-password',
    'host'      => 'your-host (localhost or cloudant)',
    'port'      => 5984,
    'timeout'   => 60,
]);

if (!$couch->isAvailable()) {
    echo "Could not find a running instance of CouchDB";
}

echo $couch->uuids(5);
```

## API reference

### \_\_construct

```
__construct(array $options)
```

**Argument(s):**

- **_options (array)_** - Configuration options array
  - **_host (string)_** - CouchDB host
  - **_user (string)_** - CouchDB username
  - **_pass (string)_** - CouchDB password
  - **_port (integer)_** - CouchDB port (defaults to 5984)
  - **_timeout (integer)_** - Request timeout (defaults to 60)

Creates an instance of the CouchDb class.

```php
const CONFIG = [
    'user'      => 'your-username',
    'pass'      => 'your-password',
    'host'      => 'your-host (localhost or cloudant)',
    'port'      => 5984,
    'timeout'   => 60,
];

$couchDb = new CouchDb(CONFIG);
```

### isAvailable

```
isAvailable(): bool
```

**Argument(s):**

> _None_

Checks if the database at the constructor-specified location is present.

```php

if ($couch->isAvailable()) {
    echo $couch->allDocs('your-database');
}
```

### uuids

```
uuids(int $count): string
```

**Argument(s):**

- **_count (integer)_** - The number of uuids to generate

Generates a specified number of Universally Unique Identifiers.

```php

echo $couch->uuids(4); //returns JSON string
```

### createDb

```
createDb(string $database): bool
```

**Argument(s):**

- **_database (string)_** - The name of the database

Creates a database.

```php
const DB_NAME = 'your-database';

if ($couch->createDb(DB_NAME)) {
    $couch->insertDocs(DB_NAME, [
        'docs' => [
            [
                '_id'       => 'FishStew',
                'servings'  => 4,
                'subtitle'  => 'Delicious with freshly baked bread'
            ],
            [
                '_id'       => 'LambStew',
                'servings'  => 6,
                'subtitle'  => 'Serve with a whole meal scone topping'
            ]
        ]
    ]);
}
```

### allDbs

```
allDbs(): string
```

**Argument(s):**

> None

Outputs a list of all the databases available on disk.

```php
$dbs = json_decode($couch->allDbs(), true);

if (!in_array('your-database', $dbs)) {
    echo 'My database does not exist';
}
```

### getDoc

```
getDoc(string $database, string $docId): string
```

**Argument(s):**

- **_database (string)_** - CouchDB database
- **_docId (string)_** - Unique document identifier

Outputs the contents of a specified document.

```php
echo $couch->getDoc('your-database', 'doc-id');
```

### allDocs

```
allDocs(string $database, array $options): string
```

**Argument(s):**

- **_database (string)_** - The name of the database
- **_options (array)_** - An array of [CouchDB-specific query options](http://docs.couchdb.org/en/stable/api/database/bulk-api.html#db-all-docs)

Outputs a list of all documents in a specified database.

```php
echo $couch->allDocs('your-database', [
    'include_docs' => 'true',
    'descending' => 'true'
]);
```

### insertDocs

```
insertDocs(string $database, array $docs): bool
```

**Argument(s):**

- **_database (string)_** - The name of the database
- **_docs (array)_** - The data to insert in the database

Inserts data into a CouchDB database.

```php
$couch->insertDocs('your-database', [
    'docs' => [
        [
            '_id'       => 'FishStew',
            'servings'  => 4,
            'subtitle'  => 'Delicious with freshly baked bread'
        ],
        [
            '_id'       => 'LambStew',
            'servings'  => 6,
            'subtitle'  => 'Serve with a whole meal scone topping'
        ]
    ]
]);
```

### search

```
search(string $database, array $query): string
```

**Argument(s):**

- **_database (string)_** - The name of the database
- **_query (array)_** - The search query to execute

Performs a Mango-query-powered search.

```php
$github = $couch->search('your-database', [
    'selector'              => [
        'name' => ['$regex' => '(?i)ich']
    ],
    'fields'                => ['_id', 'name', 'github'],
    'execution_stats'       => 'true'
]);

var_dump(json_decode($github)); //returns a user object with specified fields
```

### createIndex

```
createIndex(string $database, array $options, bool $partial = false): string
```

**Argument(s):**

- **_database (string)_** - The name of the database
- **_options (array)_** - Index creation options (see CouchDB documentation)
- **_partial (boolean)_** - Partial flag that dictates index type (set to `true` if you intend to create a partial index)

Creates a CouchDB index.

```php
echo $couch->createIndex('your-database', [
    'index' => [
      'fields' => ['servings', '_id', 'subtitle']
    ],
    'ddoc'  => 'type-servings',
    'type'  => 'json'
]);
```

> Setting the `$partial` flag to `true` means that the `partial_filter_selector` should feature in your options list.

### createDdoc

```
createDdoc(string $database, string $ddoc, array $options): bool
```

**Argument(s):**

- **_database (string)_** - The name of the database
- **_ddoc (string)_** - The name of the design document
- **_options (array)_** - ddoc options list (See documentation)

Creates a design document.

```php
$couch->createDdoc('your-database', 'profileDoc', [
    'language'  => 'javascript',
    'views'     => [
        'github-view' => [
            'map' => 'function (doc) { emit(doc._id, doc.github) }'
        ]
    ]
]);
```

### queryView

```
queryView(string $database, string $ddoc, string $view, array $opts): string
```

**Argument(s):**

- **_database (string)_** - The name of the database
- **_ddoc (string)_** - The name of the design document
- **_view (string)_** - The view to query
- **_options (array)_** - View query options (See CouchDB documentation)

Queries a view in a specified database.

```php
['key' => $key, 'value' => $val] = json_decode($couch->queryView('your-database', 'profileDoc', 'github-view', [
    'descending'    => 'true',
    'conflicts'     => 'false',
    'update'        => 'true'
]), true);
```

### deleteDoc

```
deleteDoc(string $database, string $docId, string $docRev): bool
```

**Argument(s):**

- **_database (string)_** - CouchDB database
- **_docId (string)_** - Unique document identifier
- **_docRev (string)_** - Unique document revision

Deletes a specified document from a CouchDB database.

```php

$couch->deleteDoc('your-database', 'doc-id', 'doc-rev');
```

### deleteDocs

```
deleteDocs(string $database, array $docs): bool
```

**Argument(s):**

- **_database (string)_** - CouchDB database
- **_docs (array)_** - A list of documents

> `_id` and `_rev` keys are mandatory for each `docs` array entry

Deletes documents from a CouchDB database.

```php
$couch->deleteDocs('your-database', [
    [
        '_id'   => 'Pilau',
        '_rev'  => '1-599acfa0c7b36889599bde56276e444c'
    ],
    [
        '_id'   => 'Katogo',
        '_rev'  => '1-41669894c7d25a634f5de4fef75fb982'
    ]
]);
```

### updateDoc

```
updateDoc(string $database, string $docId, string $docRev, array $doc): bool
```

**Argument(s):**

- **_database (string)_** - The name of the database
- **_docId (string)_** - Unique document identifier
- **_docRev (string)_** - Unique document revision
- **_doc (array)_** - The data containing the update contents

Updates a document in a CouchDB database.

```php
$couch->updateDoc('your-database', 'Pilau', '1-599acfa0c7b36889599bde56276e444c', [
    'servings'  => 4,
    'subtitle'  => 'A delicious amalgam of rice and spices'
]);
```

### updateDocs

```
updateDocs(string $database, array $docs): bool
```

**Argument(s):**

- **_database (string)_** - CouchDB database
- **_docs (array)_** - A list of documents to update

> `_id` and `_rev` keys are mandatory for each `docs` array entry

Updates multiple documents in a CouchDB database.

```php
$couch->updateDocs('your-database', [
    'docs' => [
        [
            '_id'       => 'fishStew',
            '_rev'      => '1-41669894c7d25a634f5de4fef75fb982'
            'servings'  => 2,
            'subtitle'  => 'Delicious with freshly baked bread'
        ],
        [
            '_id'       => 'LambStew',
            '_rev'      => '1-599acfa0c7b36889599bde56276e444c',
            'servings'  => 3,
            'subtitle'  => 'Serve with a whole meal scone topping'
        ]
    ]
]);
```

### changes

```
changes(string $database, array $options = []): string
```

**Argument(s):**

- **_database (string)_** - The name of the database
- **_options (array)_** - An array of CouchDB-specific change tracking options

Conveys changes made to a database.

```php
echo $couch->changes('your-database', [
    'conflicts'     => 'true',
    'include_docs'  => 'true',
    'descending'    => 'true'
])
```
