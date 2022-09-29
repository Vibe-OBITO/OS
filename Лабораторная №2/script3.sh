#!/bin/bash
#USAGE: ./script.name source_file exe_file_name
 
SOURCE="$1"
EXEFILE="$2"
 
if gcc -o "$EXEFILE" "$SOURCE"; then
    ./"$EXEFILE"
else
    echo "Compilation finish with error(s)"
    exit 1
fi
