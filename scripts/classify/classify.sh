cd ..
cd ..
cd bin
./classification ../models/vgg_face/deploy.prototxt ../weights/vgg_face/CK/vgg_face_train_iter_10000.caffemodel "../datasets/face/CK/4 - LMDB/train/mean.binaryproto" ../datasets/face/CK/emotion_labels.txt "../datasets/face/CK/3 - CROPPED_RESIZED/validate/fear/2.jpg"