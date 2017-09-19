cd ..
cd ..
cd build/tools/convert_audioset/Release

# train
./convert_audioset --dataset ../../../../datasets/speech/EMODB --dbpath "../../../../datasets/speech/EMODB/3 - LMDB/train" --winlength 40 --frameshift 20 --labeltype 0 --phase train --numcepstra 12 --rescale 1 --samplingrate 16000

# test
./convert_audioset --dataset ../../../../datasets/speech/EMODB --dbpath "../../../../datasets/speech/EMODB/3 - LMDB/test" --winlength 40 --frameshift 20 --labeltype 0 --phase test --numcepstra 12 --rescale 1 --samplingrate 16000

# validate
./convert_audioset --dataset ../../../../datasets/speech/EMODB --dbpath "../../../../datasets/speech/EMODB/3 - LMDB/validate" --winlength 40 --frameshift 20 --labeltype 0 --phase validate --numcepstra 12 --rescale 1 --verbose 1 --samplingrate 16000

cd ..
cd ..
cd ..
cd ..
cd bin

./compute_image_mean --backend lmdb "../datasets/speech/EMODB/3 - LMDB/train" "../datasets/speech/EMODB/3 - LMDB/train/mean.binaryproto"
./compute_image_mean --backend lmdb "../datasets/speech/EMODB/3 - LMDB/test" "../datasets/speech/EMODB/3 - LMDB/test/mean.binaryproto"
./compute_image_mean --backend lmdb "../datasets/speech/EMODB/3 - LMDB/validate" "../datasets/speech/EMODB/3 - LMDB/validate/mean.binaryproto"
