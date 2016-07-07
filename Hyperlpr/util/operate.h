//
// Created by 庾金科 on 6/26/16.
//

#ifndef HYPERLPR_OPERATE_H
#define HYPERLPR_OPERATE_H

#endif //HYPERLPR_OPERATE_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <thread>
#include <sstream>
#include <sys/time.h>
#pragma once

#define DEBUG
#define DEBUG_SHOW(name,img) imshow(name,img),waitKey(0);

using namespace cv;
using std::cout;
using std::endl;

long getCurrentTime()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

namespace LPR {


    namespace Operate {



        DEBUG inline Vec3b GreyToColorMix(int val) {
            int r, g, b;
            Vec3b rgb;
            //red
            if (val > 0) {
                rgb[0] = abs(255 - val);
                rgb[1] = abs(128 - val);
                rgb[2] = abs(0 - val);
            }
            else {

                rgb[0] = 0;
                rgb[1] = 0;
                rgb[2] = 0;

            }
            return rgb;
        }

        DEBUG void Pseudocolor(Mat Input, Mat &Pseudocolor) {


            Pseudocolor = Mat(Input.rows, Input.cols, CV_8UC3);
            //    Pseudocolor.setTo(0);
            cout << Input.size() << endl;

            for (int r = 0; r < Input.rows; r++) {

                for (int c = 0; c < Input.cols; c++) {

                    uchar tmp_color = Input.at<uchar>(r, c);
                    Pseudocolor.at<Vec3b>(r, c) = GreyToColorMix(tmp_color);


                }
            }


        }
        //[DEBUG]伪彩色增强
        //用于显示Radon 变换图
        class Conners {
        public:
            Point2f p;
            bool x_flags = false;
            bool y_flags = false;
            bool x_or_y_flags = false;
            bool x_and_y_flags = false;


        };
        //获得两点的距离
        inline void swap(int list[], int i, int j) {
            int t = list[i];
            list[i] = list[j];
            list[j] = t;
        }

        inline void EnhancePlate(Mat img, Mat &output) {
            Mat ehist_plate;
            cvtColor(img, img, CV_RGB2GRAY);

            equalizeHist(img, ehist_plate);
            Mat denoising_plate;

            bilateralFilter(ehist_plate, denoising_plate, 60, 60, 40);
            output = denoising_plate;

        }

        inline float convertToRad(float angle) {
            return angle / 180 * 3.1415;


        }

        inline Mat setRotcScale(float angle) {

            Mat rot = Mat(2, 2, CV_32F);
            rot.at<float>(0, 0) = cos(convertToRad(angle));
            rot.at<float>(1, 0) = -sin(convertToRad(angle));
            rot.at<float>(0, 1) = sin(convertToRad(angle));
            rot.at<float>(1, 1) = cos(convertToRad(angle));


            return rot;


        }

        inline void Transfrom(vector<Point2f> input, vector<Point2f> &output, Mat tranform) {
            vector<Point2f> c;

            for (int i = 0; i < input.size(); i++) {
                Point2f p = input[i];
                Point2f o = Point2f(p.x * tranform.at<float>(0, 0) + p.y * tranform.at<float>(0, 1),
                                    p.x * tranform.at<float>(1, 0) + p.y * tranform.at<float>(1, 1)
                );
                c.push_back(o);
            }
            output.clear();
            output = c;
        }

        inline void getSafeRect(Mat img, Rect &bound) {



            if (bound.x < 0) {
                bound.x = 0;

            }

            if (bound.y < 0) {
                bound.y = 0;

            }

            if (bound.x + bound.width < 0) {

                bound.width = img.cols + bound.x;


            }
            if (bound.y + bound.height < 0) {
                bound.height = img.rows + bound.y;


            }

            if (bound.x + bound.width > img.cols) {


                bound.width = img.cols - bound.x;


            }
            if (bound.y + bound.height > img.rows) {
                bound.height = img.rows - bound.y;



            }



        }

        inline Mat getSubMat(Mat img, Rect bound, int &flags) {
            getSafeRect(img,bound);
            Mat standard(bound.size(), img.type());
            Mat result;
            standard = img(bound);
            standard.copyTo(result);
            return result;


        }

