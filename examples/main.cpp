#include <iostream>
#include "../Hyperlpr/hyperlpr.h"
#include "../Hyperlpr/io/linux_dir.h"

using namespace std;
using namespace LPR;


void test_plate_located() {


    Directory_mac dir;
   // LPR::CONFIG::Config config("/Users/yujinke/Desktop/platejudge.data","/Users/yujinke/Desktop/LeNet-weights");
    HyperLPR lpr;
    //vector<string> filename = dir.GetListFiles("/Users/yujinke/Desktop/EasyPR/resources/image/extreme_test/大角度", "*.jpg");
   vector<string> filename = dir.GetListFiles("/Users/yujinke/Desktop/EasyPR/resources/image/general_test", "*.JPG");
    //vector<string> filename = dir.GetListFiles("/Users/yujinke/ClionProjects/Quick_LPR/home/", "*.JPG");

    for (int i = 0; i < filename.size(); i++) {
        if (filename[i].find(".jpg") < INT32_MAX) {
            vector<string> str =  lpr.PR_load_as_string(filename[i]);
                Mat img = imread(filename[i]);

            for(int i= 0; i<str.size();i++)
            {
                cout<<str[i]<<endl;
            }
           imshow("img",img);
            waitKey(0);





        }
    }
}


void  test_recongizer(string path){
    Mat char_ = imread(path,CV_LOAD_IMAGE_GRAYSCALE);
    resize(char_,char_,Size(20,20));
    bitwise_not(char_,char_);

    CN_PLATE_recognizer recognizer( "/Users/yujinke/Desktop/LeNet-weights");
    string val = recognizer.simple_test(char_);
    cout<<val<<endl;



}
int main() {
    test_plate_located();
    test_recongizer("/Users/yujinke/Desktop/img/Timg1.png");

    return 0;
}