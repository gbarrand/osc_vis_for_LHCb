#!/bin/sh -f

libs='-ldb -lutil -lpthread -lc'

cp /usr/lib/python2.2/config/libpython2.2.a    .

chmod a+x makshlib
./makshlib libpython2.2 "$libs"

