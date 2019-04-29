//
// Created by kriza on 13.3.2019.
//
#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/opencv.hpp"
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <chrono>
#include <math.h>

#ifndef EXAMPLES_VVAD_H
#define EXAMPLES_VVAD_H

using namespace dlib;
using namespace std;
using namespace std::chrono;

class Output {
public:
    explicit Output(bool talking, int count_of_faces, std::vector<rectangle> faces);

    bool it_talking();
    int get_count_of_faces();
    std::vector<rectangle> get_faces_positions();

private:
    bool _talking;
    int _count_of_faces;
    std::vector<rectangle> _faces;
};

class VVAD {
public:
    explicit VVAD(string &model);

    Output Frame(cv::Mat &frame);
    bool FrameForLearningThreshold(cv::Mat &frame);
    void SaveThreshodToFile(float threshold, std::string &filename);
    void LoadThresholdFromFile(std::string &filename);
    float getThreshold();
    void setThreshold(float threshold);
    bool isCalibrated();
    void isCalibrated(bool training_done);

private:
// vyber bodov, ktore sa pouziju

// 68 bodovy model
// out
    int _mouth_points_outside[6] = {48, 54, 50, 52, 56, 58};
//    int _mouth_points_inside[6] = {48, 54, 50, 52, 56, 58};
// in
//    int _mouth_points_outside[6] = {60, 64, 61, 63, 65, 67};
    int _mouth_points_inside[6] = {60, 64, 61, 63, 65, 67};

// 27 bodovy model
//    int mouthPoints1outside[6] = {48, 54, 50, 52, 56, 58};
//    int mouthPoints1inside[6] = {60, 64, 61, 63, 65, 67};

//    int _mouth_points_outside[6] = {mouthPoints1outside[0] - 48 + 7,
//                                    mouthPoints1outside[1] - 48 + 7,
//                                    mouthPoints1outside[2] - 48 + 7,
//                                    mouthPoints1outside[3] - 48 + 7,
//                                    mouthPoints1outside[4] - 48 + 7,
//                                    mouthPoints1outside[5] - 48 + 7};
//    int _mouth_points_inside[6] = {mouthPoints1outside[0] - 48 + 7,
//                                   mouthPoints1outside[1] - 48 + 7,
//                                   mouthPoints1outside[2] - 48 + 7,
//                                   mouthPoints1outside[3] - 48 + 7,
//                                   mouthPoints1outside[4] - 48 + 7,
//                                   mouthPoints1outside[5] - 48 + 7};

//   int _mouth_points_inside[6] = {mouthPoints1inside[0] - 48 + 7,
//                                   mouthPoints1inside[1] - 48 + 7,
//                                   mouthPoints1inside[2] - 48 + 7,
//                                   mouthPoints1inside[3] - 48 + 7,
//                                   mouthPoints1inside[4] - 48 + 7,
//                                   mouthPoints1inside[5] - 48 + 7};
//   int _mouth_points_outside[6] = {mouthPoints1inside[0] - 48 + 7,
//                                   mouthPoints1inside[1] - 48 + 7,
//                                   mouthPoints1inside[2] - 48 + 7,
//                                   mouthPoints1inside[3] - 48 + 7,
//                                   mouthPoints1inside[4] - 48 + 7,
//                                   mouthPoints1inside[5] - 48 + 7};

    float _last_ratio = 0;
    int _count_of_last_difference_ratios = 15;
    float _last_ten_differences_between_ratios[15];
    float _speech_threshold = 0.03;
    std::vector<float> _diferences_for_training_threshold;
    int _counter_of_frames = 0;
    bool _training_done = false;
    frontal_face_detector _detector;
    shape_predictor _pose_model;
    long _actual_time, _time_from_last_frame;
    full_object_detection _shape;
    full_object_detection _last_shape;
    bool _first_time_computing_difference_between_ratios = true;
    cv::Mat _frame;
    cv::Mat _scaled_frame;
    float _dividing_constant;

// ak by sme chceli vykreslovat body na videu
//    image_window _win;

    bool CheckSpeechInLastFrames();
    void ShiftAndAddRatioDifference(float next);
    void FindThreshold();
    float ComputeDifferenceBetweenRatios();
    cv_image<bgr_pixel> GetFrame();
    cv_image<bgr_pixel> GetScaledFrame();
};

#endif //EXAMPLES_VVAD_H
