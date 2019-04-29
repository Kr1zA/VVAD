// The contents of this file are in the public domain. See LICENSE_FOR_EXAMPLE_PROGRAMS.txt
/*

    This example program shows how to use dlib's implementation of the paper:
        One Millisecond Face Alignment with an Ensemble of Regression Trees by
        Vahid Kazemi and Josephine Sullivan, CVPR 2014
    
    In particular, we will train a face landmarking model based on a small dataset 
    and then evaluate it.  If you want to visualize the output of the trained
    model on some images then you can run the face_landmark_detection_ex.cpp
    example program with sp.dat as the input model.

    It should also be noted that this kind of model, while often used for face
    landmarking, is quite general and can be used for a variety of shape
    prediction tasks.  But here we demonstrate it only on a simple face
    landmarking task.
*/

// program na vytvaranie modeluhladania bodov na tvari
// upraveny subor - povodny je mozne najst na https://github.com/davisking/dlib/blob/master/examples/train_shape_predictor_ex.cpp


#include <dlib/image_processing.h>
#include <dlib/data_io.h>
#include <iostream>

using namespace dlib;
using namespace std;

// ----------------------------------------------------------------------------------------

std::vector<std::vector<double> > get_interocular_distances(
        const std::vector<std::vector<full_object_detection> > &objects
);
/*!
    ensures
        - returns an object D such that:    
            - D[i][j] == the distance, in pixels, between the eyes for the face represented
              by objects[i][j].
!*/

// ----------------------------------------------------------------------------------------

int main(int argc, char **argv) {
    try {

        dlib::array<array2d<unsigned char> > images_train, images_test;
        std::vector<std::vector<full_object_detection> > faces_train, faces_test;

        load_image_dataset(images_train, faces_train,
                           "../../../ibug_300W_large_face_landmark_dataset/trainUsta+7.xml");

//        pre otestovanie by bolo potrebne upravit testovaciu metodu viac info na https://github.com/davisking/dlib/blob/master/examples/train_shape_predictor_ex.cpp
//        load_image_dataset(images_test, faces_test, "../../ibug_300W_large_face_landmark_dataset/testUsta+7.xml");

        shape_predictor_trainer trainer;

        trainer.set_cascade_depth(12);
        trainer.set_num_trees_per_cascade_level(600);
        trainer.set_tree_depth(4);
        trainer.set_nu(0.1); //shrinkage faktor
        trainer.set_oversampling_amount(550);
        trainer.set_feature_pool_size(750);
        trainer.set_lambda(0.2);
        trainer.set_num_test_splits(300);

        trainer.set_num_threads(8);
        trainer.be_verbose();

        shape_predictor sp = trainer.train(images_train, faces_train);

        serialize("../data/models/27BodovyModel.dat") << sp;

    }
    catch (exception &e) {
        cout << "\nexception thrown!" << endl;
        cout << e.what() << endl;
    }
}

// ----------------------------------------------------------------------------------------

double interocular_distance(
        const full_object_detection &det
) {
    dlib::vector<double, 2> l, r;
    double cnt = 0;
    // Find the center of the left eye by averaging the points around 
    // the eye.
    for (unsigned long i = 36; i <= 41; ++i) {
        l += det.part(i);
        ++cnt;
    }
    l /= cnt;

    // Find the center of the right eye by averaging the points around 
    // the eye.
    cnt = 0;
    for (unsigned long i = 42; i <= 47; ++i) {
        r += det.part(i);
        ++cnt;
    }
    r /= cnt;

    // Now return the distance between the centers of the eyes
    return length(l - r);
}

std::vector<std::vector<double> > get_interocular_distances(
        const std::vector<std::vector<full_object_detection> > &objects
) {
    std::vector<std::vector<double> > temp(objects.size());
    for (unsigned long i = 0; i < objects.size(); ++i) {
        for (unsigned long j = 0; j < objects[i].size(); ++j) {
            temp[i].push_back(interocular_distance(objects[i][j]));
        }
    }
    return temp;
}

// ----------------------------------------------------------------------------------------

