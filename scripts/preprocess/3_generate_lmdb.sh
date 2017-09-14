cd ..
cd ..
cd bin

# train
./convert_imageset-d -backend lmdb -shuffle "/Users/diharaw/Desktop/Datasets/CK/3 - CROPPED_RESIZED/train/" "/Users/diharaw/Desktop/Datasets/CK/3 - CROPPED_RESIZED/train/labels.txt" "../datasets/CK/4 - LMDB/train"

# test
./convert_imageset-d -backend lmdb -shuffle "/Users/diharaw/Desktop/Datasets/CK/3 - CROPPED_RESIZED/test/" "/Users/diharaw/Desktop/Datasets/CK/3 - CROPPED_RESIZED/test/labels.txt" "../datasets/CK/4 - LMDB/test"

# validate
./convert_imageset-d -backend lmdb -shuffle "/Users/diharaw/Desktop/Datasets/CK/3 - CROPPED_RESIZED/validate/" "/Users/diharaw/Desktop/Datasets/CK/3 - CROPPED_RESIZED/validate/labels.txt" "../datasets/CK/4 - LMDB/validate"