        inline string getMatId(Mat input){
            std::stringstream id;
            cvtColor(input,input,CV_RGB2GRAY);
            string result;
            int id_ = 0 ;

            for(int r = 0 ;r<input.rows;r+=2)
            {
                uchar* uc = input.ptr<uchar>(r);

                for(int c= 0 ; c<input.cols;c+=2)
                {


                    id_+=(int)uc[c];


                }


            }
            id<<id_;

            id>>result;

            return result;


        }
        inline void drawPlate(Mat &img,Rect rect)
        {
            rectangle(img,rect,Scalar(255,255,0),2,CV_AA);

        }

        template<typename T> inline float getBetDis(T p1, T p2) {
            return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
        }
        template<typename T> inline float getBetangle(T p1, T p2) {
            return cvFastArctan(p2.y - p1.y, p2.x - p1.x);

        }
        template<typename T> bool CheckSeq(vector<Point2f> &coded/*coded sort must be four*/) {
            if (coded.size() != 4)return 0;
            if (coded[3].y > coded[1].y &
                coded[1].x > coded[2].x &
                coded[0].y > coded[2].y &
                coded[3].x > coded[3].y)

                return true;

            return false;

        }
        template<typename T> inline vector<T> arrangeElements(vector<T> coded, unsigned int a, unsigned int b, unsigned int c,
                                                              unsigned int d) {
            vector<Point2f> arrange;
            arrange.push_back(coded[a]);
            arrange.push_back(coded[b]);
            arrange.push_back(coded[c]);
            arrange.push_back(coded[d]);
            return arrange;

        }
        template<typename T> inline bool resetPointPers(vector<Point2f> &coded, float angle=0) {

            vector<Conners> Points_plus;
            float MAX = 0;
            Conners sig;
            if (angle > -60 & angle < -17) {
                Mat transform = setRotcScale(angle);
                Transfrom(coded, coded, transform);
            }
            for (int i = 0; i < coded.size(); i++) {
                //Init
                sig.p = coded[i];
                Points_plus.push_back(sig);
            }
            for (int sortx = 1; sortx < Points_plus.size(); sortx++) {
                Conners key = Points_plus[sortx];
                int j = sortx - 1;
                while (j >= 0 & Points_plus[j].p.x > key.p.x) {
                    Points_plus[j + 1] = Points_plus[j];
                    j--;
                    if (j < 0) break;
                }
                Points_plus[j + 1] = key;
            }
            Points_plus[2].x_flags = true;
            Points_plus[3].x_flags = true;
            for (int sorty = 1; sorty < Points_plus.size(); sorty++) {
                Conners key = Points_plus[sorty];
                int j = sorty - 1;
                while (Points_plus[j].p.y > key.p.y & j >= 0) {
                    Points_plus[j + 1] = Points_plus[j];
                    j--;
                    if (j < 0) break;

                }
                Points_plus[j + 1] = key;

            }
            Points_plus[2].y_flags = true;
            Points_plus[3].y_flags = true;
            for (int i = 0; i < coded.size(); i++) {
                Points_plus[i].x_and_y_flags = Points_plus[i].x_flags & Points_plus[i].y_flags;
                Points_plus[i].x_or_y_flags = Points_plus[i].x_flags | Points_plus[i].y_flags;
                if (Points_plus[i].x_or_y_flags) {
                    if (Points_plus[i].x_and_y_flags & Points_plus[i].x_or_y_flags)
                        coded[0] = Points_plus[i].p;
                    else if (Points_plus[i].x_flags) coded[1] = Points_plus[i].p;
                    else coded[2] = Points_plus[i].p;
                }
                if (!Points_plus[i].x_and_y_flags & !Points_plus[i].x_or_y_flags)
                    coded[3] = Points_plus[i].p;

            }
            if (angle > -60 & angle < -17) {
                Mat transform_ = setRotcScale(-angle);
                Transfrom(coded, coded, transform_);
            }
            return 0;

        }
        template<typename T> inline void remapPoints(vector<T> &seg, Point zeroadd) {
            for (int i = 0; i < seg.size(); i++) {

                seg[i].x -= zeroadd.x;
                seg[i].y -= zeroadd.y;

            }
        }
        template<typename T> inline void remapPoint(T &seg, Point zeroadd) {

            seg.x -= zeroadd.x;
            seg.y -= zeroadd.y;


        }


    }





}
