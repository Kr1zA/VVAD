//
// Created by kriza on 13.3.2019.
//

#include "VVAD.h"

#include <thread>
#include <chrono>

using namespace dlib;
using namespace std;
using namespace cv;

long getTime() {
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

int main(int argc, char **argv) {

    long timeFromLastFrame;
    long actualTime;

    string videa = "riso";

    if (argc != 3) {
        cout << "Call this program like this:" << endl;
        cout << "./testVVAD ../data/models/shape_predictor_68_face_landmarks.dat ../data/" << endl;
        return 0;
    }

    string model = argv[1];
    string umiestnenieVidei = argv[2];

    VVAD vvad(model);

// pomocou kniznice OpenCV nacitavame snimky z videa 
    VideoCapture cap(umiestnenieVidei + videa + "Trenovacie.mp4");
    if (!cap.isOpened()) {
        cerr << "Unable to connect to camera/video" << endl;
        return 1;
    }

    Mat frame;

    cout << "for start learning threshold for " << videa << " press enter" << endl;
    while (cin.get() != '\n');
    cout << "Training started!" << endl;

    long trainingStartTime = getTime();
    timeFromLastFrame = getTime();

    while (true) {
        actualTime = getTime();
        if (actualTime - timeFromLastFrame >= 33) {
            if (!cap.read(frame)) {
                break;
            }
            timeFromLastFrame = actualTime;

            if (vvad.FrameForLearningThreshold(frame)) break;
        }
    }

// informacie o celkovom case behu treningu
    cout << "cas trvania treningu: " << (getTime() - trainingStartTime) << "ms" << endl;

// ak este nedoslo trenovacie video na koniec zavrieme stream
    if (cap.isOpened()) {
        cap.release();
    }

    cap.open(umiestnenieVidei + videa + "Testovacie.mp4");
    if (!cap.isOpened()) {
        cerr << "Unable to connect to camera/video" << endl;
        return 1;
    }

    cout << "Testing started!" << endl;

    timeFromLastFrame = getTime();
    long testingStartTime = getTime();

    while (true) {
        actualTime = getTime();
        if (actualTime - timeFromLastFrame >= 33) {

            if (!cap.read(frame)) {
                break;
            }
            timeFromLastFrame = actualTime;

            Output o = vvad.Frame(frame);
            if (o.get_count_of_faces() == 1)
                cout << "prebieha rec " << o.it_talking() << endl;
        }
    }

// informacie o celkovom case behu testu
    cout << "Cas testovania: " << getTime() - testingStartTime << "ms" << endl;
}

