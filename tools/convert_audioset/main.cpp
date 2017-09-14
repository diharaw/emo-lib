label_text = open("labels.txt", "w")

for i in range(0, 6):
    label_text.write("folder/file.png %i\n" % i)

label_text.close()