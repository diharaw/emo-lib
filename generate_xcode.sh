rm -rf build
mkdir build
cd build 

cmake -DBUILD_SHARED_LIBS=OFF -DBUILD_LIBXTRACT_SAMPLE=OFF -DDISABLE_DEVICE_HOST_UNIFIED_MEMORY=ON -DBUILD_docs=OFF -DBUILD_python=OFF -DBUILD_python_layer=OFF -DBLAS="open" -DOpenCV_DIR="/Users/diharaw/opencv/build" -DCMAKE_PREFIX_PATH="../external/viennacl-dev" -G Xcode ..