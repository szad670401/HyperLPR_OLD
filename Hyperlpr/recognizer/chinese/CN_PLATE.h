//
// Created by 庾金科 on 6/26/16.
//

#ifndef HYPERLPR_CN_PLATE_H
#define HYPERLPR_CN_PLATE_H

#endif //HYPERLPR_CN_PLATE_H

#define print(x) cout<<x<<endl;

#include <iostream>
#include "../segment/chars_segment.h"
#include "../recongizer.h"

using namespace tiny_cnn;
using namespace tiny_cnn::activation;
using namespace easypr;

#ifndef UNIT_TEST_SEG_CN_PLATE_H
#define UNIT_TEST_SEG_CN_PLATE_H

#endif //UNIT_TEST_SEG_CN_PLATE_H


class CN_PLATE_recognizer : public recognizer{

private:
    const int INPUT_SIZE = 22;

public:
    CN_PLATE_recognizer(string filename_model):recognizer(filename_model){

    }

    cv::Mat extendImage(cv::Mat img){


        cv::Mat dst(INPUT_SIZE,INPUT_SIZE,CV_8UC1);
        const auto PLATE_W = img.cols;
        const auto PLATE_H = img.rows;
        vector<cv::Point2f> origin = {cv::Point2f(0, 0), //左上角
                                      cv::Point2f(PLATE_W, 0), //右上角
                                      cv::Point2f(0, PLATE_H), //左下角
                                      cv::Point2f(PLATE_W, PLATE_H)}; // 右下角

        float lw_ratio =  float(PLATE_W)/PLATE_H;


        cv::Point2f center(INPUT_SIZE/2,INPUT_SIZE/2);

        auto t_h_half = img.rows/2;




        auto t_w_half = (img.rows/2);

        vector<cv::Point2f> imit = {cv::Point2f(INPUT_SIZE/2 - t_w_half ,INPUT_SIZE/2 - t_h_half ),
                                    cv::Point2f(INPUT_SIZE/2 + t_w_half , INPUT_SIZE/2 - t_h_half ),
                                    cv::Point2f(INPUT_SIZE/2 - t_w_half , INPUT_SIZE/2 + t_h_half ),
                                    cv::Point2f(INPUT_SIZE/2 + t_w_half, INPUT_SIZE/2 + t_h_half )};



        cv::Mat Pres_mat = getPerspectiveTransform(origin,imit);
        cv::warpPerspective(img,dst,Pres_mat,cv::Size(INPUT_SIZE,INPUT_SIZE));



#ifdef  DEBUG_EXTEND
        cv::imshow("EXTEND_IAMGE",dst);
    cv::waitKey(0);

#endif

        return dst;




    }



    string recongize(Mat reprocess){


        Mat img = reprocess;

        resize(img,img,Size(136,36));


        std::vector<Mat> matChars;

        auto m_charsSegment = new CCharsSegment();
        int result = m_charsSegment->charsSegment(img, matChars);

        vector<int> chNum{31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64};
        vector<int> alphabet{41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64};
        vector<int> Province{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30};
        pair<double,double> range;
        range.first = -1;
        range.second = -1;
        ostringstream  oss;


        for(int i = 0 ; i < matChars.size();i++)
        {

            Mat char_= matChars[i];
            auto val = -1;
//            vector<pair<double, int> > result_scores_test;
//            if(i==0){
//                //char_ = extendImage(char_);
//                //bitwise_not(char_,char_);
//                recognizer::recongize(char_,result_scores_test,Province,range,10);}
//            if(i==1){
//                //char_ = extendImage(char_);
//                bitwise_not(char_,char_);
//                recognizer::recongize(char_,result_scores_test,alphabet,range,10);}
//            if(i>1)
//            {   //char_ = extendImage(char_);
//                bitwise_not(char_,char_);
//                recognizer::recongize(char_,result_scores_test,chNum,range,10);}
//            if(result_scores_test.size()>0)
//                oss<<recognizer::findmax(result_scores_test);


            vector<double > scores;
            if(i==0){
                char_ = extendImage(char_);
                bitwise_not(char_,char_);
                recognizer::recongize(char_,scores);
                oss<<recognizer::findmax(scores,0,31);
            }
            if(i==1){
                char_ = extendImage(char_);
                bitwise_not(char_,char_);
                recognizer::recongize(char_,scores);
                oss<<recognizer::findmax(scores,41,65);
            }
            if(i>1)
            {   char_ = extendImage(char_);
                bitwise_not(char_,char_);
                recognizer::recongize(char_,scores);
                oss<<recognizer::findmax(scores,31,65);
            }

#ifdef DEBUG

            //cout<<chartable[recongizer1.findmax(result_scores_test)]<<endl;
        ostringstream os;
        os<<"img:"<<i;
        imshow(os.str(),matChars[i]);
#endif



        }

        return oss.str();



    }



};

