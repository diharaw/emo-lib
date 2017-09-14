cd ..
cd ..
cd bin
./caffe time --model=../models/vgg_face/deploy.prototxt --weights=../models/vgg_face/vgg_face_train_iter_10000.caffemodel --gpu=1 > time_log_gpu.txt 2>&1