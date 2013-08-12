//
//  AI01320xDecoder.cpp
//  ZXingWidget
//
//  Created by Afrizal Chen on 9/8/13.
//
//

#include <zxing/oned/rss/expanded/decoders/AI013x0xDecoder.h>
#include <zxing/oned/rss/expanded/decoders/AI01320xDecoder.h>

using zxing::BitArray;
using zxing::Ref;
using zxing::String;
using zxing::oned::rss::expanded::decoders::AI013x0xDecoder;
using zxing::oned::rss::expanded::decoders::AI01320xDecoder;

AI01320xDecoder::AI01320xDecoder(Ref<BitArray> information)
    : AI013x0xDecoder(information) {
}

AI01320xDecoder::~AI01320xDecoder() {
}

void AI01320xDecoder::addWeightCode(Ref<String> buf, int weight) {
    if (weight < 10000) {
        buf->append("(3202)");
    } else {
        buf->append("(3203)");
    }
}

int AI01320xDecoder::checkWeight(int weight) {
    if(weight < 10000) {
        return weight;
    }
    return weight - 10000;
}