//
// Created by 庾金科 on 6/26/16.
//

#ifndef HYPERLPR_PLATE_H
#define HYPERLPR_PLATE_H

#endif //HYPERLPR_PLATE_H

#include "../util/operate.h"
#include "../judger/judger.h"

#pragma once
#define PLATE_W 136
#define PLATE_H 36

#define PLATE_DOWN_W 51
#define PLATE_DOWN_H 14

//#define DEBUG_PA
//#define DEBUG

#define ABS(T) ((((T>0)*2)-1)*T)
using namespace std;


namespace LPR {

    class _Plate {    public:
        _Plate(){

        }
        _Plate(Mat input){
            RGB = input;
        }
        string PlateName = "unknow";

        const unsigned PLATE_ID = 0;
        const unsigned PlATE_STRING = 1;
        Rect _location;
        string getPlateName();


        String getPlateImg();

        int getProvinceId(int type);

        //type
        String getChar(int loc);

        ~_Plate(){

        }

    protected:
        /*
         * Mat
         *
         *
         *
         *
         * */


        Mat RGB;


    };


    //候选车牌
    class _CandidatePlate : public _Plate {
    private:

        float computeRandonPlotAngle_m1(Mat plot,float unit);

        float computeRandonPlotAngle_m2(Mat plot,float unit);

    public:

        _CandidatePlate(Mat candidated,Mat _mask,Rect location){

            //input 必须为 24BIT
            //_mask 必须为 8bit
            _candidated  = candidated;
            mask_ = _mask;
            _location = location;




        }

        RotatedRect rotPara;

        float computeCorrectionAngle_randon(Mat img);


        float computeCorrectionAngle_contour(Mat img);

        void doRecongize(charRecognizer *which,charRecognizer *whichAssit);

        void doJudge(judger *which);

        void doRevise();

        void doGrey();

        void doThres();

        void doHSV();

        Mat correctPlate();

        bool isPlate = 0;
        //   ~_CandidatePlate();
    protected:

        Mat mask_;
        Mat _candidated;






    };
    void _CandidatePlate::doRecongize(charRecognizer *which,charRecognizer *whichAssit)
    {
        if(isPlate)
        {
            auto prepared  =  _Plate::RGB;
            _Plate::PlateName = CN_recognizer::recongize(prepared,which,whichAssit);

//#define DEBUG_RECONGIZE
#ifdef DEBUG_RECONGIZE
            imshow("re_recongize",prepared);
            waitKey(0);
#endif

        }
    }

    void _CandidatePlate::doJudge(judger *which){
        auto prepared  =  _Plate::RGB;
//#define DEBUG_RECONGIZE
#ifdef DEBUG_RECONGIZE
        imshow("prepared",prepared);
        waitKey(0);
#endif

        if(which->judge(prepared)) {
            isPlate = 1;

        }

    }


    void _CandidatePlate::doHSV() {
        cvtColor(_Plate::RGB,_Plate::RGB,CV_RGB2HSV);

    }

    float _CandidatePlate::computeRandonPlotAngle_m2(Mat samMap,float unit)
    {
        double minVal ;
        double maxVal;
        Point minLoc;
        Point maxLoc;
        minMaxLoc(samMap,&minVal,&maxVal,&minLoc,&maxLoc);
        return (maxLoc.y-16)*unit;



    }
    float _CandidatePlate::computeRandonPlotAngle_m1(Mat plot, float unit) {
        Mat samMap ;


        threshold(plot, samMap, 140, 255, CV_THRESH_BINARY);
        //DEBUG_SHOW("sampleing_map_thres",samMap);
        double _max_count = 0;
        double _recor_angle_ = 0;

        for (int _rows_peak = 0; _rows_peak < samMap.rows; _rows_peak++) {
            uchar *inData = samMap.ptr<uchar>(_rows_peak);
            //uchar *_inData = samMap.ptr<uchar>(_rows_peak);

            double _avg = 0;
            double var = 0;
            float _count_exist = 0;
            int _count_empty = 0;
            int _count_interval = 0 ;
            for (int _cols_peak = 0; _cols_peak < samMap.cols; _cols_peak++) {
                if (inData[_cols_peak] != 0) _count_exist++;
                else _count_empty++;
//                if(_cols_peak>0 && _cols_peak<  samMap.cols-1 && (
//                        (_inData[_cols_peak-1]*(_inData[_cols_peak]))<0 ||
//                        (_inData[_cols_peak+1]*(_inData[_cols_peak]))<0)
//                        )
//                    _count_interval++;

            }

            _count_exist /= _count_empty;
            // _count_exist *=_count_interval;

            if (_count_exist > _max_count) {
                _max_count = _count_exist;
                _recor_angle_ = (_rows_peak - 16) * unit ;

              //  cout<<"_rows_peak:"<<_rows_peak<<endl;

            };
        }
        return _recor_angle_;
    }

