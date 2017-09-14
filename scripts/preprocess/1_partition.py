import glob
import math
import shutil
import os
import sys

if len(sys.argv) == 1:
    print("usage: partition.py <raw_dataset_folder> <train_percentage> <test_percentage>")
    sys.exit()

kTRAINING_SET_PERCENTAGE = int(sys.argv[2])
kTEST_SET_PERCENTAGE = int(sys.argv[3])

# Delete directory if exists
if os.path.exists("%s/2 - PARTITIONED" %sys.argv[1]):
    shutil.rmtree("%s/2 - PARTITIONED" %sys.argv[1])

# Create directory
os.makedirs("%s/2 - PARTITIONED" %sys.argv[1])

def partition_sizes(size, train, test):
    dataset_size = size
    training_set_size = 0
    test_set_size = 0
    validation_set_size = 0

    training_set_size = int(round(dataset_size * (train / 100.0)))
    remainder = dataset_size - training_set_size

    test_set_size = int(round(dataset_size * (test / 100.0)))
    validation_set_size = remainder - test_set_size

    return training_set_size, test_set_size, validation_set_size    

kNEUTRAL  = 0
kANGRY    = 1
kFEARFUL  = 2
kHAPPY    = 3
kSAD      = 4
kSURPRISE = 5

emotions = ["neutral", "anger", "fear", "happy", "sadness", "surprise"]

dataset = []
test_set = []
val_set = []

dataset.append([])
dataset.append([])
dataset.append([])
dataset.append([])
dataset.append([])
dataset.append([])

for i in range(0, 6):
    os.makedirs("%s/2 - PARTITIONED/train/%s" %(sys.argv[1],emotions[i]))
    os.makedirs("%s/2 - PARTITIONED/test/%s" %(sys.argv[1],emotions[i]))
    os.makedirs("%s/2 - PARTITIONED/validate/%s" %(sys.argv[1],emotions[i]))

    dataset[i] = glob.glob("%s/1 - SORTED/%s/*" %(sys.argv[1], emotions[i]))

    train, test, val = partition_sizes(len(dataset[i]), kTRAINING_SET_PERCENTAGE, kTEST_SET_PERCENTAGE)

    print("Emotion: %s" %emotions[i])
    print("Total Set Size: %s" %len(dataset[i]))
    print("Training Set Size: %s" %train)
    print("Test Set Size: %s" %test)
    print("Validation Set Size: %s" %val)
    print("")

    # Copy the test set
    for j in range(0, test):
        shutil.copy(dataset[i].pop(), "%s/2 - PARTITIONED/test/%s" %(sys.argv[1],emotions[i]))

    # Copy the validation set
    for j in range(0, val):
        shutil.copy(dataset[i].pop(), "%s/2 - PARTITIONED/validate/%s" %(sys.argv[1],emotions[i]))

    # Copy the training set
    for j in dataset[i]:
        shutil.copy(j, "%s/2 - PARTITIONED/train/%s" %(sys.argv[1],emotions[i]))