//
//  AI013x0xDecoder.cpp
//  ZXingWidget
//
//  Created by Afrizal Chen on 9/8/13.
//
//

#include <zxing/NotFoundException.h>
#include <zxing/oned/rss/expanded/decoders/AI013x0xDecoder.h>

using zxing::BitArray;
using zxing::Ref;
using zxing::String;
using zxing::oned::rss::expanded::decoders::AI013x0xDecoder;

namespace {
    const int HEADER_SIZE = 4 + 1;
    const int WEIGHT_SIZE = 15;
}

AI013x0xDecoder::AI013x0xDecoder(Ref<BitArray> information)
    : AI01weightDecoder(information) {
}

AI013x0xDecoder::~AI013x0xDecoder() {
}

Ref<String> AI013x0xDecoder::parseInformation() {
    if (getInformation()->getSize() != HEADER_SIZE + GTIN_SIZE + WEIGHT_SIZE) {
        throw NotFoundException();
    }
    
    Ref<String> buf(new String(""));
    
    encodeCompressedGtin(buf, HEADER_SIZE);
    encodeCompressedWeight(buf, HEADER_SIZE + GTIN_SIZE, WEIGHT_SIZE);
    
    return buf;
}