//
// Created by 庾金科 on 6/27/16.
//

#ifndef HYPERLPR_COMPOSER_H
#define HYPERLPR_COMPOSER_H

#endif //HYPERLPR_COMPOSER_H

#include "hyperlpr.h"


namespace LPR{





        vector<_Plate> HyperLPR::PR_load_as_PLATE(string filename) {
            Mat load_image = imread(filename);
            return PR_existent_as_PLATE(load_image);
        }








}