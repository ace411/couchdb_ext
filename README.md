#couchdb_ext cpp

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

