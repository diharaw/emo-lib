cd ..
cd ..
cd build/tools/convert_audioset/Release

# train
./convert_audioset --dataset ../../../../datasets/speech/EMODB --dbpath "../../../../datasets/speech/EMODB/3 - LMDB/train" --winlength 40 --frameshift 20 --labeltype 1 --phase train --numcepstra 12 --rescale 1 --samplingrate 16000

# test
./convert_audioset --dataset ../../../../datasets/speech/EMODB --dbpath "../../../../datasets/speech/EMODB/3 - LMDB/test" --winlength 40 --frameshift 20 --labeltype 1 --phase test --numcepstra 12 --rescale 1 --samplingrate 16000

# validate
./convert_audioset --dataset ../../../../datasets/speech/EMODB --dbpath "../../../../datasets/speech/EMODB/3 - LMDB/validate" --winlength 40 --frameshift 20 --labeltype 1 --phase validate --numcepstra 12 --rescale 1 --samplingrate 16000