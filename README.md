# couchdb_ext

[![Build Status](https://travis-ci.org/ace411/couchdb_ext.svg?branch=master)](https://travis-ci.org/ace411/couchdb_ext)

A simple PHP extension for CouchDB.

## Requirements

- [ext-json](https://www.php.net/manual/en/book.json.php)

- [libcurl](https://https://curl.haxx.se/libcurl/)

- PHP 7.3 or greater (Preferably a Non Thread-Safe version)

## Installation

Because couchdb_ext is an extension built on top of libcurl, installation of said utility is a mandatory prerequisite for using this tool.

Installing libcurl can be done by typing the following in a console of your choosing:

```
sudo apt-get install libcurl4-openssl-dev
```

Upon successful installation of libcurl, type the following - also in a console - to install ```couchdb_ext```:

```
phpize
./configure --enable-couchdb_ext CFLAGS="-lcurl"
make && sudo make install
```

If you intend to run the tests in the tests directory, run the following command:

```
make test
```

## Rationale

I recently published the [fauxton API client](https://github.com/php-api-clients/fauxton), an asynchronous - non-blocking IO solution which is relatively more performant than the [synchronous version](https://github.com/ace411/fauxton-client) I wrote earlier.

Also, there exists an [article](https://medium.com/@agiroLoki/a-potential-php-extension-for-couchdb-9604cda48f27) whose premise is farther justifying this undertaking.

## Basic Usage

The snippet below is code that can be quite useful in not only configuring CouchDB, but also in determining if the service is available so as to subsequently generate five Universally Unique Identifiers (UUIDs).

```php
if (!extension_loaded("couchdb_ext")) {
    echo "Extension not loaded.";
}

$couch = new CouchDb('localhost', 'your-username', 'your-password', 5984, 60);

if (!$couch->isAvailable()) {
    echo "Could not find a running instance of CouchDB";
}

echo $couch->uuids(5);
```

## API reference

### __construct

```
__construct(string $host, string $user, string $pwd, int $port, int $timeout)
```

**Argument(s):**

- ***host (string)*** - CouchDB host
- ***user (string)*** - CouchDB username
- ***pwd (string)*** - CouchDB password
- ***port (integer)*** - CouchDB port
- ***timeout (integer)*** - Request timeout

Creates an instance of the CouchDb class.

```php
const CONFIG = ['localhost', 'your-username', 'your-password', 5984, 60];

$couchDb = new CouchDb(...CONFIG);
```

### isAvailable

```
isAvailable(): bool
```

**Argument(s):**

> *None*

Checks if the database at the constructor-specified location is present.

```php
...
if ($couch->isAvailable()) {
    echo $couch->allDocs('your-database');
} 
```

### uuids

```
uuids(int $count): string
```

**Argument(s):**

- ***count (integer)*** - The number of uuids to generate

Generates a specified number of Universally Unique Identifiers.

```php
...
echo $couch->uuids(4); //returns JSON string
```

### allDbs

```
allDbs(): string
```

**Argument(s):**

> None

Outputs a list of all the databases available on disk.

```php
...
$dbs = json_decode($couch->allDbs(), true);

if (!in_array('your-database', $dbs)) {
    echo 'My database does not exist';
}
```

### allDocs

```
allDocs(string $database, array $options): string
```

**Argument(s):**

- ***database (string)*** - The name of the database
- ***options (array)*** - An array of [CouchDB-specific query options](http://docs.couchdb.org/en/stable/api/database/bulk-api.html#db-all-docs)

Outputs a list of all documents in a specified database.

```php
...
echo $couch->allDocs('your-database', [
    'include_docs' => 'true',
    'descending' => 'true' 
]);
```

### insertDocs

```
insertDocs(string $database, array $data): bool
```

**Argument(s):**

- ***database (string)*** - The name of the database
- ***data (array)*** - The data to insert in the database

> Note: Please include the 'docs' key in the $data array to preempt a false response

Inserts data in a database.

```php
...
$couch->insertDocs('your-database', [
    'docs' => [
        [
            'name'      => 'Michael',
            'github'    => '@ace411',
            'twitter'   => '@agiroLoki' 
        ],
        ...
    ]
]); //outputs true or false
```

### search

```
search(string $database, array $query): string
```

**Argument(s):**

- ***database (string)*** - The name of the database
- ***query (array)*** - The search query to execute

Performs a Mango-query-powered search.

```php
...
$github = $couch->search('your-database', [
    'selector'              => [
        'name' => ['$regex' => '(?i)ich']
    ],
    'fields'                => ['_id', 'name', 'github'],
    'execution_stats'       => 'true'
]);

var_dump(json_decode($github)); //returns a user object with specified fields
```

### createDdoc

```
createDdoc(string $database, string $ddoc, array $options): bool
```

**Argument(s):**

- ***database (string)*** - The name of the database
- ***ddoc (string)*** - The name of the design document
- ***options (array)*** - ddoc options list (See documentation)

Creates a design document.

```php
...
$couch->createDdoc('testdb', 'profileDoc', [
    'views' => [
        'github-view' => [
            'map' => 'function (doc) { emit(doc._id, doc.github) }'
        ]
    ]
]);
```