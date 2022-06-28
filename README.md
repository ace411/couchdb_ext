# php-couchdb

A simple PHP extension for CouchDB.

## Requirements

- libcurl

- PHP 8 or newer

- async.h

## Installation

Because couchdb is an extension built on top of libcurl, installation of said utility is a mandatory prerequisite for using this tool.

Installing libcurl can be done by typing the following in a console of your choosing.

```sh
$ sudo apt-get install libcurl4-openssl-dev
```

### Enabling asynchronous transactions

The asynchrony in the couchdb extension - that powers its transaction feature - is a caller-based fork-join model. Considering `couchdb` cannot work without [async.h](https://github.com/naasking/async.h), you will have to install the library in any one of several possible global include directories (`/usr/include`, `/usr/local/include`, `/usr/local/opt/include`).

You can install the said library - by executing the `async.sh` shell script in this project's root directory - by typing the following.

```sh
$ chmod a+x async.sh
$ ./async.sh
```

If you have downloaded the library yourself, you can supply the path to the library directory - directly to the `async.sh` script - as a command line argument.

```sh
$ ./async.sh path/to/async.h
```

Upon successfully installing each of the aforedescribed dependencies, proceed to type the following to install `couchdb`.

```sh
$ phpize
$ ./configure --enable-couchdb --with-curl="/path/to/libcurl"
$ make && sudo make install
```

> Remember to add the `couchdb` shared object file (`extension=couchdb`) to your `php.ini` file to operationalize the extension.

If you intend to run the tests, the following should suffice.

```sh
$ make test
```

## Basic Usage

The snippet below demonstrates how to configure CouchDB for local installations, determine if the service is available - and subsequently generate five Universally Unique Identifiers (UUIDs).

```php
$basic = CouchDb::connect();

if (!$basic->isAvailable()) {
  exit();
}

echo $basic->uuids(5);
```

## API Reference

```php
class CouchDb
{
  /* Constants */
  public const RETURN_ARRAY;
  public const RETURN_JSON;

  /* Methods */
  public static connect( array $config ) : CouchDb;
  public session() : CouchDb;
  public isAvailable() : bool;
  public uuids( int $count ) : string|array;
  public databases() : string|array;
  public database( string $database ) : string|array;
  public createDatabase( string $database ) : bool;
  public deleteDatabase( string $database ) : bool;
  public insertDocuments( string $database [, array $documents ] ) : bool;
  public document( string $database [, string $documentId ] ) : string|array;
  public documents( string $database [, array $keys = null ] ) : string|array;
  public updateDocuments( string $database [, array $documents ] ) : bool;
  public deleteDocuments( string $database [, array $documents ] ) : bool;
  public find( string $database [, array $query ] ) : string|array;
  public createIndex( string $database [, array $options ] ) : bool;
  public createDesignDocument( string $database [, string $designDocument [, array $options ]] ) : bool;
  public view( string $database [, string $designDocument [, string $view [, array $options ]]] ) : string|array;
  public changes( string $database [, array $options = null ] ) : bool;
  public transaction( array $transactions ) : array;
}
```

### connect

Parametrically instantiates the CouchDb class.

```php
public CouchDb::connect(array $config);
```

Establishes the parameters for [Basic Auth-enabled](https://docs.couchdb.org/en/stable/api/server/authn.html#basic-authentication) CouchDB interactions.

#### Parameters

- **config** (array) - List of configuration options
  - **host** (string) - Base URI for CouchDB interactions
  - **user** (string) - Arbitrary CouchDB username
  - **pass** (string) - Arbitrary CouchDB password
  - **port** (string) - CouchDB port
  - **timeout** (string) - Timeout for requests made to CouchDB server
  - **type** (integer) - Return type for request data

```php
// default parameters
$basic = CouchDb::connect(
  [
    'host'    => 'http://127.0.0.1',
    'user'    => '',
    'pass'    => '',
    'token'   => '',
    'port'    => 5984,
    'timeout' => 60,
    'type'    => CouchDb::RETURN_JSON,
  ],
);
```

### session

Retrieves auth token from CouchDB.

```php
public CouchDb::session();
```

This method effectively exchanges the `user` and `password` information specified in class instantiation for an [auth token](https://docs.couchdb.org/en/stable/api/server/authn.html#cookie-authentication) and thence places the said token in the CouchDb object's parameterized state. It jettisons previously assigned `user` and `password` information.

> `session` persists all non-credential configuration from a previous `connect` call and is an [**opt-in** feature](https://).

#### Parameters

> None

```php
$basic = CouchDb::connect(
  [
    'user' => 'root',
    'pass' => 'admin',
    'type' => CouchDb::RETURN_ARRAY,
  ],
);

$session = $basic->session();
```

### available

Checks if a CouchDB instance is available the specified address.

```php
public CouchDb::available();
```

#### Parameters

> None

```php
$basic = CouchDb::connect();

if ($basic->available()) {
  echo 'Up and running!' . PHP_EOL;
}
```

### uuids

Retrieves, from CouchDB, a specified number of Universally Unique Identifiers (UUIDs).

```php
public CouchDb::uuids(int $count);
```

#### Parameters

- **count** (integer) - The number of UUIDs to generate

```php
$basic = CouchDb::connect(
  [
    'user' => 'root',
    'pass' => 'admin',
  ],
);

echo $basic->uuids(2);
```

### createDatabase

Creates a new database.

```php
public CouchDb::createDatabase(string $database);
```

#### Parameters

- **database** (string) - The name of the database to create

```php
$basic = CouchDb::connect(
  [
    'user' => 'root',
    'pass' => 'admin',
  ],
);

if ($basic->createDatabase('recipes')) {
  echo 'Database created' . PHP_EOL;
}
```

### databases

Returns a list containing the names of all the databases available to a user.

```php
public CouchDb::databases();
```

#### Parameters

> None

```php
$basic = CouchDb::connect(
  [
    'user' => 'root',
    'pass' => 'admin',
  ],
);

echo $basic->databases();
```

### database

Retrieves information about a specified database.

```php
public CouchDb::database(string $database);
```

### Parameters

- **database** (string) - The name of the database the information about which you intend to retrieve

```php
$basic = CouchDb::connect(
  [
    'user' => 'root',
    'pass' => 'admin',
  ],
);

echo $basic->database('recipes');
```

### deleteDatabase

Deletes a specified database.

```php
public CouchDb::deleteDatabase(string $database);
```

#### Parameters

- **database** (string) - The database you intend to delete

```php
$basic = CouchDb::connect(
  [
    'user' => 'root',
    'pass' => 'admin',
  ],
);

$session = $basic->session();

if ($session->deleteDatabase('vehicles')) {
  echo 'Successfully purged!' . PHP_EOL;
}
```

### document

Retrieves the contents of a specified document.

```php
public CouchDb::document(string $database, string $documentId);
```

#### Parameters

- **database** (string) - The database from which to retrieve the document
- **documentId** (string) - The unique identifier (`_id`) of the document whose information is to be retrieved

```php
$basic = CouchDb::connect(
  [
    'user' => 'root',
    'pass' => 'admin',
    'type' => CouchDb::RETURN_ARRAY,
  ],
);

$session = $basic->session();

echo $session->document('recipes', 'BeefStew');
```

### documents

Retrieves several documents in a single call.

```php
public CouchDb::documents(string $database, array $keys = null);
```

The method retrieves all documents in a specified database if a set of keys is not specified.

#### Parameters

- **database** (string) - The database from which you intend to retrieve a set of documents
- **keys** (array) - A list of document identifiers with which to tune the server response

```php
$basic = CouchDb::connect(
  [
    'user' => 'root',
    'pass' => 'admin',
  ],
);

echo $basic->documents('recipes', ['FishStew', 'LambStew']);
```

### insertDocuments

Creates multiple documents.

```php
public CouchDb::insertDocuments(string $database, array $documents);
```

#### Parameters

- **database** (string) - The database in which to insert documents
- **documents** (array) - The document data to insert into a specified database

```php
$basic = CouchDb::connect(
  [
    'user' => 'root',
    'pass' => 'admin',
  ],
);

$session = $basic->session();

if (
  $session->insertDocuments(
    'recipes',
    [
      [
        '_id'       => 'FishStew',
        'subtitle'  => 'Delicious with freshly baked bread',
      ],
      [
        '_id'       => 'LambStew',
        'subtitle'  => 'Serve with a whole meal scone topping',
      ],
    ],
  )
) {
  echo 'Documents successfully created!' . PHP_EOL;
}
```

### updateDocuments

Updates multiple documents.

```php
public updateDocuments(string $database, array $documents);
```

#### Parameters

- **database** (string) - The database whose documents you intend to modify
- **documents** (array) - The list of contents, inclusive of `_id` and `_rev` keys with which to update entries in a database

```php
$basic = CouchDb::connect(
  [
    'user' => 'root',
    'pass' => 'admin',
  ],
);

if (
  $basic->updateDocuments(
    'recipes',
    [
      [
        '_id'       => 'FishStew',
        '_rev'      => '1-41669894c7d25a634f5de4fef75fb982',
        'servings'  => 4,
      ],
      [
        '_id'       => 'LambStew',
        '_rev'      => '1-599acfa0c7b36889599bde56276e444c',
        'servings'  => 6,
      ],
    ],
  )
) {
  echo 'Database successfully updated!' . PHP_EOL;
}
```

### deleteDocuments

Deletes multiple documents.

```php
public deleteDocuments(string $database, array $documents);
```

#### Parameters

- **database** (string) - The database whose documents you intend to delete
- **documents** (array) - The list containing `_id` and `_rev` keys of the documents you intend to delete

```php
$basic = CouchDb::connect(
  [
    'user' => 'root',
    'pass' => 'admin',
  ],
);

$session = $basic->session();

if (
  $session->deleteDocuments(
    'recipes',
    [
      [
        '_id'   => 'Pilau',
        '_rev'  => '1-599acfa0c7b36889599bde56276e444c',
      ],
      [
        '_id'   => 'Katogo',
        '_rev'  => '1-41669894c7d25a634f5de4fef75fb982',
      ],
    ],
  )
) {
  echo 'Documents successfully deleted!' . PHP_EOL;
}
```

### find

Performs a parameterized Mango Query-powered database search.

```php
public CouchDb::find(string $database, array $query);
```

#### Parameters

- **database** (string) - The database on which you intend to perform the search
- **query** (array) - The database query parameters

```php
$basic = CouchDb::connect(
  [
    'user' => 'root',
    'pass' => 'admin',
    'type' => CouchDb::RETURN_ARRAY,
  ],
);

echo $basic->find(
  'recipes',
  [
    'execution_stats' => false,
    'fields'          => ['_id', 'servings'],
    'selector'        => [
      '_id'           => ['$regex' => '(?i)eef'],
    ],
  ],
);
```

### createIndex

Creates a CouchDB index for a specified database.

```php
public CouchDb::createIndex(string $database, array $options);
```

#### Parameters

- **database** (string) - The database on which to define the index
- **options** (array) - The list of options with which to configure the index

```php
$basic = CouchDb::connect(
  [
    'user' => 'root',
    'pass' => 'admin',
  ],
);

$session = $basic->session();

if (
  $session->createIndex(
    'recipes',
    [
      'ddoc'      => 'servings-index',
      'type'      => 'json',
      'index'     => [
        'fields'  => ['_id', 'servings', 'subtitle'],
      ],
    ],
  )
) {
  echo 'Index successfully created!' . PHP_EOL;
}
```

### createDesignDocument

Creates a design document in a specified database.

```php
public CouchDb::createDesignDocument(
  string $database,
  string $designDocument,
  array $options
);
```

This method is especially useful for creating views the display parameters for which can be tuned via CouchDB map-reduce functions.

#### Parameters

- **database** (string) - The database in which to create the design document
- **designDocument** (string) - The name of the design document
- **options** (array) - The list of options with which to configure the design document

```php
$basic = CouchDb::connect(
  [
    'user' => 'root',
    'pass' => 'admin',
  ],
);

if (
  $basic->createDesignDocument(
    'recipes',
    'recipesDoc',
    [
      'language'  => 'javascript',
      'views'     => [
        'servings-view' => [
          'map' => 'function (doc) { emit(doc._id, doc.servings) }'
        ],
      ],
    ],
  )
) {
  echo 'Successfully created design document' . PHP_EOL;
}
```

### view

Queries a view and retrieves all the records it is configured to project.

```php
public CouchDb::view(
  string $database,
  string $designDocument,
  string $view
  array $options = null
);
```

#### Parameters

- **database** (string) - The database the data in which the view projects
- **designDocument** (string) - The identifier of the design document
- **view** (string) - The name of the view
- **options** (array) - Additional non-mandatory query options

```php
$basic = CouchDb::connect(
  [
    'user' => 'admin',
    'pass' => 'root',
  ],
);

$session = $basic->session();

echo $basic->view(
  'recipes',
  'recipesDoc',
  'servings-view',
  [
    'descending'  => true,
    'conflicts'   => false,
    'update'      => true,
  ],
);
```

### changes

Retrieves a history of all actions performed on a database.

```php
public CouchDb::changes(string $database, array $options = null);
```

#### Parameters

- **database** (string) - The database whose history you intend to track
- **options** (array) - A list of options with which to tune the CouchDB response

```php
$basic = CouchDb::connect(
  [
    'user' => 'root',
    'pass' => 'admin',
  ],
);

echo $basic->changes(
  'recipes',
  [
    'conflicts'     => true,
    'include_docs'  => true,
    'descending'    => true,
  ],
);
```

### transaction

Concurrently executes multiple CouchDB actions in a fashion akin to transactions in SQL.

```php
public CouchDb::transaction(array $actions);
```

This function throws an exception in situations where the requisite protothreads library is not installed.

#### Parameters

- **actions** (array) - A list of PHP functions in which to subsume extension-supported CouchDB operations

```php
$basic = CouchDb::connect(
  [
    'user' => 'root',
    'pass' => 'admin',
    'type' => CouchDb::RETURN_ARRAY,
  ],
);

$session = $basic->session();

print_r(
  $session->transaction(
    [
      // create a database
      fn () => $session->database('recipes'),
      // insert documents in database
      fn () =>
        $session->insertDocuments(
          'recipes',
          [
            [
              '_id'       => 'FishStew',
              'servings'  => 4,
              'subtitle'  => 'Delicious with freshly baked bread',
            ],
            [
              '_id'       => 'LambStew',
              'servings'  => 6,
              'subtitle'  => 'Serve with a whole meal scone topping',
            ],
          ],
        ),
      // create index
      fn () =>
        $session->createIndex(
          'recipes',
          [
            'ddoc'      => 'servings-index',
            'type'      => 'json',
            'index'     => [
              'fields'  => ['servings', 'subtitle'],
            ],
          ],
        ),
      // create design document
      fn () =>
        $session->createDesignDocument(
          'recipes',
          'recipesDoc',
          [
            'language'  => 'javascript',
            'views'     => [
              'servings-view' => [
                'map' => 'function (doc) { emit(doc._id, doc.servings) }'
              ],
            ],
          ],
        ),
    ],
  ),
);
```

## Dealing with problems

Endeavor to create an issue on GitHub when the need arises or send an email to lochbm@gmail.com

## Contributing

Consider buying me a coffee if you appreciate the offerings of the project and/or would like to provide more impetus for me to continue working on it.

# <a href="https://www.buymeacoffee.com/agiroLoki" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/lato-white.png" alt="Buy Me A Coffee" style="height: 51px !important;width: 217px !important;" ></a>
