//
// Created by kriza on 13.3.2019.
//

#include "VVAD.h"

Output::Output(bool talking, int count_of_faces, std::vector<rectangle> faces) {
    _talking = talking;
    _count_of_faces = count_of_faces;
    _faces = faces;
}

int Output::get_count_of_faces() { return _count_of_faces; }

bool Output::it_talking() { return _talking; }

std::vector<rectangle> Output::get_faces_positions() { return _faces; }

VVAD::VVAD(string &model) {
    _detector = get_frontal_face_detector();
    deserialize(model) >> _pose_model;
    _time_from_last_frame = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    for (int i = 0; i < _count_of_last_difference_ratios; ++i) {
        _last_ten_differences_between_ratios[i] = 0;
    }
}

float VVAD::getThreshold() { return _speech_threshold; }

void VVAD::setThreshold(float threshold) { _speech_threshold = threshold; }

bool VVAD::isCalibrated() { return _training_done; }

void VVAD::isCalibrated(bool training_done) { _training_done = training_done; }

bool VVAD::CheckSpeechInLastFrames() {
    for (int i = _count_of_last_difference_ratios - 1; i >= 0; --i) {
        if (_last_ten_differences_between_ratios[i] > _speech_threshold)
            return true;
    }
    return false;
}

void VVAD::ShiftAndAddRatioDifference(float next) {
    for (int i = 1; i < _count_of_last_difference_ratios; ++i) {
        _last_ten_differences_between_ratios[i - 1] = _last_ten_differences_between_ratios[i];
    }
    _last_ten_differences_between_ratios[_count_of_last_difference_ratios - 1] = next;
}

void VVAD::FindThreshold() {
    isCalibrated(true);

// kedze prva hodnota zvykla byt nejaka prilis vysoka nastavime ju na 0
    _diferences_for_training_threshold[0] = 0;

// pre vypis pre vytvorenie grafov
//    std::vector<float> tmp;
//    for (int i = 0; i < _diferences_for_training_threshold.size(); ++i) {
//        tmp.push_back(_diferences_for_training_threshold[i]);
//    }

// usporiadame hodnoty
    sort(_diferences_for_training_threshold.begin(), _diferences_for_training_threshold.end());

// zoberieme 12 najvacsiu hodnotu ako prah
    float percentage = 100 - 6;
    _speech_threshold = _diferences_for_training_threshold[(int) (percentage * 2)];

// vypis pre vytvorenie grafov
//    cout << "_diferences_for_training_threshold: " << endl;
//    for (int i = 0; i < _diferences_for_training_threshold.size(); ++i) {
//        cout << tmp[i] << "\t" << _speech_threshold << endl;
//    }
//    cout << "konec" << endl;

// ulozime prah do suboru s nazvom threshold
    std::string threshold = "threshold";
    SaveThreshodToFile(_speech_threshold, threshold);
    return;
}

bool VVAD::FrameForLearningThreshold(cv::Mat &frame) {

    _frame = frame;

// s casom pracujem tak, aby neboli medzery medzi spracovavaniami snimky velke
    _actual_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    if (_actual_time - _time_from_last_frame < 50) {
        return false;
    }
    _time_from_last_frame = _actual_time - ((_actual_time - _time_from_last_frame) - 50);
    if (_actual_time - _time_from_last_frame > 50) {
        _time_from_last_frame = _actual_time;
    }

// snimok na vstupe spracujeme tak aby s nim vedele kniznica Dlib pracovat
// ziskame 2 snimky pre Dlib jeden preskalovany a jeden v origilan velkosti
    cv_image<bgr_pixel> scaledImage = GetScaledFrame();
    cv_image<bgr_pixel> originalFrame = GetFrame();

// preskalovany snimok posleme na detekciu tvari
    std::vector<rectangle> faces = _detector(scaledImage);

// ak tvar nie je jedna vraciame false
    if (faces.size() != 1) {
        cout << "There must be only 1 face! Faces count: " << faces.size() << endl;
        _diferences_for_training_threshold.push_back(0);
        return false;
    }

// preskalujeme najdenu poziciu tvare tak aby sedela v povodnom obrazku
    faces[0].bottom() *= _dividing_constant;
    faces[0].top() *= _dividing_constant;
    faces[0].left() *= _dividing_constant;
    faces[0].right() *= _dividing_constant;

// na povodnom obrazku najdeme body na tvari
    _shape = _pose_model(originalFrame, faces[0]);

// zratame abs hodnotu rozdielov pomerov medzi dvoma po sebe spracovanymi snimkami
    float differenceBetweenRatios = ComputeDifferenceBetweenRatios();

// ulozime vypocitanu hodnotu do pola 200 poslednych snimok
    _diferences_for_training_threshold.push_back(differenceBetweenRatios);

    _counter_of_frames++;

//    ak chcem vykreslovat body na videu treba odkomentovat
//    _win.clear_overlay();
//    _win.set_image(originalFrame);
//    _win.add_overlay(render_face_detections(_shape));

// ak bolo spracovanych 200 snimkov zratame prah a vratim true
    if (_counter_of_frames == 200) {
        FindThreshold();
        return true;
    }
    return false;
}

