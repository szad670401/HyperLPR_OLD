//
// Created by 庾金科 on 6/26/16.
//
#include <iostream>
#include <sstream>
#include "../segment/chars_segment.h"
#include "../charRecognizer.h"


using namespace easypr;
namespace CN_recognizer{

    const int INPUT_SIZE = 23;

    inline cv::Mat extendImage(cv::Mat img) {


        cv::Mat dst(INPUT_SIZE, INPUT_SIZE, CV_8UC1);
        const auto PLATE_W = img.cols;
        const auto PLATE_H = img.rows;
        vector<cv::Point2f> origin = {cv::Point2f(0, 0), //左上角
                                      cv::Point2f(PLATE_W, 0), //右上角
                                      cv::Point2f(0, PLATE_H), //左下角
                                      cv::Point2f(PLATE_W, PLATE_H)}; // 右下角

        float lw_ratio = float(PLATE_W) / PLATE_H;


        cv::Point2f center(INPUT_SIZE / 2, INPUT_SIZE / 2);

        auto t_h_half = img.rows / 2;


        auto t_w_half = (img.rows / 2);

        vector<cv::Point2f> imit = {cv::Point2f(INPUT_SIZE / 2 - t_w_half, INPUT_SIZE / 2 - t_h_half),
                                    cv::Point2f(INPUT_SIZE / 2 + t_w_half, INPUT_SIZE / 2 - t_h_half),
                                    cv::Point2f(INPUT_SIZE / 2 - t_w_half, INPUT_SIZE / 2 + t_h_half),
                                    cv::Point2f(INPUT_SIZE / 2 + t_w_half, INPUT_SIZE / 2 + t_h_half)};


        cv::Mat Pres_mat = getPerspectiveTransform(origin, imit);
        cv::warpPerspective(img, dst, Pres_mat, cv::Size(INPUT_SIZE, INPUT_SIZE));


#ifdef  DEBUG_EXTEND
        cv::imshow("EXTEND_IAMGE",dst);
    cv::waitKey(0);

#endif

        return dst;


    }

    string recongize(Mat reprocess, charRecognizer *ider,charRecognizer *assistIder) {


        Mat img = reprocess;

        resize(img, img, Size(136, 36));


        std::vector<Mat> matChars;

        auto m_charsSegment = new CCharsSegment();
        int result = m_charsSegment->charsSegment(img, matChars);

        ostringstream oss;


        for (int i = 0; i < matChars.size(); i++) {
            Mat char_ = matChars[i];
            int state = 0 ;

            if (i == 0) {
                //bitwise_not(char_,char_);
                char_ = extendImage(char_);
                string res_char =  ider->recognize_lite(char_, ider->PROVINCE);
                if(res_char=="*")
                    res_char = assistIder->recognize_lite(char_, ider->PROVINCE);
                oss<<res_char;

            }
            if (i == 1) {
                //bitwise_not(char_,char_);
                char_ = extendImage(char_);
                string res_char  =  ider->recognize_lite(char_, ider->LETTER);
                if(res_char=="*")
                    res_char = assistIder->recognize_lite(char_, ider->LETTER);
                oss<<res_char;

            }
            if (i > 1) {
                char_ = extendImage(char_);
                //bitwise_not(char_,char_);
                string res_char = ider->recognize_lite(char_, ider->LETTER + ider->NUMBER);
                if(res_char=="*")
                    res_char = assistIder->recognize_lite(char_, ider->LETTER + ider->NUMBER);
                oss<<res_char;

            }



        }

        return oss.str();


    }


}