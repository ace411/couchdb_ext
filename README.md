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
  exit();
}

$couch = new CouchDb([
  'user'      => 'your-username',
  'pass'      => 'your-password',
  'host'      => 'your-host (localhost or cloudant)',
  'port'      => 5984,
  'http'      => true,
  'json'      => true,
  'timeout'   => 60,
]);

if (!$couch->isAvailable()) {
  echo "Could not find a running instance of CouchDB";
}

echo $couch->uuids(5);
```

> Please check out the [examples](https://github.com/ace411/couchdb_ext/tree/master/examples) directory for more sample material

## API reference

```php
class CouchDb {

  /* Methods */
  public __construct( array $options );
  public isAvailable() : bool;
  public createDb( string $database ) : bool;
  public deleteDb( string $database ) : bool;
  public dbExists( string $database ) : bool;
  public dbInfo( string $database ) : string|array;
  public uuids( int $count ) : string|array;
  public changes( string $database [, array $options ] ) : string|array;
  public allDbs() : string|array;
  public allDocs( string $database [, array $query ] ) : string|array;
  public insertDocs( string $database [, array $docs ] ) : bool;
  public updateDocs( string $database [, array $docs ] ) : bool;
  public deleteDocs( string $database [, array $docs ] ) : bool;
  public search( string $database [, array $query ] ) : string|array;
  public explain( string $database [, array $query ] ) : string|array;
  public createIndex( string $database [, array $index ] ) : bool;
  public getDoc( string $database [, string $docId ] ) : string|array;
  public insertDoc( string $database [, array $contents [, bool $batch = false ]] ) : bool;
  public updateDoc( string $database [, string $docId [, string $rev [, array $contents ]]] ) : bool;
  public deleteDoc( string $database [, string $docId [, string $rev ]] ) : bool;
  public createDdoc( string $database [, string $ddoc [, array $contents ]] ) : bool;
  public queryView( string $database [, string $ddoc [, string $view [, array $query ]]] ) : string|array;
  public replicate( array $options ) : string|array;
}
```

`CouchDb::__construct` - Create new CouchDb instance

`CouchDb::isAvailable` - Checks if a CouchDB instance is available

`CouchDb::createDb` - Creates a new database

`CouchDb::deleteDb` - Deletes a database

`CouchDb::dbExists` - Checks if a database exists

`CouchDb::dbInfo` - Outputs database metadata

`CouchDb::uuids` - Generates a specified number of UUIDS (Universally Unique Identifiers)

`CouchDb::changes` - Returns a list of changes made to documents in a database

`CouchDb::allDbs` - Ouputs a list of all databases available on disk

`CouchDb::allDocs` - Returns all documents in a specified database

`CouchDb::insertDocs` - Inserts documents into a database

`CouchDb::updateDocs` - Updates documents in a database

`CouchDb::deleteDocs` - Deletes documents in a database

`CouchDb::search` - Performs Mango-query powered search

`CouchDb::explain` - Outputs query index-specific metadata

`CouchDb::createIndex` - Creates a search index

`CouchDb::getDoc` - Returns a single document

`CouchDb::insertDoc` - Inserts a single document into a database

`CouchDb::updateDoc` - Updates a single document in a database

`CouchDb::deleteDoc` - Deletes a single document from a database

`CouchDb::createDdoc` - Creates a new design document

`CouchDb::queryView` - Queries a view in a specified database

`CouchDb::replicate` - Configures a replication operation

## Contributing

Consider buying me a coffee if you appreciate the offerings of the project and/or would like to provide more impetus for me to continue working on it.

<a href="https://www.buymeacoffee.com/agiroLoki" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/lato-white.png" alt="Buy Me A Coffee" style="height: 51px !important;width: 217px !important;" ></a>
