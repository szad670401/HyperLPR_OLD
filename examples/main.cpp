#include <iostream>
#include "../Hyperlpr/hyperlpr.h"
#include "../Hyperlpr/io/linux_dir.h"

using namespace std;
using namespace LPR;


void test_plate_located() {


    Directory_mac dir;
   // LPR::CONFIG::Config config("/Users/yujinke/Desktop/platejudge.data","/Users/yujinke/Desktop/LeNet-weights");
    HyperLPR lpr;
   // vector<string> filename = dir.GetListFiles("/Users/yujinke/Desktop/EasyPR/resources/image/extreme_test/大角度", "*.jpg");
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


void  test(){



}
int main() {
    test_plate_located();

    return 0;
}