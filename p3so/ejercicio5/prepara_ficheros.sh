#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <directory>"
    exit 1
fi

dir=$1

if [ -d "$dir" ]; then
    rm -rf "$dir"
fi

mkdir "dir"
mkdir "dir/subdir"
touch "dir/fichero1"
echo "hola soy G" > "dir/fichero2"
ln -s "fichero2" "dir/enlaceS"
ln "dir/fichero2" "dir/enlaceH"

for file in "dir"/*; do
    stat "$file"
done
