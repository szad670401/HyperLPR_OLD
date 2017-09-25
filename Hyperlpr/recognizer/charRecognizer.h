#ifndef UNIT_TEST_SEG_RECOGNIZER_H

#define UNIT_TEST_SEG_RECOGNIZER_H


#include<vector>
#include<opencv2/opencv.hpp>
using namespace cv;
using namespace std;

//class plateChar{
//
//private:
//    int m_province_id;
//    int m_region_id;
//    int m_pos[5];
//    int m_plate_type = -1;
//public:
//    plateChar(int plate_type,int province,int region,int pos1,int pos2,int pos3,int pos4,int pos5)
//            m_plate_type = plate_type;
//            m_province_id = province;
//            m_region_id = region;
//            m_pos[0] = pos0;
//            m_pos[1] = pos1;
//            m_pos[2] = pos2;
//            m_pos[3] = pos3;
//            m_pos[4] = pos4;
//            m_pos[5] = pos5;
//    }
//
//
//    string convertToString(){
//
//    }
//
//    friend int compareWithOther(plateChar other)
//    {
//        mismatchCount = 0;
//        if(other.m_province_id != m_province_id)
//            mismatchCount++;
//        if(other.m_region_id != m_region_id)
//            mismatchCount++;
//        if(other.m_pos[0] != m_province_id)
//            mismatchCount++;
//
//    };
//

class charRecognizer {
public:
    virtual string recognize_lite(cv::Mat char_img,int type)=0;
    virtual pair<string,float> recognize(cv::Mat char_img,int type){}

    const int8_t NUMBER = 1;
    const int8_t LETTER = 2;
    const int8_t PROVINCE = 4;

};

#endif //UNIT_TEST_SEG_RECOGNIZER_H

