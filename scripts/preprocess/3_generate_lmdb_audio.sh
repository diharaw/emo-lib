cd ..
cd ..
cd build/tools/convert_audioset/Release

# train
./convert_audioset --dataset ../../../../datasets/speech/RAVDESS --dbpath "../../../../datasets/speech/RAVDESS/4 - LMDB/train" --winlength 40 --frameshift 20 --labeltype 0 --phase train --numcepstra 12 --rescale 0 --samplingrate 48000

# test
./convert_audioset --dataset ../../../../datasets/speech/RAVDESS --dbpath "../../../../datasets/speech/RAVDESS/4 - LMDB/test" --winlength 40 --frameshift 20 --labeltype 0 --phase test --numcepstra 12 --rescale 0 --samplingrate 48000

# validate
./convert_audioset --dataset ../../../../datasets/speech/RAVDESS --dbpath "../../../../datasets/speech/RAVDESS/4 - LMDB/validate" --winlength 40 --frameshift 20 --labeltype 0 --phase validate --numcepstra 12 --rescale 0 --verbose 0 --samplingrate 48000

cd ..
cd ..
cd ..
cd ..
cd bin

./compute_image_mean --backend lmdb "../datasets/speech/RAVDESS/4 - LMDB/train" "../datasets/speech/RAVDESS/4 - LMDB/train/mean.binaryproto"
./compute_image_mean --backend lmdb "../datasets/speech/RAVDESS/4 - LMDB/test" "../datasets/speech/RAVDESS/4 - LMDB/test/mean.binaryproto"
./compute_image_mean --backend lmdb "../datasets/speech/RAVDESS/4 - LMDB/validate" "../datasets/speech/RAVDESS/4 - LMDB/validate/mean.binaryproto"
