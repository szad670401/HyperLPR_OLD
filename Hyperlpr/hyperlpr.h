//
// Created by 庾金科 on 6/26/16.
//

#ifndef HYPERLPR_HYPERLPR_H
#define HYPERLPR_HYPERLPR_H

#endif //HYPERLPR_HYPERLPR_H


#include "util/config.h"
#include "util/operate.h"
#include "locator/searcher.h"
#include "./recognizer/chinese/cnn_recognizer.h"
//#include "./recognizer/chinese/tesseract_recognizer.h"
//#include "./CN_PLATE.h"
//车牌类
#define PLATE
//配置表


using std::vector;
using std::string;
using cv::Mat;


namespace LPR{
    const string test_path_judger = "/Users/yujinke/Desktop/platejudge.data";
   const string test_path_regonzier = "/Users/yujinke/Desktop/LeNet-weights";
 //   const string test_path_regonzier = "/Users/yujinke/Documents/tiny-cnn/data/LeNet-weights";
    //const string test_path_regonzier = "/Users/yujinke/Documents/tiny-cnn/data/bak1/LeNet-weights 52.67% 3 2 2 2";
    const string test_path_tesseract = "/usr/local/Cellar/tesseract/3.04.00/share/tessdata";

        class HyperLPR {

        public:
//            HyperLPR(const LPR::CONFIG::Config CONFIG_LIST){
//                recognizer = new CN_PLATE_recognizer(CONFIG_LIST.recongizer_path);
//                platejudger = new judger(CONFIG_LIST.judger_path);
//
//
//            }
            HyperLPR(){
              //  recognizer_tess = new recogizer_tesseract(test_path_regonzier);
                recognizer_CNN = new recognizer_cnn(test_path_regonzier);


                platejudger = new judger(test_path_judger);


            }
            //加载配置表

            vector<string> PR_load_as_string(string filename) {
                vector<string> plates_name;
                Mat load_image = imread(filename);
                vector<_Plate> plates_ided =  PR_existent_as_PLATE(load_image);

                for(int i = 0 ; i<plates_ided.size();i++)
                {
                    plates_name.push_back(plates_ided[i].PlateName);
#define DEBUG
#ifdef DEBUG
                    Operate::drawPlate(load_image,plates_ided[i]._location);
                    putText(load_image,plates_ided[i].PlateName,Point(plates_ided[i]._location.x,plates_ided[i]._location.y-2),0,0.5,Scalar(75,100,255));
#endif

                    cout<<plates_ided[i].PlateName<<endl;
                }

//

                return plates_name;


            }

            vector<_Plate> PR_load_as_PLATE(string filename);
            // 加载图片进行识别. 识别后输出车牌属性

            vector<_Plate> PR_existent_as_PLATE(Mat prePr_img) {
#define DEBUG_PERF
#ifdef DEBUG_PERF
               auto test_break = getCurrentTime();

#endif

                vector<_Plate> plates_ided;
                vector<_CandidatePlate> plates;
                LPR::Plate_Search::Searching_plates(prePr_img, plates, 1);
                cout<<"time-consuming-locate/per image:"<<getCurrentTime()-test_break<<"ms"<<endl;
                for(int i = 0; i < plates.size();i++ ){
                    _CandidatePlate inv =  plates[i];
                    inv.doJudge(platejudger);
                    inv.doRecongize(recognizer_CNN,recognizer_CNN);

                    //DEBUG waitKey(0);

                    if(inv.isPlate)
                        plates_ided.push_back(inv);

                }
#ifdef DEBUG_PERF
               cout<<"time-consuming-full/per image:"<<getCurrentTime()-test_break<<"ms"<<endl;

#endif
                return plates_ided;


            }

            ~HyperLPR(){
                delete recognizer_tess;
                delete  recognizer_CNN;

                delete platejudger;


            }
        protected:
          //  LPR::CONFIG::Config CONFIG_LIST_PR; //默认配置表
            charRecognizer *recognizer_tess = nullptr;
            charRecognizer *recognizer_CNN = nullptr;


            judger *platejudger;





        };




}
