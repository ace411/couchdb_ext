#!/usr/bin/sh
# +----------------------------------------------------------------------+
# | Copyright (c) The PHP Group                                          |
# +----------------------------------------------------------------------+
# | This source file is subject to version 3.01 of the PHP license,      |
# | that is bundled with this package in the file LICENSE, and is        |
# | available through the world-wide-web at the following url:           |
# | https://www.php.net/license/3_01.txt                                 |
# | If you did not receive a copy of the PHP license and are unable to   |
# | obtain it through the world-wide-web, please send a note to          |
# | license@php.net so we can mail you a copy immediately.               |
# +----------------------------------------------------------------------+
# | Author: Lochemem Bruno Michael                                       |
# +----------------------------------------------------------------------+


async=async.h
asyncs=async-sem.h

copy_lib_files() {
  cp -rf $1/async/$async $1/async/$asyncs /usr/local/include
}

install_lib() {
  git clone https://github.com/naasking/async.h.git
  copy_lib_files $async
  rm -rf $async
}

if test ! -z $1; then
  if test -d $1; then
    copy_lib_files $1
  else
    install_lib
  fi
else
  install_lib
fi
