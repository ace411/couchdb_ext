# couchdb_ext cpp

C++ version of the CouchDB PHP extension.

## Requirements

- [PHP-CPP](https://www.php-cpp.com/)

- [libcurl](https://https://curl.haxx.se/libcurl/)

- PHP 7.1 or greater (Preferably a Non Thread-Safe version)

## Installation

Because ext-func is an extension built on top of PHP-CPP and dependent on libcurl, installation of both PHP-CPP and libcurl is a mandatory prerequisite for using this tool. There exists an elaborate [installation guide](http://www.php-cpp.com/documentation/install) on the PHP-CPP website - I suggest that you peruse it. If you are, however, uninterested in following the official PHP-CPP installation guide and combing through the libcurl installation docs, run the ```install.sh``` shell script in a console of your choosing.

Once PHP-CPP is successfully installed on your system, type the following in a console to install ```couchdb_ext``` on your machine.

```
git clone https://github.com/ace411/couchdb_ext.git
cd couchdb_ext
make && sudo make install
```

## Rationale

I recently published the fauxton API client, an asynchronous - non-blocking IO solution which is relatively more performant than the synchronous version I wrote earlier.

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

### createDb

```
createDb(string $database): string
```

**Argument(s):**

- ***database (string)*** - The database to search

Creates a database.

```php
echo $couch->createDb('testdb');
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
$couch->isAvailable(); //returns true or false
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

### alldbs

```
alldbs(): string
```

**Argument(s):**

> *None*

Outputs all the databases available on disk.

```php
...
echo $couch->alldbs(); //outputs a list of databases
```

### alldocs

```
alldocs(string $database, array $opts): string
```

**Argument(s):**

- ***database (string)*** - The name of the database
- ***opts (array)*** - An array of query options. See [CouchDB docs](http://docs.couchdb.org/en/stable/api/database/bulk-api.html#db-all-docs) for more info.

Outputs all the documents in a specified database.

```php
...
$couch->alldocs('your-database', [
    'include_docs' => 'true',
    'descending' => 'false',
    'skip' => 0
]); //should return a list of documents
```

### getDoc

```
getDoc(string $database, string $docId, array $opts): string
```

**Argument(s):**

- ***database (string)*** - Database name
- ***docId (string)*** - Database document identifier
- ***opts (array)*** - An array of query options. See [CouchDB docs]() for more info.

Fetches a single document from a database.

```php
...
echo $couch->getDoc('your-database', 'document-ID', [
    'include_docs' => true,
    'skip' => 0
]);
```

### getDocsByKey

```
getDocsByKey(string $database, array $keys): string
```

**Argument(s):**

- ***database (string)*** - The database to search
- ***keys (array)*** - A list of document identifiers to include in the search

```php
...
$couch->getDocsByKey('testdb', [
    'keys' => [
        'agiro-loki',
        'ace411',
        'bruno-michael'
    ]
]);
```