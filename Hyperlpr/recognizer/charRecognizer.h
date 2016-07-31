#ifndef UNIT_TEST_SEG_RECOGNIZER_H

#define UNIT_TEST_SEG_RECOGNIZER_H


#include<vector>
#include<opencv2/opencv.hpp>
using namespace cv;
using namespace std;

class charRecognizer {
public:
    virtual string recognize_lite(cv::Mat char_img,int type)=0;
    virtual pair<string,float> recognize(cv::Mat char_img,int type){}

    const int8_t NUMBER = 1;
    const int8_t LETTER = 2;
    const int8_t PROVINCE = 4;

};

#endif //UNIT_TEST_SEG_RECOGNIZER_H

