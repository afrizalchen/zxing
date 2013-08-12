//
//  AI01392xDecoder.cpp
//  ZXingWidget
//
//  Created by Afrizal Chen on 9/8/13.
//
//

#include <sstream>
#include <zxing/NotFoundException.h>
#include <zxing/oned/rss/expanded/decoders/AI01392xDecoder.h>

using zxing::BitArray;
using zxing::Ref;
using zxing::String;
using zxing::oned::rss::expanded::decoders::AI01decoder;
using zxing::oned::rss::expanded::decoders::AI01392xDecoder;

namespace {
    const int HEADER_SIZE = 5 + 1 + 2;
    const int LAST_DIGIT_SIZE = 2;
}

AI01392xDecoder::AI01392xDecoder(Ref<BitArray> information)
    : AI01decoder(information) {
}

AI01392xDecoder::~AI01392xDecoder() {
}

Ref<String> AI01392xDecoder::parseInformation() {
    if (getInformation()->getSize() < HEADER_SIZE + GTIN_SIZE) {
        throw NotFoundException();
    }
    
    Ref<String> buf(new String(""));
    
    encodeCompressedGtin(buf, HEADER_SIZE);
    
    int lastAIdigit =
    getGeneralDecoder()->extractNumericValueFromBitArray(HEADER_SIZE + GTIN_SIZE, LAST_DIGIT_SIZE);
    buf->append("(392");
    std::ostringstream oss;
    oss << lastAIdigit;
    buf->append(oss.str());
    buf->append(')');
    
    Ref<DecodedInformation> decodedInformation =
    getGeneralDecoder()->decodeGeneralPurposeField(HEADER_SIZE + GTIN_SIZE + LAST_DIGIT_SIZE, Ref<String>());
    buf->append(decodedInformation->getNewString()->getText());
    
    return buf;
}