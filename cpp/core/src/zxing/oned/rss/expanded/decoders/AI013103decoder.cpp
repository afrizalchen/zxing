//
//  AI013103decoder.cpp
//  ZXingWidget
//
//  Created by Afrizal Chen on 9/8/13.
//
//

#include <zxing/oned/rss/expanded/decoders/AI013x0xDecoder.h>
#include <zxing/oned/rss/expanded/decoders/AI013103decoder.h>

using zxing::BitArray;
using zxing::Ref;
using zxing::String;
using zxing::oned::rss::expanded::decoders::AI013x0xDecoder;
using zxing::oned::rss::expanded::decoders::AI013103decoder;

AI013103decoder::AI013103decoder(Ref<BitArray> information)
    : AI013x0xDecoder(information) {
}

AI013103decoder::~AI013103decoder() {
}

void AI013103decoder::addWeightCode(Ref<String> buf, int weight) {
    buf->append("(3103)");
}

int AI013103decoder::checkWeight(int weight) {
    return weight;
}