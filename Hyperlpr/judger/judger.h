//
// Created by 庾金科 on 6/26/16.
//

#ifndef HYPERLPR_JUDGER_H
#define HYPERLPR_JUDGER_H

#endif //HYPERLPR_JUDGER_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include "tiny_cnn/tiny_cnn.h"
#include "../recognizer/chinese/CN_PLATE.h"
using namespace tiny_cnn;
using namespace tiny_cnn::activation;
using namespace std;



#define DEBUG_EXTEND


class judger{

private:

    network<sequential> nn;
    const int INPUT_SIZE = 96;
    template <typename Activation>
    double rescale(double x) {
        Activation a;
        return 100.0 * (x - a.scale().first) / (a.scale().second - a.scale().first);
    }

// 46 12


// convert tiny_cnn::image to cv::Mat and resize

    cv::Mat image2mat(image<>& img) {
        cv::Mat ori(img.height(), img.width(), CV_8U, &img.at(0, 0));
        cv::Mat resized;
        cv::resize(ori, resized, cv::Size(), 3, 3, cv::INTER_AREA);
        return resized;
    }

    void construct_net(network<sequential>& nn) {
        // connection table [Y.Lecun, 1998 Table.1]
#define ReLu relu
        // construct nets
        nn << convolutional_layer<ReLu>(46, 12, 19,5, 1, 6) // 32x32 in, 5x5 kernel, 1-6 fmaps conv
        << max_pooling_layer<ReLu>(8,28, 6, 4)
        <<fully_connected_layer<softmax>(6*2*7, 2);



    }



    void convert_image(cv::Mat img,
                       double minv,
                       double maxv,
                       int w,
                       int h,
                       vec_t& data) {
        if (img.data == nullptr) return; // cannot open, or it's not an image

        cv::Mat_<uint8_t> resized;



        cv::resize(img, resized, cv::Size(w, h));
        // mnist dataset is "white on black", so negate required
        std::transform(resized.begin(), resized.end(), std::back_inserter(data),
                       [=](uint8_t c) { return (255 - c) * (maxv - minv) / 255.0 + minv; });
    }

public:
    judger(string filename_model){

        construct_net(nn);

        //    load nets
        ifstream ifs(filename_model.c_str());
        ifs >> nn;

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

        auto t_h_half = (INPUT_SIZE/lw_ratio)/2;


        auto t_w_half = (INPUT_SIZE/2);

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

    void judge(cv::Mat img,vector<double>  &results){

        vec_t data;

        convert_image(img,-1, 1, 46, 12, data);
        auto res = nn.predict(data);
        vector<pair<double, int> > scores;
        auto _size = 2;

        for (int i = 0; i < 2; i++) {
            float conf_val = rescale<softmax>(res[i]);
            results.emplace_back(conf_val);
        }

    }
    bool judge(cv::Mat img){

        //if(img.rows != 96 or img.cols != 96 or img.channels() != 1) throw("Error: img.rows != 96 or img.cols != 96 or img.channels() != 1 [ in judge model function: judge(cv::Mat img,vector<double>  &results) ]");
        cvtColor(img,img,CV_BGR2GRAY);

        bitwise_not(img,img);
        resize(img,img,cv::Size(46,12));


        vec_t data;

        convert_image(img,-1, 1, 46, 12, data);


        auto res = nn.predict(data);

        vector<pair<double, int> > scores;
        if(res.size()!=2) throw("Error: Scores.size()!=2 [ in judge model function: judge(cv:Mat img) ]");
        float pre_0  = rescale<softmax>(res[0]);
        float pre_1 =  rescale<softmax>(res[1]);
        if(pre_0>=pre_1) return 1;
        return 0;

    }




};