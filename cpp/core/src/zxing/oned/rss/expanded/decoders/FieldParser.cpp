//
//  FieldParser.cpp
//  ZXingWidget
//
//  Created by Afrizal Chen on 5/8/13.
//
//

#include <sstream>
#include <iomanip>
#include <vector>
#include <zxing/NotFoundException.h>
#include <zxing/oned/rss/expanded/decoders/FieldParser.h>

using zxing::Ref;
using zxing::String;
using zxing::oned::rss::expanded::decoders::FieldParser;

namespace {
    const int VARIABLE_LENGTH = INT_MAX;
    
    #define VECTOR_INIT(v) v, v + sizeof(v)/sizeof(v[0])
    const int TWO_DIGIT_DATA_LENGTH_1[2] = { 0, 18 };
    const int TWO_DIGIT_DATA_LENGTH_2[2] = { 01, 14};
    const int TWO_DIGIT_DATA_LENGTH_3[2] = { 02, 14};
    const int TWO_DIGIT_DATA_LENGTH_4[3] = { 10, VARIABLE_LENGTH, 20};
    const int TWO_DIGIT_DATA_LENGTH_5[2] = { 11, 6};
    const int TWO_DIGIT_DATA_LENGTH_6[2] = { 12, 6};
    const int TWO_DIGIT_DATA_LENGTH_7[2] = { 13, 6};
    const int TWO_DIGIT_DATA_LENGTH_8[2] = { 15, 6};
    const int TWO_DIGIT_DATA_LENGTH_9[2] = { 17, 6};
    const int TWO_DIGIT_DATA_LENGTH_10[2] = { 20, 2};
    const int TWO_DIGIT_DATA_LENGTH_11[3] = { 21, VARIABLE_LENGTH, 20};
    const int TWO_DIGIT_DATA_LENGTH_12[3] = { 22, VARIABLE_LENGTH, 29};
    const int TWO_DIGIT_DATA_LENGTH_13[3] = { 30, VARIABLE_LENGTH, 8};
    const int TWO_DIGIT_DATA_LENGTH_14[3] = { 37, VARIABLE_LENGTH, 8};
    //internal company codes
    const int TWO_DIGIT_DATA_LENGTH_15[3] = { 90, VARIABLE_LENGTH, 30};
    const int TWO_DIGIT_DATA_LENGTH_16[3] = { 91, VARIABLE_LENGTH, 30};
    const int TWO_DIGIT_DATA_LENGTH_17[3] = { 92, VARIABLE_LENGTH, 30};
    const int TWO_DIGIT_DATA_LENGTH_18[3] = { 93, VARIABLE_LENGTH, 30};
    const int TWO_DIGIT_DATA_LENGTH_19[3] = { 94, VARIABLE_LENGTH, 30};
    const int TWO_DIGIT_DATA_LENGTH_20[3] = { 95, VARIABLE_LENGTH, 30};
    const int TWO_DIGIT_DATA_LENGTH_21[3] = { 96, VARIABLE_LENGTH, 30};
    const int TWO_DIGIT_DATA_LENGTH_22[3] = { 97, VARIABLE_LENGTH, 30};
    const int TWO_DIGIT_DATA_LENGTH_23[3] = { 98, VARIABLE_LENGTH, 30};
    const int TWO_DIGIT_DATA_LENGTH_24[3] = { 99, VARIABLE_LENGTH, 30};
    const std::vector<int> TDDL_1 = std::vector<int>(VECTOR_INIT(TWO_DIGIT_DATA_LENGTH_1));
    const std::vector<int> TDDL_2 = std::vector<int>(VECTOR_INIT(TWO_DIGIT_DATA_LENGTH_2));
    const std::vector<int> TDDL_3 = std::vector<int>(VECTOR_INIT(TWO_DIGIT_DATA_LENGTH_3));
    const std::vector<int> TDDL_4 = std::vector<int>(VECTOR_INIT(TWO_DIGIT_DATA_LENGTH_4));
    const std::vector<int> TDDL_5 = std::vector<int>(VECTOR_INIT(TWO_DIGIT_DATA_LENGTH_5));
    const std::vector<int> TDDL_6 = std::vector<int>(VECTOR_INIT(TWO_DIGIT_DATA_LENGTH_6));
    const std::vector<int> TDDL_7 = std::vector<int>(VECTOR_INIT(TWO_DIGIT_DATA_LENGTH_7));
    const std::vector<int> TDDL_8 = std::vector<int>(VECTOR_INIT(TWO_DIGIT_DATA_LENGTH_8));
    const std::vector<int> TDDL_9 = std::vector<int>(VECTOR_INIT(TWO_DIGIT_DATA_LENGTH_9));
    const std::vector<int> TDDL_10 = std::vector<int>(VECTOR_INIT(TWO_DIGIT_DATA_LENGTH_10));
    const std::vector<int> TDDL_11 = std::vector<int>(VECTOR_INIT(TWO_DIGIT_DATA_LENGTH_11));
    const std::vector<int> TDDL_12 = std::vector<int>(VECTOR_INIT(TWO_DIGIT_DATA_LENGTH_12));
    const std::vector<int> TDDL_13 = std::vector<int>(VECTOR_INIT(TWO_DIGIT_DATA_LENGTH_13));
    const std::vector<int> TDDL_14 = std::vector<int>(VECTOR_INIT(TWO_DIGIT_DATA_LENGTH_14));
    const std::vector<int> TDDL_15 = std::vector<int>(VECTOR_INIT(TWO_DIGIT_DATA_LENGTH_15));
    const std::vector<int> TDDL_16 = std::vector<int>(VECTOR_INIT(TWO_DIGIT_DATA_LENGTH_16));
    const std::vector<int> TDDL_17 = std::vector<int>(VECTOR_INIT(TWO_DIGIT_DATA_LENGTH_17));
    const std::vector<int> TDDL_18 = std::vector<int>(VECTOR_INIT(TWO_DIGIT_DATA_LENGTH_18));
    const std::vector<int> TDDL_19 = std::vector<int>(VECTOR_INIT(TWO_DIGIT_DATA_LENGTH_19));
    const std::vector<int> TDDL_20 = std::vector<int>(VECTOR_INIT(TWO_DIGIT_DATA_LENGTH_20));
    const std::vector<int> TDDL_21 = std::vector<int>(VECTOR_INIT(TWO_DIGIT_DATA_LENGTH_21));
    const std::vector<int> TDDL_22 = std::vector<int>(VECTOR_INIT(TWO_DIGIT_DATA_LENGTH_22));
    const std::vector<int> TDDL_23 = std::vector<int>(VECTOR_INIT(TWO_DIGIT_DATA_LENGTH_23));
    const std::vector<int> TDDL_24 = std::vector<int>(VECTOR_INIT(TWO_DIGIT_DATA_LENGTH_24));
    const std::vector<int> ARR_TWO_DIGIT_DATA_LENGTH[24] =
    { TDDL_1, TDDL_2, TDDL_3, TDDL_4, TDDL_5, TDDL_6, TDDL_7, TDDL_8, TDDL_9, TDDL_10,
      TDDL_11, TDDL_12, TDDL_13, TDDL_14, TDDL_15, TDDL_16, TDDL_17, TDDL_18, TDDL_19, TDDL_20,
      TDDL_21, TDDL_22, TDDL_23, TDDL_24  };
    const std::vector< std::vector<int> > TWO_DIGIT_DATA_LENGTH = std::vector< std::vector<int> >(VECTOR_INIT(ARR_TWO_DIGIT_DATA_LENGTH));
    
