//
//  AI01AndOtherAIs.cpp
//  ZXingWidget
//
//  Created by Afrizal Chen on 9/8/13.
//
//

#include <sstream>
#include <zxing/oned/rss/expanded/decoders/AI01AndOtherAIs.h>

using zxing::BitArray;
using zxing::Ref;
using zxing::String;
using zxing::oned::rss::expanded::decoders::AI01decoder;
using zxing::oned::rss::expanded::decoders::AI01AndOtherAIs;

namespace {
    const int HEADER_SIZE = 1 + 1 + 2; //first bit encodes the linkage flag,
    //the second one is the encodation method, and the other two are for the variable length
}

AI01AndOtherAIs::AI01AndOtherAIs(Ref<BitArray> information)
    : AI01decoder(information) {
}

AI01AndOtherAIs::~AI01AndOtherAIs() {
}

Ref<String> AI01AndOtherAIs::parseInformation() {
    Ref<String> buff(new String(""));
    
    buff->append("(01)");
    int initialGtinPosition = buff->length();
    int firstGtinDigit = getGeneralDecoder()->extractNumericValueFromBitArray(HEADER_SIZE, 4);
    std::ostringstream oss;
    oss << firstGtinDigit;
    buff->append(oss.str());
    
    encodeCompressedGtinWithoutAI(buff, HEADER_SIZE + 4, initialGtinPosition);
    printf("AI01AndOtherAIs::parseInformation() buff=%s\n", buff->getText().c_str());
    return getGeneralDecoder()->decodeAllCodes(buff, HEADER_SIZE + 44);
}