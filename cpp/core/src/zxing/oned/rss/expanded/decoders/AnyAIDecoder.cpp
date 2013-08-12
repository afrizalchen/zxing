//
//  AnyAIDecoder.cpp
//  ZXingWidget
//
//  Created by Afrizal Chen on 8/8/13.
//
//

#include <zxing/oned/rss/expanded/decoders/AnyAIDecoder.h>

using zxing::BitArray;
using zxing::Ref;
using zxing::String;
using zxing::oned::rss::expanded::decoders::AbstractExpandedDecoder;
using zxing::oned::rss::expanded::decoders::AnyAIDecoder;

namespace {
    const int HEADER_SIZE = 2 + 1 + 2;
}

AnyAIDecoder::AnyAIDecoder(Ref<BitArray> information)
    : AbstractExpandedDecoder(information) {
}

AnyAIDecoder::~AnyAIDecoder() {
    
}

Ref<String> AnyAIDecoder::parseInformation() {
    Ref<String> buf(new String(""));
    return getGeneralDecoder()->decodeAllCodes(buf, HEADER_SIZE);
}