    const int THREE_DIGIT_DATA_LENGTH_1[3] = { 240, VARIABLE_LENGTH, 30};
    const int THREE_DIGIT_DATA_LENGTH_2[3] = { 241, VARIABLE_LENGTH, 30};
    const int THREE_DIGIT_DATA_LENGTH_3[3] = { 242, VARIABLE_LENGTH, 6};
    const int THREE_DIGIT_DATA_LENGTH_4[3] = { 250, VARIABLE_LENGTH, 30};
    const int THREE_DIGIT_DATA_LENGTH_5[3] = { 251, VARIABLE_LENGTH, 30};
    const int THREE_DIGIT_DATA_LENGTH_6[3] = { 253, VARIABLE_LENGTH, 17};
    const int THREE_DIGIT_DATA_LENGTH_7[3] = { 254, VARIABLE_LENGTH, 20};
    const int THREE_DIGIT_DATA_LENGTH_8[3] = { 400, VARIABLE_LENGTH, 30};
    const int THREE_DIGIT_DATA_LENGTH_9[3] = { 401, VARIABLE_LENGTH, 30};
    const int THREE_DIGIT_DATA_LENGTH_10[2] = { 402, 17};
    const int THREE_DIGIT_DATA_LENGTH_11[3] = { 403, VARIABLE_LENGTH, 30};
    const int THREE_DIGIT_DATA_LENGTH_12[2] = { 410, 13};
    const int THREE_DIGIT_DATA_LENGTH_13[2] = { 411, 13};
    const int THREE_DIGIT_DATA_LENGTH_14[2] = { 412, 13};
    const int THREE_DIGIT_DATA_LENGTH_15[2] = { 413, 13};
    const int THREE_DIGIT_DATA_LENGTH_16[2] = { 414, 13};
    const int THREE_DIGIT_DATA_LENGTH_17[3] = { 420, VARIABLE_LENGTH, 20};
    const int THREE_DIGIT_DATA_LENGTH_18[3] = { 421, VARIABLE_LENGTH, 15};
    const int THREE_DIGIT_DATA_LENGTH_19[2] = { 422, 3};
    const int THREE_DIGIT_DATA_LENGTH_20[3] = { 423, VARIABLE_LENGTH, 15};
    const int THREE_DIGIT_DATA_LENGTH_21[2] = { 424, 3};
    const int THREE_DIGIT_DATA_LENGTH_22[2] = { 425, 3};
    const int THREE_DIGIT_DATA_LENGTH_23[2] = { 426, 3};
    const std::vector<int> ARR_THREE_DIGIT_DATA_LENGTH[23] = {
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_DATA_LENGTH_1)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_DATA_LENGTH_2)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_DATA_LENGTH_3)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_DATA_LENGTH_4)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_DATA_LENGTH_5)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_DATA_LENGTH_6)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_DATA_LENGTH_7)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_DATA_LENGTH_8)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_DATA_LENGTH_9)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_DATA_LENGTH_10)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_DATA_LENGTH_11)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_DATA_LENGTH_12)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_DATA_LENGTH_13)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_DATA_LENGTH_14)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_DATA_LENGTH_15)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_DATA_LENGTH_16)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_DATA_LENGTH_17)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_DATA_LENGTH_18)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_DATA_LENGTH_19)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_DATA_LENGTH_20)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_DATA_LENGTH_21)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_DATA_LENGTH_22)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_DATA_LENGTH_23)) };
    const std::vector< std::vector<int> > THREE_DIGIT_DATA_LENGTH = std::vector< std::vector<int> >(VECTOR_INIT(ARR_THREE_DIGIT_DATA_LENGTH));
    
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_1[2] = { 310, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_2[2] = { 311, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_3[2] = { 312, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_4[2] = { 313, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_5[2] = { 314, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_6[2] = { 315, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_7[2] = { 316, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_8[2] = { 320, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_9[2] = { 321, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_10[2] = { 322, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_11[2] = { 323, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_12[2] = { 324, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_13[2] = { 325, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_14[2] = { 326, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_15[2] = { 327, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_16[2] = { 328, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_17[2] = { 329, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_18[2] = { 330, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_19[2] = { 331, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_20[2] = { 332, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_21[2] = { 333, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_22[2] = { 334, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_23[2] = { 335, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_24[2] = { 336, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_25[2] = { 340, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_26[2] = { 341, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_27[2] = { 342, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_28[2] = { 343, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_29[2] = { 344, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_30[2] = { 345, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_31[2] = { 346, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_32[2] = { 347, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_33[2] = { 348, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_34[2] = { 349, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_35[2] = { 350, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_36[2] = { 351, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_37[2] = { 352, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_38[2] = { 353, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_39[2] = { 354, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_40[2] = { 355, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_41[2] = { 356, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_42[2] = { 357, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_43[2] = { 360, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_44[2] = { 361, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_45[2] = { 362, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_46[2] = { 363, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_47[2] = { 364, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_48[2] = { 365, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_49[2] = { 366, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_50[2] = { 367, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_51[2] = { 368, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_52[2] = { 369, 6};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_53[3] = { 390, VARIABLE_LENGTH, 15};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_54[3] = { 391, VARIABLE_LENGTH, 18};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_55[3] = { 392, VARIABLE_LENGTH, 15};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_56[3] = { 393, VARIABLE_LENGTH, 18};
    const int THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_57[3] = { 703, VARIABLE_LENGTH, 30};
    const std::vector<int> ARR_THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH[57] = {
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_1)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_2)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_3)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_4)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_5)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_6)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_7)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_8)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_9)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_10)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_11)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_12)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_13)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_14)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_15)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_16)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_17)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_18)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_19)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_20)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_21)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_22)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_23)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_24)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_25)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_26)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_27)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_28)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_29)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_30)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_31)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_32)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_33)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_34)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_35)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_36)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_37)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_38)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_39)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_40)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_41)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_42)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_43)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_44)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_45)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_46)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_47)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_48)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_49)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_50)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_51)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_52)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_53)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_54)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_55)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_56)),
        std::vector<int>(VECTOR_INIT(THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH_57)) };
    const std::vector< std::vector<int> > THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH = std::vector< std::vector<int> >(VECTOR_INIT(ARR_THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH));
    
    const int FOUR_DIGIT_DATA_LENGTH_1[2] = { 7001, 13};
    const int FOUR_DIGIT_DATA_LENGTH_2[3] = { 7002, VARIABLE_LENGTH, 30};
    const int FOUR_DIGIT_DATA_LENGTH_3[2] = { 7003, 10};
    const int FOUR_DIGIT_DATA_LENGTH_4[2] = { 8001, 14};
    const int FOUR_DIGIT_DATA_LENGTH_5[3] = { 8002, VARIABLE_LENGTH, 20};
    const int FOUR_DIGIT_DATA_LENGTH_6[3] = { 8003, VARIABLE_LENGTH, 30};
    const int FOUR_DIGIT_DATA_LENGTH_7[3] = { 8004, VARIABLE_LENGTH, 30};
    const int FOUR_DIGIT_DATA_LENGTH_8[2] = { 8005, 6};
    const int FOUR_DIGIT_DATA_LENGTH_9[2] = { 8006, 18};
    const int FOUR_DIGIT_DATA_LENGTH_10[3] = { 8007, VARIABLE_LENGTH, 30};
    const int FOUR_DIGIT_DATA_LENGTH_11[3] = { 8008, VARIABLE_LENGTH, 12};
    const int FOUR_DIGIT_DATA_LENGTH_12[2] = { 8018, 18};
    const int FOUR_DIGIT_DATA_LENGTH_13[3] = { 8020, VARIABLE_LENGTH, 25};
    const int FOUR_DIGIT_DATA_LENGTH_14[2] = { 8100, 6};
    const int FOUR_DIGIT_DATA_LENGTH_15[2] = { 8101, 10};
    const int FOUR_DIGIT_DATA_LENGTH_16[2] = { 8102, 2};
    const int FOUR_DIGIT_DATA_LENGTH_17[3] = { 8110, VARIABLE_LENGTH, 70};
    const int FOUR_DIGIT_DATA_LENGTH_18[3] = { 8200, VARIABLE_LENGTH, 70};
    const std::vector<int> ARR_FOUR_DIGIT_DATA_LENGTH[18] = {
        std::vector<int>(VECTOR_INIT(FOUR_DIGIT_DATA_LENGTH_1)),
        std::vector<int>(VECTOR_INIT(FOUR_DIGIT_DATA_LENGTH_2)),
        std::vector<int>(VECTOR_INIT(FOUR_DIGIT_DATA_LENGTH_3)),
        std::vector<int>(VECTOR_INIT(FOUR_DIGIT_DATA_LENGTH_4)),
        std::vector<int>(VECTOR_INIT(FOUR_DIGIT_DATA_LENGTH_5)),
        std::vector<int>(VECTOR_INIT(FOUR_DIGIT_DATA_LENGTH_6)),
        std::vector<int>(VECTOR_INIT(FOUR_DIGIT_DATA_LENGTH_7)),
        std::vector<int>(VECTOR_INIT(FOUR_DIGIT_DATA_LENGTH_8)),
        std::vector<int>(VECTOR_INIT(FOUR_DIGIT_DATA_LENGTH_9)),
        std::vector<int>(VECTOR_INIT(FOUR_DIGIT_DATA_LENGTH_10)),
        std::vector<int>(VECTOR_INIT(FOUR_DIGIT_DATA_LENGTH_11)),
        std::vector<int>(VECTOR_INIT(FOUR_DIGIT_DATA_LENGTH_12)),
        std::vector<int>(VECTOR_INIT(FOUR_DIGIT_DATA_LENGTH_13)),
        std::vector<int>(VECTOR_INIT(FOUR_DIGIT_DATA_LENGTH_14)),
        std::vector<int>(VECTOR_INIT(FOUR_DIGIT_DATA_LENGTH_15)),
        std::vector<int>(VECTOR_INIT(FOUR_DIGIT_DATA_LENGTH_16)),
        std::vector<int>(VECTOR_INIT(FOUR_DIGIT_DATA_LENGTH_17)),
        std::vector<int>(VECTOR_INIT(FOUR_DIGIT_DATA_LENGTH_18)) };
    const std::vector< std::vector<int> > FOUR_DIGIT_DATA_LENGTH = std::vector< std::vector<int> >(VECTOR_INIT(ARR_FOUR_DIGIT_DATA_LENGTH));
}

