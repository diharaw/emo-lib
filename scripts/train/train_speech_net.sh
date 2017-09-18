cd ..
cd ..
cd bin
# ./caffe train --solver=../models/speech_net/conv2/solver.prototxt --weights=../models/speech_net/conv2/train_test.prototxt_iter_20000.caffemodel 2>&1 | tee training_log.txt

./caffe train --solver=../models/speech_net/conv4/solver.prototxt