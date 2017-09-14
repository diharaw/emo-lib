cd .. 
cd ..
cd bin
classification.exe ../models/vgg_face/deploy.prototxt ../models/vgg_face/vgg_face_train_iter_10000.caffemodel "../datasets/CK/4 - LMDB/train/mean.binaryproto" ../datasets/CK/emotion_labels.txt "../datasets/CK/3 - CROPPED_RESIZED/validate/anger/3.jpg"