//
// Created by 庾金科 on 6/26/16.
//

#ifndef HYPERLPR_CONFIG_H
#define HYPERLPR_CONFIG_H

#endif //HYPERLPR_CONFIG_H

#include <string>

namespace LPR{

    namespace CONFIG{

        class Config{


        public:



            std::string judger_path;
            std::string recongizer_path;



            int _depth_scale_space = 2; //尺度空间深度

            int _scale_factor = 3;//尺度空间系数

            int _max_plates = 4; //同一张图片中最大可识别的车牌个数


            int METHOD_DETECT = 0 ;
            //检测方法
            // 0 多尺度启发式检测
            // 1 非尺度检测


            int METHOD_CORRECT = 0 ;
            //校正车牌方法
            //0 radon变换
            //1 hough变换 none
            //2 透视变换
            //3 基于矩的校正

            int METHOD_JUDGE = 2 ;

            //车牌判断方法

            // 0 SVM(支持向量机)分类器                  by:easy_pr
            // 1 NORMAL_Byes(朴素贝叶斯)分类器 trained  by:jack yu
            // 2 CNN(卷积神经网络)分类器       trained  by:jack yu

            int METHOD_RECONGIZE = 0 ;
            //识别方法

            // 0 R_METHOD_TESSERACT  使用 tesseract 进行识别                 by:[tesseract_ocr]
            // 1 R_METHOD_CNN        使用 CNN(卷积神经网络) 进行识别 trained   by:jack yu
            // 2 R_METHOD_EASY_PR    使用 NN 进行识别 权值文件                 by:easy_pr

            //开关

            bool SWITCH_CRFs  = 0;         //条件随机场开关 v x.2

            bool SWITCH_ROT_CORRECT_ = 1 ; //是否开启多角度识别
            bool SWITCH_WB = 0;            //是否自动校准白平衡. 用于 非工业相机


            bool ACC_OPTION_TBB = 0 ;
            bool ACC_OPTION_AVX = 0 ;


            Config(std::string judger_path_,std::string recongizer_path_){
                judger_path = judger_path_;
                recongizer_path = recongizer_path_;

            }
            Config();



        };




    }


}