//
//  AI01decoder.cpp
//  ZXingWidget
//
//  Created by Afrizal Chen on 8/8/13.
//
//

#include <sstream>
#include <zxing/oned/rss/expanded/decoders/AI01decoder.h>

using zxing::BitArray;
using zxing::Ref;
using zxing::String;
using zxing::oned::rss::expanded::decoders::AbstractExpandedDecoder;
using zxing::oned::rss::expanded::decoders::AI01decoder;

const int AI01decoder::GTIN_SIZE = 40;

AI01decoder::AI01decoder(Ref<BitArray> information)
    :AbstractExpandedDecoder(information) {
}

AI01decoder::~AI01decoder() {
    
}

void AI01decoder::encodeCompressedGtin(Ref<String> buf, int currentPos) {
    buf->append("(01)");
    int initialPosition = buf->length();
    buf->append('9');
    
    encodeCompressedGtinWithoutAI(buf, currentPos, initialPosition);
}

void AI01decoder::encodeCompressedGtinWithoutAI(Ref<String> buf, int currentPos, int initialBufferPosition) {
    for(int i = 0; i < 4; ++i){
        int currentBlock = getGeneralDecoder()->extractNumericValueFromBitArray(currentPos + 10 * i, 10);
        if (currentBlock / 100 == 0) {
            buf->append('0');
        }
        if (currentBlock / 10 == 0) {
            buf->append('0');
        }
        std::ostringstream oss;
        oss << currentBlock;
        buf->append(oss.str());
    }
    
    appendCheckDigit(buf, initialBufferPosition);
}

void AI01decoder::appendCheckDigit(Ref<String> buf, int currentPos) {
    int checkDigit = 0;
    for (int i = 0; i < 13; i++) {
        int digit = buf->charAt(i + currentPos) - '0';
        checkDigit += (i & 0x01) == 0 ? 3 * digit : digit;
    }
    
    checkDigit = 10 - (checkDigit % 10);
    if (checkDigit == 10) {
        checkDigit = 0;
    }
    std::ostringstream oss;
    oss << checkDigit;
    buf->append(oss.str());
}