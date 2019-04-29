# VVAD - Video Voice Activity Detection

###buildovanie:
Hlavny Cmakelist sa nachadza vo VVAD/VVAD/CMakeList

Pri buildovani inak ako pomocou CMakelistov:
* cmake .. -DUSE_SSE4_INSTRUCTIONS=ON
    * (pomalsia optimalizacia)

* cmake .. -DUSE_AVX_INSTRUCTIONS=ON
    * (rychaljsia optimalizacia)

###Pouzivanie
Ak chceme pouzit namiesto videi obraz z kamery staci zmenit `VideoCapture cap(umiestnenieVidei + videa + "XXX.mp4");` na `VideoCapture cap(0);` a upravit zdrojak `testVVAD.cpp` tak aby posielal snimky neustale a nie po 33ms

###Urobene upravy:
Upravil som render_face_detection.h aby vedelo zobrat nauceny model s:
* 6 bodmi tvare (perami)
* 20 bidmi tvare (perami)
* 27 bodmi tvare (pery + brada, nos, oci, ...)
* 68 bodmi tvare - povodny model
