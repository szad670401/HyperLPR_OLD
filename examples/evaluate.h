//
// Created by 庾金科 on 7/10/16.
//
#include "../Hyperlpr/hyperlpr.h"
#include "../Hyperlpr/io/linux_dir.h"

#ifndef HYPERLPR_EVALUATE_H
#define HYPERLPR_EVALUATE_H

#endif //HYPERLPR_EVALUATE_H
using namespace std;
using namespace LPR;

int len(char* A){
    int len = 0;
    for(;A[len]!='.';len++);
    return len;

}

int stringdiff(char* A,char* B)
{

    int matched = 0 ;
    auto len_A = len(A);
    auto len_B = len(B);
    for(int i = 0; i<8;i++)
    {
        if (A[len_A-i] != B[len_B-i])
            matched++;
    }

    return matched;

}
//
//void eval_recongize(){
//    Directory_mac dir;
//    int start_time = getCurrentTime();
//
//    vector<string> filename = dir.GetListFiles("/Users/yujinke/Downloads/labeled", "*.jpg");
//    CN_PLATE_recognizer *recognizer = new CN_PLATE_recognizer( "/Users/yujinke/Documents/tiny-cnn/data/bak1/LeNet-weights 52.67% 3 2 2 2");
//    int count=  0;
//
//    for (int i = 0; i < filename.size(); i++) {
//        if (filename[i].find(".jpg") < INT32_MAX) {
//            Mat img = imread(filename[i]);
//
//            string res = recognizer->recongize(img);
//            cout<<res<<endl;
//            //imwrite("/Users/yujinke/Desktop/labeled/"+res+".jpg",img);
//            char* pos_dst =(char*)filename[i].data();
//            char* pos_res = (char*)(res+".").data();
//            int diff = stringdiff(pos_dst,pos_res);
//            if(diff == 0 )
//            {
//                count++;
//            }
////            cout<<pos_dst<<endl;
////            cout<<pos_res<<endl;
////
////
////            imshow("img", img);
////            waitKey(0);
//        }
//
//    }
//
//    int time_consuming =getCurrentTime() -  start_time;
//    cout<<"耗时:"<<time_consuming/1000.0<<"s"<<endl;
//    cout<<"平均耗时:"<<time_consuming/float(filename.size())<<"ms"<<endl;
//    cout<<"一共"<<filename.size()<<"张"<<endl;
//    cout<<"完全识别出"<<count<<"张"<<endl;
//    cout<<"识别率:"<<(float(count)/filename.size())*100<<"%"<<endl;
//
//}



void evaluate() {


    Directory_mac dir;
    // LPR::CONFIG::Config config("/Users/yujinke/Desktop/platejudge.data","/Users/yujinke/Desktop/LeNet-weights");
    HyperLPR lpr;
    //vector<string> filename = dir.GetListFiles("/Users/yujinke/Desktop/EasyPR/resources/image/extreme_test/大角度", "*.jpg");
    ///vector<string> filename = dir.GetListFiles("/Users/yujinke/Desktop/EasyPR/resources/image/extreme_test/逆光", "*.jpg");

//vector<string> filename = dir.GetListFiles("/Users/yujinke/Desktop/test_home", "*.png");
   // vector<string> filename = dir.GetListFiles("/Users/yujinke/Desktop/EasyPR/resources/image/general_test", "*.JPG");
    vector<string> filename = dir.GetListFiles("/Users/yujinke/Downloads/label", "*.JPG");

  //  vector<string> filename = dir.GetListFiles("/Users/yujinke/Desktop/undetectedplate/", "*.JPG");

    //vector<string> filename = dir.GetListFiles("/Users/yujinke/ClionProjects/Quick_LPR/home/", "*.JPG");
    int start_time = getCurrentTime();

    int Num_correct = 0;
    int Num_located = 0;
    bool Num_correct_flag = 0 ;
    bool Num_located_flag = 0 ;

    for (int i = 0; i < filename.size(); i++) {
        if (filename[i].find(".jpg") < INT32_MAX) {
            char* dst_str = (char*)filename[i].data();
            vector<string> str =  lpr.PR_load_as_string(filename[i]);
            Mat img = imread(filename[i]);
            Num_correct_flag= 0 ;
            Num_located_flag = 0 ;


            for(int i= 0; i<str.size();i++)
            {
                char* res_str = (char*)(str[i]+".").data();
                int diff = stringdiff(dst_str,res_str);
                //cout<<res_str<<endl;
                if(!diff&&!Num_correct_flag) {
                    Num_correct++;
                    Num_correct_flag=1;
                }

                if(!Num_located_flag) {
                    Num_located++;
                    Num_located_flag=1;
                }
            }
            if(!Num_located_flag)
            {
                string path = "/Users/yujinke/Desktop/undetectedplate/";
              //  imwrite(path + Operate::getMatId(img) + ".jpg", img);
                Num_located_flag = 1;
            }
        }
        //cout<<"识别成功的车牌数目:"<<Num_correct<<endl;

    }


    int time_consuming =getCurrentTime() -  start_time;
    cout<<"一共"<<filename.size()<<"张车牌"<<endl;
    cout<<"耗时:"<<time_consuming/1000.0<<"s"<<endl;
    cout<<"平均耗时:"<<time_consuming/float(filename.size())<<"ms"<<endl;
    cout<<"定位成功的车牌:"<<Num_located<<"张"<<endl;
    cout<<"后六位完全匹配:"<<Num_correct<<"张"<<endl;
    cout<<"定位率:"<<(float(Num_located)/filename.size())*100<<"%"<<endl;
    cout<<"识别率:"<<(float(Num_correct)/filename.size())*100<<"%"<<endl;
    cout<<"定位识别率:"<<((float(Num_correct)/filename.size())/(float(Num_located)/filename.size()))*100<<"%"<<endl;

}
