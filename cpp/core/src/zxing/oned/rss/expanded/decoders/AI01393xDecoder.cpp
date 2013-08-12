//
//  AI01393xDecoder.cpp
//  ZXingWidget
//
//  Created by Afrizal Chen on 9/8/13.
//
//

#include <sstream>
#include <zxing/NotFoundException.h>
#include <zxing/oned/rss/expanded/decoders/AI01393xDecoder.h>

using zxing::BitArray;
using zxing::Ref;
using zxing::String;
using zxing::oned::rss::expanded::decoders::AI01decoder;
using zxing::oned::rss::expanded::decoders::AI01393xDecoder;

namespace {
    const int HEADER_SIZE = 5 + 1 + 2;
    const int LAST_DIGIT_SIZE = 2;
    const int FIRST_THREE_DIGITS_SIZE = 10;
}

AI01393xDecoder::AI01393xDecoder(Ref<BitArray> information)
    : AI01decoder(information) {
}

AI01393xDecoder::~AI01393xDecoder() {
}

Ref<String> AI01393xDecoder::parseInformation() {
    if(getInformation()->getSize() < HEADER_SIZE + GTIN_SIZE) {
        throw NotFoundException();
    }
    
    Ref<String> buf(new String(""));
    
    encodeCompressedGtin(buf, HEADER_SIZE);
    
    int lastAIdigit =
    getGeneralDecoder()->extractNumericValueFromBitArray(HEADER_SIZE + GTIN_SIZE, LAST_DIGIT_SIZE);
    
    buf->append("(393");
    std::ostringstream oss;
    oss << lastAIdigit;
    buf->append(oss.str());
    buf->append(')');
    
    int firstThreeDigits =
    getGeneralDecoder()->extractNumericValueFromBitArray(HEADER_SIZE + GTIN_SIZE + LAST_DIGIT_SIZE, FIRST_THREE_DIGITS_SIZE);
    if(firstThreeDigits / 100 == 0) {
        buf->append('0');
    }
    if(firstThreeDigits / 10 == 0) {
        buf->append('0');
    }
    oss.clear();
    oss.str("");
    oss << firstThreeDigits;
    buf->append(oss.str());
    
    Ref<DecodedInformation> generalInformation =
    getGeneralDecoder()->decodeGeneralPurposeField(HEADER_SIZE + GTIN_SIZE + LAST_DIGIT_SIZE + FIRST_THREE_DIGITS_SIZE, Ref<String>());
    buf->append(generalInformation->getNewString()->getText());
    
    return buf;
}