FieldParser::FieldParser() { }

Ref<String> FieldParser::processFixedAI(int aiSize, int fieldSize, Ref<String> rawInformation) {
    if (rawInformation->length() < aiSize) {
        throw NotFoundException();
    }
    
    std::string ai = rawInformation->getText().substr(0, aiSize);
    
    if(rawInformation->length() < aiSize + fieldSize) {
        throw NotFoundException();
    }
    
    std::string field = rawInformation->getText().substr(aiSize, aiSize + fieldSize);
    Ref<String> remaining = rawInformation->substring(aiSize + fieldSize);
    std::string result = std::string("(").append(ai).append(")").append(field);
    Ref<String> parsedAI = parseFieldsInGeneralPurpose(remaining);
    return parsedAI.empty() ? Ref<String>(new String(result)) : Ref<String>(new String(result.append(parsedAI->getText())));
}

Ref<String> FieldParser::processVariableAI(int aiSize, int variableFieldSize, Ref<String> rawInformation) {
    std::string ai = rawInformation->getText().substr(0, aiSize);
    int maxSize;
    if (rawInformation->length() < aiSize + variableFieldSize) {
        maxSize = rawInformation->length();
    } else {
        maxSize = aiSize + variableFieldSize;
    }
    std::string field = rawInformation->getText().substr(aiSize, maxSize);
    Ref<String> remaining = rawInformation->substring(maxSize);
    std::string result = std::string("(").append(ai).append(")").append(field);
    Ref<String> parsedAI = parseFieldsInGeneralPurpose(remaining);
    return parsedAI.empty() ? Ref<String>(new String(result)) : Ref<String>(new String(result.append(parsedAI->getText())));
}

