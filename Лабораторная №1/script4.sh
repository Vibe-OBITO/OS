#!/bin/bash

IFS=$'\n'
ERR_LOG="/tmp/err.log"

exec 6>&2 2>$ERR_LOG

find $(readlink -f "/home/noname/021731_Аблакулов_Амаль") -type f -uid $(id -u "noname") -printf "%p %s\n" 1>"noname"

cat "noname" | wc -l

exec 2>&6 6>&-
sed "s/.[a-zA-Z ]*:/`basename noname`:/" < $ERR_LOG 1>&2

rm $ERR_LOG

