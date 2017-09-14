import glob
import cv2
import sys
import os
import shutil

if len(sys.argv) == 1:
    print("usage: crop_resize.py <raw_dataset_folder> <height> <width> <is_color>")
    sys.exit()

# Remove folder
if os.path.exists("%s/3 - CROPPED_RESIZED" %sys.argv[1]):
    shutil.rmtree("%s/3 - CROPPED_RESIZED" %sys.argv[1])

os.makedirs("%s/3 - CROPPED_RESIZED" %sys.argv[1])

faceDet = cv2.CascadeClassifier("haarcascade_frontalface_default.xml")
faceDet2 = cv2.CascadeClassifier("haarcascade_frontalface_alt2.xml")
faceDet3 = cv2.CascadeClassifier("haarcascade_frontalface_alt.xml")
faceDet4 = cv2.CascadeClassifier("haarcascade_frontalface_alt_tree.xml")

emotions = ["neutral", "anger", "fear", "happy", "sadness", "surprise"] #Define emotions
sets = ["train", "validate", "test"]

def detect_faces(set_type, emotion, index, file):
    files = glob.glob("%s/2 - PARTITIONED/%s/%s/*" %(sys.argv[1], set_type, emotion))

    filenumber = 0
    for f in files:
        frame = cv2.imread(f) 
        # gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        
        face = faceDet.detectMultiScale(frame, scaleFactor=1.1, minNeighbors=10, minSize=(5, 5), flags=cv2.CASCADE_SCALE_IMAGE)
        face2 = faceDet2.detectMultiScale(frame, scaleFactor=1.1, minNeighbors=10, minSize=(5, 5), flags=cv2.CASCADE_SCALE_IMAGE)
        face3 = faceDet3.detectMultiScale(frame, scaleFactor=1.1, minNeighbors=10, minSize=(5, 5), flags=cv2.CASCADE_SCALE_IMAGE)
        face4 = faceDet4.detectMultiScale(frame, scaleFactor=1.1, minNeighbors=10, minSize=(5, 5), flags=cv2.CASCADE_SCALE_IMAGE)

        if len(face) == 1:
            facefeatures = face
        elif len(face2) == 1:
            facefeatures == face2
        elif len(face3) == 1:
            facefeatures = face3
        elif len(face4) == 1:
            facefeatures = face4
        else:
            facefeatures = ""
        
        for (x, y, w, h) in facefeatures: 
            print("face found in file: %s" %f)
            frame = frame[y:y+h, x:x+w]
            
            try:
                out = cv2.resize(frame, (int(sys.argv[2]), int(sys.argv[3])))
                cv2.imwrite("%s/3 - CROPPED_RESIZED/%s/%s/%s.jpg" %(sys.argv[1], set_type, emotion, filenumber), out)
                file.write("%s/%s.jpg %i\n" %(emotion, filenumber, index))
            except:
               pass 
        filenumber += 1 

for set_type in sets:
    os.makedirs("%s/3 - CROPPED_RESIZED/%s" %(sys.argv[1], set_type))
    label_text = open("%s/3 - CROPPED_RESIZED/%s/labels.txt" %(sys.argv[1], set_type), "w")

    for i in range(0, 6): 
        os.makedirs("%s/3 - CROPPED_RESIZED/%s/%s" %(sys.argv[1], set_type, emotions[i]))
        detect_faces(set_type, emotions[i], i, label_text)

    label_text.close()