Ref<String> FieldParser::parseFieldsInGeneralPurpose(Ref<String> rawInformation) {
    if (rawInformation.empty() || rawInformation->length() == 0) {
        return Ref<String>();
    }
    
    // Processing 2-digit AIs
    
    if(rawInformation->length() < 2) {
        throw NotFoundException();
    }
    
    Ref<String> firstTwoDigits(new String(rawInformation->getText().substr(0, 2)));
    
    for (std::vector< std::vector<int> >::const_iterator it = TWO_DIGIT_DATA_LENGTH.begin(); it != TWO_DIGIT_DATA_LENGTH.end(); it++) {
        std::vector<int> dataLength = *it;
        std::ostringstream oss;
        oss << std::setw(2) << std::setfill('0') << dataLength[0];
        std::string dataLength0 = oss.str();
        if (dataLength0.compare(firstTwoDigits->getText()) == 0) {
            if (dataLength[1] == VARIABLE_LENGTH) {
                return processVariableAI(2, dataLength[2], rawInformation);
            }
            return processFixedAI(2, dataLength[1], rawInformation);
        }
    }
    
    if(rawInformation->length() < 3) {
        throw NotFoundException();
    }
    
    Ref<String> firstThreeDigits(new String(rawInformation->getText().substr(0, 3)));
    for (std::vector< std::vector<int> >::const_iterator it = THREE_DIGIT_DATA_LENGTH.begin(); it != THREE_DIGIT_DATA_LENGTH.end(); it++) {
        std::vector<int> dataLength = *it;
        std::ostringstream oss;
        oss << std::setw(3) << std::setfill('0') << dataLength[0];
        std::string dataLength0 = oss.str();
        if (dataLength0.compare(firstThreeDigits->getText()) == 0) {
            if (dataLength[1] == VARIABLE_LENGTH) {
                return processVariableAI(3, dataLength[2], rawInformation);
            }
            return processFixedAI(3, dataLength[1], rawInformation);
        }
    }
    
    for (std::vector< std::vector<int> >::const_iterator it = THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH.begin(); it != THREE_DIGIT_PLUS_DIGIT_DATA_LENGTH.end(); it++) {
        std::vector<int> dataLength = *it;
        std::ostringstream oss;
        oss << std::setw(3) << std::setfill('0') << dataLength[0];
        std::string dataLength0 = oss.str();
        if (dataLength0.compare(firstThreeDigits->getText()) == 0) {
            if (dataLength[1] == VARIABLE_LENGTH) {
                return processVariableAI(4, dataLength[2], rawInformation);
            }
            return processFixedAI(4, dataLength[1], rawInformation);
        }
    }
    
    if(rawInformation->length() < 4) {
        throw NotFoundException();
    }
    
    Ref<String> firstFourDigits(new String(rawInformation->getText().substr(0, 4)));
    for (std::vector< std::vector<int> >::const_iterator it = FOUR_DIGIT_DATA_LENGTH.begin(); it != FOUR_DIGIT_DATA_LENGTH.end(); it++) {
        std::vector<int> dataLength = *it;
        std::ostringstream oss;
        oss << std::setw(4) << std::setfill('0') << dataLength[0];
        std::string dataLength0 = oss.str();
        if (dataLength0.compare(firstFourDigits->getText()) == 0) {
            if (dataLength[1] == VARIABLE_LENGTH) {
                return processVariableAI(4, dataLength[2], rawInformation);
            }
            return processFixedAI(4, dataLength[1], rawInformation);
        }
    }
    
    throw NotFoundException();
}