    float _CandidatePlate::computeCorrectionAngle_randon(Mat img){

#ifdef DEBUG_PA
        DEBUG auto c_t_2 = getCurrentTime();
#endif


        Mat load_img = img;

        //校正矩形
        Operate::EnhancePlate(load_img, load_img);
        //增强图像

        const double pi = 3.1415926;
        const double Pi_o_48 = pi / 36;
        //角度精度
        // (180/48)度
        // randon 变换图 的 rows为角度
        //


        Mat thres_plate;
        int rows = load_img.rows;
        int cols = load_img.cols;
        int depth_sample = 36;

        //采样深度 每次扫描的每步的长度为 (1/36)*length

        Mat sampleing_map(32, cols, CV_32F);
        sampleing_map.setTo(0);
        //adaptiveThreshold(plateImg, thres_plate, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 21, 1);
        threshold(load_img, thres_plate, 160, 255, CV_THRESH_BINARY);


        //阈值操作

        //Randon变换 过程
        for (int i = -16; i < 17; i++) {

            //当前角度
            auto angle = Pi_o_48 * (i);
            auto _Op_per = rows / cos(angle) / depth_sample;
            int _fit_unit_p = abs(int(tan(angle) * rows)) ;

            // 二选一
            //int _fit_unit_m = 0;
            //satisfy either of i< 0 or i>0
            //
            for (int _cols_i = ((i <= 0) * _fit_unit_p);  // if (i<0)   (i<0)= 1;

                 _cols_i < (cols - ((i > 0) * _fit_unit_p));  // if (i>0)    (i>0)= 0;

                 _cols_i++)
            {
                float _count_ = 1;

                for (int _sample_i = 0; _sample_i < depth_sample; _sample_i++) {
                    auto _len_Op = _Op_per * _sample_i;
                    int _x = _cols_i + int(_len_Op * sin(angle));
                    int _y = int(_len_Op * cos(angle));
                    uchar *inData = thres_plate.ptr<uchar>(_y);
                    char pix = inData[_x];
                    if (pix) _count_++;
                    //  inData[_x]  = 255;
                }
                _count_ /= depth_sample;

                sampleing_map.at<float>(i + 16, _cols_i) = _count_;
            }


        }




#ifdef DEBUG_PA
        DEBUG auto c_t_3 = getCurrentTime();
        DEBUG cout<<"correctPlate Distort"<<c_t_3-c_t_2<<"ms"<<endl;
#endif

        // *********************************************************************************************************
        // - Created by Jackyu 16-01-12
        //*********************************************************************************************************
        //find basic peak
        //*********************************************************************************************************
        //*********************************************************************************************************

        Mat sampleing_map_thres;
        // 采样图
        sampleing_map = sampleing_map * 255;
        sampleing_map.convertTo(sampleing_map_thres, CV_8UC1);


//sampleing_map_thres = sampleing_map_thres * 255;
        Mat Show;
        resize(sampleing_map_thres,Show,Size(sampleing_map_thres.cols*3,sampleing_map_thres.rows*3));
        //minmax()
        //     computeRandonPlotAngle_m2(sampleing_map_thres,Pi_o_48);

        return computeRandonPlotAngle_m1(sampleing_map_thres,Pi_o_48);




    }


