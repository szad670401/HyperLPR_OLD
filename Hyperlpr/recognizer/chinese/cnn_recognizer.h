//
// Created by 庾金科 on 6/25/16.
//

#define RECONGIZER_IMP

#ifdef RECONGIZER_IMP

#include <iostream>
#include "../charRecognizer.h"
#include "tiny_cnn/tiny_cnn.h"

#endif //UNIT_TEST_SEG_RECONGIZE_H




#define print(x) cout<<x<<endl;
//#define DEBUG_CNN_VIS



using namespace tiny_cnn;
using namespace tiny_cnn::activation;

using namespace std;

class recognizer_cnn:public charRecognizer {

private:
    vector<string> chars{"京","沪","津","渝","冀","晋","蒙","辽","吉","黑","苏","浙","皖","闽","赣","鲁","豫","鄂","湘","粤","桂","琼","川","贵","云","藏","陕","甘","青","宁","新","0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F","G","H","J","K","L","M","N","P","Q","R","S","T","U","V","W","X","Y","Z"};

    network<sequential> nn;
    template <typename Activation>
    double rescale(double x) {
        Activation a;
        return 100.0 * (x - a.scale().first) / (a.scale().second - a.scale().first);
    }



// convert tiny_cnn::image to cv::Mat and resize

    template <typename T> bool checkExist(vector<T> range,T exist){
        for(int i = 0 ; i<range.size();i++){
            if(range[i] ==  exist) return true;

        }
        return false;

    }
    cv::Mat image2mat(image<>& img) {
        cv::Mat ori(img.height(), img.width(), CV_8U, &img.at(0, 0));
        cv::Mat resized;
        cv::resize(ori, resized, cv::Size(), 3, 3, cv::INTER_AREA);
        return resized;
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

    void construct_net(network<sequential>& nn) {
        // connection table [Y.Lecun, 1998 Table.1]
#define ReLu relu

        // construct nets

        nn << convolutional_layer<tan_h>(28, 28 ,5, 1, 6)  // C1, 1@32x32-in, 6@28x28-out
        << average_pooling_layer<tan_h>(24, 24, 6, 2)   // S2, 6@28x28-in, 6@14x14-out
        << convolutional_layer<tan_h>(12, 12, 5, 6, 16)
        << average_pooling_layer<tan_h>(8, 8, 16, 2)  // S4, 16@10x10-in, 16@5x5-out
        << convolutional_layer<tan_h>(4, 4, 4, 16, 120) // C5, 16@5x5-in, 120@1x1-out
        << fully_connected_layer<tan_h>(120, 65);       // F6, 120-in, 10-out
    }

public:

#define CHAR_SIZE  28

    recognizer_cnn(string filename_model)
    {


        construct_net(nn);

        // load nets
        ifstream ifs(filename_model.c_str());
        ifs >> nn;

    }
//    void load_model(string filename_model)
//    {
//        construct_net(nn);
//
//        // load nets
//        ifstream ifs(filename_model.c_str());
//        ifs >> nn;
//
//
//    }


/*recongzie
 *
 *
 * recognize(cv::Mat img,                       待识别图像
 *          vector<int> range,                  字符的范围
 *          pair<double,double> confidence,     置信度区间
 *          vector<pair<double, int> > results  结果
 *          int max                             允许的最大结果数量
 *
 *          )
 * */


    string findmax(vector<double> vec,int lo = 0,int hi = 65){
        double f = -1;
        int idx = -1;


        for(int i = lo ; i < hi;i++)
        {
           // cout<<"["<<chars[i]<<"]:"<<vec[i]<<endl;
            //cout<<"["<<i<<"] "<<vec[i]<<endl;

            if(vec[i] > f) {
                f = vec[i];
                idx = i;

            }
        }

//        cout<<"id:"<<idx<<"char:"<<chars[idx]<<endl;

        return chars[idx];

    }

//
//    void end_to_end_recongize(cv::Mat img,int patchwidth,cv::Mat &plot){
//
//        auto r_cols = img.cols -patchwidth+1 ;
//        auto r_rows = img.rows;
//
//
//        for(int i = 0; i<r_cols ; i++)
//        {
//            cv::Rect patch(i,0,patchwidth,r_rows);
//            cv::Mat  patch_img = img(patch);
//            cv::Mat size_img;
//
//            cv::resize(patch_img,size_img,cv::Size(20,20));
//
//            cv::threshold(size_img,size_img,0,255,CV_THRESH_BINARY + CV_THRESH_OTSU);
////            cv::imshow("size_img",size_img);
////            cv::waitKey(0);
//
//
//            vector<double>  result_scores;
//
//#define TEST
//#ifdef TEST
//            vector<int> va{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33};
//            vector<pair<double, int> > result_scores_test;
//            pair<double,double> range;
//            range.first = -1;
//            range.second = -1;
//            recongize(size_img,result_scores_test,va,range,65);
//            cout<<findmax(result_scores_test);
//#endif
//
//            //   recongize(size_img,result_scores);
//
//            cv::Mat singular_p(result_scores);
//            plot.push_back(singular_p);
//
//        }
//
//
//        plot = plot.reshape(0,r_cols);
//        plot = plot.t();
//
//
//
//    }

    string recognize_lite(cv::Mat img,int type){

        vec_t data;
        cv::copyMakeBorder(img,img,4,4,4,4,cv::BORDER_CONSTANT);
        cv::bitwise_not(img,img);
        convert_image(img,-1, 1, CHAR_SIZE, CHAR_SIZE, data);



        auto res = nn.predict(data);
        vector<pair<double, int> > scores;
        auto _size = 65;
        vector<double>  results;

        for (int i = 0; i < 65; i++) {
           // float conf_val = rescale<softmax>(res[i]);
            float conf_val = rescale<tan_h>(res[i]);
            results.emplace_back(conf_val);
        }
        pair<int,int> range;


        if(type == NUMBER + LETTER){
            range.first = 31;
            range.second =65;

        }
        else if(type == NUMBER){
            range.first = 31;
            range.second =41;

        }
        else if(type == LETTER){
            range.first = 41;
            range.second =65;
        }
        else if(type == PROVINCE)
        {
            range.first = 0 ;
            range.second = 31;

        }
        else
        {
            range.first = 31;
            range.second =65;
        }

        return findmax(results,range.first,range.second);

    }

//
//    void recongize(cv::Mat img,vector<pair<double, int> > &results
//            ,vector<int> range,pair<double,double> confidence,int maxOutput)
//
//
//    {
//        bitwise_not(img,img);
//        vec_t data;
//        convert_image(img,0, 1, CHAR_SIZE, CHAR_SIZE, data);
//        auto res = nn.predict(data);
//        vector<pair<double, int> > scores;
//        auto _size = 65;
//
//        double con_low = confidence.first;
//        double con_hi = confidence.second;
//        if(con_hi< 0) con_hi  = MAXFLOAT;
//        vector<pair<double,int>> exist_scores;
//
//
//        for (int i = 0; i < 65; i++) {
//            float conf_val = rescale<softmax>(res[i]);
//
//            if (conf_val > con_low && conf_val < con_hi) {
//                scores.emplace_back(conf_val, i);
//            }
//        }
//        auto num =  0 ;
//        for (int i = 0; i <scores.size() ; i++){
//
//            if(checkExist(range, scores[i].second)) {
//                exist_scores.emplace_back(scores[i]);
//            }
//        }
//
//
//
//        sort(exist_scores.begin(), exist_scores.end(), greater<pair<double, int>>());
//        for(int i = 0 ; i < maxOutput ; i++)
//        {
//            results.push_back(exist_scores[i]);
//
//        }
//#ifdef DEBUG_CNN_VIS
//
//
//        for (int i = 0; i < maxOutput; i++)
//            cout << results[i].second << "," << results[i].first << endl;
//
//        // visualize outputs of each layer
//        for (size_t i = 0; i < nn.layer_size(); i++) {
//            auto out_img = nn[i]->output_to_image();
//            cv::imshow("layer:" + std::to_string(i), image2mat(out_img));
//        }
//        // visualize filter shape of first convolutional layer
//
//        cv::waitKey(0);
//
//#endif
//
//    }

};
