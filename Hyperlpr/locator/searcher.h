//
// Created by 庾金科 on 6/26/16.
//

#ifndef HYPERLPR_SEARCHER_H
#define HYPERLPR_SEARCHER_H

#endif //HYPERLPR_SEARCHER_H

#include "../plate/plate.h"
#include "../util/operate.h"

#define DRAWCONTOUR(dst,contours,idx) drawContours( dst, contours, idx, Scalar(255,255,255), CV_FILLED, 8);
#define MIN_PLATE_AREA 1000
#define DEBUG
//#define DEBUG_PA
#define DEBUG_SHOW(name,img) imshow(name,img),waitKey(0);

namespace LPR {


    typedef int METHOD_SEARCH;

    namespace Plate_Search {

        //检测规格
        inline bool _verify(vector<Point> contour) {

            RotatedRect rotRecot = minAreaRect(contour);

            float error = 0.4;
            const float aspect = 4;
            int min = 10 * aspect * 10;
            int max = 125 * aspect * 125;
            float rmin = aspect - aspect * error;
            float rmax = aspect + aspect * error;
            int area = rotRecot.size.height * rotRecot.size.width;
            float area_candidate = contourArea(contour);

            float r = (float) rotRecot.size.width / (float) rotRecot.size.height;
            Size boundingsize = rotRecot.boundingRect().size();
            //DEBUG cout<<"轮廓面积占比:"<<area_candidate/area<<endl;

            //    if((area_candidate/area)<0.6) return false;

            if (r < 1)
                r = 1 / r;
            if ((area < min || area > max) || boundingsize.height > boundingsize.width * 1.2) {
                return false;

            }

            return true;
        }

        inline float calcRelativefactor(Mat A, Mat B) {
            float Aera_A = A.rows*A.cols;
            if(Aera_A < 50 ) return MAXFLOAT;
            float Aera_B = B.rows*B.cols;
            if(Aera_B < 50)  return MAXFLOAT;


            if(abs(Aera_A-Aera_B)>min(Aera_A,Aera_B)*8) return MAXFLOAT;

            Mat block1,block2;

            cvtColor(A, block1, CV_BGR2HSV);
            cvtColor(B, block2, CV_BGR2HSV);
            int h_bins = 50;
            int s_bins = 60;
            int histSize[] = {h_bins, s_bins};
            float h_ranges[] = {0, 256};
            float s_ranges[] = {0, 180};
            const float *ranges[] = {h_ranges, s_ranges};
            int channels[] = {0, 1};
            MatND hist_block1;
            MatND hist_block2;
            calcHist(&block1, 1, channels, Mat(), hist_block1, 2, histSize, ranges, true, false);
            normalize(hist_block1, hist_block1, 0, 1, NORM_MINMAX, -1, Mat());
            calcHist(&block2, 1, channels, Mat(), hist_block2, 2, histSize, ranges, true, false);
            normalize(hist_block2, hist_block2, 0, 1, NORM_MINMAX, -1, Mat());
            double result = compareHist(hist_block1, hist_block2, CV_COMP_CHISQR);
            return result;

        }
        //尺度空间扩大后的第二次搜索.

        inline Mat emphasisLiVerticalEdge(Mat &input,int intensity = 8){
            //使离散的垂直边缘聚合
            int w_kernel = intensity>>1;
            for(int r = 0 ; r < input.rows; r++){
                uchar* _rdata = input.ptr<uchar>(r);
                for(int c = w_kernel ; c < (input.cols - w_kernel + 1 ) ; c++) {
                    uchar L =  _rdata[c-w_kernel];
                    uchar R =  _rdata[c+w_kernel];
                    if(L&&R) for(int p =  c - w_kernel ; p < (c +w_kernel) ;  _rdata[p++]=255);
                }
            }
            return input;
        }
        // 削弱水平边缘

