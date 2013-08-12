//
//  AI01weightDecoder.cpp
//  ZXingWidget
//
//  Created by Afrizal Chen on 9/8/13.
//
//

#include <sstream>
#include <zxing/oned/rss/expanded/decoders/AI01weightDecoder.h>

using zxing::BitArray;
using zxing::Ref;
using zxing::String;
using zxing::oned::rss::expanded::decoders::AI01decoder;
using zxing::oned::rss::expanded::decoders::AI01weightDecoder;

AI01weightDecoder::AI01weightDecoder(Ref<BitArray> information)
    : AI01decoder(information) {
}

AI01weightDecoder::~AI01weightDecoder() {
}

void AI01weightDecoder::encodeCompressedWeight(Ref<String> buf, int currentPos, int weightSize) {
    int originalWeightNumeric = getGeneralDecoder()->extractNumericValueFromBitArray(currentPos, weightSize);
    addWeightCode(buf, originalWeightNumeric);
    
    int weightNumeric = checkWeight(originalWeightNumeric);
    
    int currentDivisor = 100000;
    for(int i = 0; i < 5; ++i){
        if (weightNumeric / currentDivisor == 0) {
            buf->append('0');
        }
        currentDivisor /= 10;
    }
    std::ostringstream oss;
    oss << weightNumeric;
    buf->append(oss.str());
}
