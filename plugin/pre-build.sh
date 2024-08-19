
echo "Pre-build script..."
echo "PWD"
pwd

cd ../../gui-web

npm run build > ../plugin/include/BlackwatchStudios/index.html