        inline void weakenHorizonEdge(Mat &input,int intensity = 8,int w_exist = 3 ){
            auto kernel = getStructuringElement(MORPH_RECT, Size(4, 4));
            erode(input,input,kernel);
            dilate(input,input,getStructuringElement(MORPH_RECT, Size(5, 5)));
            //erode(input,input,kernel);
            //dilate(input,input,getStructuringElement(MORPH_RECT, Size(1, 5)));


        }

        void Rect_threshold(Mat input,int h = 10,int w = 10 )
        {
            for(int r = 0 ; r<input.rows;r+=h)
            {
                for(int c = 0 ; c<input.cols;c+=w)
                {
                    Rect R(c,r,w,h);
                    Operate::getSafeRect(input,R);

                    int flag = 0 ;
                    Mat i  =  input(R);
                    threshold(i,i,0,255,CV_THRESH_OTSU);







                }
            }



        }

        //this function copyed from EasyPR

        Mat findPlateByColor(Mat RGB, int color, Mat &output_thres) {
            //code copyed from Project Easy_PR

            bool adaptive_minsv = true;

            const float max_sv = 255;
            const float minref_sv = 64;

            const float minabs_sv = 95;


            const int min_blue = 100;  // 100
            const int max_blue = 140;  // 140


            const int min_yellow = 10;  // 15
            const int max_yellow = 45;  // 40


            const int min_white = 0;   // 15
            const int max_white = 30;  // 40

            Mat src_hsv;

            cvtColor(RGB, src_hsv, CV_BGR2HSV);

            vector<Mat> hsvSplit;
            split(src_hsv, hsvSplit);
            equalizeHist(hsvSplit[2], hsvSplit[2]);
            merge(hsvSplit, src_hsv);


            int min_h = 0;
            int max_h = 0;
            switch (color) {
                case 0:
                    min_h = min_blue;
                    max_h = max_blue;
                    break;
                case 1:
                    min_h = min_yellow;
                    max_h = max_yellow;
                    break;
                case 2:
                    min_h = min_white;
                    max_h = max_white;
                    break;
                default:
                    // Color::UNKNOWN
                    break;
            }

            float diff_h = float((max_h - min_h) / 2);
            float avg_h = min_h + diff_h;

            int channels = src_hsv.channels();
            int nRows = src_hsv.rows;
            //ÕºœÒ ˝æ›¡––Ë“™øº¬«Õ®µ¿ ˝µƒ”∞œÏ£ª
            int nCols = src_hsv.cols * channels;

            if (src_hsv.isContinuous())  //¡¨–¯¥Ê¥¢µƒ ˝æ›£¨∞¥“ª––¥¶¿Ì
            {
                nCols *= nRows;
                nRows = 1;
            }

            int i, j;
            uchar *p;
            float s_all = 0;
            float v_all = 0;
            float count = 0;
            for (i = 0; i < nRows; ++i) {
                p = src_hsv.ptr<uchar>(i);
                for (j = 0; j < nCols; j += 3) {
                    int H = int(p[j]);      // 0-180
                    int S = int(p[j + 1]);  // 0-255
                    int V = int(p[j + 2]);  // 0-255

                    s_all += S;
                    v_all += V;
                    count++;

                    bool colorMatched = false;

                    if (H > min_h && H < max_h) {
                        float Hdiff = 0;
                        if (H > avg_h)
                            Hdiff = H - avg_h;
                        else
                            Hdiff = avg_h - H;

                        float Hdiff_p = float(Hdiff) / diff_h;

                        float min_sv = 0;
                        if (true == adaptive_minsv)
                            min_sv =
                                    minref_sv -
                                    minref_sv / 2 *
                                    (1 - Hdiff_p);
                        else
                            min_sv = minabs_sv;

                        if ((S > min_sv && S < max_sv) && (V > min_sv && V < max_sv))
                            colorMatched = true;
                    }

                    if (colorMatched == true) {
                        p[j] = 0;
                        p[j + 1] = 0;
                        p[j + 2] = 255;
                    }
                    else {
                        p[j] = 0;
                        p[j + 1] = 0;
                        p[j + 2] = 0;
                    }
                }
            }

            Mat src_grey;
            vector<Mat> hsvSplit_done;
            split(src_hsv, hsvSplit_done);
            src_grey = hsvSplit_done[2];

            output_thres = src_grey;

            return src_grey;
        }