    Mat  _CandidatePlate::correctPlate() {

#ifdef DEBUG_PA
        DEBUG auto c_t_1 = getCurrentTime();
#endif

        Mat preprocess_RGB =_candidated;
        Mat mask = mask_ ;/*Mask would be changed*/
        //!检测二值图像
        //旋转矩形
        vector<vector<Point>> contours;
        findContours(mask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
        //find max area
        double max = 0;
        int _max_id = -1;
        for (int _fmax = 0; _fmax < contours.size(); _fmax++) {
            auto area = contourArea(contours[_fmax]);
            if (area>400 && area > max) {
                max = area;
                _max_id = _fmax;
            }
        }

        if (_max_id == -1) throw "if (_max_id == -1)";// throw "_max_id == -1";//error 错误抛Y异常 [N]
        RotatedRect rotRecot = minAreaRect(contours[_max_id]);
        Rect boundingRect = rotRecot.boundingRect();
        int x;
        Point2f rot_rect_point[4];

        rotRecot.points(rot_rect_point);
        float angle = rotRecot.angle;

        vector<Point2f> active_points{rot_rect_point[0],
                                      rot_rect_point[1],
                                      rot_rect_point[2],
                                      rot_rect_point[3]};
        //固定点
        vector<Point2f> pers_points = {Point2f(PLATE_W, PLATE_H),
                                       Point2f(0, PLATE_H),
                                       Point2f(0, 0),
                                       Point2f(PLATE_W, 0)};
        vector<Point2f> approx_Points_coded;
        //       Operate::remapPoints(active_points, Point(boundingRect.x, boundingRect.y));
        Operate::resetPointPers<Point2f>(active_points, angle);
        Operate::resetPointPers<Point2f>(pers_points);
        Mat pers_mat = getPerspectiveTransform(active_points, pers_points);
        Mat _result_RBG(PLATE_H, PLATE_W, CV_8UC3);
        //[input]  load_img is rgb
        //[output] _result  is rgb
        warpPerspective(preprocess_RGB, _result_RBG, pers_mat, _result_RBG.size());

        auto c_t_3 = getCurrentTime() ;
        // cvtColor(_result_RBG, grey_result, CV_BGR2GRAY);
        Mat load_img;
        // _result_RBG.copyTo(load_img);
        resize(_result_RBG,load_img,Size(PLATE_DOWN_W,PLATE_DOWN_H));




        /*
         *
         *
         * 垂直方向校正
         *
         *
         *
         *
         */




        auto angle_cor = computeCorrectionAngle_randon(load_img);
        const int padding = 5;
        Point2f _distorted[3] = {
                Point2f(0 - _result_RBG.rows * (tan(angle_cor)) *
                            (angle_cor < 0),
                        0), Point2f(_result_RBG.rows * tan(angle_cor) *
                                    (angle_cor > 0),
                                    _result_RBG.rows),
                Point2f(_result_RBG.cols + (_result_RBG.rows * tan(angle_cor) * (angle_cor < 0)), _result_RBG.rows)
        };


        Mat corrent_mat(PLATE_H, PLATE_W + padding*2, CV_8SC3);
        Point2f _corrected[3] = {
                Point2f(padding, 0), Point2f(padding, corrent_mat.rows), Point2f(corrent_mat.cols - padding, corrent_mat.rows)
        };
        Mat warp_mat = getAffineTransform(_distorted, _corrected);
        corrent_mat.setTo(0);
        warpAffine(_result_RBG, corrent_mat, warp_mat, corrent_mat.size());

#ifdef DEBUG_PA
        DEBUG auto c_t_4 = getCurrentTime();
        DEBUG cout<<"correctPlate Distort Correct"<<c_t_4-c_t_3<<"ms"<<endl;
#endif


#ifdef DEBUG
    //   DEBUG_SHOW("corrent_mat",corrent_mat);
#endif
        corrent_mat.copyTo(_Plate::RGB);

        return corrent_mat;


    }


}


