#!/bin/bash

set -e

echo "Pre-build script..."
echo "PWD"
pwd

cd "$(dirname "$0")"


echo "Building html file..."
cd ../gui-web
mkdir -p ../plugin/include/BlackwatchStudios
npm run build > ../plugin/include/BlackwatchStudios/index.html



