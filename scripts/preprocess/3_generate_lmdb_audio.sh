cd ..
cd ..
cd build/tools/convert_audioset/Release

# train
./convert_audioset --dataset ../../../../datasets/speech/EMODB --dbpath "../../../../datasets/speech/EMODB/3 - LMDB/train" --winlength 20 --frameshift 10 --labeltype 1 --phase train --numcepstra 12 --rescale 10--samplingrate 16000

# test
./convert_audioset --dataset ../../../../datasets/speech/EMODB --dbpath "../../../../datasets/speech/EMODB/3 - LMDB/test" --winlength 20 --frameshift 10 --labeltype 1 --phase test --numcepstra 12 --rescale 0 --samplingrate 16000

# validate
./convert_audioset --dataset ../../../../datasets/speech/EMODB --dbpath "../../../../datasets/speech/EMODB/3 - LMDB/validate" --winlength 20 --frameshift 10 --labeltype 1 --phase validate --numcepstra 12 --rescale 0 --samplingrate 16000