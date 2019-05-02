# VVAD - Video Voice Activity Detection

* multiplatformova C++ kniznica pre detekciu reci
* celkovy popis kniznice je v [diplomovej praci](https://github.com/Kr1zA/VVAD/blob/master/diplomka.PDF)

### buildovanie:
Hlavny Cmakelist sa nachadza vo [VVAD/VVAD/CMakeList](https://github.com/Kr1zA/VVAD/blob/master/VVAD/VVAD/CMakeLists.txt)

Pri buildovani inak ako pomocou CMakelistov:
* cmake .. -DUSE_SSE4_INSTRUCTIONS=ON
    * (pomalsia optimalizacia)

* cmake .. -DUSE_AVX_INSTRUCTIONS=ON
    * (rychlejsia optimalizacia)
    
Je potrebne mat nainstalovanu kniznicu OpenCV 3.4.5.

### Pouzivanie
Ak chceme pouzit namiesto videi obraz z kamery staci zmenit `VideoCapture cap(umiestnenieVidei + videa + "XXX.mp4");` na `VideoCapture cap(0);` a upravit zdrojak `testVVAD.cpp` tak aby posielal snimky neustale a nie po 33ms

* metoda `FrameForLearningThreshold` urci prah, ktory sa nasledne pouziva na detekciu reci pomocou metody `Frame`
* metoda Frame dava na vystup objekt triedy Output, teda (true/false - rec detegovana/nedetegovana, pocet tvari, pozicie tvari)

### Urobene upravy:
Upravil som render_face_detection.h aby vedelo zobrat nauceny model s:
* 6 bodmi tvare (perami)
* 20 bidmi tvare (perami)
* 27 bodmi tvare (pery + brada, nos, oci, ...)
* 68 bodmi tvare - povodny model

### Dataset
V zlozke [ibug_300W_large_face_landmark_dataset](https://github.com/Kr1zA/VVAD/tree/master/ibug_300W_large_face_landmark_dataset)sa nachadza upraveny dataset [300-W](https://ibug.doc.ic.ac.uk/resources/300-W_IMAVIS/)ktory bol pouzity na natrenovanie modelov detekcie bodov na tvari v zlozke [models](https://github.com/Kr1zA/VVAD/tree/master/VVAD/VVAD/data/models).