        void subSearchInside(Mat mask, Mat RGB, vector<_CandidatePlate> &plates) {//掩膜操作 Sub_Prepick 必须是一个二值图像
            //    imshow("RGB",RGB);
            Mat Preprocess;
            Mat _Masked;

            mask.copyTo(_Masked);
            Mat grey_img;


            cvtColor(RGB,grey_img,CV_RGB2GRAY);
            cvtColor(_Masked,_Masked,CV_GRAY2BGR);
            bitwise_and(RGB,_Masked,Preprocess);

            if (RGB.size() != mask.size())
                throw "Sub_Prepick.size()!=mask.size()";

            vector<vector<Point>> contours;
            //    bitwise_and(RGB, mask, _Masked);
            findContours(mask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
            float _n = -1; //直方图的误差
            float _min_ = MAXFLOAT; //误差最小的直方图
            const float MIN_ALLOWED = 50 ;
            RotatedRect _A_ ,_B_ ;// 当最小的时候 i 和 o

            vector<_CandidatePlate> plates_picked;

            for (int i = 0; i < contours.size(); i++) {
                vector<Point> current_contour = contours[i];
                RotatedRect rot_rect = minAreaRect(current_contour);
                int _flags = 0;

                Mat sub_mat_rgb = Operate::getSubMat(RGB, rot_rect.boundingRect(), _flags);

                Mat thres_blue ;



                /* 使用颜色进行搜索
                 *
                findPlateByColor(sub_mat_rgb,0,thres_blue);
                */

                Mat sub_mat_rgb_c;
                sub_mat_rgb.copyTo(sub_mat_rgb_c);

                cvtColor(sub_mat_rgb_c, sub_mat_rgb_c, CV_BGR2GRAY);
                blur(sub_mat_rgb_c, sub_mat_rgb_c, Size(3, 3));

                Sobel(sub_mat_rgb_c, sub_mat_rgb_c, CV_8U, 2, 0, 3, 1, 0);
                threshold(sub_mat_rgb_c, sub_mat_rgb_c, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
                Mat elemet = getStructuringElement(MORPH_RECT, Size(8, 2));
                morphologyEx(sub_mat_rgb_c, thres_blue, CV_MOP_CLOSE, elemet);
                emphasisLiVerticalEdge(thres_blue,18);
                //8:20:67%
                //8:18:70.2%
                //8:16:71.2%
                //8:14:69.2%
                //8:10:57


                weakenHorizonEdge(thres_blue);
                  //DEBUG imshow("thres_blue",thres_blue);
                //  DEBUG imshow("thres_blue",thres_blue);

                //cvtColor(thres_blue,thres_blue,CV_GRAY2BGR);
                //Mat and_thres_blue;

                //bitwise_and(thres_blue,sub_mat_rgb,and_thres_blue);

                vector<vector<Point>> contours_bounding;

                findContours(thres_blue,contours_bounding,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);

                for(int c= 0 ; c<contours_bounding.size();c++) {
                    //for each contours of BoundingRect which of the first scaned.
                    Mat single_contour(sub_mat_rgb.rows, sub_mat_rgb.cols, CV_8UC1);
                    single_contour.setTo(0);
                    auto area = contourArea(contours_bounding[c]);

                    if(area > 1000) {

                        DRAWCONTOUR(single_contour, contours_bounding, c);

                        //获取到车牌
                        //single_contour为 掩膜图像
                        //sub_mat_rgb 为 RGB 图像
                        _CandidatePlate candidate_Plate(sub_mat_rgb,single_contour,rot_rect.boundingRect());
                        candidate_Plate.correctPlate();

                        plates.push_back(candidate_Plate);


#ifdef DEBUG_PA
                        DEBUG auto c_t = getCurrentTime();

#endif

#ifdef DEBUG_PA
                        cout<<getCurrentTime()-c_t<<"ms"<<endl;
#endif
                        // DEBUG_SHOW("single",single_contour);


                    }
                }
            }

        }

        void Searching_plates(Mat input, vector<_CandidatePlate> &plates, METHOD_SEARCH method) {


            Mat input_level_2;

            Mat greyImg, greyImg_level_2;
            Mat greySobel, greySobel_level_2;
            Mat thresImg, thresImg_level_2;

            //建立尺度空间
            // pyrDown(input, input_level_2, Size(input.cols / 2, input.rows / 2));
            resize(input, input_level_2, Size(input.cols / 2, input.rows / 2));

            cvtColor(input_level_2, greyImg_level_2, CV_BGR2GRAY);
            blur(greyImg_level_2, greyImg_level_2, Size(3, 3));

            Sobel(greyImg_level_2, greyImg_level_2, CV_8U, 2, 0, 3, 1, 0);
            //Rect_threshold(greyImg_level_2,10,16);
            threshold(greyImg_level_2, thresImg_level_2, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

            Mat elemet = getStructuringElement(MORPH_RECT, Size(9, 2));


            //DEBUG_SHOW("greyImg_level_2",thresImg_level_2);



            morphologyEx(thresImg_level_2, thresImg_level_2, CV_MOP_CLOSE, elemet);
            emphasisLiVerticalEdge(thresImg_level_2,10);

       //     DEBUG_SHOW("greyImg_level_2",thresImg_level_2);

            Mat tofindcontours,tofindcontours_level_2,thres_image_bak;

            //  bitwise_not(thresImg, thresImg_INV);
            thresImg_level_2.copyTo(tofindcontours_level_2);
            resize(tofindcontours_level_2, tofindcontours, Size(input.cols,input.rows));


            //threshold(tofindcontours, tofindcontours, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
            Rect_threshold(tofindcontours,3);

            tofindcontours.copyTo(thres_image_bak); //


            vector<_CandidatePlate> revisePlate;//候选车牌

            vector<vector<Point>> contours;
//
//
//#ifdef DEBUG
//
//            DEBUG imshow("tofindcontours",thresImg_level_2);
//            DEBUG waitKey(0);
//
//#endif


            findContours(tofindcontours_level_2, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
            for (int i = 0; i < contours.size(); i++) {
                vector<Point> currentContour = contours[i];


                RotatedRect rotRecot = minAreaRect(contours[i]);

                if (_verify(currentContour)) {

                    int flags = -1;
                    Rect rect_bounding= rotRecot.boundingRect();
                    Mat sub_bd = Operate::getSubMat(input_level_2,rect_bounding,flags);

                    cvtColor(sub_bd,sub_bd,CV_RGB2GRAY);


                    Mat _sub_bounding_mat_mask(tofindcontours_level_2.rows, tofindcontours_level_2.cols, CV_8UC1);
                    _sub_bounding_mat_mask.setTo(0);
                    drawContours(_sub_bounding_mat_mask, contours, i, Scalar(255), CV_FILLED, 8);
                    resize(_sub_bounding_mat_mask, _sub_bounding_mat_mask, Size(input.cols, input.rows), 0, 0,
                           CV_INTER_NN);


                    Mat _sub_bounding_mat_rgb;
                    input.copyTo(_sub_bounding_mat_rgb);

                    //  DRAWCONTOUR(_sub_boungding_mat_mask,contours,i);

                  //DEBUG imshow("_sub_boungding_mat_mask",_sub_bounding_mat_rgb);
                  //DEBUG imshow("_sub_boungding_mat_rgb",_sub_bounding_mat_mask);
//                  DEBUG waitKey(0);
//                    //交给下个尺度



                    subSearchInside(_sub_bounding_mat_mask, _sub_bounding_mat_rgb,plates);

                }


            }













        }


    }

}

