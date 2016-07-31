//
// Created by 庾金科 on 7/3/16.
//
#define RECONGIZER_IMP

#ifdef RECONGIZER_IMP

#include <tesseract/baseapi.h>

#include "../charRecognizer.h"

#endif
using namespace cv;

using namespace std;


class recogizer_tesseract: public charRecognizer{
private:
    string chars_chi = "京沪津渝冀晋蒙辽吉黑苏浙皖闽赣鲁豫鄂湘粤桂琼川贵云藏陕甘青宁新";
    string chars_letter = "ABCDEFGHJKLMNPQRSTUVWXYZ";
    string chars_num = "0123456789";

    tesseract::TessBaseAPI tess_chi;
    tesseract::TessBaseAPI tess_eng;
    void set_mode(tesseract::TessBaseAPI *tess,string mode)
    {
        tess->SetVariable("tessedit_char_whitelist",mode.c_str());

    }

    char* tess_recognize(tesseract::TessBaseAPI *tess,cv::Mat char_inv)
    {
        tess->SetImage((uchar*)char_inv.data,char_inv.cols,char_inv.rows,1,char_inv.cols);
        return tess->GetUTF8Text();
    }

public:

#define CHAR_SIZE  20



    recogizer_tesseract(string filename_model)
    {
        tess_chi.Init(filename_model.c_str(),"chi_sim",tesseract::OEM_DEFAULT);
        tess_chi.SetPageSegMode(tesseract::PSM_SINGLE_CHAR);
        tess_eng.Init(filename_model.c_str(),"eng",tesseract::OEM_DEFAULT);
        tess_eng.SetPageSegMode(tesseract::PSM_SINGLE_CHAR);

    }


    string recognize_lite(cv::Mat char_inv,int type){
        string range = "";
        tesseract::TessBaseAPI *tess = NULL;

        if(type == NUMBER + LETTER){
            range+=chars_num;
            range+=chars_letter;
            tess = &tess_eng;
        }
        else if(type == NUMBER)
        {
            range+= chars_num;
            tess = &tess_eng;

        }
        else if(type == LETTER)
        {
            range+= chars_letter;
            tess = &tess_eng;
        }
        else if(type == PROVINCE){
            range+= chars_chi;
            tess = &tess_chi;

        }
        else{
            range+=chars_num;
            range+=chars_letter;
            tess = &tess_eng;
        }
        set_mode(tess,range);
        char* result = tess_recognize(tess,char_inv);
        result[(type == PROVINCE)*2 + 1]=0x00;
        if(result[0]==0)
            return "*";
        return result;


    }


};