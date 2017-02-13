#!/usr/bin/env bash

version=`cat version.txt`

version_add=$(bc <<< "${version}+0.01")
zip_file_name="logger-v$version_add";

cp /dev/null version.txt
echo ${version_add} >> version.txt

echo "INICIANDO BUILD $version_add"
cd ../
npm run build
mkdir build/.ebextensions/
cp aws/app.config build/.ebextensions/app.config
cp aws/package.json build/
zip -r ../../${zip_file_name}.zip .
cd ../../
rm -rf build-web
clear
echo "COMPLETADA BUILD $version_add"