Output VVAD::Frame(cv::Mat &frame) {

    _frame = frame;

// s casom pracujem tak, aby neboli medzery medzi spracovavaniami snimky velke
    _actual_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    if (_actual_time - _time_from_last_frame < 50) {
        std::vector<rectangle> faces;
        return Output(false, -1, faces);
    }
    _time_from_last_frame = _actual_time - ((_actual_time - _time_from_last_frame) - 50);
    if (_actual_time - _time_from_last_frame > 50) {
        _time_from_last_frame = _actual_time;
    }

// snimok na vstupe spracujeme tak aby s nim vedele kniznica Dlib pracovat
// ziskame 2 snimky pre Dlib jeden preskalovany a jeden v origilan velkosti
    cv_image<bgr_pixel> scaledFrame = GetScaledFrame();
    cv_image<bgr_pixel> originalFrame = GetFrame();

// preskalovany snimok posleme na detekciu tvari
    std::vector<rectangle> faces = _detector(scaledFrame);

    //  ak tvar nie je jedna vraciame false
    if (faces.size() != 1) {
        ShiftAndAddRatioDifference(0);
        return Output(false, faces.size(), faces);
    }

// preskalujeme najdenu poziciu tvare tak aby sedela v povodnom obrazku
    faces[0].bottom() *= _dividing_constant;
    faces[0].top() *= _dividing_constant;
    faces[0].left() *= _dividing_constant;
    faces[0].right() *= _dividing_constant;

// na povodnom obrazku najdeme body na tvari
    _shape = _pose_model(originalFrame, faces[0]);

// zratame abs hodnotu rozdielov pomerov medzi dvoma po sebe spracovanymi snimkami
    float differenceBetweenRatios = ComputeDifferenceBetweenRatios();

// pridam hodnotu to pola 15 poslednych zratanych hodnot
    ShiftAndAddRatioDifference(differenceBetweenRatios);

// poslem zratanu hodnotu na spracovanie
    bool isTalking = CheckSpeechInLastFrames();

// ak chceme vypisovat informacie pre vytvorenie grafov
//    if (isTalking) {
//        cout << differenceBetweenRatios << "\t0.07\t" << _speech_threshold << endl;
//    } else {
//        cout << differenceBetweenRatios << "\t\t" << _speech_threshold << endl;
//    }

//    ak chceme vykreslovat body na videu treba odkomentovat
//    _win.clear_overlay();
//    _win.set_image(originalFrame);
//    _win.add_overlay(render_face_detections(_shape));

    return Output(isTalking, faces.size(), faces);
}

float VVAD::ComputeDifferenceBetweenRatios() {
    float percentage = 0.15;

    if (_first_time_computing_difference_between_ratios) {
        _last_shape = _shape;
        _first_time_computing_difference_between_ratios = false;
    } else {
// vyhladzovanie bodov tak, ze aktualna pozicia bodov na tvari je 15% predchadzajucich pozicii a 85% aktualnych pozicii
        for (int i = 0; i < _shape.num_parts(); ++i) {
            _shape.part(i) = (1 - percentage) * _shape.part(i) + (percentage) * _last_shape.part(i);
        }
        _last_shape = _shape;

    }

// vypocitam pravy, lavy, horny a dolny bod na perach
    dpoint left = (_shape.part(_mouth_points_outside[0]) + _shape.part(_mouth_points_inside[0])) / 2;
    dpoint right = (_shape.part(_mouth_points_outside[1]) + _shape.part(_mouth_points_inside[1])) / 2;

    dpoint up = (_shape.part(_mouth_points_outside[2]) +
                 _shape.part(_mouth_points_outside[3]) +
                 _shape.part(_mouth_points_inside[2]) +
                 _shape.part(_mouth_points_inside[3])) / 4;
    dpoint down = (_shape.part(_mouth_points_outside[4]) +
                   _shape.part(_mouth_points_outside[5]) +
                   _shape.part(_mouth_points_inside[4]) +
                   _shape.part(_mouth_points_inside[5])) / 4;

// z vypocitanych bodov zratam vysku a sirku pier a pomer pier
    float width = length(right - left);
    float height = length(up - down);

    float ratio = height / width;

    float differenceBetweenRatios = abs(_last_ratio - ratio);
    _last_ratio = ratio;

    return differenceBetweenRatios;
}

void VVAD::SaveThreshodToFile(float threshold, string &filename) {
    serialize(filename) << threshold;
}

void VVAD::LoadThresholdFromFile(string &filename) {
    float threshold = -1;
    deserialize(filename) >> threshold;
    _speech_threshold = threshold;
}

cv_image<bgr_pixel> VVAD::GetFrame() {
    cv_image<bgr_pixel> cimg(_frame);
    return cimg;
}

cv_image<bgr_pixel> VVAD::GetScaledFrame() {
    int cols = _frame.cols;
    _dividing_constant = cols / 400.0;
    resize(_frame, _scaled_frame, cv::Size(), 1.0 / _dividing_constant, 1.0 / _dividing_constant);
    cv_image<bgr_pixel> cimg(_scaled_frame);
    return cimg;
}
