from __future__ import division
import pickle
import pandas as pd
import numpy as np
import librosa
import matplotlib.pyplot as plt
import thinkdsp
import thinkplot
import os
import sys
import glob
import shutil
from itertools import compress
from itertools import compress
from collections import Counter
from pydub import AudioSegment
from pydub.utils import db_to_float
from pydub.silence import detect_nonsilent

emotions0 = ["neutral", "anger", "fear", "happy", "sadness", "disgust"]
emotions1 = ["neutral", "anger", "fear", "happy", "sadness", "surprise"]
emotions = [emotions0, emotions1]
sets = ["train", "validate", "test"]

if len(sys.argv) == 1:
    print("usage: trim.py <dataset_folder> <threshold> <min_size> <label_type> <graph>")
    sys.exit()

DATASET_ROOT = sys.argv[1]
SILENCE_THRESHOLD = int(sys.argv[2])
MIN_SIZE = int(sys.argv[3])
LABEL_TYPE = int(sys.argv[4])
SHOW_GRAPH = int(sys.argv[5])

# Remove folder
if os.path.exists("%s/3 - TRIMMED" %DATASET_ROOT):
    shutil.rmtree("%s/3 - TRIMMED" %DATASET_ROOT)

file_count = 0

for set_type in sets:
    os.makedirs("%s/3 - TRIMMED/%s" %(DATASET_ROOT, set_type))

    for index in range(0, 6): 
        os.makedirs("%s/3 - TRIMMED/%s/%s" %(DATASET_ROOT, set_type, emotions[LABEL_TYPE][index]))
        files = glob.glob("%s/2 - PARTITIONED/%s/%s/*" %(DATASET_ROOT, set_type, emotions[LABEL_TYPE][index]))

        for f in files:
            segment = AudioSegment.from_file(f, format="wav")
            samples = np.array(segment.get_array_of_samples())
            length = len(samples)/segment.frame_rate

            print("Processing file : %s" % f)
            print("Length : %ss" % length)

            # Split ranges
            ranges = detect_nonsilent(audio_segment=segment, min_silence_len=5, silence_thresh=-SILENCE_THRESHOLD)

            # Time
            t = np.arange(len(samples))/segment.frame_rate

            # Plot graph
            fig = None
            ax = None
            start = 0
            end = len(samples)

            if SHOW_GRAPH == 1:
                i = int(round(start * segment.frame_rate))
                j = int(round(end * segment.frame_rate))
                fig, ax = plt.subplots()
                ax.set_title(f)
                ax.plot(t[i:j],samples[i:j])

            trimmed = None

            for range_item in ranges:
                r0 = (range_item[0]/len(segment)) * length
                r1 = (range_item[1]/len(segment)) * length
                range_size = range_item[1] - range_item[0]

                if range_size > MIN_SIZE:
                    if trimmed == None:
                        trimmed = segment[range_item[0]:range_item[1]]
                    else:
                        trimmed += segment[range_item[0]:range_item[1]]

                    if SHOW_GRAPH == 1:
                        ax.axvspan(r0, r1, alpha=0.5, color='red')
            
            if SHOW_GRAPH == 1:
                plt.show()

            trimmed.export("%s/3 - TRIMMED/%s/%s/%s.wav" %(DATASET_ROOT, set_type, emotions[LABEL_TYPE][index], file_count), format="wav")
            file_count = file_count + 1
