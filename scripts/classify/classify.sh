cd ..
cd ..
cd bin
./classification ../models/vgg_face/deploy.prototxt ../models/vgg_face/vgg_face_train_iter_10000.caffemodel "../datasets/face/CK/4 - LMDB/train/mean.binaryproto" ../datasets/face/CK/emotion_labels.txt "../datasets/face/CK/3 - CROPPED_RESIZED/test/sadness/1.jpg"