cd external

rm -rf caffe
git clone https://github.com/BVLC/caffe.git
cd caffe
# git checkout opencl
cd ..

rm -rf viennacl-dev
git clone https://github.com/viennacl/viennacl-dev.git