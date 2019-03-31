# couchdb_ext

This version of the extension is one written primarily in C++ but with C bindings.

## Requirements

- [PHP-CPP](https://www.php-cpp.com/)

- [libcurl](https://https://curl.haxx.se/libcurl/)

- PHP 7.1 or greater (Preferably a Non Thread-Safe version)

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

- ***database (string)*** - The database to create

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
